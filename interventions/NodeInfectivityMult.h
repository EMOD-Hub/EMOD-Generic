
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
    class NodeInfectivityMult : public BaseNodeIntervention
    {
        IMPLEMENT_DEFAULT_REFERENCE_COUNTING()
        DECLARE_FACTORY_REGISTERED(NodeIVFactory, NodeInfectivityMult, INodeDistributableIntervention)

    public:
        NodeInfectivityMult();
        NodeInfectivityMult(const NodeInfectivityMult&);
        virtual ~NodeInfectivityMult();

        virtual bool Configure(const Configuration*) override;
        virtual bool Distribute(INodeEventContext* context, IEventCoordinator2* pEC = nullptr) override;
        virtual void Update(float) override;

        static void serialize(IArchive&, NodeInfectivityMult*);

    protected:
        float                      duration;
        TransmissionRoute::Enum    tx_route;
        InterpolatedValueMap       mult_by_duration;
    };
}
