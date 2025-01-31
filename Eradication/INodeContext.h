/***************************************************************************************************

Copyright (c) 2018 Intellectual Ventures Property Holdings, LLC (IVPH) All rights reserved.

EMOD is licensed under the Creative Commons Attribution-Noncommercial-ShareAlike 4.0 License.
To view a copy of this license, visit https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode

***************************************************************************************************/

#pragma once
#include "IdmApi.h"
#include "ISerializable.h"
#include "suids.hpp"
#include "ITransmissionGroups.h"
#include "SimulationEnums.h"
#include "EventTrigger.h"
#include "ExternalNodeId.h"

#define INDEX_RST_TOT_INF         (0)
#define INDEX_RST_CON_INF         (1)
#define INDEX_RST_CONTAGION       (2)
#define INDEX_RST_NEW_INF         (3)

#define ADULT_AGE_YRS                 (15.0f)
#define BIRTHRATE_SANITY_VALUE        (0.005f)

namespace Kernel
{
    struct IdmDateTime;
    class RANDOMBASE;
    struct IInfectable;
    struct IMigrationInfo;
    struct IMigrationInfoFactory;
    struct NodeDemographics;
    class NodeDemographicsFactory;
    struct NodeDemographicsDistribution;
    struct NodeParams;
    class  Climate;
    class  ClimateFactory;
    struct INodeEventContext;
    struct IIndividualHuman;
    struct ISimulationContext;
    class  INodeVector;
    class  INodeMalaria;
    class  INodeSTI;
    struct INodeTBHIV;
    struct INodePolio;
    class NPKeyValueContainer;

    struct IDMAPI INodeContext : ISerializable
    {
        // TODO/OPTION:
        // could have an PostMigratingIndividual interface too if individuals will call back to do migration....

        // DMB 9-9-2014 May need to consider implementing the equality operator for ISupports
        virtual bool operator==( const INodeContext& rThat ) const
        {
            throw NotYetImplementedException( __FILE__, __LINE__, __FUNCTION__, "INodeContect::operator==() needs to be implemented by subclass.");
        } ;

        virtual bool operator!=( const INodeContext& rThat ) const
        { 
            return !(*this == rThat);
        } ;

        virtual ISimulationContext* GetParent() = 0;
        virtual INodeVector*        GetNodeVector()    = 0;
        virtual INodeMalaria*       GetNodeMalaria()   = 0;
        virtual INodeSTI*           GetNodeSTI()       = 0;
        virtual INodeTBHIV*         GetNodeTBHIV()     = 0;
        virtual INodePolio*         GetNodePolio()     = 0;

        virtual const NodeParams* GetParams() const = 0;

        //individual can get an id of their parent to compare against, for instance, their home node id
        virtual suids::suid GetSuid() const = 0;

        virtual void SetupMigration( IMigrationInfoFactory * migration_factory ) = 0;

        virtual void SetContextTo( ISimulationContext* ) = 0;
        virtual void SetParameters( NodeDemographicsFactory *demographics_factory, ClimateFactory *climate_factory ) = 0;
        virtual void PopulateFromDemographics() = 0;
        virtual void InitializeTransmissionGroupPopulations() = 0;

        virtual suids::suid GetNextInfectionSuid() = 0;
        virtual RANDOMBASE* GetRng() = 0; 
        virtual void SetRng( RANDOMBASE* prng ) = 0;

        virtual void Update(float dt) = 0;
        virtual IIndividualHuman* processImmigratingIndividual( IIndividualHuman* ) = 0;
        virtual void SortHumans() = 0;
        virtual const std::vector<IIndividualHuman*>& GetHumans() const = 0;

        // heterogeneous intra-node transmission
        virtual void ChangePropertyMatrix( const std::string& propertyName, const ScalingMatrix_t& newScalingMatrix ) = 0;
        virtual void ExposeIndividual(IInfectable* candidate, TransmissionGroupMembership_t individual, float dt, TransmissionRoute::Enum route=TransmissionRoute::CONTACT) = 0;
        virtual void DepositFromIndividual( const IStrainIdentity& strain_IDs, float contagion_quantity, TransmissionGroupMembership_t individual, TransmissionRoute::Enum route=TransmissionRoute::CONTACT) = 0;
        virtual void GetGroupMembershipForIndividual(TransmissionRoute::Enum route, const tProperties& properties, TransmissionGroupMembership_t& membershipOut ) = 0;
        virtual void UpdateTransmissionGroupPopulation(const tProperties& properties, float size_changes,float mc_weight) = 0;
        virtual std::map<TransmissionRoute::Enum, float> GetContagionByRoute() const = 0;
        virtual float GetTotalContagion( void ) = 0;
        virtual const RouteList_t& GetTransmissionRoutes() const = 0;

        virtual float GetContagionByRouteAndProperty( TransmissionRoute::Enum route, const IPKeyValue& property_value ) = 0;

        // Discrete HINT contagion
        virtual act_prob_vec_t DiscreteGetTotalContagion( void ) = 0;

        virtual IMigrationInfo* GetMigrationInfo() = 0;
        virtual NPKeyValueContainer& GetNodeProperties() = 0;

        // reporting interfaces
        virtual const IdmDateTime&  GetTime()                   const = 0;
        virtual const Climate*      GetLocalWeather()           const = 0;
        virtual float               GetInfected()               const = 0;
        virtual float               GetSymptomatic()            const = 0;
        virtual float               GetNewlySymptomatic()       const = 0;
        virtual float               GetStatPop()                const = 0;
        virtual float               GetBirths()                 const = 0;
        virtual float               GetCampaignCost()           const = 0;
        virtual float               GetInfectivity()            const = 0;
        virtual float               GetInfectionRate()          const = 0;
        virtual float               GetLatitudeDegrees()              = 0;
        virtual float               GetLongitudeDegrees()             = 0;
        virtual float               GetSusceptDynamicScaling()  const = 0;
        virtual long int            GetPossibleMothers()        const = 0;
        virtual uint64_t            GetTotalGenomes()           const = 0;

        virtual float GetNonDiseaseMortalityRateByAgeAndSex( float age, Gender::Enum sex ) const = 0;

        // Interface for strain tracking reporter data
        virtual std::map<std::pair<uint32_t,uint64_t>, std::vector<float>>& GetStrainData()   = 0;

        // Interfaces for network infectivity
        virtual const float                   GetNetInfectFrac()                        const = 0;
        virtual       void                    SetNetInfectFrac(float)                         = 0;
        virtual const sparse_contagion_repr&  GetNetInfRep()                            const = 0;
        virtual       void                    DepositNetInf(sparse_contagion_id,float)        = 0;

        // This method will ONLY be used for reporting by input node ID, don't use it elsewhere!
        virtual ExternalNodeId_t GetExternalID() const = 0;

        // for interventions
        virtual INodeEventContext* GetEventContext() = 0;
        virtual void AddEventsFromOtherNodes( const std::vector<EventTrigger::Enum>& rTriggerList ) = 0;

        virtual bool IsEveryoneHome() const = 0;
        virtual void SetWaitingForFamilyTrip( suids::suid migrationDestination, 
                                              MigrationType::Enum migrationType, 
                                              float timeUntilTrip, 
                                              float timeAtDestination,
                                              bool isDestinationNewHome ) = 0;

        virtual float GetMaxInfectionProb( TransmissionRoute::Enum tx_route )        const = 0;

        virtual float initiatePregnancyForIndividual( int individual_id, float dt ) = 0;
        virtual bool updatePregnancyForIndividual( int individual_id, float duration ) = 0;
        virtual void populateNewIndividualsByBirth(int count_new_individuals = 100) = 0;

        virtual const NodeDemographicsDistribution* GetImmunityDistribution()        const = 0;
        virtual const NodeDemographicsDistribution* GetFertilityDistribution()       const = 0;
        virtual const NodeDemographicsDistribution* GetMortalityDistribution()       const = 0;
        virtual const NodeDemographicsDistribution* GetMortalityDistributionMale()   const = 0;
        virtual const NodeDemographicsDistribution* GetMortalityDistributionFemale() const = 0;
        virtual const NodeDemographicsDistribution* GetAgeDistribution()             const = 0;

    };
}

