/***************************************************************************************************

Copyright (c) 2018 Intellectual Ventures Property Holdings, LLC (IVPH) All rights reserved.

EMOD is licensed under the Creative Commons Attribution-Noncommercial-ShareAlike 4.0 License.
To view a copy of this license, visit https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode

***************************************************************************************************/

#pragma once
#include "stdafx.h"
#include "PiecewiseDistributionBase.h"

namespace Kernel
{
    PiecewiseDistributionBase::PiecewiseDistributionBase() 
        : m_interpolatedValueMap()
        , m_x( 0.0f )
    {
    }

    PiecewiseDistributionBase::PiecewiseDistributionBase( const PiecewiseDistributionBase& master ) 
        : m_interpolatedValueMap( master.m_interpolatedValueMap )
        , m_x( master.m_x )
    {
    }
    
    void PiecewiseDistributionBase::SetX( NonNegativeFloat x )
    {
        m_x = x;
    }
    
    IPiecewiseDistribution* PiecewiseDistributionBase::GetIPiecewiseDistribution()
    {
        return this;
    }

    void PiecewiseDistributionBase::serialize(IArchive& ar, PiecewiseDistributionBase* id)
    {        
        PiecewiseDistributionBase& pdd = *id;
        ar.labelElement( "m_interpolatedValueMap" ) & pdd.m_interpolatedValueMap;
        ar.labelElement( "m_x" ) & pdd.m_x;
    }
}