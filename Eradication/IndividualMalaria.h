/***************************************************************************************************

Copyright (c) 2018 Intellectual Ventures Property Holdings, LLC (IVPH) All rights reserved.

EMOD is licensed under the Creative Commons Attribution-Noncommercial-ShareAlike 4.0 License.
To view a copy of this license, visit https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode

***************************************************************************************************/

#pragma once

#include "IndividualVector.h"
#include "IMalariaAntibody.h"
#include "MalariaContexts.h"

namespace Kernel
{
    struct IIndividualHumanEventContext;
    struct IIndividualHumanInterventionsContext;
    struct IIndividualHumanEventContext;

    class IndividualHumanMalariaConfig : public JsonConfigurable 
    {
        friend class SimulationMalaria;

        IMPLEMENT_DEFAULT_REFERENCE_COUNTING()
        GET_SCHEMA_STATIC_WRAPPER( IndividualHumanMalariaConfig )

    public:
        virtual bool Configure( const Configuration* config );

        static float mean_sporozoites_per_bite;
        static float base_sporozoite_survival_fraction;
        static float antibody_csp_killing_threshold;
        static float antibody_csp_killing_invwidth;
        static MalariaModel::Enum malaria_model;
    };

    class IndividualHumanMalaria : public IndividualHumanVector, public IMalariaHumanContext
    {
        friend class SimulationMalaria;

        IMPLEMENT_DEFAULT_REFERENCE_COUNTING()
        DECLARE_SERIALIZABLE( IndividualHumanMalaria )

    public:
        static IndividualHumanMalaria *CreateHuman(INodeContext *context, suids::suid _suid, double monte_carlo_weight = 1.0f, double initial_age = 0.0f, int gender = 0);
        virtual ~IndividualHumanMalaria();

        virtual IMalariaHumanContext* GetIndividualMalaria() override;

        // IMalariaHumanContext methods
        virtual void  PerformMalariaTest(int test_type) override;
        virtual void  CountPositiveSlideFields(RANDOMBASE* rng, int nfields, float uL_per_field, int& positive_asexual_fields, int& positive_gametocyte_fields) const override;
        virtual bool  CheckForParasitesWithTest(int test_type) const override;
        virtual float CheckParasiteCountWithTest(int test_type) const override;
        virtual float CheckGametocyteCountWithTest(int test_type) const override; // the value with sensitivity and variability of a blood test
        virtual float GetGametocyteDensity() const override;                      // the exact value in the model
        virtual bool  HasFever() const override;
        virtual void  AddClinicalSymptom(ClinicalSymptomsEnum::Enum symptom) override;
        virtual bool  HasClinicalSymptom(ClinicalSymptomsEnum::Enum symptom) const override;
        virtual IMalariaSusceptibility* GetMalariaSusceptibilityContext() const override;
        virtual std::vector< std::pair<uint32_t,uint64_t> > GetInfectingStrainIds() const override;

        virtual bool ChallengeWithBites( int n_infectious_bites ) override;
        virtual bool ChallengeWithSporozoites( int n_sporozoites ) override;

        virtual void CreateSusceptibility(float = 1.0f, float = 1.0f) override; // TODO: why isn't this protected (EAW)
        virtual void ClearNewInfectionState() override;
        virtual void setupMaternalAntibodies(IIndividualHumanContext* mother, INodeContext* node) override;

        virtual void ExposeToInfectivity(float dt) override;
        virtual void UpdateInfectiousness(float dt) override;

        virtual void SetContextTo(INodeContext* context);

        // infectivity debugging
        virtual void Drug_Report();
        // void malaria_infectivity_report();

        IMalariaSusceptibility* malaria_susceptibility; // now the Individual could have a full pointer to Suscept and Inf, but let's try using limited interface for now

    protected:
        double  m_inv_microliters_blood;   // 1/current blood volume (microliters) based on age

        // mature male and female gametocytes (total)
        int64_t m_male_gametocytes;
        int64_t m_female_gametocytes;

        // ...and by strain
        typedef std::map< StrainIdentity, int64_t > gametocytes_strain_map_t;
        typedef gametocytes_strain_map_t::value_type  gametocytes_strain_t;
        gametocytes_strain_map_t m_female_gametocytes_by_strain;

        float m_parasites_detected_by_blood_smear;
        float m_parasites_detected_by_new_diagnostic;
        float m_gametocytes_detected;
        bool  m_clinical_symptoms[ClinicalSymptomsEnum::CLINICAL_SYMPTOMS_COUNT]; // ...or we could store in a bitmask to be even lighter

        IMalariaAntibody* m_CSP_antibody;
        int m_initial_infected_hepatocytes;

        virtual void setupInterventionsContainer() override;
        virtual IInfection* createInfection(suids::suid _suid) override;

        virtual void ApplyTotalBitingExposure() override;
        int  CalculateInfectiousBites();

        virtual void PropagateContextToDependents() override;

        friend void serialize(IArchive&, gametocytes_strain_map_t&);

    private:
        IndividualHumanMalaria(suids::suid id = suids::nil_suid(), double monte_carlo_weight = 1.0, double initial_age = 0.0, int gender = 0);
        IndividualHumanMalaria(INodeContext *context);
        virtual IIndividualHumanContext* GetContextPointer() override { return (IIndividualHumanContext*)this; }
        void ResetClinicalSymptoms();
        void UpdateGametocyteCounts(float dt);
        void DepositInfectiousnessFromGametocytes();
        void DepositFractionalContagionByStrain(float weight, IVectorInterventionsEffects* ivie, uint32_t cladeID, uint64_t geneticID);
    };
}
