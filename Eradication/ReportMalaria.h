
#pragma once

#include "ReportVector.h"

namespace Kernel
{
    class ReportMalaria : public ReportVector
    {
    public:
        ReportMalaria();
        virtual ~ReportMalaria() {};

        static IReport* ReportMalaria::CreateReport() { return new ReportMalaria(); }

        virtual void LogNodeData( Kernel::INodeContext * pNC );

    protected:
        virtual void populateSummaryDataUnitsMap( std::map<std::string, std::string> &units_map );
        virtual void postProcessAccumulatedData();
    };
}
