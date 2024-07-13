/***************************************************************************************************

Copyright (c) 2018 Intellectual Ventures Property Holdings, LLC (IVPH) All rights reserved.

EMOD is licensed under the Creative Commons Attribution-Noncommercial-ShareAlike 4.0 License.
To view a copy of this license, visit https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode

***************************************************************************************************/

#include "stdafx.h"
#include "TriggeredEventCoordinator.h"
#include "SimulationEventContext.h"

SETUP_LOGGING( "TriggeredEventCoordinator" )

namespace Kernel
{
    IMPLEMENT_FACTORY_REGISTERED( TriggeredEventCoordinator )
    IMPL_QUERY_INTERFACE2( TriggeredEventCoordinator, IEventCoordinator, IConfigurable )

    TriggeredEventCoordinator::TriggeredEventCoordinator()
        : StandardInterventionDistributionEventCoordinator( false )//false=don't use standard demographic coverage
        , m_IsActive( false )
        , m_IsStarting( false )
        , m_IsStopping( false )
        , m_Duration( -1.0f )
        , m_DurationExpired( false )
        , m_CoordinatorName( "TriggeredEventCoordinator" )
        , m_CompletionEvent()
        , m_InputNumRepetitions(0)
    {
        initSimTypes( 1, "*" );
    }
    TriggeredEventCoordinator::~TriggeredEventCoordinator()
    {
        //not called
    }

    bool TriggeredEventCoordinator::Configure( const Configuration * inputJson )
    {
        initVectorConfig( "Start_Trigger_Condition_List",
                          m_StartTriggerConditionList,
                          inputJson,
                          MetadataDescriptor::Enum(
                            "Start_Trigger_Condition_List",
                            TEC_Start_Trigger_Condition_List_DESC_TEXT ,
                            MDD_ENUM_ARGS(EventTrigger) ) );
        initVectorConfig( "Stop_Trigger_Condition_List",
                          m_StopTriggerConditionList,
                          inputJson,
                          MetadataDescriptor::Enum(
                            "Stop_Trigger_Condition_List",
                            TEC_Stop_Trigger_Condition_List_DESC_TEXT ,
                            MDD_ENUM_ARGS(EventTrigger)));

        initConfig( "Completion_Event", m_CompletionEvent, inputJson, MetadataDescriptor::Enum("Completion_Event", TEC_Completion_Event_DESC_TEXT, MDD_ENUM_ARGS( EventTrigger ) ) );
        initConfigTypeMap( "Coordinator_Name", &m_CoordinatorName, TEC_Coordinator_Name_DESC_TEXT, "TriggeredEventCoordinator" );
        initConfigTypeMap( "Duration", &m_Duration, TEC_Duration_DESC_TEXT, -1.0f, FLT_MAX, -1.0f );

        bool retValue = StandardInterventionDistributionEventCoordinator::Configure( inputJson );

        if( retValue && !JsonConfigurable::_dryrun )
        {
            CheckConfigTriggers(inputJson);
            m_InputNumRepetitions = num_repetitions;
        }

        return retValue;
    }

    void TriggeredEventCoordinator::CheckConfigTriggers( const Configuration * inputJson )
    {
        if( m_CompletionEvent == EventTrigger::NoTrigger )
        {
            LOG_WARN( "Completion_Event is not defined which is ok, but event will not be broadcasted.\n" );
        }
        if( m_StartTriggerConditionList.empty() )
        {
            std::stringstream ss;
            ss << "Start_Trigger_Condition_List of " << GetName() << " is empty, there is thus no way to start this TriggeredEventCoordinator.\n";
            throw InitializationException( __FILE__, __LINE__, __FUNCTION__, ss.str().c_str() );
        }
    }

    void TriggeredEventCoordinator::Update( float dt )
    {        
        if( m_Duration != -1.0f )
        {
            if( m_Duration <= 0 )
            {
                m_DurationExpired = true;
            }
            else
            {
                m_Duration -= dt;
            }
        }
        if( m_IsStarting )
        {
            m_IsStarting = false;
            m_IsActive = true;
            num_repetitions = m_InputNumRepetitions;
            tsteps_since_last = tsteps_between_reps-1;
            distribution_complete = false;
        }
        if( m_IsStopping )
        {
            m_IsStopping = false;
            m_IsActive = false;
        }

        if ( !m_IsActive ) return;
        StandardInterventionDistributionEventCoordinator::Update( dt );
    }

    void TriggeredEventCoordinator::UpdateNodes( float dt )
    {       
        if( !m_IsActive ) return;

        //Finsish current repetition, then stop
        if( IsTimeToUpdate(dt) && m_IsStopping )
        {
            m_IsActive = false;
        }
        StandardInterventionDistributionEventCoordinator::UpdateNodes( dt );
    }

    void TriggeredEventCoordinator::SetContextTo( ISimulationEventContext *isec )
    {
        parent = isec;
        Register();
    }

    void TriggeredEventCoordinator::Register()
    {
        for ( EventTriggerCoordinator& etc : m_StartTriggerConditionList )
        {
            parent->GetCoordinatorEventBroadcaster()->RegisterObserver( this, etc );
            LOG_INFO_F( "%s: registered Start_Trigger: %s\n", m_CoordinatorName.c_str(), EventTrigger::pairs::lookup_key( etc ).c_str() );
        }
        for ( EventTriggerCoordinator& etc : m_StopTriggerConditionList )
        {
            parent->GetCoordinatorEventBroadcaster()->RegisterObserver( this, etc );
            LOG_INFO_F( "%s: registered Stop_Trigger: %s\n", m_CoordinatorName.c_str(), EventTrigger::pairs::lookup_key( etc ).c_str() );
        }
    }

    void TriggeredEventCoordinator::UpdateRepetitions()
    {
        StandardInterventionDistributionEventCoordinator::UpdateRepetitions();
        if ( num_repetitions == 0 )
        {
            BroadcastCompletionEvent();
        }        
    }

    bool TriggeredEventCoordinator::notifyOnEvent( IEventCoordinatorEventContext *pEntity, const EventTrigger::Enum& trigger )
    {
        LOG_INFO_F( "%s: notifyOnEvent received: %s\n", m_CoordinatorName.c_str(), EventTrigger::pairs::lookup_key( trigger ).c_str());
        auto it_start = find( m_StartTriggerConditionList.begin(), m_StartTriggerConditionList.end(), trigger );
        if( it_start != m_StartTriggerConditionList.end() )
        {
            LOG_INFO_F( "%s: notifyOnEvent received start: %s\n", m_CoordinatorName.c_str(), EventTrigger::pairs::lookup_key( trigger ).c_str() );
            m_IsStarting = true;
            m_IsStopping = false;
        }
        auto it_stop = find( m_StopTriggerConditionList.begin(), m_StopTriggerConditionList.end(), trigger );
        if( it_stop != m_StopTriggerConditionList.end() )
        {
            LOG_INFO_F( "%s: notifyOnEvent received stop: %s\n", m_CoordinatorName.c_str(), EventTrigger::pairs::lookup_key( trigger ).c_str() );
            m_IsStarting = false;
            m_IsStopping = true;
        }
        return true;
    }

    void TriggeredEventCoordinator::Unregister()
    {
        for( EventTriggerCoordinator& etc : m_StartTriggerConditionList )
        {
            parent->GetCoordinatorEventBroadcaster()->UnregisterObserver( this, etc );
            LOG_INFO_F( "%s: Unregistered Start_Trigger: %s\n", m_CoordinatorName.c_str(), EventTrigger::pairs::lookup_key( etc ).c_str() );
        }
        for( EventTriggerCoordinator& etc: m_StopTriggerConditionList )
        {
            parent->GetCoordinatorEventBroadcaster()->UnregisterObserver( this, etc );
            LOG_INFO_F( "%s: Unregistered Stop_Trigger: %s\n", m_CoordinatorName.c_str(), EventTrigger::pairs::lookup_key( etc ).c_str() );
        }       
    }
  
    void TriggeredEventCoordinator::BroadcastCompletionEvent()
    {
        parent->GetCoordinatorEventBroadcaster()->TriggerObservers( this, m_CompletionEvent );
    }

    const std::string& TriggeredEventCoordinator::GetName() const
    {
        return m_CoordinatorName;
    }

    const IdmDateTime& TriggeredEventCoordinator::GetTime() const
    {
        return parent->GetSimulationTime();
    }

    bool TriggeredEventCoordinator::IsFinished()
    {
        if( m_DurationExpired )
        {
            Unregister();
        }
        return m_DurationExpired;
    }
}
