/***************************************************************************************************

Copyright (c) 2018 Intellectual Ventures Property Holdings, LLC (IVPH) All rights reserved.

EMOD is licensed under the Creative Commons Attribution-Noncommercial-ShareAlike 4.0 License.
To view a copy of this license, visit https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode

***************************************************************************************************/

#pragma once

#include "Drugs.h"
#include "TBDrugTypeParameters.h"

namespace Kernel
{
    struct ITBInterventionsContainer;

    class AntiTBDrug : public GenericDrug
    {
        DECLARE_FACTORY_REGISTERED(IndividualIVFactory, AntiTBDrug, IDistributableIntervention);

    public:
        AntiTBDrug();
        virtual ~AntiTBDrug() {};
        virtual bool Configure( const Configuration * ) override;

        virtual void SetContextTo(IIndividualHumanContext *context) override;

        // IDistributableIntervention
        virtual bool Distribute(IIndividualHumanInterventionsContext *context, ICampaignCostObserver * const pCCO ) override;

    protected:
        virtual void  ConfigureDrugTreatment( IIndividualHumanInterventionsContext * ivc ) override;

        // These have same names as analogous methods on container but are internal for this drug itself.
        virtual float GetDrugInactivationRate() const;
        virtual float GetDrugClearanceRate() const;
        virtual float GetDrugResistanceRate() const;
        virtual float GetDrugRelapseRate() const;
        virtual float GetDrugMortalityRate() const;

        // inherited from base class Drugs.cpp
        virtual void ApplyEffects() override;

        TBDrugType::Enum drug_type;

        float TB_drug_inactivation_rate;
        float TB_drug_cure_rate;
        float TB_drug_resistance_rate;
        float TB_drug_relapse_rate;
        float TB_drug_mortality_rate;
        ITBInterventionsContainer* itbda;

        virtual void Expire() override;

        DECLARE_SERIALIZABLE(AntiTBDrug);
    };
}
