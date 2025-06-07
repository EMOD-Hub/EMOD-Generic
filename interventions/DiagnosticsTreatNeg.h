
#pragma once

#include <string>
#include <list>
#include <vector>

#include "Diagnostics.h"

namespace Kernel
{
    class DiagnosticTreatNeg : public SimpleDiagnostic
    {
        IMPLEMENT_DEFAULT_REFERENCE_COUNTING()
        DECLARE_FACTORY_REGISTERED(IndividualIVFactory, DiagnosticTreatNeg, IDistributableIntervention)

    public:
        DiagnosticTreatNeg();
        DiagnosticTreatNeg( const DiagnosticTreatNeg& master );
        virtual ~DiagnosticTreatNeg();
        virtual bool Configure( const Configuration* pConfig ) override;

        virtual bool Distribute(IIndividualHumanInterventionsContext *context, ICampaignCostObserver * const pICCO ) override;
        virtual void Update(float dt) override;

        virtual bool positiveTestResult() override;
        void onDiagnosisComplete( float dt );
        virtual void onNegativeTestResult() override;
        virtual void negativeTestDistribute();
        virtual void onPatientDefault() override;

    protected:
        virtual float getTreatmentFractionNegative() const;

        EventTrigger::Enum negative_diagnosis_event;
        IndividualInterventionConfig negative_diagnosis_config;
        EventTrigger::Enum defaulters_event;
        IndividualInterventionConfig defaulters_config;

    private:
        bool m_gets_positive_test_intervention;

        DECLARE_SERIALIZABLE(DiagnosticTreatNeg);
    };
}
