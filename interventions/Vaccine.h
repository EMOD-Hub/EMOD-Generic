
#pragma once

#include "Interventions.h"
#include "Configuration.h"
#include "InterventionFactory.h"
#include "InterventionEnums.h"
#include "Configure.h"
#include "IWaningEffect.h"
#include "InterpolatedValueMap.h"

namespace Kernel
{
    struct IVaccineConsumer;
    struct ICampaignCostObserver;

    class Vaccine : public BaseIntervention
    {
        DECLARE_FACTORY_REGISTERED(IndividualIVFactory, Vaccine, IDistributableIntervention)

        DECLARE_SERIALIZABLE(Vaccine);

    public:
        Vaccine();
        Vaccine(const Vaccine&);
        virtual ~Vaccine();

        virtual bool Configure(const Configuration*) override;

        // IDistributableIntervention
        virtual bool Distribute(IIndividualHumanInterventionsContext*, ICampaignCostObserver* const) override;
        virtual void SetContextTo(IIndividualHumanContext*) override;
        virtual void Update(float dt) override;
        virtual bool NeedsInfectiousLoopUpdate() const;

    protected:

        bool  vaccine_took;

        float vaccine_take;
        float frac_acq_blocking_take;

        IVRoute::Enum  vax_route;

        InterpolatedValueMap  take_by_age_map;
        InterpolatedValueMap  init_acq_by_effect_map;
        InterpolatedValueMap  init_trn_by_effect_map;
        InterpolatedValueMap  init_mor_by_effect_map;

        IWaningEffect* effect_acquire;
        IWaningEffect* effect_transmit;
        IWaningEffect* effect_mortality;

        IVaccineConsumer* ivc;
    };
}
