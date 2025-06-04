
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
#include "IRelationship.h"

namespace Kernel
{
    struct ISTIInterventionsContainer; 

    class STIBarrier : public BaseIntervention
    {
        DECLARE_FACTORY_REGISTERED(IndividualIVFactory, STIBarrier, IDistributableIntervention)

    public:
        STIBarrier();
        virtual ~STIBarrier() { }

        virtual bool Configure( const Configuration * config ) override;

        // IDistributableIntervention
        virtual bool Distribute(IIndividualHumanInterventionsContext *context, ICampaignCostObserver * const pCCO ) override;
        virtual void SetContextTo(IIndividualHumanContext *context) override;
        virtual void Update(float dt) override;

    protected:
        float early;
        float late;
        float midyear;
        float rate;
        RelationshipType::Enum rel_type;
        ISTIInterventionsContainer* isticontainer;

        DECLARE_SERIALIZABLE(STIBarrier);
    };
}
