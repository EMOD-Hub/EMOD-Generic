
#pragma once
#include "Node.h"
#include "IndividualAirborne.h" // for serialization only

namespace Kernel
{
    class NodeAirborne : public Node
    {
    public:
        virtual ~NodeAirborne(void);
        static NodeAirborne *CreateNode(ISimulationContext *_parent_sim, ExternalNodeId_t externalNodeId, suids::suid node_suid);

    protected:
        NodeAirborne();
        NodeAirborne(ISimulationContext *_parent_sim, ExternalNodeId_t externalNodeId, suids::suid node_suid);

        // Factory methods
        virtual IIndividualHuman* createHuman(suids::suid suid, float monte_carlo_weight, float initial_age, int gender) override;

        DECLARE_SERIALIZABLE(NodeAirborne);
    };
}
