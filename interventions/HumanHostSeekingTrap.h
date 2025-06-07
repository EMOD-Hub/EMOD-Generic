
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
#include "IWaningEffect.h"

namespace Kernel
{
    struct IVectorInterventionsEffects;

    class HumanHostSeekingTrap : public BaseIntervention
    {
        DECLARE_FACTORY_REGISTERED(IndividualIVFactory, HumanHostSeekingTrap, IDistributableIntervention)

    public:
        HumanHostSeekingTrap();
        HumanHostSeekingTrap( const HumanHostSeekingTrap& );
        virtual ~HumanHostSeekingTrap();

        virtual bool Configure( const Configuration * config ) override;

        virtual bool Distribute(IIndividualHumanInterventionsContext *context, ICampaignCostObserver * const pCCO ) override;
        virtual void SetContextTo(IIndividualHumanContext *context) override;
        virtual void Update(float dt) override;

    protected:
        IWaningEffect* killing_effect;
        IWaningEffect* attract_effect;
        IVectorInterventionsEffects *ivies;

        DECLARE_SERIALIZABLE(HumanHostSeekingTrap);
    };
}
