
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
        DECLARE_FACTORY_REGISTERED(IndividualIVFactory, OutbreakIndividual, IDistributableIntervention)

    public:
        OutbreakIndividual();
        virtual ~OutbreakIndividual() { }

        virtual bool Configure(const Configuration* config) override;
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
