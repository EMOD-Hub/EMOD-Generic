
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
                                                 ClimateFactory *climate_factory ) override;
    
        DECLARE_SERIALIZABLE(SimulationAirborne);
    };

}
