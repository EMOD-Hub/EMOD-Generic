/***************************************************************************************************

Copyright (c) 2018 Intellectual Ventures Property Holdings, LLC (IVPH) All rights reserved.

EMOD is licensed under the Creative Commons Attribution-Noncommercial-ShareAlike 4.0 License.
To view a copy of this license, visit https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode

***************************************************************************************************/

#pragma once

#include <string>
#include <list>
#include <vector>

#include "BoostLibWrapper.h"

#include "Interventions.h"
#include "Configuration.h"
#include "InterventionFactory.h"
#include "InterventionEnums.h"
#include "Configure.h"
#include "HealthSeekingBehavior.h"

namespace Kernel
{
    class TargetAgeArrayConfig : public JsonConfigurable, public IComplexJsonConfigurable
    {
        IMPLEMENT_DEFAULT_REFERENCE_COUNTING()
        virtual QueryResult QueryInterface(iid_t iid, void **ppvObject) { return e_NOINTERFACE; }

        public:
            TargetAgeArrayConfig() {}
            virtual void ConfigureFromJsonAndKey( const Configuration* inputJson, const std::string& key ) override;
            virtual json::QuickBuilder GetSchema() override;
            virtual bool  HasValidDefault() const override { return false; }
            std::map< float, float > age2ProbabilityMap;
            bool dropout;

            static void serialize(IArchive&, TargetAgeArrayConfig&);
    };

    class IVCalendar : public BaseIntervention
    {
        DECLARE_FACTORY_REGISTERED(InterventionFactory, IVCalendar, IDistributableIntervention)

    public:
        // We inherit AddRef/Release abstractly through IHealthSeekBehavior,
        // even though BaseIntervention has a non-abstract version.
        virtual int32_t AddRef() override { return BaseIntervention::AddRef(); }
        virtual int32_t Release() override { return BaseIntervention::Release(); }

        IVCalendar();
        virtual ~IVCalendar();
        bool Configure( const Configuration* config ) override;

        // IDistributingDistributableIntervention
        virtual bool Distribute(IIndividualHumanInterventionsContext *context, ICampaignCostObserver * const pICCO ) override;
        virtual QueryResult QueryInterface(iid_t iid, void **ppvObject) override;
        virtual void Update(float dt) override;

    protected:
        TargetAgeArrayConfig target_age_array; // json list of floats
        IndividualInterventionConfig actual_intervention_config;
        bool dropout;

        DECLARE_SERIALIZABLE(IVCalendar);

    private:
        std::list<float> scheduleAges;
        std::string dumpCalendar();
    };
}
