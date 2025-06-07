
#pragma once
#include "Configure.h"
#include "IdmApi.h"
#include "ISerializable.h"
#include "MathFunctions.h"
#include "InterpolatedValueMap.h"

namespace Kernel
{
    class IPiecewiseDistribution
    {
    public:
        virtual void SetX( NonNegativeFloat x ) = 0;
    };
}