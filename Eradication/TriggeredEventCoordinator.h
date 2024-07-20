/***************************************************************************************************

Copyright (c) 2018 Intellectual Ventures Property Holdings, LLC (IVPH) All rights reserved.

EMOD is licensed under the Creative Commons Attribution-Noncommercial-ShareAlike 4.0 License.
To view a copy of this license, visit https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode

***************************************************************************************************/

#pragma once

#include "StandardEventCoordinator.h"
#include "BroadcasterObserver.h"

namespace Kernel
{
    class TriggeredEventCoordinator : public StandardInterventionDistributionEventCoordinator
        , public ICoordinatorEventObserver, public IEventCoordinatorEventContext
    {
        DECLARE_FACTORY_REGISTERED_EXPORT( EventCoordinatorFactory, TriggeredEventCoordinator, IEventCoordinator )
        DECLARE_CONFIGURED( TriggeredEventCoordinator )
        IMPLEMENT_DEFAULT_REFERENCE_COUNTING()
        DECLARE_QUERY_INTERFACE()

    public:
        TriggeredEventCoordinator();
        ~TriggeredEventCoordinator();
        virtual bool notifyOnEvent( IEventCoordinatorEventContext *pEntity, const EventTriggerCoordinator& trigger ) override;
        virtual void SetContextTo( ISimulationEventContext *isec ) override;
        void Register();
        virtual IEventCoordinatorEventContext* GetEventContext() override { return this; }
        void CheckConfigTriggers( const Configuration * inputJson );
        virtual void Update( float dt );
        virtual void UpdateNodes( float dt ) override;
        bool IsFinished() override;

        // IEventCoordinatorEventContext
        virtual const std::string& GetName() const override;
        virtual const IdmDateTime& GetTime() const override;
        virtual IEventCoordinator* GetEventCoordinator() override { return this; };
        virtual ISurveillanceReporting* GetSurveillanceReporting() override { return nullptr; };

    protected:
        void BroadcastCompletionEvent();
        std::vector<EventTriggerCoordinator> m_StartTriggerConditionList, m_StopTriggerConditionList;
        EventTriggerCoordinator m_CompletionEvent;  
        virtual void UpdateRepetitions() override;
        void Unregister();

    protected:
        bool m_IsActive;
        bool m_IsStarting;
        bool m_IsStopping;
        bool m_DurationExpired;
        std::string m_CoordinatorName;
        float m_Duration;
        int m_InputNumRepetitions;
    };
}
