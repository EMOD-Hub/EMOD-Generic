
#include "stdafx.h"

#ifndef DISABLE_AIRBORNE

#include "SusceptibilityAirborne.h"

namespace Kernel
{
    SusceptibilityAirborne *SusceptibilityAirborne::CreateSusceptibility(IIndividualHumanContext *context, float immmod, float riskmod)
    {
        SusceptibilityAirborne *newsusceptibility = _new_ SusceptibilityAirborne(context);
        newsusceptibility->Initialize(immmod, riskmod);

        return newsusceptibility;
    }

    SusceptibilityAirborne::~SusceptibilityAirborne(void) { }
    SusceptibilityAirborne::SusceptibilityAirborne() { }
    SusceptibilityAirborne::SusceptibilityAirborne(IIndividualHumanContext *context) : Susceptibility(context) { }

    void SusceptibilityAirborne::Initialize(float _immmod, float _riskmod)
    {
        Susceptibility::Initialize(_immmod, _riskmod);
    }

    REGISTER_SERIALIZABLE(SusceptibilityAirborne);

    void SusceptibilityAirborne::serialize(IArchive& ar, SusceptibilityAirborne* obj)
    {
        Susceptibility::serialize(ar, obj);
        SusceptibilityAirborne& susceptibility = *obj;
    }
}

#endif // DISABLE_AIRBORNE
