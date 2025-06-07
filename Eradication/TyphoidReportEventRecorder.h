
#pragma once

#include "ReportEventRecorder.h"

namespace Kernel
{
    class TyphoidReportEventRecorder : public ReportEventRecorder
    {
        GET_SCHEMA_STATIC_WRAPPER( TyphoidReportEventRecorder )
    public:
        static IReport* CreateReport();

    protected:
        TyphoidReportEventRecorder();
        virtual ~TyphoidReportEventRecorder();

    protected:
        virtual std::string GetTimeHeader() const;
        virtual float GetTime( IIndividualHumanEventContext* pEntity ) const override;
    };
}
