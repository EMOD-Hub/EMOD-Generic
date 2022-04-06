/***************************************************************************************************

Copyright (c) 2018 Intellectual Ventures Property Holdings, LLC (IVPH) All rights reserved.

EMOD is licensed under the Creative Commons Attribution-Noncommercial-ShareAlike 4.0 License.
To view a copy of this license, visit https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode

***************************************************************************************************/

#include "stdafx.h"

#include <sstream>
#include "Debug.h"
#include "Sugar.h"
#include "Environment.h"
#include "StandardEventCoordinator.h"
#include "Configuration.h"
#include "FactorySupport.h"
#include "InterventionFactory.h"
#include "IIndividualHuman.h"
#include "IIndividualHumanContext.h"
#include "INodeContext.h"
#include "SimulationEventContext.h"
#include "NodeEventContext.h"
#include "Log.h"
#include "IdmString.h"
#include "RANDOM.h"

SETUP_LOGGING( "StandardEventCoordinator" )


// Let's have some fun and customize this. Log out which nodes get the intervention, and which inviduals, track
// them in a map, make bednet's conditional on some node info we get, and some individual info (not just coverage/
// randomness, then lets' repeat in 30 days 1 time.
// Then test out taking bednets away from migrators (just as a test).
namespace Kernel
{

    IMPLEMENT_FACTORY_REGISTERED(StandardInterventionDistributionEventCoordinator)

    IMPL_QUERY_INTERFACE2(StandardInterventionDistributionEventCoordinator, IEventCoordinator, IConfigurable)

    // ctor
    StandardInterventionDistributionEventCoordinator::StandardInterventionDistributionEventCoordinator( bool useDemographicCoverage ) 
    : parent(nullptr)
    , distribution_complete(false)
    , num_repetitions(1)
    , tsteps_between_reps(-1)
    , tsteps_since_last(0)
    //, include_emigrants(false)
    //, include_immigrants(false)
    , intervention_activated(false)
    , intervention_config()
    , cached_nodes()
    , node_suids()
    , _di( nullptr ) 
    , demographic_restrictions( true, TargetDemographicType::Everyone, useDemographicCoverage )
    , has_node_level_intervention(false)
    , demographic_coverage(1.0)
    , node_property_restrictions()
    , log_intervention_name()
    {
        LOG_DEBUG("StandardInterventionDistributionEventCoordinator ctor\n");
    }

    bool
    StandardInterventionDistributionEventCoordinator::Configure(
        const Configuration * inputJson
    )
    {
        initConfigComplexType( "Intervention_Config", &intervention_config, Intervention_Config_DESC_TEXT );

        InitializeRepetitions( inputJson );

        demographic_restrictions.ConfigureRestrictions( this, inputJson );

        initConfigComplexType( "Node_Property_Restrictions", &node_property_restrictions, SEC_Node_Property_Restriction_DESC_TEXT );

        bool retValue = JsonConfigurable::Configure( inputJson );
        if( retValue && !JsonConfigurable::_dryrun)
        {
            demographic_restrictions.CheckConfiguration();

            CheckRepetitionConfiguration();

            InterventionTypeValidation::Enum found_type = InterventionValidator::ValidateIntervention( 
                                                              GetTypeName(),
                                                              InterventionTypeValidation::EITHER,
                                                              intervention_config._json,
                                                              inputJson->GetDataLocation() );

            has_node_level_intervention = (found_type == InterventionTypeValidation::NODE);
            if( has_node_level_intervention )
            {
                // ---------------------------------------------------------------------------
                // --- If the user is attempting to define demographic restrictions when they
                // --- are using a node level intervention, then we need to error because these
                // --- restrictions are not doing anything.
                // ---------------------------------------------------------------------------
                if( !demographic_restrictions.HasDefaultRestrictions() )
                {
                    std::ostringstream msg ;
                    msg << "In StandardInterventionDistributionEventCoordinator, demographic restrictions such as 'Demographic_Coverage'\n";
                    msg << "and 'Target_Gender' do not apply when distributing node level interventions such as ";
                    msg << std::string( json::QuickInterpreter(intervention_config._json)["class"].As<json::String>() );
                    msg << ".\nThe node level intervention must handle the demographic restrictions.";
                    throw GeneralConfigurationException( __FILE__, __LINE__, __FUNCTION__, msg.str().c_str() );
                }
            }
        }

        return retValue;
    }

    void StandardInterventionDistributionEventCoordinator::InitializeRepetitions( const Configuration* inputJson )
    {
        initConfigTypeMap( "Number_Repetitions", &num_repetitions, Number_Repetitions_DESC_TEXT, -1, 1000, 1 );
        initConfigTypeMap( "Timesteps_Between_Repetitions", &tsteps_between_reps, Timesteps_Between_Repetitions_DESC_TEXT, -1, 10000 /*undefined*/, -1 /*off*/ ); // -1 = repeat without end, 0 is meaningless. want to think this one through more
    }

    void StandardInterventionDistributionEventCoordinator::CheckRepetitionConfiguration()
    {
        if( num_repetitions == 0 )
        {
            throw GeneralConfigurationException( __FILE__, __LINE__, __FUNCTION__, "'Number_Repetitions' cannot equal zero.");
        }
    }

    void StandardInterventionDistributionEventCoordinator::CheckStartDay( float campaignStartDay ) const
    { }

    void StandardInterventionDistributionEventCoordinator::SetContextTo( ISimulationEventContext *isec )
    {
        parent = isec;
        regenerateCachedNodeContextPointers();
    }

    // AddNode
    // EventCoordinators track nodes. Nodes can be used to get individuals, who can be queried for an intervention
    void
    StandardInterventionDistributionEventCoordinator::AddNode(
        const suids::suid& node_suid
    )
    {
        if( !intervention_activated )
        {
            intervention_activated = true;
            tsteps_since_last = tsteps_between_reps -1; // -1 is hack because Update is called before UpdateNodes and we inc in Update and check in UpdateNodes
        }

        // Store uids and node (event context) pointers
        node_suids.push_back(node_suid);
        cached_nodes.push_back(parent->GetNodeEventContext(node_suid));

        INodeEventContext * pNec = parent->GetNodeEventContext(node_suid);
        // Register unconditionally to be notified when individuals arrive at our node so we can zap them!
        // TODO: Make this param driven
        /*
        if( include_immigrants )
        {
            pNec->RegisterTravelDistributionSource( this, INodeEventContext::Arrival );
        }
        if( include_emigrants )
        {
            pNec->RegisterTravelDistributionSource( this, INodeEventContext::Departure );
        }
        */
    }

    void StandardInterventionDistributionEventCoordinator::Update( float dt )
    {
        // Check if it's time for another distribution
        if( intervention_activated && num_repetitions)
        {
            tsteps_since_last++;
        }
    }

    void StandardInterventionDistributionEventCoordinator::preDistribute()
    {
        return;
    }

    bool StandardInterventionDistributionEventCoordinator::IsTimeToUpdate( float dt )
    {
        // Only call VisitNodes on first call and if countdown == 0
        bool is_time_to_update = !((tsteps_since_last != tsteps_between_reps) || distribution_complete );
        return is_time_to_update;
    }

    void StandardInterventionDistributionEventCoordinator::UpdateNodes( float dt )
    {
        if( !IsTimeToUpdate( dt ) )
        {
            return;
        }

        ExtractInterventionNameForLogging();

        InitializeInterventions();

        LOG_DEBUG_F("[UpdateNodes] visiting %d nodes per NodeSet\n", cached_nodes.size());

        for (auto event_context : cached_nodes)
        {
            if( !node_property_restrictions.Qualifies( event_context->GetNodeContext()->GetNodeProperties() ) )
            {
                continue;
            }

            if( has_node_level_intervention )
            {
                DistributeInterventionsToNodes( event_context );
            }
            else
            {
                preDistribute();

                DistributeInterventionsToIndividuals( event_context );
            }
        }

        UpdateRepetitions();
    }

    void StandardInterventionDistributionEventCoordinator::UpdateRepetitions()
    {
        tsteps_since_last = 0;
        num_repetitions--;
        if( num_repetitions == 0 )
        {
            distribution_complete = true; // we're done, signal disposal ok
        }

        // this signals each process individually that its ok to clean up, in general if the completion times might be different on different nodes 
        // we'd want to coordinate the cleanup signal in Update()
    }

    bool StandardInterventionDistributionEventCoordinator::TargetedIndividualIsCovered(IIndividualHumanEventContext *ihec)
    {
        float demographic_coverage = demographic_restrictions.GetDemographicCoverage();
        return ihec->GetInterventionsContext()->GetParent()->GetRng()->SmartDraw( demographic_coverage );
    }

    bool
    StandardInterventionDistributionEventCoordinator::visitIndividualCallback( 
        IIndividualHumanEventContext *ihec,
        float & incrementalCostOut,
        ICampaignCostObserver * pICCO
    )
    {
        bool distributed = true;
        {
            // Add real checks on demographics based on intervention demographic targetting. 
            // Return immediately if we hit a non-distribute condition
            if( qualifiesDemographically( ihec ) == false )
            {
                LOG_DEBUG("Individual not given intervention because not in target demographic\n");
                return false;
            }
            LOG_DEBUG("Individual meets demographic targeting criteria\n"); 

            if (!TargetedIndividualIsCovered(ihec))
            {
                incrementalCostOut = 0;
                return false;
            }
            else
            {
                incrementalCostOut = 0;

                distributed = DistributeInterventionsToIndividual( ihec, incrementalCostOut, pICCO );
            }
        }
        return distributed;
    }

    void StandardInterventionDistributionEventCoordinator::regenerateCachedNodeContextPointers()
    {
        // regenerate the cached INodeEventContext* pointers fromthe cached node suids
        // the fact that this needs to happen is probably a good argument for the EC to own the NodeSet, since it needs to query the SEC for the node ids and context pointers anyway
        cached_nodes.clear();
        for (auto& node_id : node_suids)
        {
            cached_nodes.push_back(parent->GetNodeEventContext(node_id));
        }
    }

    void StandardInterventionDistributionEventCoordinator::formatInterventionClassNames( std::ostringstream& intervention_name, json::QuickInterpreter* actual_intervention_config)
    {
        if ( actual_intervention_config->Exist("Actual_Intervention_Config") )
        {
            // find actual_intervention_config if it exists
            actual_intervention_config = &( (*actual_intervention_config)["Actual_Intervention_Config"] );

            // append class name to intervention name
            intervention_name << " -> " << std::string( (*actual_intervention_config)["class"].As<json::String>() );

            // keep looking recursively for more actual_intervention_config layers
            formatInterventionClassNames( intervention_name, actual_intervention_config );
        }
        else if ( actual_intervention_config->Exist("Actual_Intervention_Configs") )
        {
            // maybe it was an array of actual interventions
            const json::Array& actual_interventions_array = (*actual_intervention_config)["Actual_Intervention_Configs"].As<json::Array>();

            // loop over array
            intervention_name << " -> ";
            int array_size = actual_interventions_array.Size();
            for( int idx = 0; idx < array_size; idx++ )
            {
                if( idx == 0 && array_size > 1 )
                    intervention_name << "[ ";
                else if ( idx > 0 )
                    intervention_name << ", ";

                // accumulate individual class names
                const json::Object& actual_intervention = json_cast<const json::Object&>( actual_interventions_array[idx] );
                actual_intervention_config = &json::QuickInterpreter(actual_intervention);
                intervention_name << std::string( (*actual_intervention_config)["class"].As<json::String>() );

                // recursively search for each for more layers!
                formatInterventionClassNames( intervention_name, actual_intervention_config );
            }
            if( array_size > 1 ) intervention_name << " ]";
        }
        else if ( actual_intervention_config->Exist("Positive_Diagnosis_Config") )
        {
            // maybe it was a diagnostic with positive_diagnosis_config if it exists (TODO: can this part be merged with the acutal_intervention_config block?)
            actual_intervention_config = &( (*actual_intervention_config)["Positive_Diagnosis_Config"] );

            // append class name to intervention name
            intervention_name << " -> " << std::string( (*actual_intervention_config)["class"].As<json::String>() );

            // keep looking recursively for more actual_intervention_config layers
            formatInterventionClassNames( intervention_name, actual_intervention_config );
        }
        else
        {
            // end of recursive call
        }
    }

    bool 
    StandardInterventionDistributionEventCoordinator::IsFinished()
    {
        return distribution_complete;
    }

    // private/protected
    bool
    StandardInterventionDistributionEventCoordinator::qualifiesDemographically(
        const IIndividualHumanEventContext* pIndividual
    )
    {
        return demographic_restrictions.IsQualified( pIndividual );
    }

    float StandardInterventionDistributionEventCoordinator::GetDemographicCoverage() const
    {
        return demographic_restrictions.GetDemographicCoverage();
    }

    int StandardInterventionDistributionEventCoordinator::GetMaxEvents() const
    {
        return demographic_restrictions.GetMaxEvents();
    }

    TargetDemographicType::Enum StandardInterventionDistributionEventCoordinator::GetTargetDemographic() const
    {
        return demographic_restrictions.GetTargetDemographic();
    }

    float StandardInterventionDistributionEventCoordinator::GetMinimumAge() const
    {
        return demographic_restrictions.GetMinimumAge();
    }

    float StandardInterventionDistributionEventCoordinator::GetMaximumAge() const
    {
        return demographic_restrictions.GetMaximumAge();
    }

    void StandardInterventionDistributionEventCoordinator::ProcessDeparting(
        IIndividualHumanEventContext *pInd
    )
    {
        LOG_INFO("Individual departing from node receiving intervention. TODO: enforce demographic and other qualifiers.\n");
        float incrementalCostOut = 0.0f;
        visitIndividualCallback( pInd, incrementalCostOut, nullptr /* campaign cost observer */ );
    } // these do nothing for now

    void
    StandardInterventionDistributionEventCoordinator::ProcessArriving(
        IIndividualHumanEventContext *pInd
    )
    {
        LOG_INFO("Individual arriving at node receiving intervention. TODO: enforce demographic and other qualifiers.\n");
        float incrementalCostOut = 0.0f; 
        visitIndividualCallback( pInd, incrementalCostOut, nullptr /* campaign cost observer */ );
    }

    void StandardInterventionDistributionEventCoordinator::ExtractInterventionNameForLogging()
    {
        // intervention class names for informative logging
        if( LOG_LEVEL( INFO ) && (log_intervention_name.str().size() == 0) )
        {
            log_intervention_name.str("");
            log_intervention_name << std::string( json::QuickInterpreter( intervention_config._json )[ "class" ].As<json::String>() );

#ifdef WIN32
            // DMB This method was crashing in Linux trying to get the class name out of Positive_Diagnosis_Config.  Not sure why.
            // including deeper information for "distributing" interventions (e.g. calendars)
            formatInterventionClassNames( log_intervention_name, &json::QuickInterpreter( intervention_config._json ) );
#endif
        }
    }

    void StandardInterventionDistributionEventCoordinator::InitializeInterventions()
    {
        if( !has_node_level_intervention && (_di == nullptr) )
        {
            auto config = Configuration::CopyFromElement( (intervention_config._json), "campaign" );
            _di = InterventionFactory::getInstance()->CreateIntervention( config );
            delete config;
            config = nullptr;
        }
    }

    void StandardInterventionDistributionEventCoordinator::DistributeInterventionsToNodes( INodeEventContext* event_context )
    {
        auto config = Configuration::CopyFromElement( (intervention_config._json), "campaign" );
        INodeDistributableIntervention *ndi = InterventionFactory::getInstance()->CreateNDIIntervention( config );
        if( ndi == nullptr )
        {
            throw IllegalOperationException( __FILE__, __LINE__, __FUNCTION__, "Should have constructed a node-level intervention." );
        }
        if( ndi->Distribute( event_context, this ) )
        {
            LOG_INFO_F( "UpdateNodes() distributed '%s' intervention to node %d\n", log_intervention_name.str().c_str(), event_context->GetId().data );
        }
        ndi->Release();
        delete config;
        config = nullptr;
    }

    void StandardInterventionDistributionEventCoordinator::DistributeInterventionsToIndividuals( INodeEventContext* event_context )
    {
        int totalIndivGivenIntervention = event_context->VisitIndividuals( this );

        if( LOG_LEVEL( INFO ) )
        {
            // Create log message 
            std::stringstream ss;
            ss << "UpdateNodes() gave out " << totalIndivGivenIntervention << " '" << log_intervention_name.str().c_str() << "' interventions ";
            std::string restriction_str = demographic_restrictions.GetPropertyRestrictionsAsString();
            if( !restriction_str.empty() )
            {
                ss << " with property restriction(s) " << restriction_str << " ";
            }
            ss << "at node " << event_context->GetExternalId() << "\n";
            LOG_INFO( ss.str().c_str() );
        }
    }

    bool StandardInterventionDistributionEventCoordinator::DistributeInterventionsToIndividual( IIndividualHumanEventContext *ihec,
                                                                                                float & incrementalCostOut,
                                                                                                ICampaignCostObserver * pICCO )
    {
        // instantiate and distribute intervention
        LOG_DEBUG_F( "Attempting to instantiate intervention of class %s\n", std::string( json::QuickInterpreter( intervention_config._json )[ "class" ].As<json::String>() ).c_str() );
        IDistributableIntervention *di = _di->Clone();
        release_assert( di );

        di->AddRef();
        bool distributed = di->Distribute( ihec->GetInterventionsContext(), pICCO );
        di->Release(); // a bit wasteful for now, could cache it for the next fellow

        if( distributed )
        {
            LOG_DEBUG_F( "Distributed an intervention (%p) to individual %d at a cost of %f\n",
                    di->GetName().c_str(), ihec->GetSuid().data, incrementalCostOut );
        }
        return distributed;
    }
}

#if 0
namespace Kernel
{
    template<class Archive>
    void serialize(Archive &ar, StandardInterventionDistributionEventCoordinator &ec, const unsigned int v)
    {
        ar & ec.coverage;
        ar & ec.distribution_complete;
        ar & ec.num_repetitions;
        ar & ec.tsteps_between_reps;
        ar & ec.demographic_coverage;
        ar & ec.target_demographic;
        ar & ec.target_age_min;
        ar & ec.target_age_max;
        ar & ec.include_emigrants;
        ar & ec.include_immigrants;
        ar & ec.tsteps_since_last;
        ar & ec.intervention_activated;
        ar & ec.intervention_config;

        // need to save the list of suids and restore from them, rather than saving the context pointers
        //ar & cached_nodes;
        ar & ec.node_suids;
    }
}
#endif

