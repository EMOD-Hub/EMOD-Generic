
#include "stdafx.h"

#ifndef DISABLE_AIRBORNE

#include "ReportAirborne.h" // for base class

ReportAirborne::ReportAirborne()
{}

void ReportAirborne::populateSummaryDataUnitsMap(std::map<std::string, std::string> &units_map)
{
    Report::populateSummaryDataUnitsMap(units_map);
}

#endif // DISABLE_AIRBORNE
