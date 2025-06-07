
#pragma once

#include <map>
#include "HIVSimpleDiagnostic.h"
#include "EventTrigger.h"

namespace Kernel
{
    class HIVRandomChoice : public HIVSimpleDiagnostic
    {
        IMPLEMENT_DEFAULT_REFERENCE_COUNTING()
        DECLARE_FACTORY_REGISTERED(IndividualIVFactory, HIVRandomChoice, IDistributableIntervention)

    public: 
        virtual bool Configure( const Configuration * inputJson ) override;

        // HIVSimpleDiagnostic
        virtual bool positiveTestResult() override;
        virtual void positiveTestDistribute() override;
        
    protected:
        virtual void ProcessChoices(std::vector<std::string> &names, std::vector<float> &values);

        std::vector<EventTrigger::Enum> event_names;
        std::vector<float> event_probabilities;

        DECLARE_SERIALIZABLE(HIVRandomChoice);
    };
}
