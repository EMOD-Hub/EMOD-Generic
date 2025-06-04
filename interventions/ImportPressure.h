
#pragma once

#include "Interventions.h"
#include "Configuration.h"
#include "InterventionFactory.h"
#include "InterventionEnums.h"
#include "EventCoordinator.h"
#include "Configure.h"
#include "StrainIdentity.h"
#include "Common.h"
#include "Outbreak.h"
#include "Types.h"
#include <vector>

namespace Kernel
{
    class ImportPressure : public Outbreak
    {
        IMPLEMENT_DEFAULT_REFERENCE_COUNTING()
        DECLARE_FACTORY_REGISTERED(NodeIVFactory, ImportPressure, INodeDistributableIntervention)

    public:
        ImportPressure();
        virtual ~ImportPressure();

        virtual bool Configure(const Configuration* config) override;
        virtual void Update(float dt);
        virtual void SetContextTo(INodeEventContext *context);
        virtual bool Distribute(INodeEventContext *context, IEventCoordinator2* pEC);

    protected:
        INodeEventContext* parent;

        float duration_counter;

        std::vector<float> durations; 
        std::vector<float> daily_import_pressures;

        std::vector<std::pair<float, float>> durations_and_pressures;
    };
}
