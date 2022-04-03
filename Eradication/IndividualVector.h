/***************************************************************************************************

Copyright (c) 2018 Intellectual Ventures Property Holdings, LLC (IVPH) All rights reserved.

EMOD is licensed under the Creative Commons Attribution-Noncommercial-ShareAlike 4.0 License.
To view a copy of this license, visit https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode

***************************************************************************************************/

#pragma once

#include "BoostLibWrapper.h"

#include "VectorContexts.h"
#include "Individual.h"

#include "IInfection.h"
#include "IContagionPopulation.h"
#include "StrainIdentity.h"

namespace Kernel
{
    // To store randomly selected strains and their weights from various pools an individual is exposed to
    typedef std::pair<StrainIdentity, float> strain_exposure_t;

    class VectorInterventionsContainer;

    class IndividualHumanVector : public IndividualHuman, public IIndividualHumanVectorContext
    {
        friend class SimulationVector;
        friend class NodeVector;
        //friend class VectorPopulation;
        IMPLEMENT_DEFAULT_REFERENCE_COUNTING()
        DECLARE_QUERY_INTERFACE()
        //DECLARE_SERIALIZABLE( IndividualHumanVector )

    public:
        // TODO change double to float
        static IndividualHumanVector *CreateHuman(INodeContext *context, suids::suid _suid, double monte_carlo_weight = 1.0, double initial_age = 0.0, int gender = 0);
        virtual ~IndividualHumanVector();
        static void InitializeStaticsVector( const Configuration * config );

        virtual void CreateSusceptibility(float immunity_modifier = 1.0, float risk_modifier = 1.0) override;
        virtual void ExposeToInfectivity(float dt, TransmissionGroupMembership_t transmissionGroupMembership) override;
        virtual void UpdateInfectiousness(float dt) override;
        virtual void Expose( const IContagionPopulation* cp, float dt, TransmissionRoute::Enum tranmsission_route = TransmissionRoute::TRANSMISSIONROUTE_CONTACT ) override;

        virtual void UpdateGroupPopulation(float size_changes) override;

        // IIndividualHumanVectorContext methods
        virtual float GetRelativeBitingRate(void) const override;

        // These used to live in NodeVector but that makes it hard to support component-level operation of STI intrahost (pymod)
        // without linking in spurious classes.
        static TransmissionGroupMembership_t human_indoor;
        static TransmissionGroupMembership_t human_outdoor;
        static TransmissionGroupMembership_t vector_indoor;
        static TransmissionGroupMembership_t vector_outdoor;

    protected:
        // cumulative exposure by pool stored along with randomly selected strain from each pool + total exposure
        std::vector<strain_exposure_t>  m_strain_exposure; // cumulative strain exposure is sorted for fast weighted random strain selection (although now it is only indoor+outdoor)
        float m_total_exposure;
        IVectorSusceptibilityContext * vector_susceptibility;
        VectorInterventionsContainer * vector_interventions; // cache this so we don't have to QI for it all the time. It won't change over time, but careful with malaria sims

        // TODO change double to float
        IndividualHumanVector(suids::suid id = suids::nil_suid(), double monte_carlo_weight = 1.0, double initial_age = 0.0, int gender = 0);
        IndividualHumanVector(INodeContext *context);

        virtual IInfection *createInfection(suids::suid _suid) override;
       
        virtual void setupInterventionsContainer() override;
        virtual void ApplyTotalBitingExposure();

        virtual void PropagateContextToDependents() override;

        void ReportInfectionState() override;
    
        DECLARE_SERIALIZABLE(IndividualHumanVector);
    };

    IArchive& serialize(IArchive&, std::vector<strain_exposure_t>&);

    struct compare_strain_exposure_float_less
    {
        bool operator() (const strain_exposure_t &lhs, const float rhs)
        {
            return lhs.second < rhs;
        }
    };
}
