
#pragma once

#include "ISupports.h"
#include "IStrainIdentity.h"
#include "Types.h"

namespace Kernel
{
    struct IContagionPopulation: public IStrainIdentity, public ISupports
    {
        virtual float GetTotalContagion( void ) const = 0;
        virtual act_prob_vec_t GetProbabilities() const = 0;
        virtual NaturalNumber GetInfectorID( void ) const = 0;
    };
}
