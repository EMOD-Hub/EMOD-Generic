
#include "stdafx.h"

#include <string>
#include "STIReportEventRecorder.h"
#include "IdmDateTime.h"
#include "NodeEventContext.h"

SETUP_LOGGING( "STIReportEventRecorder" )

namespace Kernel
{
    GET_SCHEMA_STATIC_WRAPPER_IMPL( STIReportEventRecorder, STIReportEventRecorder )

    IReport* STIReportEventRecorder::CreateReport()
    {
        return new STIReportEventRecorder();
    }

    STIReportEventRecorder::STIReportEventRecorder()
        : ReportEventRecorder()
    {
    }

    STIReportEventRecorder::~STIReportEventRecorder()
    {
    }

    std::string STIReportEventRecorder::GetTimeHeader() const
    {
        return "Year";
    }

    float STIReportEventRecorder::GetTime( IIndividualHumanEventContext* pEntity ) const
    {
        return pEntity->GetNodeEventContext()->GetTime().Year();
    }
}
