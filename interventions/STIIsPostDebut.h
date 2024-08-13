/***************************************************************************************************

Copyright (c) 2018 Intellectual Ventures Property Holdings, LLC (IVPH) All rights reserved.

EMOD is licensed under the Creative Commons Attribution-Noncommercial-ShareAlike 4.0 License.
To view a copy of this license, visit https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode

***************************************************************************************************/

#pragma once

#include "Diagnostics.h"

namespace Kernel
{
    class IDMAPI STIIsPostDebut : public SimpleDiagnostic
    {
        IMPLEMENT_DEFAULT_REFERENCE_COUNTING()
        DECLARE_FACTORY_REGISTERED(IndividualIVFactory, STIIsPostDebut, IDistributableIntervention)

    public:
        STIIsPostDebut();
        STIIsPostDebut( const STIIsPostDebut& ); // copy ctor
        virtual bool Configure( const Configuration* pConfig ) override;

        // SimpleDiagnostic
        virtual bool positiveTestResult() override;    // Test if recipient "tests positive"
        virtual void onNegativeTestResult() override;  // What to do if recipient "tests negative"

    protected:
#pragma warning( push )
#pragma warning( disable: 4251 ) // See IdmApi.h for details
        EventTrigger::Enum negative_diagnosis_event;

        DECLARE_SERIALIZABLE(STIIsPostDebut);
#pragma warning( pop )
    };
}
