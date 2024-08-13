/***************************************************************************************************

Copyright (c) 2018 Intellectual Ventures Property Holdings, LLC (IVPH) All rights reserved.

EMOD is licensed under the Creative Commons Attribution-Noncommercial-ShareAlike 4.0 License.
To view a copy of this license, visit https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode

***************************************************************************************************/

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
