
#pragma once

#include "HIVSimpleDiagnostic.h"
#include "IHealthSeekingBehavior.h"

namespace Kernel
{
    class HIVSigmoidByYearAndSexDiagnostic : public HIVSimpleDiagnostic
    {
        IMPLEMENT_DEFAULT_REFERENCE_COUNTING()
        DECLARE_FACTORY_REGISTERED(IndividualIVFactory, HIVSigmoidByYearAndSexDiagnostic, IDistributableIntervention)

    public: 
        HIVSigmoidByYearAndSexDiagnostic();
        HIVSigmoidByYearAndSexDiagnostic( const HIVSigmoidByYearAndSexDiagnostic& );
        virtual bool positiveTestResult() override;

    protected:

        float rampMin;
        float rampMax;
        float rampMidYear;
        float rampRate;
        float femaleMultiplier;

        DECLARE_SERIALIZABLE(HIVSigmoidByYearAndSexDiagnostic);
    };
}
