
#pragma once

#include "HIVSimpleDiagnostic.h"
#include "IHIVInterventionsContainer.h"

namespace Kernel
{
    struct IHIVMedicalHistory;

    class HIVRapidHIVDiagnostic : public HIVSimpleDiagnostic
    {
        IMPLEMENT_DEFAULT_REFERENCE_COUNTING()
        DECLARE_FACTORY_REGISTERED(IndividualIVFactory, HIVRapidHIVDiagnostic, IDistributableIntervention)

    public: 
        HIVRapidHIVDiagnostic();
        HIVRapidHIVDiagnostic( const HIVRapidHIVDiagnostic& );

    protected:
        virtual void onNegativeTestResult() override;
        virtual void positiveTestDistribute() override;
        virtual void onReceivedResult( IHIVMedicalHistory* pMedHistory, bool resultIsHivPositive );

        float m_ProbReceivedResults;

        DECLARE_SERIALIZABLE(HIVRapidHIVDiagnostic);
    };
}
