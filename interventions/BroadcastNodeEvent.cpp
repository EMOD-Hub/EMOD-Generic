/***************************************************************************************************

Copyright (c) 2018 Intellectual Ventures Property Holdings, LLC (IVPH) All rights reserved.

EMOD is licensed under the Creative Commons Attribution-Noncommercial-ShareAlike 4.0 License.
To view a copy of this license, visit https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode

***************************************************************************************************/

#include "stdafx.h"
#include "BroadcastNodeEvent.h"
#include "NodeEventContext.h"
#include "SimulationEventContext.h"
#include "ISimulationContext.h"

SETUP_LOGGING( "BroadcastNodeEvent" )

namespace Kernel
{
    IMPLEMENT_FACTORY_REGISTERED( BroadcastNodeEvent )

    BroadcastNodeEvent::BroadcastNodeEvent()
        : BaseNodeIntervention()
        , m_EventToBroadcast()
    {
    }

    BroadcastNodeEvent::BroadcastNodeEvent( const BroadcastNodeEvent& master )
        : BaseNodeIntervention( master )
        , m_EventToBroadcast( master.m_EventToBroadcast )
    {
    }

    BroadcastNodeEvent::~BroadcastNodeEvent()
    {
    }

    bool BroadcastNodeEvent::Configure( const Configuration * inputJson )
    {
        //initConfigTypeMap( "Broadcast_Event", &m_EventToBroadcast, BNE_Broadcast_Event_DESC_TEXT );
        initConfig( "Broadcast_Event", m_EventToBroadcast, inputJson, MetadataDescriptor::Enum("Broadcast_Event", HIV_Broadcast_Event_DESC_TEXT, MDD_ENUM_ARGS( EventTrigger ) ) );
        initConfigTypeMap( "Cost_To_Consumer", &cost_per_unit, BNE_Cost_To_Consumer_DESC_TEXT, 0.0f, 999999, 0.0f );

        bool retValue = JsonConfigurable::Configure( inputJson );

        //if( retValue && !JsonConfigurable::_dryrun ) {
        if( !JsonConfigurable::_dryrun && m_EventToBroadcast == EventTrigger::NoTrigger )
        {
            std::stringstream ss;
            ss << "BroadcastNodeEvent was configured with empty (or uninitialized) Broadcast_Event.\n";
            throw InvalidInputDataException( __FILE__, __LINE__, __FUNCTION__, ss.str().c_str() );
        }

        return retValue;
    }

    void BroadcastNodeEvent::Update( float dt )
    {
        if( !BaseNodeIntervention::UpdateNodesInterventionStatus() ) return;

        INodeEventBroadcaster* broadcaster = parent->GetNodeContext()->GetParent()->GetSimulationEventContext()->GetNodeEventBroadcaster();
        broadcaster->TriggerObservers( parent, m_EventToBroadcast );

        expired = true;
    }

    // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    // !!! TODO - can't do until BaseNodeIntervention is serializable
    // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    //REGISTER_SERIALIZABLE(BroadcastNodeEvent);

    //void BroadcastNodeEvent::serialize(IArchive& ar, BroadcastNodeEvent* obj)
    //{
    //    BaseNodeIntervention::serialize( ar, obj );
    //    BroadcastNodeEvent& mf = *obj;
    //    ar.labelElement("m_EventToBroadcast") & mf.m_EventToBroadcast;
    //    ar.labelElement("cost_per_unit"     ) & mf.cost_per_unit;
    //}
}
