/***************************************************************************************************

Copyright (c) 2018 Intellectual Ventures Property Holdings, LLC (IVPH) All rights reserved.

EMOD is licensed under the Creative Commons Attribution-Noncommercial-ShareAlike 4.0 License.
To view a copy of this license, visit https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode

***************************************************************************************************/

#include "stdafx.h"
#include <typeinfo>

#include "ISimulationContext.h"
#include "NodeEventContext.h"
#include "NodeEventContextHost.h"
#include "Debug.h"
#include "NodeDemographics.h"
#include "Node.h"
#include "IIndividualHuman.h"
#include "IIndividualHumanContext.h"
#include "EventTrigger.h"
#include "StrainIdentity.h"
#include "IdmDateTime.h"
#include "Log.h"
#include "Exceptions.h"
#include "TransmissionGroupsBase.h"

SETUP_LOGGING( "NodeEventContext" )

namespace Kernel
{
    NodeEventContextHost::NodeEventContextHost()
    : node(nullptr)
    , arrival_distribution_sources()
    , departure_distribution_sources()
    , interventions()
    , node_interventions()
    , broadcaster_impl()
    {
        arrival_distribution_sources.clear();
        //individual_event_observers.resize( EventTrigger::NUM_EVENT_TRIGGERS );
        //disposed_observers.resize( EventTrigger::NUM_EVENT_TRIGGERS );
    }

    NodeEventContextHost::NodeEventContextHost(Node* _node)
    : node(_node)
    , arrival_distribution_sources()
    , departure_distribution_sources()
    , interventions()
    , node_interventions()
    , broadcaster_impl()
    {
        arrival_distribution_sources.clear();
        //individual_event_observers.resize( EventTrigger::NUM_EVENT_TRIGGERS );
        //disposed_observers.resize( EventTrigger::NUM_EVENT_TRIGGERS );
    }

    // This was done with macros, but prefer actual code.
    Kernel::QueryResult NodeEventContextHost::QueryInterface( iid_t iid, void** ppinstance )
    {
        release_assert(ppinstance); // todo: add a real message: "QueryInterface requires a non-NULL destination!");

        if ( !ppinstance )
            return e_NULL_POINTER;

        ISupports* foundInterface;

        if ( iid == GET_IID(INodeEventContext)) 
            foundInterface = static_cast<INodeEventContext*>(this);
        else if (iid == GET_IID(INodeInterventionConsumer))
            foundInterface = static_cast<INodeInterventionConsumer*>(this);
        else if (iid == GET_IID(IOutbreakConsumer))
            foundInterface = static_cast<IOutbreakConsumer*>(this);
        else if (iid == GET_IID(ICampaignCostObserver))
            foundInterface = static_cast<ICampaignCostObserver*>(this);
        else if (iid == GET_IID( IIndividualEventBroadcaster ))
            foundInterface = static_cast<IIndividualEventBroadcaster*>(this);
        // -->> add support for other I*Consumer interfaces here <<--      
        else if (iid == GET_IID(IGlobalContext))
            node->QueryInterface(iid, (void**) &foundInterface);
        else if (iid == GET_IID(INodeContext))
            foundInterface = (INodeContext*)node;
        else
            foundInterface = nullptr;

        QueryResult status = e_NOINTERFACE;
        if ( foundInterface )
        {
            foundInterface->AddRef();
            status = s_OK;
            //status = InterventionsContainer::QueryInterface(iid, (void**)&foundInterface);
        }

        *ppinstance = foundInterface;
        return status;
    }

    void
    NodeEventContextHost::SetContextTo(
        INodeContext* context
    )
    {
        
        PropagateContextToDependents();
    }

    // method 1 for VisitIndividuals uses a functor/lambda function
    void NodeEventContextHost::VisitIndividuals( individual_visit_function_t func )
    {
        for( auto individual : node->individualHumans)
        {
            // TODO: I would like this function to return bool so we can keep a total of 'successful' calls, but
            // compiler threw up on me when I tried that! :(
            func(individual->GetEventContext());
        }
    }

    // method 2 for VisitIndividuals uses an interface
    int
    NodeEventContextHost::VisitIndividuals(
        IVisitIndividual * pEventCoordinator
    )
    {
        int retTotal = 0;
        float nodeCostThisInterventionThisTimestep = 0.0f;
        for( auto individual : node->individualHumans)
        {
            float costForThisIntervention = 0.0f;
            if( pEventCoordinator->visitIndividualCallback(individual->GetEventContext(), costForThisIntervention, this))
            {
                retTotal++;
                //nodeCostThisInterventionThisTimestep += (float)( costForThisIntervention * ih->GetMonteCarloWeight() );
            }
        }

        IncrementCampaignCost( nodeCostThisInterventionThisTimestep );
        LOG_DEBUG_F("Node CampaignCost = %f\n", nodeCostThisInterventionThisTimestep);
        return retTotal;
    }

    void NodeEventContextHost::notifyCampaignExpenseIncurred(
        float expenseIncurred,
        const IIndividualHumanEventContext * pIndiv
    )
    {
        release_assert( node );
        if( expenseIncurred > 0 )
        {
            LOG_DEBUG("Campaign expense was incurred\n");
        }
        float cost = expenseIncurred;
        if( pIndiv != nullptr )
        {
            cost *= float( pIndiv->GetMonteCarloWeight() );
        }
        IncrementCampaignCost( cost );
    }

    // First cut of this function writes the intervention report to stdout. It is an abbreviated,
    // 1-line-per-intervention report.
    void NodeEventContextHost::notifyCampaignEventOccurred(
        /*const*/ ISupports * pDistributedIntervention,
        /*const*/ ISupports * pDistributor,// for tb, cool to know parent intervention that 'gave', including tendency if it's an HSB.
        /*const*/ IIndividualHumanContext * pDistributeeIndividual
    )
    {
        if( Environment::getInstance()->Log->CheckLogLevel(Logger::DEBUG, "EEL") )
        {
            // intervention recipient
            std::stringstream msg;
            float recipientAge = float(pDistributeeIndividual->GetEventContext()->GetAge());
            msg << "hum_id="
                << pDistributeeIndividual->GetSuid().data
                << ",ra="
                << recipientAge
                << std::endl;
            Environment::getInstance()->Log->Log(Logger::DEBUG, "EEL","%s\n", msg.str().c_str() );
        }
    }

    const NodeDemographics& NodeEventContextHost::GetDemographics()
    {
        return node->demographics;
    }

    const suids::suid &
    NodeEventContextHost::GetId()
    const
    {
        return node->suid;
    }

    void NodeEventContextHost::ProcessArrivingIndividual( IIndividualHuman* ih )
    {
        for (auto& entry : arrival_distribution_sources)
        {
            for (int k = 0; k < entry.second; k++)
                entry.first->ProcessArriving(ih->GetEventContext());
        }
    }

    void NodeEventContextHost::ProcessDepartingIndividual( IIndividualHuman* ih )
    {
        //LOG_DEBUG( "ProcessDepartingIndividual\n" );
        for (auto& entry : departure_distribution_sources)
        {
            LOG_DEBUG( "ProcessDepartingIndividual for event coordinator\n" );
            for (int k = 0; k < entry.second; k++)
            {
                LOG_DEBUG( "ProcessDepartingIndividual: Calling ProcessDeparting for individual\n" );
                entry.first->ProcessDeparting(ih->GetEventContext());
            }
        }
    }

    void NodeEventContextHost::RegisterTravelDistributionSource( ITravelLinkedDistributionSource *tlds, TravelEventType type )
    {
        travel_distribution_source_map_t *sources = sourcesMapForType(type);        
        if (sources->find(tlds) != sources->end())
            (*sources)[tlds]++;
        else
            (*sources)[tlds] = 1;

        tlds->AddRef();
    }

    void NodeEventContextHost::UnregisterTravelDistributionSource( ITravelLinkedDistributionSource *tlds, TravelEventType type )
    {
        travel_distribution_source_map_t *sources = sourcesMapForType(type);        
        if (sources->find(tlds) != sources->end())
        {
            (*sources)[tlds]--;
            tlds->Release();

            if ((*sources)[tlds] == 0)
            {
                sources->erase(tlds);
            }
        } else
        {
            // unregistering something that wasnt registered...probably an error
        }
    }

    void NodeEventContextHost::RegisterObserver(
        IIndividualEventObserver * pObserver,
        const EventTrigger::Enum& trigger
    )
    { 
        broadcaster_impl.RegisterObserver( pObserver, trigger );
    }

    void NodeEventContextHost::UnregisterObserver(
        IIndividualEventObserver * pObserver,
        const EventTrigger::Enum& trigger
    )
    {
        //LOG_INFO( "[UnregisterIndividualEventObserver] Putting individual event observer into the disposed observers list .\n" );
        broadcaster_impl.UnregisterObserver( pObserver, trigger );
        //LOG_INFO( "[UnregisterIndividualEventObserver] Putting individual event observer into the disposed observers list .\n" );
    }

    void NodeEventContextHost::TriggerObservers(
        IIndividualHumanEventContext *pIndiv,
        const EventTrigger::Enum& trigger
    )
    {
        broadcaster_impl.TriggerObservers( pIndiv, trigger );
    }

    void NodeEventContextHost::PropagateContextToDependents()
    {
        for (auto intervention : node_interventions)
        {
            intervention->SetContextTo( this );
        }
    }

    void NodeEventContextHost::IncrementCampaignCost(float cost)
    {
        node->Campaign_Cost += cost;
    }

    void NodeEventContextHost::UpdateInterventions(float dt)
    {
        std::vector<INodeDistributableIntervention*> expired_list;
        for( auto intervention : node_interventions )
        {
            intervention->Update( dt );
            if( intervention->Expired() )
            {
                expired_list.push_back( intervention );
            }
        }

        for( auto intven : expired_list )
        {
            node_interventions.remove( intven );
            delete intven;
        }

        broadcaster_impl.DisposeOfUnregisteredObservers();
    }

    bool NodeEventContextHost::GiveIntervention( INodeDistributableIntervention* iv )
    {
        node_interventions.push_back( iv );
        iv->AddRef();

        IBaseIntervention * pBaseIV = nullptr;
        if( iv->QueryInterface( GET_IID(IBaseIntervention), (void**)&pBaseIV ) == s_OK ) 
        {
            IncrementCampaignCost( pBaseIV->GetCostPerUnit() );
            pBaseIV->Release();
        }
        else
        {
            LOG_WARN_F("Unsuccessful in querying for IBaseIntervention from INodeDistributableIntervention (%s) for passing Campaign_Cost back to Node.\n", typeid(*iv).name());
        }
        iv->SetContextTo( this );
        return true;
    }

    std::list<INodeDistributableIntervention*> NodeEventContextHost::GetInterventionsByType(const std::string& type_name)
    {
        std::list<INodeDistributableIntervention*> interventions_of_type;
        LOG_INFO_F("Looking for intervention of type %s", type_name.c_str());
        for (auto intervention : node_interventions)
        {
            std::string cur_iv_type_name = typeid( *intervention ).name();
            if( cur_iv_type_name == type_name )
            {
                LOG_INFO( "Found one..." );
                interventions_of_type.push_back( intervention );
            }
        }

        return interventions_of_type;
    }

    void NodeEventContextHost::PurgeExisting( const std::string& iv_name )
    {
        std::list<INodeDistributableIntervention*> iv_list = GetInterventionsByType(iv_name);

        for (auto iv_ptr : iv_list)
        {
            LOG_INFO_F("Found an existing intervention by that name (%s) which we are purging\n", iv_name.c_str());
            node_interventions.remove( iv_ptr );
            delete iv_ptr;
        }
    }

    NodeEventContextHost::~NodeEventContextHost()
    {
        cleanupDistributionSourceMap(arrival_distribution_sources);
        cleanupDistributionSourceMap(departure_distribution_sources);

        for (auto intervention : node_interventions)
        {
            delete intervention;
        }
    }

    NodeEventContextHost::travel_distribution_source_map_t* NodeEventContextHost::sourcesMapForType( INodeEventContext::TravelEventType type )
    {
        travel_distribution_source_map_t *sources = nullptr;
        switch (type)
        {
        case Arrival: sources = &arrival_distribution_sources; break;
        case Departure: sources = &departure_distribution_sources; break;
        default: break;
        }
        return sources;
    }

    void NodeEventContextHost::cleanupDistributionSourceMap( travel_distribution_source_map_t &sources )
    {
        for (auto& entry : sources)
        {
            for (int k = 0; k < entry.second; k++)
                entry.first->Release();
        }

        sources.clear();
    }

    void NodeEventContextHost::AddImportCases( const StrainIdentity* outbreak_strainID, float import_age, int num_cases_per_node, float female_prob, float mc_weight )
    {
        for (int i = 0; i < num_cases_per_node; i++)
        {
            // Add individual (init_prev = 0.0f; init_mod_acquire = 1.0f)
            IIndividualHuman* new_individual = node->configureAndAddNewIndividual(mc_weight, import_age, 0.0f, female_prob, 1.0f);

            // Start as infectious (incubation_period = 0)
            new_individual->AcquireNewInfection( outbreak_strainID, 0.0f );
        }
    }

    // These are all pass-throughs in order to get rid of GetNode() as an interface method.
    bool NodeEventContextHost::IsInPolygon(float* vertex_coords, int numcoords)
    {
        return node->IsInPolygon( vertex_coords, numcoords );
    }

    bool NodeEventContextHost::IsInPolygon( const json::Array &poly )
    {
        return node->IsInPolygon( poly );
    }

    bool NodeEventContextHost::IsInExternalIdSet( const std::list<ExternalNodeId_t>& nodelist )
    {
        return node->IsInExternalIdSet( nodelist );
    }

    const IdmDateTime& NodeEventContextHost::GetTime() const
    {
        return node->GetTime();
    }

    RANDOMBASE*  NodeEventContextHost::GetRng()
    {
        return node->GetRng();
    } 

    INodeContext* NodeEventContextHost::GetNodeContext()
    {
        return static_cast<INodeContext*>(node);
    }

    int NodeEventContextHost::GetIndividualHumanCount() const
    {
        size_t nodePop = node->individualHumans.size();
        return int(nodePop);
    }

    ExternalNodeId_t NodeEventContextHost::GetExternalId() const
    {
        ExternalNodeId_t nodeId = node->GetExternalID();
        return nodeId;
    }

    IIndividualEventBroadcaster* NodeEventContextHost::GetIndividualEventBroadcaster()
    {
        return this;
    }
}
