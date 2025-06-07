
#pragma once

#include <string>
#include <vector>

#include "Configuration.h"
#include "InterventionFactory.h"
#include "Interventions.h"

namespace Kernel
{
    class ChangeIPMatrix : public BaseNodeIntervention
    {
        IMPLEMENT_DEFAULT_REFERENCE_COUNTING()
        DECLARE_FACTORY_REGISTERED( NodeIVFactory, ChangeIPMatrix, INodeDistributableIntervention )

    public:
        ChangeIPMatrix();
        virtual ~ChangeIPMatrix();

        virtual bool Configure( const Configuration* pConfig ) override;
        virtual bool Distribute(INodeEventContext *context, IEventCoordinator2* pEC);
        virtual void Update(float dt);

    protected:
        std::string                        target_property_name;
        std::vector<std::vector<float>>    revised_matrix;
    };
}
