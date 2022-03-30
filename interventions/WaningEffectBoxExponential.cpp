/***************************************************************************************************

Copyright (c) 2018 Intellectual Ventures Property Holdings, LLC (IVPH) All rights reserved.

EMOD is licensed under the Creative Commons Attribution-Noncommercial-ShareAlike 4.0 License.
To view a copy of this license, visit https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode

***************************************************************************************************/

#include "stdafx.h"

#include "WaningEffect.h"
#include "CajunIncludes.h"
#include "ConfigurationImpl.h"

SETUP_LOGGING( "WaningEffectBoxExponential" )

namespace Kernel
{
    // --------------------------- WaningEffectBoxExponential ---------------------------
    IMPLEMENT_FACTORY_REGISTERED(WaningEffectBoxExponential)
    IMPL_QUERY_INTERFACE2(WaningEffectBoxExponential, IWaningEffect, IConfigurable)

    WaningEffectBoxExponential::WaningEffectBoxExponential()
    : WaningEffectConstant()
    , boxDuration( 0.0f )
    , decayTimeConstant( 0.0f )
    {
    }

    WaningEffectBoxExponential::WaningEffectBoxExponential( const WaningEffectBoxExponential& rOrig )
    : WaningEffectConstant( rOrig )
    , boxDuration( rOrig.boxDuration )
    , decayTimeConstant( rOrig.decayTimeConstant )
    {
    }

    IWaningEffect* WaningEffectBoxExponential::Clone()
    {
        return new WaningEffectBoxExponential( *this );
    }

    bool WaningEffectBoxExponential::Configure( const Configuration * pInputJson )
    {
        initConfigTypeMap("Box_Duration",        &boxDuration,       WEBE_Box_Duration_DESC_TEXT,        0, 100000, 100);
        initConfigTypeMap("Decay_Time_Constant", &decayTimeConstant, WEBE_Decay_Time_Constant_DESC_TEXT, 0, 100000, 100);
        return WaningEffectConstant::Configure( pInputJson );
    }

    void  WaningEffectBoxExponential::Update(float dt)
    {
        if ( boxDuration > 0 )
        {
            boxDuration -= dt;
        }
        else if ( decayTimeConstant > dt )
        {
            currentEffect *= (1 - dt/decayTimeConstant);
        }
        else
        {
            currentEffect = 0;
        }
    }

    REGISTER_SERIALIZABLE(WaningEffectBoxExponential);

    void WaningEffectBoxExponential::serialize(IArchive& ar, WaningEffectBoxExponential* obj)
    {
        WaningEffectConstant::serialize( ar, obj );
        WaningEffectBoxExponential& effect = *obj;
        ar.labelElement( "boxDuration"       ) & effect.boxDuration;
        ar.labelElement( "decayTimeConstant" ) & effect.decayTimeConstant;
    }
}
