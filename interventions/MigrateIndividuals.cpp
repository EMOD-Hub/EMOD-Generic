/***************************************************************************************************

Copyright (c) 2018 Intellectual Ventures Property Holdings, LLC (IVPH) All rights reserved.

EMOD is licensed under the Creative Commons Attribution-Noncommercial-ShareAlike 4.0 License.
To view a copy of this license, visit https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode

***************************************************************************************************/

#include "stdafx.h"
#include "ConfigParams.h"
#include "MigrateIndividuals.h"
#include "IMigrate.h"
#include "IIndividualHuman.h"
#include "IIndividualHumanContext.h"
#include "NodeEventContext.h"  // for INodeEventContext (ICampaignCostObserver)
#include "IMigrationInfo.h"
#include "INodeContext.h"
#include "ISimulationContext.h"
#include "DistributionFactory.h"

SETUP_LOGGING( "MigrateIndividuals" )

namespace Kernel
{
    BEGIN_QUERY_INTERFACE_BODY(MigrateIndividuals)
        HANDLE_INTERFACE(IConfigurable)
        HANDLE_INTERFACE(IDistributableIntervention)
        HANDLE_INTERFACE(IBaseIntervention)
        HANDLE_ISUPPORTS_VIA(IDistributableIntervention)
    END_QUERY_INTERFACE_BODY(MigrateIndividuals)

    IMPLEMENT_FACTORY_REGISTERED(MigrateIndividuals)

    bool MigrateIndividuals::Configure( const Configuration * inputJson )
    {
        DistributionFunction::Enum before_leaving_function(DistributionFunction::NOT_INITIALIZED);
        initConfig( "Duration_Before_Leaving_Distribution", before_leaving_function, inputJson, MetadataDescriptor::Enum( "Duration_Before_Leaving_Distribution_Type", Duration_Before_Leaving_Distribution_DESC_TEXT, MDD_ENUM_ARGS(DistributionFunction) ) );
        duration_before_leaving = DistributionFactory::CreateDistribution( this, before_leaving_function, "Duration_Before_Leaving", inputJson );

        DistributionFunction::Enum at_node_function(DistributionFunction::NOT_INITIALIZED);
        initConfig( "Duration_At_Node_Distribution", at_node_function, inputJson, MetadataDescriptor::Enum( "Duration_At_Node_Distribution_Type", Duration_At_Node_Distribution_DESC_TEXT, MDD_ENUM_ARGS(DistributionFunction) ) );

        duration_at_node = DistributionFactory::CreateDistribution( this, at_node_function, "Duration_At_Node", inputJson );

        initConfigTypeMap( "NodeID_To_Migrate_To", &destination_external_node_id, NodeID_To_Migrate_To_DESC_TEXT, 0, UINT_MAX, 0 );
        initConfigTypeMap( "Is_Moving", &is_moving, Is_Moving_DESC_TEXT, false );

        return BaseIntervention::Configure( inputJson );
    }

    MigrateIndividuals::MigrateIndividuals()
        : BaseIntervention()
        , destination_external_node_id( 0 )
        , duration_before_leaving( nullptr )
        , duration_at_node( nullptr )
        , is_moving( false )
    { }

    MigrateIndividuals::MigrateIndividuals( const MigrateIndividuals& master )
        : BaseIntervention( master )
        , destination_external_node_id( master.destination_external_node_id )
        , duration_before_leaving( master.duration_before_leaving->Clone() )
        , duration_at_node( master.duration_at_node->Clone() )
        , is_moving( master.is_moving )
    { }

    MigrateIndividuals::~MigrateIndividuals()
    {
       delete duration_before_leaving;
       delete duration_at_node;
    }

    bool MigrateIndividuals::Distribute(IIndividualHumanInterventionsContext *context, ICampaignCostObserver * const pICCO )
    {
        IMigrationInfo* p_minfo = context->GetParent()->GetParent()->GetMigrationInfo();
        if( !p_minfo ||  p_minfo->GetParams()->migration_structure == MigrationStructure::NO_MIGRATION )
        {
            std::stringstream msg;
            msg << _module << " cannot be used when 'Migration_Model' = 'NO_MIGRATION'.";
            throw GeneralConfigurationException( __FILE__, __LINE__, __FUNCTION__, msg.str().c_str() );
        }

        return BaseIntervention::Distribute( context, pICCO );
    }

    void MigrateIndividuals::Update( float dt )
    {
        if( !BaseIntervention::UpdateIndividualsInterventionStatus() ) return;

        // expire the intervention
        expired = true;

        INodeContext* p_node_context = parent->GetEventContext()->GetNodeEventContext()->GetNodeContext();

        suids::suid destination_id = p_node_context->GetParent()->GetNodeSuid( destination_external_node_id );

        float duration_before = duration_before_leaving->Calculate( parent->GetRng() );
        float duration_at = duration_at_node->Calculate( parent->GetRng() );

        IMigrate* im = parent->GetIndividual()->GetIMigrate();
        im->SetMigrating( destination_id, MigrationType::INTERVENTION_MIGRATION, duration_before, duration_at, is_moving );
    }

    REGISTER_SERIALIZABLE(MigrateIndividuals);

    void MigrateIndividuals::serialize(IArchive& ar, MigrateIndividuals* obj)
    {
        BaseIntervention::serialize( ar, obj );
        MigrateIndividuals& mt = *obj;
        ar.labelElement("destination_external_node_id") & mt.destination_external_node_id;
        ar.labelElement("duration_before_leaving"     ) & mt.duration_before_leaving;
        ar.labelElement("duration_at_node"            ) & mt.duration_at_node;
        ar.labelElement("is_moving"                   ) & mt.is_moving;
    }
}
