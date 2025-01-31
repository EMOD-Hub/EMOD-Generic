/***************************************************************************************************

Copyright (c) 2018 Intellectual Ventures Property Holdings, LLC (IVPH) All rights reserved.

EMOD is licensed under the Creative Commons Attribution-Noncommercial-ShareAlike 4.0 License.
To view a copy of this license, visit https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode

***************************************************************************************************/

#pragma once

#include <string>
#include <list>
#include <vector>

#include "Interventions.h"
#include "Configuration.h"
#include "InterventionFactory.h"
#include "InterventionEnums.h"
#include "FactorySupport.h"
#include "Configure.h"
#include "EventTrigger.h"

namespace Kernel
{
    struct ISTIInterventionsContainer;

    struct ICircumcision : ISupports
    {
        virtual bool ApplyIfHigherReducedAcquire() const = 0;
        virtual float GetReducedAcquire() const = 0;
    };

    class MaleCircumcision : public BaseIntervention, public ICircumcision
    {
        IMPLEMENT_DEFAULT_REFERENCE_COUNTING()
        DECLARE_FACTORY_REGISTERED(IndividualIVFactory, MaleCircumcision, IDistributableIntervention)

    public:
        MaleCircumcision();
        virtual ~MaleCircumcision() { }

        virtual bool Configure( const Configuration * config ) override;

        virtual bool Distribute( IIndividualHumanInterventionsContext *context, ICampaignCostObserver * const pCCO ) override;
        virtual void SetContextTo(IIndividualHumanContext *context) override;
        virtual void Update(float dt) override;

        // ICircumcision
        virtual bool ApplyIfHigherReducedAcquire() const override;
        virtual float GetReducedAcquire() const override;

    protected:
        float m_ReducedAcquire;
        bool m_ApplyIfHigherReducedAcquire;
        EventTrigger::Enum m_DistrbutedEventTrigger;
        bool has_been_applied;

        ISTIInterventionsContainer* ivc;

        DECLARE_SERIALIZABLE(MaleCircumcision);
    };
}
