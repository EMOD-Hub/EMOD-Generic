/***************************************************************************************************

Copyright (c) 2018 Intellectual Ventures Property Holdings, LLC (IVPH) All rights reserved.

EMOD is licensed under the Creative Commons Attribution-Noncommercial-ShareAlike 4.0 License.
To view a copy of this license, visit https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode

***************************************************************************************************/

#include "stdafx.h"

#pragma warning(disable:4996)

#ifdef ENABLE_PYTHON

#include <numeric> // for std::accumulate
#include "ReportPy.h" // for base class
#include "NodePy.h" // for base class
#include "Debug.h" // for base class

SETUP_LOGGING( "ReportPy" )


namespace Kernel {

static const std::string _num_chronic_carriers_label     = "Number of Chronic Carriers";
static const std::string _num_subclinic_infections_label = "Number of New Sub-Clinical Infections";
static const std::string _num_acute_infections_label     = "Number of New Acute Infections";

GET_SCHEMA_STATIC_WRAPPER_IMPL(ReportPy,ReportPy)

ReportPy::ReportPy()
{
}

bool ReportPy::Configure( const Configuration * inputJson )
{
    bool ret = JsonConfigurable::Configure( inputJson );
    return ret ;
}

void ReportPy::EndTimestep( float currentTime, float dt )
{
    Report::EndTimestep( currentTime, dt );

    // Make sure we push at least one zero per timestep
    Accumulate( _num_chronic_carriers_label, 0 );
    Accumulate( _num_subclinic_infections_label, 0 );
    Accumulate( _num_acute_infections_label, 0 );
}

void ReportPy::postProcessAccumulatedData()
{
    LOG_DEBUG( "postProcessAccumulatedData\n" );
    Report::postProcessAccumulatedData();
}

void ReportPy::populateSummaryDataUnitsMap( std::map<std::string, std::string> &units_map )
{
    Report::populateSummaryDataUnitsMap(units_map);
}

void ReportPy::LogIndividualData( IIndividualHuman* individual )
{
    Report::LogIndividualData( individual );
}

void ReportPy::LogNodeData( INodeContext* pNC )
{
    Report::LogNodeData( pNC );
}

}

#endif // ENABLE_PYTHON
