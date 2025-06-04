
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
