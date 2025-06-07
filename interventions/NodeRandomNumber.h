
#pragma once

#include "Interventions.h"
#include "ISupports.h"
#include "InterventionFactory.h"
#include "FactorySupport.h"
#include "Configuration.h"
#include "SimulationEnums.h"
#include "InterpolatedValueMap.h"

namespace Kernel
{
    class NodeRandomNumber : public BaseNodeIntervention
    {
        IMPLEMENT_DEFAULT_REFERENCE_COUNTING()
        DECLARE_FACTORY_REGISTERED(NodeIVFactory, NodeRandomNumber, INodeDistributableIntervention)

    public:
        NodeRandomNumber();
        NodeRandomNumber(const NodeRandomNumber&);
        virtual ~NodeRandomNumber();

        virtual bool Configure(const Configuration*) override;
        virtual bool Distribute(INodeEventContext* context, IEventCoordinator2* pEC = nullptr) override;
        virtual void Update(float) override;

        static void serialize(IArchive&, NodeRandomNumber*);

    protected:
        int total_num;
    };
}
