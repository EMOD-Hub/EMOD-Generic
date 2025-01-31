/***************************************************************************************************

Copyright (c) 2018 Intellectual Ventures Property Holdings, LLC (IVPH) All rights reserved.

EMOD is licensed under the Creative Commons Attribution-Noncommercial-ShareAlike 4.0 License.
To view a copy of this license, visit https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode

***************************************************************************************************/

#pragma once
#include "IdmApi.h"
#include "EnumSupport.h"
#include "Types.h"
#include <set>
#include <functional>

#include "suids.hpp"
#include "RelationshipType.h"

namespace Kernel
{
    class RANDOMBASE;
    struct IRelationshipManager;
    struct ISociety;

    struct IIndividualHumanSTI;
    typedef std::set< IIndividualHumanSTI* > tRelationshipMembers;

#define MAX_SLOTS (63)

    struct IDMAPI IRelationship : ISerializable
    {
        virtual RelationshipState::Enum GetState() const = 0;
        virtual RelationshipState::Enum GetPreviousState() const = 0;
        virtual RelationshipMigrationAction::Enum GetMigrationAction( RANDOMBASE* prng ) const =0;
        virtual RelationshipTerminationReason::Enum GetTerminationReason() const = 0;
        virtual void Pause( IIndividualHumanSTI* departee ) = 0;
        virtual void Terminate( RelationshipTerminationReason::Enum terminationReason ) = 0;
        virtual void Migrate() = 0;
        virtual void Resume( IRelationshipManager* pRelMan, ISociety* pSociety, IIndividualHumanSTI* returnee ) = 0;
        virtual void UpdatePaused() = 0;

        virtual bool Update( float dt ) = 0;
        virtual void Consummate( RANDOMBASE* pRNG, float dt ) = 0;

        virtual IIndividualHumanSTI* MalePartner() const = 0;
        virtual IIndividualHumanSTI* FemalePartner() const = 0;
        virtual IIndividualHumanSTI* GetPartner( IIndividualHumanSTI* pIndiv ) const = 0;
        virtual const suids::suid& GetSuid() const = 0;
        virtual const std::string& GetPropertyKey() = 0;
        virtual const std::string& GetPropertyName() const = 0;
        virtual unsigned int GetSlotNumberForPartner( bool forPartnerB ) const = 0;
        virtual const tRelationshipMembers GetMembers() const = 0;
        virtual bool IsDiscordant() const = 0;
        virtual float GetDuration() const = 0;
        virtual float GetTimer() const = 0;
        virtual float GetStartTime() const = 0;
        virtual float GetScheduledEndTime() const = 0;
        virtual bool GetUsingCondom() const = 0;
        virtual suids::suid GetPartnerId( const suids::suid& myID ) const = 0;
        virtual suids::suid GetMalePartnerId() const = 0;
        virtual suids::suid GetFemalePartnerId() const = 0;
        virtual bool IsMalePartnerAbsent() const = 0;
        virtual bool IsFemalePartnerAbsent() const = 0;

        virtual RelationshipType::Enum GetType() const = 0;
        virtual unsigned int GetOriginalNodeId() const = 0;
        virtual float GetCoitalRate() const = 0;
        virtual ProbabilityNumber getProbabilityUsingCondomThisAct() const =0;
        virtual unsigned int GetNumCoitalActs() const = 0;
        virtual bool HasMigrated() const = 0;

        virtual ~IRelationship() {}
    };

    typedef std::function<void(IIndividualHumanSTI*,IIndividualHumanSTI*)> RelationshipCreator;

    struct IDMAPI RelationshipSetSorter
    {
        bool operator()( const IRelationship *rel1, const IRelationship *rel2 ) const;
    };

    typedef std::set<IRelationship*, RelationshipSetSorter> RelationshipSet_t;
}
