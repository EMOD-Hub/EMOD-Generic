
#pragma once

#include <sqlite3.h>
#include "ReportEventRecorder.h"

namespace Kernel
{
    class IndividualForSQL
    {
    public:
        IndividualForSQL();
        virtual ~IndividualForSQL();

        unsigned int time;
        unsigned int node_id;
        unsigned int individual_id;
        std::string  event;
        float        mcw;
        float        age;
        uint64_t     label;
        std::string  IPs;
    };


    class SQLReporter : public ReportEventRecorder
    {
        GET_SCHEMA_STATIC_WRAPPER( SQLReporter )
    public:
        static IReport* CreateReport();

    protected:
        SQLReporter();
        virtual ~SQLReporter();

        virtual bool Configure( const Configuration* inputJson ) override;
        virtual void Initialize( unsigned int nrmSize )          override;
        virtual void EndTimestep( float currentTime, float dt )  override;
        virtual void Finalize()                                  override;

    protected:
        virtual bool notifyOnEvent( IIndividualHumanEventContext* context, const EventTrigger::Enum& trigger )  override;

        sqlite3* db;

        float busy_timeout;
        float start_time;

        std::vector<IndividualForSQL> event_list;

        jsonConfigurable::tDynamicStringSet sql_properties_to_report;
    };
}
