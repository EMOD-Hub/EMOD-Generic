
#pragma once

#include <string>
#include <list>
#include <vector>

#include "DiagnosticsTreatNeg.h"

namespace Kernel
{
    class MDRDiagnostic : public DiagnosticTreatNeg 
    {
        IMPLEMENT_DEFAULT_REFERENCE_COUNTING()
        DECLARE_FACTORY_REGISTERED(IndividualIVFactory, MDRDiagnostic, IDistributableIntervention)

    public: 
        MDRDiagnostic();
        virtual ~MDRDiagnostic();// { }
        virtual bool Configure( const Configuration* pConfig ) override;
        virtual bool positiveTestResult() override;
        virtual float getTreatmentFractionNegative() const override;

    protected:

        float treatment_fraction_neg;
    };
}
