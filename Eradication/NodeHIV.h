
#pragma once
#include "NodeSTI.h"
#include "INodeHIV.h"

namespace Kernel
{
    class NodeHIV : public NodeSTI, public INodeHIV
    {
        IMPLEMENT_DEFAULT_REFERENCE_COUNTING()

    public:
        virtual ~NodeHIV(void);
        static NodeHIV *CreateNode(ISimulationContext *_parent_sim, ExternalNodeId_t externalNodeId, suids::suid node_suid);

    protected:
        NodeHIV();
        NodeHIV(ISimulationContext *_parent_sim, ExternalNodeId_t externalNodeId, suids::suid node_suid);

        // Factory methods
        virtual IIndividualHuman* createHuman( suids::suid suid, float monte_carlo_weight, float initial_age, int gender) override;

        DECLARE_SERIALIZABLE(NodeHIV);
    };
}
