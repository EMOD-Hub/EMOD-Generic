/***************************************************************************************************

Copyright (c) 2018 Intellectual Ventures Property Holdings, LLC (IVPH) All rights reserved.

EMOD is licensed under the Creative Commons Attribution-Noncommercial-ShareAlike 4.0 License.
To view a copy of this license, visit https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode

***************************************************************************************************/

#pragma once

#include "EventTrigger.h"
#include "BaseEventTrigger.h"
#include "EventCoordinator.h"
#include "Configure.h"
#include "DemographicRestrictions.h"
#include "Interventions.h"
#include "EventTrigger.h"
#include "NodeEventContext.h"
#include "JsonConfigurableCollection.h"
#include "EventTriggerNode.h"
#include "EventTriggerCoordinator.h"
#include "ReportStatsByIP.h"


namespace Kernel
{
    struct INodeEventContext;
    struct IIndividualHumanEventContext;

    ENUM_DEFINE( ThresholdType,
        ENUM_VALUE_SPEC( COUNT,             1 )
        ENUM_VALUE_SPEC( PERCENTAGE,        2 )
        ENUM_VALUE_SPEC( PERCENTAGE_EVENTS, 3 ) )

    // ------------------------------------------------------------------------
    // --- Action
    // ------------------------------------------------------------------------

    class Action : public JsonConfigurable
    {
    public:
        IMPLEMENT_NO_REFERENCE_COUNTING()
        DECLARE_QUERY_INTERFACE()

        Action();
        virtual ~Action();

        // JsonConfigurable methods
        virtual bool Configure( const Configuration * inputJson ) override;
        void CheckConfigurationTriggers();
       

        // Other methods
        float GetThreshold() const;

        //const std::string& GetEventToBroadcast() const;
        const EventTrigger::Enum& GetEventToBroadcast() const;
        
        EventType::Enum GetEventType() { return m_EventType; };
        EventTrigger::Enum& GetEventToBroadcastCoordinator() { return m_TriggerCoordinator; };
        EventTrigger::Enum& GetEventToBroadcastNode() { return m_TriggerNode; };
        EventTrigger::Enum& GetEventToBroadcastIndividual() { return m_TriggerIndividual; };


    private:
        float        m_Threshold;
        EventTrigger::Enum  m_EventToBroadcast;
        EventType::Enum  m_EventType;
        EventTrigger::Enum m_TriggerCoordinator;
        EventTrigger::Enum m_TriggerNode;
        EventTrigger::Enum m_TriggerIndividual;
    };

    // ------------------------------------------------------------------------
    // --- ActionList
    // ------------------------------------------------------------------------

    class ActionList : public JsonConfigurableCollection<Action>
    {
    public:
        ActionList();
        virtual ~ActionList();

        virtual void CheckConfiguration() override;

    protected:
        virtual Action* CreateObject() override;
    };

    // ------------------------------------------------------------------------
    // --- Responder
    // ------------------------------------------------------------------------

    class Responder : public JsonConfigurable, public IVisitIndividual
    {
    public:
        IMPLEMENT_NO_REFERENCE_COUNTING()
        DECLARE_QUERY_INTERFACE()

        Responder();
        virtual ~Responder();

        // JsonConfigurable methods
        virtual bool Configure( const Configuration * inputJson ) override;
        virtual void CheckConfiguration( const Configuration * inputJson );

        // IVisitIndividual methods
        virtual bool visitIndividualCallback( IIndividualHumanEventContext *ihec, float & incrementalCostOut, ICampaignCostObserver * pICCO );

        // Other methods
        virtual bool Distribute( const std::vector<INodeEventContext*>& nodes, uint32_t numIncidences, uint32_t qualifyingPopulation );
        void SetContextTo(ISimulationEventContext* sim, IEventCoordinatorEventContext *isec) { m_sim = sim; m_Parent = isec; };

        const Action* GetCurrentAction() const;

    protected:
        float CalculateIncidence( uint32_t numIncidences, uint32_t qualifyingPopulation ) const;
        Action* GetAction( float value );

        ThresholdType::Enum m_ThresholdType;
        ActionList m_ActionList;
        Action* m_pCurrentAction;
        ISimulationEventContext* m_sim;
        IEventCoordinatorEventContext* m_Parent;
    };

    // ------------------------------------------------------------------------
    // --- IncidenceCounter
    // ------------------------------------------------------------------------

    class IncidenceCounter : public JsonConfigurable, public IIndividualEventObserver
    {
    public:
        IMPLEMENT_NO_REFERENCE_COUNTING()
        DECLARE_QUERY_INTERFACE()

        IncidenceCounter();
        virtual ~IncidenceCounter();

        virtual bool Configure( const Configuration * inputJson ) override;
        virtual void ConfigureTriggers( const Configuration * inputJson );
        virtual void CheckConfigurationTriggers();

        // IIndividualEventObserver methods
        virtual bool notifyOnEvent( IIndividualHumanEventContext *context,
                                    const EventTrigger::Enum& StateChange ) override;

        // Other methods
        uint32_t GetCount() const;
        virtual uint32_t GetCountOfQualifyingPopulation( const std::vector<INodeEventContext*>& rNodes );
        virtual void StartCounting();
        virtual void Update( float dt );
        virtual bool IsDoneCounting() const;
        virtual void RegisterForEvents( ISimulationEventContext* pSEC );
        virtual void UnregisterForEvents( ISimulationEventContext* pSEC );
        virtual void RegisterForEvents( INodeEventContext* pNEC );
        virtual void UnregisterForEvents( INodeEventContext* pNEC );
        int32_t GetCountEventsForNumTimeSteps() const;

        // for ReportStatsByIP
        bool IsNodeQualified( INodeEventContext* pNEC );
        individual_qualified_function_t GetIndividualQualifiedFunction();

    protected:
        uint32_t m_Count;
        PropertyRestrictions<NPKey, NPKeyValue, NPKeyValueContainer> m_NodePropertyRestrictions;
        DemographicRestrictions   m_DemographicRestrictions;
        std::vector<EventTrigger::Enum> m_TriggerConditionList;

    private:
        int32_t m_CountEventsForNumTimeSteps;
        int32_t m_NumTimeStepsCounted;
        bool m_IsDoneCounting;

    };

    // ------------------------------------------------------------------------
    // --- IncidenceEventCoordinator
    // ------------------------------------------------------------------------

    class IncidenceEventCoordinator : public IEventCoordinator, public JsonConfigurable
    {
        DECLARE_FACTORY_REGISTERED_EXPORT( EventCoordinatorFactory, IncidenceEventCoordinator, IEventCoordinator )
    public:
        IMPLEMENT_DEFAULT_REFERENCE_COUNTING()
        DECLARE_QUERY_INTERFACE()

        IncidenceEventCoordinator();
        virtual ~IncidenceEventCoordinator();

        virtual bool Configure( const Configuration * inputJson ) override;
        virtual QuickBuilder GetSchema() override;

        // IEventCoordinator methods
        virtual void SetContextTo( ISimulationEventContext *isec ) override;
        virtual void CheckStartDay( float campaignStartDay ) const override;
        virtual void InitializeTiming( const IdmDateTime& ) override;
        virtual void AddNode( const suids::suid& suid ) override;
        virtual void Update( float dt ) override;
        virtual void ConsiderResponding();
        virtual void UpdateNodes( float dt ) override;
        virtual bool IsFinished() override;
        virtual IEventCoordinatorEventContext* GetEventContext() override { return nullptr; }

        // Other methods - for testing
        int GetNumTimeStepsInRep() const { return m_NumTimestepsInRep; }
        int GetNumReps() const { return m_NumReps; }
        int GetEventCount() const { return m_pIncidenceCounter->GetCount(); }
        uint32_t GetQualifyingCount() { return m_pIncidenceCounter->GetCountOfQualifyingPopulation( m_CachedNodes ); }

    protected:
        virtual void ConfigureRepetitions( const Configuration * inputJson );
        virtual void CheckConfigureRepetitions();

        IncidenceEventCoordinator( IncidenceCounter* ic, Responder* pResponder );

        ISimulationEventContext*        m_Parent;
        std::vector<INodeEventContext*> m_CachedNodes;
        bool                            m_IsExpired;
        bool                            m_HasBeenDistributed;
        int32_t                         m_NumReps;
        int32_t                         m_NumTimestepsBetweenReps;
        int32_t                         m_NumTimestepsInRep;
        IncidenceCounter*               m_pIncidenceCounter;
        Responder*                      m_pResponder;
    };
}

