
#pragma once

#include "NodeLevelHealthTriggeredIV.h"
#include "InterpolatedValueMap.h"
#include "InterventionEnums.h"

namespace Kernel
{
    class NodeLevelHealthTriggeredIVScaleUpSwitch : public NodeLevelHealthTriggeredIV
    {
        DECLARE_FACTORY_REGISTERED(NodeIVFactory, NodeLevelHealthTriggeredIVScaleUpSwitch, INodeDistributableIntervention)

    public:        
        NodeLevelHealthTriggeredIVScaleUpSwitch();
        virtual ~NodeLevelHealthTriggeredIVScaleUpSwitch();
        virtual bool Configure( const Configuration* config ) override;

    protected:

        ScaleUpProfile::Enum demographic_coverage_time_profile;
        float initial_demographic_coverage;
        float primary_time_constant;
        InterpolatedValueMap coverage_vs_time_map;

        virtual float getDemographicCoverage() const override;

        virtual void onDisqualifiedByCoverage( IIndividualHumanEventContext *pIndiv );
        IndividualInterventionConfig not_covered_intervention_configs; // TBD
        bool campaign_contains_not_covered_config;
    };
}
