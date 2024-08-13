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
#include "EventCoordinator.h"
#include "Configure.h"

namespace Kernel
{
    class StrainIdentity;
    struct IIndividualHumanContext;

    struct IOutbreakIndividual : public ISupports
    {
        virtual uint32_t GetClade()   const = 0;
        virtual uint64_t GetGenome()  const = 0;

        virtual ~IOutbreakIndividual() { }; // needed for cleanup via interface pointer
    };

    class OutbreakIndividual : public IOutbreakIndividual, public BaseIntervention
    {
        IMPLEMENT_DEFAULT_REFERENCE_COUNTING()
        DECLARE_CONFIGURED(OutbreakIndividual)
        DECLARE_FACTORY_REGISTERED(IndividualIVFactory, OutbreakIndividual, IDistributableIntervention)

    public:
        OutbreakIndividual();
        virtual ~OutbreakIndividual() { }

        virtual bool Distribute( IIndividualHumanInterventionsContext *context, ICampaignCostObserver * const pCCO );
        virtual void SetContextTo(IIndividualHumanContext *context) { /* not needed for this intervention */ }
        virtual void Update(float dt);

        // IOutbreakIndividual
        virtual uint32_t GetClade()  const  { return clade; }
        virtual uint64_t GetGenome() const  { return genome; }


    protected:
        uint32_t clade;
        uint64_t genome;

        bool   ignoreImmunity;
        float  incubation_period_override;
    };
}
