
#pragma once

#include "IdmApi.h"
#include "IdmDateTime.h"
#include "Configure.h"
#include "IRelationship.h"


namespace Kernel {

    class RANDOMBASE;
    struct IIdGeneratorSTI;
    struct IPairFormationRateTable;
    struct IPairFormationAgent;
    struct IPairFormationStats;
    struct IRelationshipParameters;
    struct IConcurrency;

    struct IDMAPI ISociety : public JsonConfigurable
    {
        virtual void BeginUpdate() = 0;
        virtual void UpdatePairFormationRates( const IdmDateTime& rCurrentTime, float dt) = 0;
        virtual void UpdatePairFormationAgents( const IdmDateTime& rCurrentTime, float dt ) = 0;
        
        virtual const IPairFormationRateTable* GetRates(RelationshipType::Enum) = 0;
        virtual IPairFormationAgent* GetPFA(RelationshipType::Enum) = 0;
        virtual IPairFormationStats* GetStats(RelationshipType::Enum) = 0;

        virtual void SetParameters( RANDOMBASE* pRNG, IIdGeneratorSTI* pIdGen, const Configuration* config ) = 0;
        virtual IRelationshipParameters* GetRelationshipParameters( RelationshipType::Enum type ) = 0;

        virtual IConcurrency* GetConcurrency() = 0;

        // JsonConfigurable - making public
        virtual bool Configure(const Configuration *config) = 0;

        virtual ~ISociety() {}
    };
}