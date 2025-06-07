
#pragma once
#include "Configure.h"
#include "MathFunctions.h"
#include "DistributionBase.h"
#include "InterpolatedValueMap.h"
#include "IPiecewiseDistribution.h"


namespace Kernel
{
    class PiecewiseDistributionBase : public DistributionBase, public IPiecewiseDistribution
    {
    public:
        PiecewiseDistributionBase();
        PiecewiseDistributionBase( const PiecewiseDistributionBase& master );

        virtual void SetX( NonNegativeFloat x ) override;
        IPiecewiseDistribution* GetIPiecewiseDistribution();

    protected:
        static void serialize( IArchive& ar, PiecewiseDistributionBase* obj );
    
        InterpolatedValueMap m_interpolatedValueMap;
        NonNegativeFloat m_x;
    };
}