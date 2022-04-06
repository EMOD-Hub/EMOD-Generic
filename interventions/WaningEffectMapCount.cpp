/***************************************************************************************************

Copyright (c) 2018 Intellectual Ventures Property Holdings, LLC (IVPH) All rights reserved.

EMOD is licensed under the Creative Commons Attribution-Noncommercial-ShareAlike 4.0 License.
To view a copy of this license, visit https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode

***************************************************************************************************/

#include "stdafx.h"

#include "WaningEffect.h"
#include "IndividualEventContext.h"

SETUP_LOGGING( "WaningEffectMapCount" )

namespace Kernel
{
    // ------------------------------------------------------------------------
    // --- WaningEffectMapCount
    // ------------------------------------------------------------------------
    IMPLEMENT_FACTORY_REGISTERED( WaningEffectMapCount )

    BEGIN_QUERY_INTERFACE_DERIVED( WaningEffectMapCount, WaningEffectMapPiecewise )
        HANDLE_INTERFACE( IWaningEffectCount )
    END_QUERY_INTERFACE_DERIVED( WaningEffectMapCount, WaningEffectMapPiecewise )

    WaningEffectMapCount::WaningEffectMapCount()
        : WaningEffectMapPiecewise( 0.0f, 999999.0f ) // 0 <= time <= 999999
        , m_SetCountCalled(false)
    {
    }

    WaningEffectMapCount::WaningEffectMapCount( const WaningEffectMapCount& rOrig )
        : WaningEffectMapPiecewise( rOrig )
        , m_SetCountCalled( rOrig.m_SetCountCalled )
    {
    }

    WaningEffectMapCount::~WaningEffectMapCount()
    {
    }

    IWaningEffect* WaningEffectMapCount::Clone()
    {
        return new WaningEffectMapCount( *this );
    }

    void WaningEffectMapCount::Update( float dt )
    {
    }

    float WaningEffectMapCount::Current() const
    {
        if( !m_SetCountCalled )
        {
            throw IllegalOperationException( __FILE__, __LINE__, __FUNCTION__, "WaningEffectCount is only supported in interventions that support the IWaningEffectCount interface." );
        }
        return WaningEffectMapPiecewise::Current();
    }

    void WaningEffectMapCount::SetCount( uint32_t numCounts )
    {
        m_SetCountCalled = true;
        m_TimeSinceStart = float(numCounts);
        WaningEffectMapCount::UpdateEffect();
    }

    bool WaningEffectMapCount::IsValidConfiguration( uint32_t maxCount ) const
    {
        bool valid = ( m_DurationMap.size() == maxCount );
        if( valid )
        {
            uint32_t count = 0;
            for( auto it = m_DurationMap.begin(); valid && (it != m_DurationMap.end()); ++it )
            {
                ++count;
                valid = ( count == uint32_t(it->first) );
            }
            release_assert( count == maxCount );
        }
        return valid;
    }

    bool WaningEffectMapCount::ConfigureExpiration( const Configuration* config )
    {
        // disable m_ExpireAtDurationMapEnd
        return true;
    }

    bool WaningEffectMapCount::ConfigureReferenceTimer( const Configuration* config )
    {
        // disable Reference_Timer
        return true;
    }


    REGISTER_SERIALIZABLE( WaningEffectMapCount );

    void WaningEffectMapCount::serialize( IArchive& ar, WaningEffectMapCount* obj )
    {
        WaningEffectMapPiecewise::serialize( ar, obj );
        WaningEffectMapCount& effect = *obj;
    }
}
