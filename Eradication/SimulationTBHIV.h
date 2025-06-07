
#pragma once
#include "SimulationAirborne.h"
#include "NodeTBHIV.h" // for serialization forward reg ONLY
#include "IndividualCoInfection.h"

namespace Kernel
{
    class SimulationTBHIV : public SimulationAirborne
    {
        IMPLEMENT_DEFAULT_REFERENCE_COUNTING()
       
    public:
        static   SimulationTBHIV *CreateSimulation();
        static   SimulationTBHIV *CreateSimulation(const ::Configuration *config);
        virtual ~SimulationTBHIV(void);

    protected:
        SimulationTBHIV();

        virtual void Initialize() override;
        virtual void Initialize( const ::Configuration *config ) override;

        virtual bool ValidateConfiguration(const ::Configuration *config) override;

        // Allows correct type of Node to be added by classes derived from Simulation
        virtual void addNewNodeFromDemographics( ExternalNodeId_t externalNodeId,
                                                 suids::suid node_suid,
                                                 NodeDemographicsFactory *nodedemographics_factory,
                                                 ClimateFactory *climate_factory ) override;
        
        DECLARE_SERIALIZABLE(SimulationTBHIV);
    };

   
}
