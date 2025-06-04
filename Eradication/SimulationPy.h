
#pragma once

#include "Simulation.h"
#include "NodePy.h"
#include "InfectionPy.h"
#include "SusceptibilityPy.h"
#include "SimulationFactory.h"

namespace Kernel
{
    class NodePy;

    class SimulationPy : public Simulation
    {
    public:
        static SimulationPy *CreateSimulation();
        static SimulationPy *CreateSimulation(const ::Configuration *config);
        virtual ~SimulationPy(void) { }
        virtual void Reports_CreateBuiltIn();

    protected:
        SimulationPy();

        virtual void Initialize() override;
        virtual void Initialize(const ::Configuration *config) override;

        virtual bool ValidateConfiguration(const ::Configuration *config) override;

        // Allows correct type of community to be added by derived class Simulations
        virtual void addNewNodeFromDemographics( ExternalNodeId_t externalNodeId,
                                                 suids::suid node_suid,
                                                 NodeDemographicsFactory *nodedemographics_factory,
                                                 ClimateFactory *climate_factory ) override;

        virtual void resolveMigration();

    private:

        friend class Kernel::SimulationFactory; // allow them to create us
    };
}
