
#include "stdafx.h"
#include "ConfigParams.h"
#include "CampaignEventByYear.h"
#ifndef DISABLE_STI
#include "SimulationSTI.h"
#endif
#ifdef ENABLE_TYPHOID
#include "SimulationTyphoid.h"
#endif

SETUP_LOGGING("CampaignEventByYear")

namespace Kernel
{
#if !defined(DISABLE_STI) || defined(ENABLE_TYPHOID)
    IMPLEMENT_FACTORY_REGISTERED(CampaignEventByYear)

    CampaignEventByYear::CampaignEventByYear()
        : start_year(0.0f)
    { }

    CampaignEventByYear::~CampaignEventByYear()
    { }

    bool CampaignEventByYear::Configure(const Configuration* inputJson)
    {
        initConfigTypeMap( "Start_Year", &start_year, Start_Year_DESC_TEXT, MIN_YEAR, MAX_YEAR, MIN_YEAR );
        initConfigComplexType( "Nodeset_Config", &nodeset_config, Nodeset_Config_DESC_TEXT );
        initConfigComplexType( "Event_Coordinator_Config", &event_coordinator_config, Event_Coordinator_Config_DESC_TEXT );

        // Bypasss CampaignEvent base class so that we don't break without Start_Day!
        bool ret = JsonConfigurable::Configure( inputJson );

        return ret;
    }

    bool CampaignEventByYear::Validate( const ISimulationContext* parent_sim )
    {
        if( parent_sim->GetSimParams().sim_type != SimType::STI_SIM    &&
            parent_sim->GetSimParams().sim_type != SimType::HIV_SIM    &&
            parent_sim->GetSimParams().sim_type != SimType::TYPHOID_SIM  )
        {
            throw IllegalOperationException( __FILE__, __LINE__, __FUNCTION__, "CampainEventByYear can only be used in TYPHOID, STI, and HIV simulations." );
        }

        start_day = (start_year - parent_sim->GetSimParams().sim_time_base_year) * DAYSPERYEAR;

        return true;
    }

#endif
}
