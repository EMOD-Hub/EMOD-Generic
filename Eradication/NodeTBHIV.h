/***************************************************************************************************

Copyright (c) 2018 Intellectual Ventures Property Holdings, LLC (IVPH) All rights reserved.

EMOD is licensed under the Creative Commons Attribution-Noncommercial-ShareAlike 4.0 License.
To view a copy of this license, visit https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode

***************************************************************************************************/

#pragma once
#include "stdafx.h"

#include "NodeTB.h"
#include "IndividualCoInfection.h" // for serialization junk
#include "TBHIVContexts.h"


namespace Kernel
{
    class SpatialReportTB;
    class ReportTBHIV; 
    class CD4TrajectoryChangeObserver : public IIndividualEventObserver
    {
    virtual int32_t AddRef() { return 1; };
    virtual int32_t Release() { return 0; };
    public:
        virtual bool notifyOnEvent(IIndividualHumanEventContext *context, const EventTrigger::Enum& trigger);
    };


    class NodeTBHIV : public NodeTB, public INodeTBHIV
    {
        IMPLEMENT_DEFAULT_REFERENCE_COUNTING()

    public:
        virtual ~NodeTBHIV(void); 
        static NodeTBHIV *CreateNode(ISimulationContext *_parent_sim, ExternalNodeId_t externalNodeId, suids::suid node_suid); 
        virtual void Initialize();

        virtual INodeTBHIV* GetNodeTBHIV() override;

        virtual void SetNewInfectionState(InfectionStateChange::_enum inf_state_change, IndividualHuman *ih) override;

        virtual const NodeDemographicsDistribution* GetHIVCoinfectionDistribution() const override { return HIVCoinfectionDistribution; }
        virtual const NodeDemographicsDistribution* GetHIVMortalityDistribution()   const override { return HIVMortalityDistribution; }

    protected:
        NodeTBHIV();
        NodeTBHIV(ISimulationContext *_parent_sim, ExternalNodeId_t externalNodeId, suids::suid node_suid);
         virtual IIndividualHuman* addNewIndividual(
            float monte_carlo_weight = 1.0,
            float initial_age = 0,
            int gender = 0,
            int initial_infections = 0,
            float immunity_parameter = 1.0,
            float risk_parameter = 1.0);

         // Factory methods
        virtual IIndividualHuman *createHuman(suids::suid suid, float monte_carlo_weight, float initial_age, int gender);
        
        //observers in the node and setup methods
        CD4TrajectoryChangeObserver* cd4observer;
        void RegisterObservers();
  
        virtual void processEmigratingIndividual(IIndividualHuman *i) override;
        virtual IIndividualHuman* processImmigratingIndividual(IIndividualHuman* immigrant) override;

        virtual void resetNodeStateCounters( void ) override;

        virtual void LoadOtherDiseaseSpecificDistributions(const NodeDemographics* demog_ptr) override;

        NodeDemographicsDistribution* HIVCoinfectionDistribution;
        NodeDemographicsDistribution* HIVMortalityDistribution;

        DECLARE_SERIALIZABLE(NodeTBHIV);
    };



}
