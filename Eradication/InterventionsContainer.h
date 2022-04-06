/***************************************************************************************************

Copyright (c) 2018 Intellectual Ventures Property Holdings, LLC (IVPH) All rights reserved.

EMOD is licensed under the Creative Commons Attribution-Noncommercial-ShareAlike 4.0 License.
To view a copy of this license, visit https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode

***************************************************************************************************/

#pragma once

#include <list>

#include "Interventions.h"

namespace Kernel
{
    // this container becomes a help implementation member of the relevant IndividualHuman class 
    // it needs to implement consumer interfaces for all the relevant intervention types
    // InterventionsContainer is a generic 

    struct IVaccineConsumer : public ISupports
    {
        virtual void UpdateVaccineAcquireRate(   float acq,  bool isMultiplicative = true ) = 0;
        virtual void UpdateVaccineTransmitRate(  float xmit, bool isMultiplicative = true ) = 0;
        virtual void UpdateVaccineMortalityRate( float mort, bool isMultiplicative = true ) = 0;

        virtual float GetInterventionReducedAcquire()   const = 0;
        virtual float GetInterventionReducedTransmit()  const = 0;
        virtual float GetInterventionReducedMortality() const = 0;
    };

    class InterventionsContainer : public IIndividualHumanInterventionsContext, 
                                   public IVaccineConsumer,
                                   public IInterventionConsumer
    {
        IMPLEMENT_DEFAULT_REFERENCE_COUNTING()

    public:
        InterventionsContainer();
        virtual ~InterventionsContainer();

        // IIndividualHumanInterventionsContext
        virtual void SetContextTo(IIndividualHumanContext* context) override;
        virtual IIndividualHumanContext* GetParent() override;
        virtual std::list<IDistributableIntervention*> GetInterventionsByType(const std::string& type_name) override;
        virtual std::list<IDistributableIntervention*> GetInterventionsByName(const std::string &intervention_name) override;
        virtual std::list<void*>                       GetInterventionsByInterface( iid_t iid ) override;
        virtual void PurgeExisting( const std::string& iv_name ) override;
        virtual void PurgeExistingByName( const std::string& iv_name ) override;
        virtual bool ContainsExisting( const std::string &iv_name ) override;
        virtual bool ContainsExistingByName( const std::string &name ) override;
        virtual void ChangeProperty( const char *property, const char* new_value ) override;

        // IUnknown
        virtual QueryResult QueryInterface(iid_t iid, void** pinstance) override;

        // IVaccineConsumer
        virtual void UpdateVaccineAcquireRate(   float acq,  bool isMultiplicative = true ) override;
        virtual void UpdateVaccineTransmitRate(  float xmit, bool isMultiplicative = true ) override;
        virtual void UpdateVaccineMortalityRate( float mort, bool isMultiplicative = true ) override;

        virtual float GetInterventionReducedAcquire()   const override;
        virtual float GetInterventionReducedTransmit()  const override;
        virtual float GetInterventionReducedMortality() const override;

        virtual bool GiveIntervention( IDistributableIntervention * pIV ) override;

        virtual void PreInfectivityUpdate( float dt );  // update only interventions that need updating before accumulating infectivity
        virtual void InfectiousLoopUpdate( float dt );  // update only interventions that need updating in Infectious Update loop
        virtual void Update( float dt );                // update non-infectious loop update interventions once per time step

    protected:
        float drugVaccineReducedAcquire;
        float drugVaccineReducedTransmit;
        float drugVaccineReducedMortality;
        std::list<IDistributableIntervention*> interventions;

        virtual void PropagateContextToDependents(); // pass context to interventions if they need it

        IIndividualHumanContext *parent;    // context for this interventions container

    private:
        DECLARE_SERIALIZABLE(InterventionsContainer);
    };
}
