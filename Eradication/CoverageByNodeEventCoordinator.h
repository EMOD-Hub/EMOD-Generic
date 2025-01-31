/***************************************************************************************************

Copyright (c) 2018 Intellectual Ventures Property Holdings, LLC (IVPH) All rights reserved.

EMOD is licensed under the Creative Commons Attribution-Noncommercial-ShareAlike 4.0 License.
To view a copy of this license, visit https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode

***************************************************************************************************/

#pragma once

#include "StandardEventCoordinator.h"

namespace Kernel
{
    class CoverageByNodeEventCoordinator : public StandardInterventionDistributionEventCoordinator
    {
        DECLARE_FACTORY_REGISTERED_EXPORT(EventCoordinatorFactory, CoverageByNodeEventCoordinator, IEventCoordinator)
        IMPLEMENT_DEFAULT_REFERENCE_COUNTING()

    public:
        CoverageByNodeEventCoordinator();
        virtual bool Configure( const Configuration * inputJson );

    protected:
        virtual bool TargetedIndividualIsCovered(IIndividualHumanEventContext *ihec);

        std::map<uint32_t, float> node_coverage_map;
    };
}
