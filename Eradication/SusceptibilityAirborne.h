
#pragma once
#include "Susceptibility.h"

namespace Kernel
{
    class SusceptibilityAirborneConfig : public SusceptibilityConfig
    {
    protected:
        friend class SusceptibilityAirborne;
    };
    
    class SusceptibilityAirborne : public Susceptibility
    {
    public:
        virtual ~SusceptibilityAirborne(void);
        static SusceptibilityAirborne *CreateSusceptibility(IIndividualHumanContext *context, float immmod, float riskmod);

    protected:

        SusceptibilityAirborne();
        SusceptibilityAirborne(IIndividualHumanContext *context);
        virtual void Initialize(float immmod, float riskmod) override;

        DECLARE_SERIALIZABLE(SusceptibilityAirborne);
    };
}
