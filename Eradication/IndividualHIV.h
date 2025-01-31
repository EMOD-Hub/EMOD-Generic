/***************************************************************************************************

Copyright (c) 2018 Intellectual Ventures Property Holdings, LLC (IVPH) All rights reserved.

EMOD is licensed under the Creative Commons Attribution-Noncommercial-ShareAlike 4.0 License.
To view a copy of this license, visit https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode

***************************************************************************************************/

#pragma once

#include <set>
#include "NodeHIV.h"
#include "IndividualSTI.h"
#include "IIndividualHumanHIV.h"

namespace Kernel
{
    class IndividualHumanHIVConfig : public IndividualHumanSTIConfig
    {
        friend class IndividualHumanHIV;

        static float maternal_transmission_ART_multiplier;
    public:
        virtual bool Configure( const Configuration* config ) override;

    };

    class IHIVInfection;
    class IHIVSusceptibility;
    class IndividualHumanHIV : public IndividualHumanSTI, public IIndividualHumanHIV
    {
        friend class SimulationHIV;

        IMPLEMENT_NO_REFERENCE_COUNTING()
        DECLARE_SERIALIZABLE( IndividualHumanHIV )

    public:
        static IndividualHumanHIV *CreateHuman(INodeContext *context, suids::suid _suid, float monte_carlo_weight = 1.0f, float initial_age = 0.0f, int gender = int(Gender::MALE));
        virtual ~IndividualHumanHIV(void);

        virtual void InitializeHuman() override;

        // Infections and Susceptibility
        virtual void CreateSusceptibility( float imm_mod=1.0f, float risk_mod=1.0f ) override;

        virtual bool IsSymptomatic() const override;

        virtual bool HasHIV() const override;

        virtual IInfectionHIV* GetHIVInfection() const override;
        virtual ISusceptibilityHIV* GetHIVSusceptibility() const override;
        virtual IHIVInterventionsContainer* GetHIVInterventionsContainer() const override;
        virtual IIndividualHumanHIV* GetIndividualHIV() override;
        virtual bool UpdatePregnancy(float dt=1) override;
        virtual ProbabilityNumber getProbMaternalTransmission() const override;

        // healthcare interactions
        virtual std::string toString() const override;

        IndividualHumanHIV( suids::suid id = suids::nil_suid(), 
                            float monte_carlo_weight = 1.0f, 
                            float initial_age = 0.0f, 
                            int gender = 0 );
        
    virtual bool ShouldAcquire( float contagion, float dt, float suscept_mod, TransmissionRoute::Enum tx_route ) const;

    protected:
        virtual IInfection* createInfection(suids::suid _suid) override;
        virtual void setupInterventionsContainer() override;
        ISusceptibilityHIV * hiv_susceptibility;

        // variables for reporting
        unsigned int pos_num_partners_while_CD4500plus;
        unsigned int neg_num_partners_while_CD4500plus;

    };
}
