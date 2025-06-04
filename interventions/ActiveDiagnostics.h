
#pragma once

#include <string>
#include <list>
#include <vector>

#include "Diagnostics.h"

namespace Kernel
{
    class ActiveDiagnostic : public SimpleDiagnostic
    {
        IMPLEMENT_DEFAULT_REFERENCE_COUNTING()
        DECLARE_FACTORY_REGISTERED(IndividualIVFactory, ActiveDiagnostic, IDistributableIntervention)

    public:
        ActiveDiagnostic();
        virtual bool Configure( const Configuration* pConfig ) override;
        virtual ~ActiveDiagnostic();

    protected:
        virtual bool positiveTestResult() override;

        DECLARE_SERIALIZABLE(ActiveDiagnostic);
    };
}
