
#pragma once

#include <fstream>
#include <iostream>
#include <math.h>
#include <vector>

#include "Climate.h"

namespace Kernel
{
    class RANDOMBASE;

    class ClimateConstant : public Climate
    {
    public:
        static ClimateConstant * CreateClimate( ClimateUpdateResolution::Enum update_resolution,
                                                INodeContext * _parent,
                                                float start_time,
                                                RANDOMBASE* pRNG );

        virtual void UpdateWeather( float, float, RANDOMBASE* pRNG ) override;

    protected:
        ClimateConstant();
        ClimateConstant(ClimateUpdateResolution::Enum update_resolution, INodeContext * _parent);

        virtual bool IsPlausible();
    };
}
