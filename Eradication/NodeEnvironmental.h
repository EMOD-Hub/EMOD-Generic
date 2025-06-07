
#pragma once
#include "Node.h"
#include "IndividualEnvironmental.h"
#include <math.h>

namespace Kernel
{
    class NodeEnvironmental : public Node
    {
        IMPLEMENT_DEFAULT_REFERENCE_COUNTING()

    public:
        static NodeEnvironmental *CreateNode(ISimulationContext *_parent_sim, ExternalNodeId_t externalNodeId, suids::suid node_suid);
        virtual ~NodeEnvironmental(void);

    protected:
        NodeEnvironmental( ISimulationContext *_parent_sim, ExternalNodeId_t externalNodeId, suids::suid node_suid );
        NodeEnvironmental();

        virtual IIndividualHuman* createHuman( suids::suid suid, float monte_carlo_weight, float initial_age, int gender) override;

        DECLARE_SERIALIZABLE(NodeEnvironmental);
    };
}
