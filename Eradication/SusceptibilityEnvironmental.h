
#pragma once
#include "Susceptibility.h"

namespace Kernel
{
    class SusceptibilityEnvironmental : public Susceptibility
    {
    public:
        static SusceptibilityEnvironmental *CreateSusceptibility(IIndividualHumanContext *context, float immmod, float riskmod);
        virtual ~SusceptibilityEnvironmental(void);

    protected:
        SusceptibilityEnvironmental();
        SusceptibilityEnvironmental(IIndividualHumanContext *context);

        virtual void Initialize(float immmod, float riskmod) override;

        DECLARE_SERIALIZABLE(SusceptibilityEnvironmental);
    };
}
