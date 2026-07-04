
#pragma once

#include <string>
#include <list>
#include <vector>

#include "Diagnostics.h"

namespace Kernel
{
    class TyphoidCarrierDiagnostic : public SimpleDiagnostic
    {
        IMPLEMENT_DEFAULT_REFERENCE_COUNTING()
        DECLARE_FACTORY_REGISTERED(IndividualIVFactory, TyphoidCarrierDiagnostic, IDistributableIntervention)

    public:
        TyphoidCarrierDiagnostic();
        virtual bool Configure( const Configuration* pConfig ) override;
        virtual ~TyphoidCarrierDiagnostic();

    protected:
        virtual bool positiveTestResult() override;

        DECLARE_SERIALIZABLE(TyphoidCarrierDiagnostic);
    };
}

