
#pragma once

#include "ISupports.h"
#include "Common.h"

namespace Kernel
{
    struct NodeDemographicsDistribution;
    class IndividualHuman;

    struct INodeTBHIV : public ISupports
    {
        virtual const NodeDemographicsDistribution* GetHIVCoinfectionDistribution() const = 0;
        virtual const NodeDemographicsDistribution* GetHIVMortalityDistribution()   const = 0;
        virtual void SetNewInfectionState(InfectionStateChange::_enum inf_state_change, IndividualHuman *ih) = 0;
    };
}
