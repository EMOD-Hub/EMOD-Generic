
#pragma once

#include <map>

#include "Interventions.h"
#include "InterventionFactory.h"
#include "Configuration.h"
#include "IWaningEffect.h"
#include "Configure.h"

namespace Kernel
{
    struct IVectorInterventionsEffects;

    class Ivermectin : public BaseIntervention
    {
        DECLARE_FACTORY_REGISTERED(IndividualIVFactory, Ivermectin, IDistributableIntervention)

    public:
        Ivermectin();
        Ivermectin( const Ivermectin& );
        virtual ~Ivermectin();

        virtual bool Configure( const Configuration * config ) override;

        virtual bool Distribute(IIndividualHumanInterventionsContext *context, ICampaignCostObserver  * const pCCO ) override;
        virtual void SetContextTo(IIndividualHumanContext *context) override;
        virtual void Update(float dt) override;

    protected:
        IWaningEffect* killing_effect;
        IVectorInterventionsEffects* m_pIVIES; // aka individual vector interventions container

        DECLARE_SERIALIZABLE(Ivermectin);
    };
}
