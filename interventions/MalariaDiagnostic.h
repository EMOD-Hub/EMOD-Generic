
#pragma once

#include "Diagnostics.h"
#include "InterventionEnums.h"

namespace Kernel
{
    class MalariaDiagnostic : public SimpleDiagnostic 
    {
        IMPLEMENT_DEFAULT_REFERENCE_COUNTING()
        DECLARE_FACTORY_REGISTERED(IndividualIVFactory, MalariaDiagnostic, IDistributableIntervention)

    public: 
        MalariaDiagnostic();
        MalariaDiagnostic( const MalariaDiagnostic& );

        virtual bool Configure( const Configuration* pConfig ) override;
        virtual ~MalariaDiagnostic();

    protected:
        virtual bool positiveTestResult() override;

        MalariaDiagnosticType::Enum malaria_diagnostic_type;
        float detection_threshold;

        DECLARE_SERIALIZABLE(MalariaDiagnostic);
    };
}


