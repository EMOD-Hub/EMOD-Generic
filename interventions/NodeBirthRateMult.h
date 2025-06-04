
#pragma once

#include "Interventions.h"
#include "ISupports.h"
#include "InterventionFactory.h"
#include "FactorySupport.h"
#include "Configuration.h"
#include "InterpolatedValueMap.h"

namespace Kernel
{
    class NodeBirthRateMult : public BaseNodeIntervention
    {
        IMPLEMENT_DEFAULT_REFERENCE_COUNTING()
        DECLARE_FACTORY_REGISTERED(NodeIVFactory, NodeBirthRateMult, INodeDistributableIntervention)

    public:
        NodeBirthRateMult();
        NodeBirthRateMult(const NodeBirthRateMult&);
        virtual ~NodeBirthRateMult();

        virtual bool Configure(const Configuration*) override;
        virtual bool Distribute(INodeEventContext* context, IEventCoordinator2* pEC = nullptr) override;
        virtual void Update(float) override;

        static void serialize(IArchive&, NodeBirthRateMult*);

    protected:
        float                 duration;

        InterpolatedValueMap  mult_by_duration;
    };
}
