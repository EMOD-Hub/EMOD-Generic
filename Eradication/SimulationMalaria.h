
#pragma once

#include "MalariaContexts.h"
#include "SimulationVector.h"

namespace Kernel
{
    class SimulationMalaria : public SimulationVector
    {
        IMPLEMENT_DEFAULT_REFERENCE_COUNTING()

    public:
        static SimulationMalaria *CreateSimulation();
        static SimulationMalaria *CreateSimulation(const ::Configuration *config);
        virtual ~SimulationMalaria();

        // Allows correct type of community to be added by derived class Simulations
        virtual void addNewNodeFromDemographics( ExternalNodeId_t externalNodeId,
                                                 suids::suid node_suid,
                                                 NodeDemographicsFactory *nodedemographics_factory,
                                                 ClimateFactory *climate_factory ) override;

    protected:
        SimulationMalaria();

        virtual void Initialize() override;
        virtual void Initialize(const ::Configuration *config) override;

        virtual bool ValidateConfiguration(const ::Configuration *config) override;

        DECLARE_SERIALIZABLE(SimulationMalaria);

    private:
        virtual ISimulationContext *GetContextPointer() override;
    };
}
