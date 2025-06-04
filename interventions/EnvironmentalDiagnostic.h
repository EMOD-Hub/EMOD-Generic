
#pragma once

#include "Configuration.h"
#include "Configure.h"
#include "InterventionFactory.h"
#include "Interventions.h"
#include "INodeContext.h"

namespace Kernel
{
    class EnvironmentalDiagnostic: public BaseNodeIntervention
    {
        IMPLEMENT_DEFAULT_REFERENCE_COUNTING()
        DECLARE_FACTORY_REGISTERED(NodeIVFactory, EnvironmentalDiagnostic, INodeDistributableIntervention)

    public:
        EnvironmentalDiagnostic();
        EnvironmentalDiagnostic(const EnvironmentalDiagnostic&);
        virtual ~EnvironmentalDiagnostic() {};
        virtual bool Configure(const Configuration* pConfig);
        virtual void Update(float dt) override;
        virtual bool Distribute(INodeEventContext *context, IEventCoordinator2* pEC = nullptr ) override;

    protected:
        virtual void performTest();
        float sample_threshold;
        float base_specificity;
        float base_sensitivity;
        EventTrigger::Enum negative_diagnosis_event;
        EventTrigger::Enum positive_diagnosis_event;
        IPKeyValue environment_ip_key_value;
    };
}
