/***************************************************************************************************

Copyright (c) 2018 Intellectual Ventures Property Holdings, LLC (IVPH) All rights reserved.

EMOD is licensed under the Creative Commons Attribution-Noncommercial-ShareAlike 4.0 License.
To view a copy of this license, visit https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode

***************************************************************************************************/

#include "stdafx.h"
#ifndef WIN32
#include <cxxabi.h>
#endif

#include "BroadcastCoordinatorEvent.h"
#include "Log.h"
#include "NodeEventContext.h"
#include "SimulationEventContext.h"

SETUP_LOGGING( "BroadcastCoordinatorEvent" )

namespace Kernel
{
    IMPLEMENT_FACTORY_REGISTERED( BroadcastCoordinatorEvent )

    BroadcastCoordinatorEvent::BroadcastCoordinatorEvent()
        : JsonConfigurable()
        , m_Parent( nullptr )
        , m_EventToBroadcast()
        , m_CampaignCost( 0.0f )
        , m_pNodeForCampaignCost( nullptr )
    {
    }

    BroadcastCoordinatorEvent::~BroadcastCoordinatorEvent()
    {
    }

    QuickBuilder BroadcastCoordinatorEvent::GetSchema()
    {
        return QuickBuilder( jsonSchemaBase );
    }

    bool BroadcastCoordinatorEvent::Configure( const Configuration * inputJson )
    {
        // ------------------------------------------------------------------
        // --- Must calculate default name in Configure(). You can't do it
        // --- in the constructor because the pointer doesn't know what object
        // --- it is yet.
        // ------------------------------------------------------------------
        m_CoordinatorName = typeid(*this).name();
#ifdef WIN32
        m_CoordinatorName = m_CoordinatorName.substr( 14 ); // remove "class Kernel::"
#else
        m_CoordinatorName = abi::__cxa_demangle( m_CoordinatorName.c_str(), 0, 0, nullptr );
        m_CoordinatorName = m_CoordinatorName.substr( 8 ); // remove "Kernel::"
#endif
        std::string default_name = m_CoordinatorName;

        initConfigTypeMap( "Coordinator_Name", &m_CoordinatorName,  BCE_Coordinator_Name_DESC_TEXT, default_name );
        initConfigTypeMap( "Cost_To_Consumer", &m_CampaignCost,     BCE_Cost_To_Consumer_DESC_TEXT, 0.0f, FLT_MAX, 0.0f );
        initConfig( "Broadcast_Event", m_EventToBroadcast, inputJson, MetadataDescriptor::Enum("Broadcast_Event", HIV_Broadcast_Event_DESC_TEXT, MDD_ENUM_ARGS( EventTrigger ) ) );

        bool retValue = JsonConfigurable::Configure( inputJson );

        if( retValue && !JsonConfigurable::_dryrun )
        {
            if( m_EventToBroadcast == EventTrigger::NoTrigger )
            {
                throw GeneralConfigurationException( __FILE__, __LINE__, __FUNCTION__, "Broadcast_Event must be defined and it cannot be empty");
            }
        }

        return retValue;
    }

    void BroadcastCoordinatorEvent::CheckStartDay( float campaignStartDay ) const
    { }

    void BroadcastCoordinatorEvent::SetContextTo( ISimulationEventContext *isec )
    {
        m_Parent = isec;
    }

    void BroadcastCoordinatorEvent::AddNode( const suids::suid& node_suid )
    {
        if( m_pNodeForCampaignCost == nullptr )
        {
            m_pNodeForCampaignCost = m_Parent->GetNodeEventContext( node_suid );
        }
    }

    // Update() is called for all the coordinators before calling UpdateNodes()
    void BroadcastCoordinatorEvent::Update( float dt )
    {
        m_Parent->GetCoordinatorEventBroadcaster()->TriggerObservers( this, m_EventToBroadcast );

        if( m_pNodeForCampaignCost && m_CampaignCost )
        {
            m_pNodeForCampaignCost->GetCampaignCostObserver()->notifyCampaignExpenseIncurred( m_CampaignCost, nullptr );
        }
    }

    void BroadcastCoordinatorEvent::UpdateNodes( float dt )
    {
        // do nothing
    }

    bool BroadcastCoordinatorEvent::IsFinished()
    {
        return true;
    }

    const std::string& BroadcastCoordinatorEvent::GetName() const
    {
        return m_CoordinatorName;
    }

    const IdmDateTime& BroadcastCoordinatorEvent::GetTime() const
    {
        return m_Parent->GetSimulationTime();
    }
}

