/***************************************************************************************************

Copyright (c) 2018 Intellectual Ventures Property Holdings, LLC (IVPH) All rights reserved.

EMOD is licensed under the Creative Commons Attribution-Noncommercial-ShareAlike 4.0 License.
To view a copy of this license, visit https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode

***************************************************************************************************/

#pragma once

#include "IVectorCohort.h"
#include "IVectorHabitat.h"
#include "ISerializable.h"

namespace Kernel
{
    struct ITransmissionGroups;
    struct INodeContext;
    struct IMigrationInfo;

    struct IVectorPopulation : ISerializable
    {
        virtual const std::string& get_SpeciesID() const = 0;
        virtual void SetContextTo( INodeContext *context ) = 0;
        virtual void SetupIntranodeTransmission( ITransmissionGroups* txIndoor, ITransmissionGroups* txOutdoor ) = 0;
        virtual void SetupLarvalHabitat( INodeContext *context ) = 0;
        virtual void SetVectorMortality( bool mortality ) = 0;

        // The function that NodeVector calls into once per species per timestep
        virtual void UpdateVectorPopulation( float dt ) = 0;

        // For NodeVector to calculate # of migrating vectors (processEmigratingVectors) and put them in new node (processImmigratingVector)
        virtual void Vector_Migration( float dt, IMigrationInfo* pMigInfo, VectorCohortVector_t* pMigratingQueue ) = 0;
        virtual void AddImmigratingVector( IVectorCohort* pvc ) = 0;

        // Supports MosquitoRelease intervention
        virtual void AddVectors( const VectorMatingStructure& _vector_genetics, uint32_t releasedNumber ) = 0;

        // Reporting
        virtual float GetEIRByPool(VectorPoolIdEnum::Enum pool_id)      const = 0;
        virtual float GetHBRByPool(VectorPoolIdEnum::Enum pool_id)      const = 0;
        virtual uint32_t getAdultCount()                                const = 0;
        virtual uint32_t getInfectedCount(   IStrainIdentity* pStrain ) const = 0;
        virtual uint32_t getInfectiousCount( IStrainIdentity* pStrain ) const = 0;
        virtual uint32_t getMaleCount()                                 const = 0;
        virtual uint32_t getNewEggsCount()                              const = 0;
        virtual uint32_t getNewAdults()                                 const = 0;
        virtual uint32_t getNumDiedBeforeFeeding()                      const = 0;
        virtual uint32_t getNumDiedDuringFeedingIndoor()                const = 0;
        virtual uint32_t getNumDiedDuringFeedingOutdoor()               const = 0;
        virtual double  getInfectivity()                                const = 0;
        virtual const VectorHabitatList_t& GetHabitats()                const = 0;
        virtual std::vector<uint64_t> GetNewlyInfectedVectorIds()       const = 0;
        virtual std::vector<uint64_t> GetInfectiousVectorIds()          const = 0;
    };
}
