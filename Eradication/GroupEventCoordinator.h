
#pragma once

#include <string>
#include <list>
#include <vector>

#include "StandardEventCoordinator.h"

namespace Kernel
{

    class GroupInterventionDistributionEventCoordinator : public StandardInterventionDistributionEventCoordinator
    {
        DECLARE_FACTORY_REGISTERED(EventCoordinatorFactory, GroupInterventionDistributionEventCoordinator, IEventCoordinator)    
    public:
        IMPLEMENT_DEFAULT_REFERENCE_COUNTING()
        GroupInterventionDistributionEventCoordinator();

        virtual bool Configure(const Configuration* config) override;
        virtual bool qualifiesDemographically( IIndividualHumanEventContext* pIndividual ) override;

    protected:

        TargetGroupType::Enum target_disease_state;
    };
}
