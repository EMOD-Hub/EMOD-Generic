/***************************************************************************************************

Copyright (c) 2018 Intellectual Ventures Property Holdings, LLC (IVPH) All rights reserved.

EMOD is licensed under the Creative Commons Attribution-Noncommercial-ShareAlike 4.0 License.
To view a copy of this license, visit https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode

***************************************************************************************************/

#pragma once

#include "SimulationFactory.h"
#include "Simulation.h"

namespace Kernel
{
    class SimulationEnvironmental : public Simulation
    {
    public:
        static SimulationEnvironmental *CreateSimulation();
        static SimulationEnvironmental *CreateSimulation(const ::Configuration *config);
        virtual ~SimulationEnvironmental(void);

    protected:
        SimulationEnvironmental();

        virtual void Initialize() override;
        virtual void Initialize(const ::Configuration *config) override;

        virtual bool ValidateConfiguration(const ::Configuration *config) override;

        // Allows correct type of Node to be added by derived class Simulations
        virtual void addNewNodeFromDemographics( ExternalNodeId_t externalNodeId,
                                                 suids::suid node_suid,
                                                 NodeDemographicsFactory *nodedemographics_factory,
                                                 ClimateFactory *climate_factory ) override;

    private:
        friend class Kernel::SimulationFactory; // allow them to create us
    };
}
