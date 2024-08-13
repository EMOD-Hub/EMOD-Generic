/***************************************************************************************************

Copyright (c) 2018 Intellectual Ventures Property Holdings, LLC (IVPH) All rights reserved.

EMOD is licensed under the Creative Commons Attribution-Noncommercial-ShareAlike 4.0 License.
To view a copy of this license, visit https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode

***************************************************************************************************/

#pragma once

#include <map>
#include "ReportSTI.h"
#include "IndividualEventContext.h"
#include "Interventions.h"

namespace Kernel 
{
    class ReportHIV : public ReportSTI, 
                      public IIndividualEventObserver
    {
        GET_SCHEMA_STATIC_WRAPPER(ReportHIV)
    public:
        ReportHIV();
        virtual ~ReportHIV();

        static IReport* ReportHIV::CreateReport() { return new ReportHIV(); }

        virtual bool Configure( const Configuration* inputJson ) override;
        virtual void UpdateEventRegistration( float currentTime, 
                                              float dt, 
                                              std::vector<INodeEventContext*>& rNodeEventContextList,
                                              ISimulationEventContext* pSimEventContext ) override;

        virtual void LogIndividualData( IIndividualHuman* individual ) override;
        virtual void EndTimestep( float currentTime, float dt ) override;
        virtual void Reduce() override;

        // for IIndividualEventObserver
        virtual bool notifyOnEvent( IIndividualHumanEventContext *context, 
                                    const EventTrigger::Enum& trigger ) override;

        // ISupports
        IMPLEMENT_DEFAULT_REFERENCE_COUNTING()

    protected:
        virtual void populateSummaryDataUnitsMap( std::map<std::string, std::string> &units_map ) override;
        virtual void postProcessAccumulatedData() override;

        float num_acute;
        float num_latent;
        float num_aids_without;
        float num_aids_with;
        float num_hiv_cd4_lo_non_ART;
        float num_hiv_cd4_hi_non_ART;
        float num_hiv_cd4_lo_on_ART;
        float num_hiv_cd4_hi_on_ART;
        float num_on_ART;
        float num_ART_dropouts;
        unsigned int num_events;
        bool counting_all_events;
        std::vector<uint32_t> event_counter_vector; // indexed by EventTrigger index

        std::vector< EventTrigger::Enum > eventTriggerList;
        std::vector<IIndividualEventBroadcaster*> broadcaster_list;
    };
}
