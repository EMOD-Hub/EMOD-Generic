
#pragma once

#include <vector>

#include "Interventions.h"
#include "Configuration.h"
#include "InterventionFactory.h"
#include "InterventionEnums.h"
#include "EventCoordinator.h"
#include "Configure.h"
#include "VectorEnums.h"
#include "VectorDefs.h"
#include "Common.h"

namespace Kernel
{
    class InputEIR : public BaseNodeIntervention
    {
        IMPLEMENT_DEFAULT_REFERENCE_COUNTING()
        DECLARE_FACTORY_REGISTERED(NodeIVFactory, InputEIR, INodeDistributableIntervention)

    public:
        InputEIR();
        InputEIR( const InputEIR& master );
        virtual ~InputEIR() { }

        virtual bool Configure(const Configuration* config) override;
        virtual void Update(float dt) override;

    protected:
        AgeDependentBitingRisk::Enum age_dependence;
        std::vector<float> monthly_EIR; // 12 values of EIR by month
        float today;
        float daily_EIR;
        tAgeBitingFunction risk_function;
    };
}
