
#pragma once

#include <string>
#include <list>
#include <vector>

#include "Diagnostics.h"

namespace Kernel
{
    class SmearDiagnostic : public SimpleDiagnostic 
    {
        IMPLEMENT_DEFAULT_REFERENCE_COUNTING()
        DECLARE_FACTORY_REGISTERED(IndividualIVFactory, SmearDiagnostic, IDistributableIntervention)

    public: 
        SmearDiagnostic();
        SmearDiagnostic( const SmearDiagnostic& );
        virtual ~SmearDiagnostic();// { }
        virtual bool Configure( const Configuration* pConfig ) override;
        virtual bool positiveTestResult() override;

        DECLARE_SERIALIZABLE(SmearDiagnostic);
    };
}
