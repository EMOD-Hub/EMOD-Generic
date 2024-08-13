/***************************************************************************************************

Copyright (c) 2018 Intellectual Ventures Property Holdings, LLC (IVPH) All rights reserved.

EMOD is licensed under the Creative Commons Attribution-Noncommercial-ShareAlike 4.0 License.
To view a copy of this license, visit https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode

***************************************************************************************************/

#pragma once

#include "AgeDiagnostic.h"

namespace Kernel
{
    class CD4Diagnostic : public AgeDiagnostic 
    {
        IMPLEMENT_DEFAULT_REFERENCE_COUNTING()
        DECLARE_FACTORY_REGISTERED(IndividualIVFactory, CD4Diagnostic, IDistributableIntervention)

    public: 
        CD4Diagnostic();
        CD4Diagnostic( const CD4Diagnostic& );
        virtual ~CD4Diagnostic();

    protected:
        virtual void ConfigureRangeThresholds( const Configuration* inputJson ) override;
        virtual float GetValue() const override;

        DECLARE_SERIALIZABLE(CD4Diagnostic);
    };
}
