
#pragma once

#include "IRelationship.h"
#include "MathFunctions.h"
#include "Sigmoid.h"

namespace Kernel
{
    struct IRelationshipParameters;

    // this container becomes a help implementation member of the IndividualHumanSTI class 
    // it needs to implement consumer interfaces for all the relevant intervention types

    struct ISTIInterventionsContainer : public ISupports
    {
        virtual void SpreadStiCoInfection() = 0;
        virtual void CureStiCoInfection() = 0;

        virtual void UpdateSTIBarrierProbabilitiesByType( RelationshipType::Enum rel_type, const Sigmoid& config_overrides ) = 0;
        virtual const Sigmoid& GetSTIBarrierProbabilitiesByRelType( const IRelationshipParameters* pRelParams ) const = 0;

        virtual bool IsCircumcised( void ) const = 0;
        virtual float GetCircumcisedReducedAcquire() const = 0;
        virtual void ApplyCircumcision( float reduceAcquire ) = 0;
    };
}
