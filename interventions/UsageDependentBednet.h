/***************************************************************************************************

Copyright (c) 2018 Intellectual Ventures Property Holdings, LLC (IVPH) All rights reserved.

EMOD is licensed under the Creative Commons Attribution-Noncommercial-ShareAlike 4.0 License.
To view a copy of this license, visit https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode

***************************************************************************************************/

#pragma once

#include "Bednet.h"
#include "Timers.h"
#include "IDistribution.h"

namespace Kernel
{

    struct IBednetConsumer;

    class UsageDependentBednet : public AbstractBednet
    {
        DECLARE_FACTORY_REGISTERED(InterventionFactory, UsageDependentBednet, IDistributableIntervention)

    public:
        UsageDependentBednet();
        UsageDependentBednet( const UsageDependentBednet& );
        virtual ~UsageDependentBednet();

        // IDistributableIntervention
        virtual bool Distribute(IIndividualHumanInterventionsContext *context, ICampaignCostObserver * const pCCO ) override;
        virtual QueryResult QueryInterface(iid_t iid, void **ppvObject) override;
        virtual void SetExpired( bool isExpired ) override;
        virtual void SetContextTo( IIndividualHumanContext *context ) override;

    protected:
        virtual bool ConfigureUsage(  const Configuration* config ) override; // should call JsonConfigurable::Configure()
        virtual bool ConfigureEvents( const Configuration* config ) override; // should call JsonConfigurable::Configure()

        virtual void UpdateUsage( float dt ) override;
        virtual bool IsUsingBednet() const override;
        virtual bool CheckExpiration( float dt ) override;
        virtual float GetEffectUsage() const override;

        virtual void Callback( float dt );

        std::vector<IWaningEffect*> m_UsageEffectList; 
        EventTrigger::Enum m_TriggerReceived;
        EventTrigger::Enum m_TriggerUsing;
        EventTrigger::Enum m_TriggerDiscard;
        IDistribution* m_ExpirationDuration;

        CountdownTimer m_ExpirationTimer;
        bool m_TimerHasExpired;

        DECLARE_SERIALIZABLE( UsageDependentBednet );
    };
}
