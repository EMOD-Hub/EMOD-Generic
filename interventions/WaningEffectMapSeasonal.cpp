/***************************************************************************************************

Copyright (c) 2018 Intellectual Ventures Property Holdings, LLC (IVPH) All rights reserved.

EMOD is licensed under the Creative Commons Attribution-Noncommercial-ShareAlike 4.0 License.
To view a copy of this license, visit https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode

***************************************************************************************************/

#include "stdafx.h"

#include "WaningEffectMapSeasonal.h"
#include "Common.h"

SETUP_LOGGING( "WaningEffectMapSeasonal" )

namespace Kernel
{
    // ------------------------------------------------------------------------
    // --- WaningEffectMapSeasonal
    // ------------------------------------------------------------------------
    IMPLEMENT_FACTORY_REGISTERED( WaningEffectMapLinearSeasonal )
    BEGIN_QUERY_INTERFACE_DERIVED( WaningEffectMapLinearSeasonal, WaningEffectMapLinear )
    END_QUERY_INTERFACE_DERIVED( WaningEffectMapLinearSeasonal, WaningEffectMapLinear )

    WaningEffectMapLinearSeasonal::WaningEffectMapLinearSeasonal()
        : WaningEffectMapLinear( DAYSPERYEAR )
        , m_Parent( nullptr )
    {
    }

    WaningEffectMapLinearSeasonal::WaningEffectMapLinearSeasonal( const WaningEffectMapLinearSeasonal& rOrig )
        : WaningEffectMapLinear( rOrig )
        , m_Parent( nullptr )
    {
    }

    WaningEffectMapLinearSeasonal::~WaningEffectMapLinearSeasonal()
    {
    }

    IWaningEffect* WaningEffectMapLinearSeasonal::Clone()
    {
        return new WaningEffectMapLinearSeasonal( *this );
    }

    void  WaningEffectMapLinearSeasonal::Update( float dt )
    {
        WaningEffectMapLinear::Update( dt );
        if( m_TimeSinceStart >= DAYSPERYEAR )
        {
            m_TimeSinceStart = 0.0;
        }
    }

    bool WaningEffectMapLinearSeasonal::ConfigureExpiration( const Configuration* config )
    {
        // disable m_ExpireAtDurationMapEnd
        return true;
    }

    bool WaningEffectMapLinearSeasonal::ConfigureReferenceTimer( const Configuration* config )
    {
        // disable Reference_Timer
        return true;
    }

    void WaningEffectMapLinearSeasonal::SetContextTo( IIndividualHumanContext *context )
    {
        m_Parent = context;
    }

    REGISTER_SERIALIZABLE( WaningEffectMapLinearSeasonal );

    void WaningEffectMapLinearSeasonal::serialize( IArchive& ar, WaningEffectMapLinearSeasonal* obj )
    {
        WaningEffectMapLinear::serialize( ar, obj );
        WaningEffectMapLinearSeasonal& effect = *obj;
    }
}
