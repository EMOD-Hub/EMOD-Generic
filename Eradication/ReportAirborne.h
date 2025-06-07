
#pragma once

#include "Report.h"

class ReportAirborne : public Report
{
public:
    ReportAirborne();
    virtual ~ReportAirborne(){};

protected:
    virtual void populateSummaryDataUnitsMap( std::map<std::string, std::string> &units_map );
};
