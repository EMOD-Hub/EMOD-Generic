/***************************************************************************************************

Copyright (c) 2018 Intellectual Ventures Property Holdings, LLC (IVPH) All rights reserved.

EMOD is licensed under the Creative Commons Attribution-Noncommercial-ShareAlike 4.0 License.
To view a copy of this license, visit https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode

***************************************************************************************************/

#pragma once
#include "stdafx.h"
#include "Configure.h"
#include "MathFunctions.h"
#include "IdmApi.h"
#include "DistributionBase.h"
#include "IDistribution.h"

namespace Kernel
{
    DistributionBase::DistributionBase()
        : IDistribution()
        , type( DistributionFunction::NOT_INITIALIZED )
        , m_Param1( 0.0f )
        , m_Param2( 0.0f )
        , m_Param3( 0.0f )
    {}

    DistributionBase::DistributionBase( const DistributionBase& master ) 
        : IDistribution()
        , type( master.type )
        , m_Param1( master.m_Param1 )
        , m_Param2( master.m_Param2 )
        , m_Param3( master.m_Param3 )
    {}

    DistributionFunction::Enum DistributionBase::GetType() const 
    { 
        return type; 
    }

    void DistributionBase::SetParameters( double param1, double param2, double param3 )
    {
        m_Param1 = param1;
        m_Param2 = param2;
        m_Param3 = param3;
    }

    float DistributionBase::GetParam1() const
    {
        return m_Param1;
    }

    float DistributionBase::GetParam2() const
    {
        return m_Param2;
    }

    float DistributionBase::GetParam3() const
    {
        return m_Param3;
    }

    IPiecewiseDistribution* DistributionBase::GetIPiecewiseDistribution()
    {
        return nullptr;
    }

    void DistributionBase::serialize( IArchive& ar, DistributionBase* id )
    {
        DistributionBase& dd = *id;
        ar.labelElement( "m_Param1" ) & dd.m_Param1;
        ar.labelElement( "m_Param2" ) & dd.m_Param2;
        ar.labelElement( "m_Param3" ) & dd.m_Param3;
    }
}