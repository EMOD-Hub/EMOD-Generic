/***************************************************************************************************

Copyright (c) 2018 Intellectual Ventures Property Holdings, LLC (IVPH) All rights reserved.

EMOD is licensed under the Creative Commons Attribution-Noncommercial-ShareAlike 4.0 License.
To view a copy of this license, visit https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode

***************************************************************************************************/

#include "stdafx.h"
#include "IncidenceCounterSurveillance.h"
#include "Exceptions.h"

SETUP_LOGGING("IncidenceCounterSurveillance")


namespace Kernel
{
    BEGIN_QUERY_INTERFACE_BODY( IncidenceCounterSurveillance )
    END_QUERY_INTERFACE_BODY( IncidenceCounterSurveillance )

    IncidenceCounterSurveillance::IncidenceCounterSurveillance()
        : IncidenceCounter()
        , m_CounterPeriod( 0 ) 
        , m_CounterPeriod_current( 0 )
        , m_CounterEventType( EventType::INDIVIDUAL )
        , m_TriggerConditionListIndividual()
        , m_TriggerConditionListNode()
        , m_TriggerConditionListCoordinator()
        , m_PercentageEventsToCountIndividual()
        , m_PercentageEventsToCountNode()
        , m_PercentageEventsToCountCoordinator()
        , m_PercentageEventsCounted( 0 )
    {
        initSimTypes(1, "*");
    }

    IncidenceCounterSurveillance::~IncidenceCounterSurveillance()
    {
    }

    void IncidenceCounterSurveillance::StartCounting()
    {
        m_CounterPeriod_current = m_CounterPeriod;
        m_Count = 0;
        m_PercentageEventsCounted = 0;
        LOG_INFO_F( "StartCounting(),  m_CounterPeriod_current = %f m_Count = %f\n", m_CounterPeriod_current, m_Count );
    }

    void IncidenceCounterSurveillance::Update( float dt )
    {
        m_CounterPeriod_current -= dt;
    }

    bool IncidenceCounterSurveillance::IsDoneCounting() const
    {
        return (m_CounterPeriod_current <= 0);
    }

    bool IncidenceCounterSurveillance::Configure( const Configuration * inputJson )
    {
        initConfig( "Counter_Event_Type", m_CounterEventType, inputJson, MetadataDescriptor::Enum("Counter_Event_Type", ICS_Counter_Event_Type_DESC_TEXT, MDD_ENUM_ARGS(EventType)) );
        initConfigTypeMap( "Counter_Period", &m_CounterPeriod, ICS_Counter_Period_DESC_TEXT, 1.0f, 1000.0f, 1.0f );        

        bool retValue = IncidenceCounter::Configure( inputJson );
        if( retValue && !JsonConfigurable::_dryrun )
        {
            m_CounterPeriod_current = m_CounterPeriod;
        }
        return retValue;
    }

    void IncidenceCounterSurveillance::ConfigureTriggers( const Configuration * inputJson )
    {
        initVectorConfig( "Trigger_Condition_List",
                          m_TriggerConditionList,
                          inputJson,
                          MetadataDescriptor::Enum(
                            "Trigger_Condition_List",
                            ICS_Trigger_Condition_List_DESC_TEXT ,
                            MDD_ENUM_ARGS(EventTrigger)));
    }

    void IncidenceCounterSurveillance::CheckConfigurationTriggers()
    {
        switch( m_CounterEventType )
        {
            case EventType::INDIVIDUAL:
            {
                for( auto elem : m_TriggerConditionList )
                {
                    m_TriggerConditionListIndividual.push_back( elem );
                }
                break;
            }
            case EventType::NODE:
            {
                for( auto elem : m_TriggerConditionList )
                {
                    m_TriggerConditionListNode.push_back( elem );
                }
                break;
            }
            case EventType::COORDINATOR:
            {
                for( auto elem : m_TriggerConditionList )
                {
                    m_TriggerConditionListCoordinator.push_back( elem );
                }
                break;
            }
            default:
                throw BadEnumInSwitchStatementException( __FILE__, __LINE__, __FUNCTION__, "Counter_Event_Type", m_CounterEventType, EventType::pairs::lookup_key( m_CounterEventType ) );
        }
    }

    void IncidenceCounterSurveillance::SetPercentageEventsToCount( const std::vector<std::string>& rPercentageEvents )
    {
        // TBD TBD: Need to add some for loops or std::copy's here.
        switch( m_CounterEventType )
        {
            case EventType::INDIVIDUAL:
            {
                //m_PercentageEventsToCountIndividual = EventTriggerFactory::GetInstance()->CreateTriggerList( "Percentage_Events_To_Count", rPercentageEvents );
                break;
            }
            case EventType::NODE:
            {
                //m_PercentageEventsToCountNode = EventTriggerNodeFactory::GetInstance()->CreateTriggerList( "Percentage_Events_To_Count", rPercentageEvents );
                break;
            }
            case EventType::COORDINATOR:
            {
                //m_PercentageEventsToCountCoordinator = EventTriggerCoordinatorFactory::GetInstance()->CreateTriggerList( "Percentage_Events_To_Count", rPercentageEvents );
                break;
            }
            default:
                throw BadEnumInSwitchStatementException( __FILE__, __LINE__, __FUNCTION__, "Counter_Event_Type", m_CounterEventType, EventType::pairs::lookup_key( m_CounterEventType ) );
        }
    }

    template<typename Trigger>
    bool Find( const std::vector<Trigger>& rList, const Trigger& rTrigger )
    {
        return std::find( rList.begin(), rList.end(), rTrigger ) != rList.end();
    }

    void IncidenceCounterSurveillance::RegisterForEvents( INodeEventContext* pNEC )
    {
        IncidenceCounter::RegisterForEvents( pNEC );
        for( EventTrigger::Enum& ect : m_PercentageEventsToCountIndividual )
        {
            auto eventAsString = EventTrigger::pairs::lookup_key( ect );
            if( std::find( m_TriggerConditionListIndividual.begin(), m_TriggerConditionListIndividual.end(), ect ) != m_TriggerConditionList.end() )
            {
                pNEC->GetIndividualEventBroadcaster()->RegisterObserver( this, ect );
                LOG_INFO_F( "Registered Percentage_Events_To_Count: %s\n", eventAsString.c_str()  );
            }
        }
    }

    void IncidenceCounterSurveillance::UnregisterForEvents( INodeEventContext* pNEC )
    {
        IncidenceCounter::UnregisterForEvents( pNEC );
        for( EventTrigger::Enum& ect : m_PercentageEventsToCountIndividual )
        {
            auto eventAsString = EventTrigger::pairs::lookup_key( ect );
            if( std::find( m_TriggerConditionListIndividual.begin(), m_TriggerConditionListIndividual.end(), ect ) != m_TriggerConditionList.end() )
            {
                pNEC->GetIndividualEventBroadcaster()->UnregisterObserver( this, ect );
                LOG_INFO_F( "Unregistered Percentage_Events_To_Count: %s\n", EventTrigger::pairs::lookup_key( ect ).c_str() );
            }
        }
    }

    void IncidenceCounterSurveillance::RegisterForEvents( ISimulationEventContext* context )
    {
        switch( m_CounterEventType )
        {
            case EventType::NODE:
            {
                for( EventTrigger::Enum& ect : m_TriggerConditionListNode )
                {
                    context->GetNodeEventBroadcaster()->RegisterObserver( this, ect );
                    LOG_INFO_F( "Registered Start_Trigger: %s\n", EventTrigger::pairs::lookup_key( ect ).c_str() );
                }
                for( EventTrigger::Enum& ect : m_PercentageEventsToCountNode )
                {
                    if( !Find( m_TriggerConditionListNode, ect ) )
                    {
                        context->GetNodeEventBroadcaster()->RegisterObserver( this, ect );
                        LOG_INFO_F( "Registered Percentage_Events_To_Count: %s\n", EventTrigger::pairs::lookup_key( ect ).c_str() );
                    }
                }
                break;
            }
            case EventType::COORDINATOR:
            {
                for( EventTriggerCoordinator& ect : m_TriggerConditionListCoordinator )
                {
                    context->GetCoordinatorEventBroadcaster()->RegisterObserver( this, ect );
                    LOG_INFO_F( "Registered Start_Trigger: %s\n", EventTrigger::pairs::lookup_key( ect ).c_str() );
                }
                for( EventTriggerCoordinator& ect : m_PercentageEventsToCountCoordinator )
                {
                    if( !Find( m_TriggerConditionListCoordinator, ect ) )
                    {
                        context->GetCoordinatorEventBroadcaster()->RegisterObserver( this, ect );
                        LOG_INFO_F( "Registered Percentage_Events_To_Count: %s\n", EventTrigger::pairs::lookup_key( ect ).c_str() );
                    }
                }
                break;
            }
            // no default here 
         }
    }

    void IncidenceCounterSurveillance::UnregisterForEvents( ISimulationEventContext* context )
    {
        switch( m_CounterEventType )
        {
            case   EventType::NODE:
            {
                for( EventTriggerNode& ect : m_TriggerConditionListNode )
                {
                    context->GetNodeEventBroadcaster()->UnregisterObserver( this, ect );
                    LOG_INFO_F( "Unregister Trigger: %s\n", EventTrigger::pairs::lookup_key( ect ).c_str() );
                }
                for( EventTriggerNode& ect : m_PercentageEventsToCountNode )
                {
                    if( !Find( m_TriggerConditionListNode, ect ) )
                    {
                        context->GetNodeEventBroadcaster()->UnregisterObserver( this, ect );
                        LOG_INFO_F( "Unregister Percentage_Events_To_Count: %s\n", EventTrigger::pairs::lookup_key( ect ).c_str() );
                    }
                }
                break;
            }
            case EventType::COORDINATOR:
            {
                for( EventTriggerCoordinator& ect : m_TriggerConditionListCoordinator )
                {
                    context->GetCoordinatorEventBroadcaster()->UnregisterObserver( this, ect );
                    //LOG_INFO_F( "Unregister Trigger: %s\n", ect.c_str() );
                    LOG_INFO_F( "Unregister Trigger: %s\n", EventTrigger::pairs::lookup_key( ect ).c_str() );
                }
                for( EventTriggerCoordinator& ect : m_PercentageEventsToCountCoordinator )
                {
                    if( !Find( m_TriggerConditionListCoordinator, ect ) )
                    {
                        context->GetCoordinatorEventBroadcaster()->UnregisterObserver( this, ect );
                        LOG_INFO_F( "Unregister Percentage_Events_To_Count: %s\n", EventTrigger::pairs::lookup_key( ect ).c_str() );
                    }
                }
                break;
            }
            // no default here 
        }
    }

    bool IncidenceCounterSurveillance::notifyOnEvent( IIndividualHumanEventContext *pEntity, const EventTrigger::Enum& trigger )
    {
        LOG_INFO_F( "notifyOnEvent received: %s\n", EventTrigger::pairs::lookup_key( trigger ).c_str() );
        if ( m_NodePropertyRestrictions.Qualifies(pEntity->GetNodeEventContext()->GetNodeContext()->GetNodeProperties() ) &&
            m_DemographicRestrictions.IsQualified(pEntity) &&
            !IsDoneCounting() )
        {           
            if( Find( m_PercentageEventsToCountIndividual, trigger ) )
            {
                ++m_PercentageEventsCounted;
            }
            auto eventAsStr = EventTrigger::pairs::lookup_key( trigger ); // tcl is strings instead of enums right now. TBD.
            if( std::find( m_TriggerConditionList.begin(), m_TriggerConditionList.end(), trigger ) != m_TriggerConditionList.end() )
            {
                ++m_Count;
                LOG_INFO_F( "notifyOnEvent received: %s   m_Count: %d\n", eventAsStr.c_str(), m_Count );
            }
        }
        return true;
    }

    bool IncidenceCounterSurveillance::notifyOnEvent( INodeEventContext *pEntity, const EventTrigger::Enum& trigger )
    {
        LOG_INFO_F( "notifyOnEvent received: %s\n", EventTrigger::pairs::lookup_key( trigger ).c_str() );
        if ( m_NodePropertyRestrictions.Qualifies( pEntity->GetNodeContext()->GetNodeProperties() ) &&
            !IsDoneCounting() )
        {
            if( Find( m_PercentageEventsToCountNode, trigger ) )
            {
                ++m_PercentageEventsCounted;
            }
            if( Find( m_TriggerConditionListNode, trigger ) )
            {
                ++m_Count;
                LOG_INFO_F( "notifyOnEvent received: %s   m_Count: %d\n", EventTrigger::pairs::lookup_key( trigger ).c_str(), m_Count );
            }
        }
        return true;
    }

    bool IncidenceCounterSurveillance::notifyOnEvent( IEventCoordinatorEventContext *pEntity, const EventTrigger::Enum& trigger )
    {
        LOG_INFO_F(" notifyOnEvent received: %s,  %s\n", pEntity->GetName().c_str(), EventTrigger::pairs::lookup_key( trigger ).c_str());
        if( Find( m_PercentageEventsToCountCoordinator, trigger ) )
        {
            ++m_PercentageEventsCounted;
        }
        if( Find( m_TriggerConditionListCoordinator, trigger ) )
        {
            ++m_Count;
            LOG_INFO_F( "notifyOnEvent received: %s   m_Count: %d\n", EventTrigger::pairs::lookup_key( trigger ).c_str(), m_Count );
        }
        return true;
    }

    uint32_t IncidenceCounterSurveillance::GetCountOfQualifyingPopulation( const std::vector<INodeEventContext*>& rNodes )
    {
        if( (m_PercentageEventsToCountIndividual.size()  != 0) ||
            (m_PercentageEventsToCountNode.size()        != 0) ||
            (m_PercentageEventsToCountCoordinator.size() != 0) )
        {
            return m_PercentageEventsCounted;
        }

        //Took Dan's methood from https://github.com/Bridenbecker/DtkTrunk/pull/10#discussion_r171112313
        // Not finished/tested yet
        int count = 0;
        switch( m_CounterEventType )
        {
            case EventType::COORDINATOR:
            {
                count = 1;
                break;
            }
            case EventType::NODE:
            {
                for( INodeEventContext* const& inec : rNodes )
                {
                    if( m_NodePropertyRestrictions.Qualifies( inec->GetNodeContext()->GetNodeProperties() ) )
                    {
                        ++count;
                    }
                }
                break;
            }
            case EventType::INDIVIDUAL:
            {
                count = IncidenceCounter::GetCountOfQualifyingPopulation( rNodes );
                break;
            }
            default:
                throw BadEnumInSwitchStatementException( __FILE__, __LINE__, __FUNCTION__, "Counter_Event_Type", m_CounterEventType, EventType::pairs::lookup_key( m_CounterEventType ) );
        }
        return count;
    }
}
