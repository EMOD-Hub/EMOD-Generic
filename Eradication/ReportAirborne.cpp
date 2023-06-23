/***************************************************************************************************

Copyright (c) 2018 Intellectual Ventures Property Holdings, LLC (IVPH) All rights reserved.

EMOD is licensed under the Creative Commons Attribution-Noncommercial-ShareAlike 4.0 License.
To view a copy of this license, visit https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode

***************************************************************************************************/

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
