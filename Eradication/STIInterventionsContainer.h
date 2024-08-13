/***************************************************************************************************

Copyright (c) 2018 Intellectual Ventures Property Holdings, LLC (IVPH) All rights reserved.

EMOD is licensed under the Creative Commons Attribution-Noncommercial-ShareAlike 4.0 License.
To view a copy of this license, visit https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode

***************************************************************************************************/

#pragma once

#include <string>
#include <list>
#include <vector>

#include "ISTIInterventionsContainer.h"
#include "Interventions.h"
#include "InterventionsContainer.h"
#include "IRelationship.h"

namespace Kernel
{
    class STIInterventionsContainer
        : public InterventionsContainer
        , public ISTIInterventionsContainer
    {
        IMPLEMENT_DEFAULT_REFERENCE_COUNTING()

    public:
        STIInterventionsContainer();
        virtual ~STIInterventionsContainer();

        // IIndividualHumanInterventionsContext
        virtual void ChangeProperty( const char *property, const char* new_value) override;
        virtual ISTIInterventionsContainer* GetContainerSTI() override;

        // ISTIInterventionsContainer
        virtual void SpreadStiCoInfection() override;
        virtual void CureStiCoInfection() override;

        virtual void UpdateSTIBarrierProbabilitiesByType( RelationshipType::Enum rel_type, const Sigmoid& config_overrides ) override;
        virtual const Sigmoid& GetSTIBarrierProbabilitiesByRelType( const IRelationshipParameters* pRelParams ) const override;

        virtual bool IsCircumcised( void ) const override;
        virtual float GetCircumcisedReducedAcquire() const override;
        virtual void ApplyCircumcision( float reduceAcquire ) override;

    protected:
        bool is_circumcised;
        float circumcision_reduced_require;

        std::map< RelationshipType::Enum, Sigmoid > STI_blocking_overrides;

        DECLARE_SERIALIZABLE(STIInterventionsContainer);
    };
}
