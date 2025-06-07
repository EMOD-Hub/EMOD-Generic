
#pragma once

#include "Report.h"
#include "TransmissionGroupMembership.h"
#include "ISimulationContext.h"

namespace Kernel
{
    class ReportTyphoid : public Report
    {
        GET_SCHEMA_STATIC_WRAPPER(ReportTyphoid)
        public:
            ReportTyphoid();
            virtual ~ReportTyphoid() {};

            static IReport* ReportTyphoid::CreateReport() { return new ReportTyphoid(); }

            virtual void BeginTimestep() override;

            virtual void LogIndividualData( IIndividualHuman * individual);
            virtual void LogNodeData( Kernel::INodeContext * pNC );

        private:
            float chron_carriers_counter;
            float subclinical_infections_counter;
            float acute_infections_counter;
    };
}
