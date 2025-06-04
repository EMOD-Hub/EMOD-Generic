
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
