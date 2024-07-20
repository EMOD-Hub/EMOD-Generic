/***************************************************************************************************

Copyright (c) 2018 Intellectual Ventures Property Holdings, LLC (IVPH) All rights reserved.

EMOD is licensed under the Creative Commons Attribution-Noncommercial-ShareAlike 4.0 License.
To view a copy of this license, visit https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode

***************************************************************************************************/

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
