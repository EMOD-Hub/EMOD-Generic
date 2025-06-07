
#pragma once

#include <string>
#include <list>
#include <vector>

#include "Interventions.h"
#include "InterventionFactory.h"
#include "Configuration.h"
#include "InterventionEnums.h"
#include "Configure.h"
#include "IWaningEffect.h"

namespace Kernel
{
    struct IVectorInterventionsEffects;

    class SimpleIndividualRepellent : public BaseIntervention
    {
        IMPLEMENT_DEFAULT_REFERENCE_COUNTING()
        DECLARE_FACTORY_REGISTERED(IndividualIVFactory, SimpleIndividualRepellent, IDistributableIntervention)

    public:
        virtual bool Configure( const Configuration * config ) override;

        SimpleIndividualRepellent();
        SimpleIndividualRepellent( const SimpleIndividualRepellent& );
        virtual ~SimpleIndividualRepellent();

        virtual bool Distribute(IIndividualHumanInterventionsContext *context, ICampaignCostObserver  * const pCCO ) override;
        virtual void SetContextTo(IIndividualHumanContext *context) override;
        virtual void Update(float dt);

    protected:
        IWaningEffect* blocking_effect;
        IVectorInterventionsEffects* m_pIRC; // aka individual or individual vector interventions container

        DECLARE_SERIALIZABLE(SimpleIndividualRepellent);
    };
}
