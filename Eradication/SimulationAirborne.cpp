/***************************************************************************************************

Copyright (c) 2018 Intellectual Ventures Property Holdings, LLC (IVPH) All rights reserved.

EMOD is licensed under the Creative Commons Attribution-Noncommercial-ShareAlike 4.0 License.
To view a copy of this license, visit https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode

***************************************************************************************************/

#include "stdafx.h"

#ifndef DISABLE_AIRBORNE

#include "SimulationAirborne.h"

#include "InfectionAirborne.h"
#include "NodeAirborne.h"
#include "SusceptibilityAirborne.h"

SETUP_LOGGING( "SimulationAirborne" )

namespace Kernel
{
    SimulationAirborne::~SimulationAirborne(void) { }
    SimulationAirborne::SimulationAirborne() { }

    void SimulationAirborne::Initialize()
    {
        Simulation::Initialize();
    }

    void SimulationAirborne::Initialize(const ::Configuration *config)
    {
        Simulation::Initialize(config);

        InfectionAirborneConfig  air_infection_config;

        air_infection_config.Configure( config );
    }

    SimulationAirborne *SimulationAirborne::CreateSimulation()
    {
        SimulationAirborne *newsimulation = _new_ SimulationAirborne();
        newsimulation->Initialize();

        return newsimulation;
    }

    SimulationAirborne *SimulationAirborne::CreateSimulation(const ::Configuration *config)
    {
        SimulationAirborne *newsimulation = _new_ SimulationAirborne();

        if (newsimulation)
        {
            // This sequence is important: first
            // Creation-->Initialization-->Validation
            newsimulation->Initialize(config);
            if(!newsimulation->ValidateConfiguration(config))
            {
                delete newsimulation;
                throw GeneralConfigurationException( __FILE__, __LINE__, __FUNCTION__, "AIRBORNE_SIM requested with invalid configuration." );
            }
        }

        return newsimulation;
    }

    bool SimulationAirborne::ValidateConfiguration(const ::Configuration *config)
    {
        // TODO: any disease-specific validation goes here.

        return Simulation::ValidateConfiguration(config);
    }

    void SimulationAirborne::addNewNodeFromDemographics( ExternalNodeId_t externalNodeId,
                                                         suids::suid node_suid,
                                                         NodeDemographicsFactory *nodedemographics_factory, 
                                                         ClimateFactory *climate_factory, 
                                                         bool white_list_enabled )
    {
        NodeAirborne *node = NodeAirborne::CreateNode(this, externalNodeId, node_suid);
        addNode_internal( node, nodedemographics_factory, climate_factory, white_list_enabled );
    }

    REGISTER_SERIALIZABLE(SimulationAirborne);

    void SimulationAirborne::serialize(IArchive& ar, SimulationAirborne* obj)
    {
        Simulation::serialize(ar, obj);
        // Nothing to do here
    }
}

#endif // DISABLE_AIRBORNE
