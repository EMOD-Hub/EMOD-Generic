/***************************************************************************************************

Copyright (c) 2018 Intellectual Ventures Property Holdings, LLC (IVPH) All rights reserved.

EMOD is licensed under the Creative Commons Attribution-Noncommercial-ShareAlike 4.0 License.
To view a copy of this license, visit https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode

***************************************************************************************************/

#pragma once
#include "Simulation.h"
#include "IndividualAirborne.h"
#include "Sugar.h" // for DECLARE_VIRTUAL_BASE

namespace Kernel
{
    class SimulationAirborne : public Simulation
    {
        IMPLEMENT_DEFAULT_REFERENCE_COUNTING()
    public:
        virtual ~SimulationAirborne(void);
        static SimulationAirborne *CreateSimulation();
        static SimulationAirborne *CreateSimulation(const ::Configuration *config);

    protected:
        SimulationAirborne();

        virtual void Initialize() override;
        virtual void Initialize(const ::Configuration *config) override;

        virtual bool ValidateConfiguration(const ::Configuration *config) override;

        // Allows correct type of Node to be added by classes derived from Simulation
        virtual void addNewNodeFromDemographics( ExternalNodeId_t externalNodeId,
                                                 suids::suid node_suid,
                                                 NodeDemographicsFactory *nodedemographics_factory,
                                                 ClimateFactory *climate_factory,
                                                 bool white_list_enabled ) override;
    
        DECLARE_SERIALIZABLE(SimulationAirborne);
    };

}
