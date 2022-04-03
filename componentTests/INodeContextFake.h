/***************************************************************************************************

Copyright (c) 2018 Intellectual Ventures Property Holdings, LLC (IVPH) All rights reserved.

EMOD is licensed under the Creative Commons Attribution-Noncommercial-ShareAlike 4.0 License.
To view a copy of this license, visit https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode

***************************************************************************************************/

#pragma once

#include "ISimulationContext.h"
#include "NodeEventContext.h"
#include "IIndividualHumanContext.h"
#include "IdmDateTime.h"
#include "INodeContext.h"
#include "TransmissionGroupMembership.h"

using namespace Kernel;

struct INodeContextFake : INodeContext
{
private:
    suids::suid m_suid ;
    INodeEventContext* m_pNEC;
    NPKeyValueContainer m_NodeProperties;
    IdmDateTime m_Time;

public:
    INodeContextFake( int id = 1 )
    : m_suid()
    , m_pNEC(nullptr)
    , m_NodeProperties()
    , m_Time()
    {
        m_suid.data = id ;
    }

    INodeContextFake( const suids::suid& rSuid, INodeEventContext* pNEC = nullptr )
    : m_suid(rSuid)
    , m_pNEC(pNEC)
    , m_NodeProperties()
    , m_Time()
    {
        if( m_pNEC != nullptr )
        {
            m_pNEC->SetContextTo( this );
        }
    }

    virtual bool operator==( const INodeContext& rThat ) const override
    {
        const INodeContextFake* pThat = dynamic_cast<const INodeContextFake*>(&rThat) ;
        if( pThat == nullptr ) return false ;

        if( this->m_suid.data != pThat->m_suid.data ) return false ;

        return true ;
    }

    virtual ISimulationContext* GetParent() override
    {
        throw Kernel::NotYetImplementedException( __FILE__, __LINE__, __FUNCTION__, "The method or operation is not implemented.");
    }

    virtual suids::suid GetSuid() const override
    {
        return m_suid ;
    }

    virtual suids::suid GetNextInfectionSuid() override
    {
        throw Kernel::NotYetImplementedException( __FILE__, __LINE__, __FUNCTION__, "The method or operation is not implemented.");
    }

    virtual RANDOMBASE* GetRng() override
    {
        throw Kernel::NotYetImplementedException( __FILE__, __LINE__, __FUNCTION__, "The method or operation is not implemented.");
    }

    virtual void SetRng( RANDOMBASE* prng ) override
    {
        throw Kernel::NotYetImplementedException( __FILE__, __LINE__, __FUNCTION__, "The method or operation is not implemented." );
    }

    virtual void ExposeIndividual( IInfectable* candidate, TransmissionGroupMembership_t individual, float dt ) override
    {
        throw Kernel::NotYetImplementedException( __FILE__, __LINE__, __FUNCTION__, "The method or operation is not implemented.");
    }

    virtual void DepositFromIndividual( const IStrainIdentity& strain_IDs, float contagion_quantity, TransmissionGroupMembership_t individual, TransmissionRoute::Enum route ) override
    {
        throw Kernel::NotYetImplementedException( __FILE__, __LINE__, __FUNCTION__, "The method or operation is not implemented.");
    }

    virtual void GetGroupMembershipForIndividual( const RouteList_t& route, const tProperties& properties, TransmissionGroupMembership_t& membershipOut ) override
    {
        throw Kernel::NotYetImplementedException( __FILE__, __LINE__, __FUNCTION__, "The method or operation is not implemented.");
    }

    virtual void UpdateTransmissionGroupPopulation( const tProperties& properties, float size_changes,float mc_weight ) override
    {
        throw Kernel::NotYetImplementedException( __FILE__, __LINE__, __FUNCTION__, "The method or operation is not implemented.");
    }

    virtual std::map< std::string, float > GetContagionByRoute() const override
    {
        throw Kernel::NotYetImplementedException( __FILE__, __LINE__, __FUNCTION__, "The method or operation is not implemented.");
    }

    virtual float GetTotalContagion( void ) override
    {
        throw Kernel::NotYetImplementedException( __FILE__, __LINE__, __FUNCTION__, "The method or operation is not implemented.");
    }

    virtual RouteList_t& GetTransmissionRoutes() const override
    {
        throw Kernel::NotYetImplementedException( __FILE__, __LINE__, __FUNCTION__, "The method or operation is not implemented.");
    }

    virtual float GetContagionByRouteAndProperty( const std::string& route, const IPKeyValue& property_value )
    {
        throw Kernel::NotYetImplementedException( __FILE__, __LINE__, __FUNCTION__, "The method or operation is not implemented." );
    }

    virtual float getSinusoidalCorrection(float sinusoidal_amplitude, float sinusoidal_phase) const override
    {
        throw Kernel::NotYetImplementedException( __FILE__, __LINE__, __FUNCTION__, "The method or operation is not implemented.");
    }

    virtual float getBoxcarCorrection(float boxcar_amplitude, float boxcar_start_time, float boxcar_end_time) const override
    {
        throw Kernel::NotYetImplementedException( __FILE__, __LINE__, __FUNCTION__, "The method or operation is not implemented.");
    }

    virtual IMigrationInfo* GetMigrationInfo() override
    {
        throw Kernel::NotYetImplementedException( __FILE__, __LINE__, __FUNCTION__, "The method or operation is not implemented.");
    }

    virtual const NodeDemographics* GetDemographics() const override
    {
        throw Kernel::NotYetImplementedException( __FILE__, __LINE__, __FUNCTION__, "The method or operation is not implemented.");
    }

    virtual NPKeyValueContainer& GetNodeProperties() override
    {
        return m_NodeProperties;
    }

    virtual const IdmDateTime& GetTime() const override
    {
        if( m_pNEC != nullptr )
        {
            return m_pNEC->GetTime();
        }
        else
        {
            static IdmDateTime time(0);
            return time;
        }
    }

    virtual float GetInfected() const override
    {
        throw Kernel::NotYetImplementedException( __FILE__, __LINE__, __FUNCTION__, "The method or operation is not implemented.");
    }

    virtual float GetSymptomatic() const override
    {
        throw Kernel::NotYetImplementedException( __FILE__, __LINE__, __FUNCTION__, "The method or operation is not implemented." );
    }

    virtual float GetNewlySymptomatic() const override
    {
        throw Kernel::NotYetImplementedException( __FILE__, __LINE__, __FUNCTION__, "The method or operation is not implemented." );
    }

    virtual float GetStatPop() const override
    {
        throw Kernel::NotYetImplementedException( __FILE__, __LINE__, __FUNCTION__, "The method or operation is not implemented.");
    }

    virtual float GetBirths() const override
    {
        throw Kernel::NotYetImplementedException( __FILE__, __LINE__, __FUNCTION__, "The method or operation is not implemented.");
    }

    virtual float GetCampaignCost() const override
    {
        throw Kernel::NotYetImplementedException( __FILE__, __LINE__, __FUNCTION__, "The method or operation is not implemented.");
    }

    virtual float GetInfectivity() const override
    {
        throw Kernel::NotYetImplementedException( __FILE__, __LINE__, __FUNCTION__, "The method or operation is not implemented.");
    }

    virtual float GetInfectionRate() const override
    {
        throw Kernel::NotYetImplementedException( __FILE__, __LINE__, __FUNCTION__, "The method or operation is not implemented.");
    }

    virtual float GetSusceptDynamicScaling() const override
    {
        throw Kernel::NotYetImplementedException( __FILE__, __LINE__, __FUNCTION__, "The method or operation is not implemented.");
    }

    virtual const Climate* GetLocalWeather() const override
    {
        throw Kernel::NotYetImplementedException( __FILE__, __LINE__, __FUNCTION__, "The method or operation is not implemented.");
    }

    virtual INodeEventContext* GetEventContext() override
    {
        release_assert( m_pNEC );
        return m_pNEC;
    }

    virtual void AddEventsFromOtherNodes( const std::vector<EventTrigger::Enum>& rTriggerList ) override
    {
        throw Kernel::NotYetImplementedException( __FILE__, __LINE__, __FUNCTION__, "The method or operation is not implemented.");
    }

    virtual QueryResult QueryInterface( iid_t iid, void** pinstance ) override
    {
        throw Kernel::NotYetImplementedException( __FILE__, __LINE__, __FUNCTION__, "The method or operation is not implemented.");
    }

    virtual int32_t AddRef() override
    {
        throw Kernel::NotYetImplementedException( __FILE__, __LINE__, __FUNCTION__, "The method or operation is not implemented.");
    }

    virtual int32_t Release() override
    {
        throw Kernel::NotYetImplementedException( __FILE__, __LINE__, __FUNCTION__, "The method or operation is not implemented.");
    }

    act_prob_vec_t DiscreteGetTotalContagion( void ) override
    {
        throw Kernel::NotYetImplementedException( __FILE__, __LINE__, __FUNCTION__, "The method or operation is not implemented.");
    }

    virtual long int GetPossibleMothers() const override
    {
        throw Kernel::NotYetImplementedException( __FILE__, __LINE__, __FUNCTION__, "The method or operation is not implemented.");
    }

    virtual bool IsEveryoneHome() const override
    {
        throw Kernel::NotYetImplementedException( __FILE__, __LINE__, __FUNCTION__, "The method or operation is not implemented.");
    }

    virtual void SetWaitingForFamilyTrip( suids::suid migrationDestination, MigrationType::Enum migrationType, float timeUntilTrip, float timeAtDestination, bool isDestinationNewHome ) override
    {
        throw Kernel::NotYetImplementedException( __FILE__, __LINE__, __FUNCTION__, "The method or operation is not implemented.");
    }

    virtual float GetMeanAgeInfection() const override
    {
        throw Kernel::NotYetImplementedException( __FILE__, __LINE__, __FUNCTION__, "The method or operation is not implemented.");
    }
    virtual const NodeDemographicsDistribution* GetImmunityDistribution()        const override { throw Kernel::NotYetImplementedException( __FILE__, __LINE__, __FUNCTION__, "The method or operation is not implemented."); }
    virtual const NodeDemographicsDistribution* GetFertilityDistribution()       const override { throw Kernel::NotYetImplementedException( __FILE__, __LINE__, __FUNCTION__, "The method or operation is not implemented."); }
    virtual const NodeDemographicsDistribution* GetMortalityDistribution()       const override { throw Kernel::NotYetImplementedException( __FILE__, __LINE__, __FUNCTION__, "The method or operation is not implemented."); }
    virtual const NodeDemographicsDistribution* GetMortalityDistributionMale()   const override { throw Kernel::NotYetImplementedException( __FILE__, __LINE__, __FUNCTION__, "The method or operation is not implemented."); }
    virtual const NodeDemographicsDistribution* GetMortalityDistributionFemale() const override { throw Kernel::NotYetImplementedException( __FILE__, __LINE__, __FUNCTION__, "The method or operation is not implemented."); }
    virtual const NodeDemographicsDistribution* GetAgeDistribution()             const override { throw Kernel::NotYetImplementedException( __FILE__, __LINE__, __FUNCTION__, "The method or operation is not implemented."); }

    virtual std::vector<bool> GetMigrationTypeEnabledFromDemographics() const override
    {
        std::vector<bool> enabled_list;

        enabled_list.push_back( true );
        enabled_list.push_back( true );
        enabled_list.push_back( true );
        enabled_list.push_back( true );
        enabled_list.push_back( true );

        return enabled_list;
    }

    virtual ExternalNodeId_t GetExternalID() const override
    {
        return m_suid.data ;
    }

    virtual float GetLatitudeDegrees() override
    {
        throw Kernel::NotYetImplementedException( __FILE__, __LINE__, __FUNCTION__, "The method or operation is not implemented.");
    }

    virtual float GetLongitudeDegrees() override
    {
        throw Kernel::NotYetImplementedException( __FILE__, __LINE__, __FUNCTION__, "The method or operation is not implemented.");
    }

    virtual void SetupMigration( IMigrationInfoFactory * migration_factory, 
                                 MigrationStructure::Enum ms,
                                 const boost::bimap<ExternalNodeId_t, suids::suid>& rNodeIdSuidMap ) override
    { 
        throw Kernel::NotYetImplementedException( __FILE__, __LINE__, __FUNCTION__, "The method is not implemented.");
    }

    virtual void SetParameters( NodeDemographicsFactory *demographics_factory, ClimateFactory *climate_factory, bool white_list_enabled ) override
    {
        throw Kernel::NotYetImplementedException( __FILE__, __LINE__, __FUNCTION__, "The method is not implemented.");
    }

    virtual void SetContextTo(ISimulationContext*)                                                             override { throw Kernel::NotYetImplementedException( __FILE__, __LINE__, __FUNCTION__, "The method is not implemented."); }
    virtual void PopulateFromDemographics()                                                                    override { throw Kernel::NotYetImplementedException( __FILE__, __LINE__, __FUNCTION__, "The method is not implemented."); }
    virtual void InitializeTransmissionGroupPopulations()                                                      override { throw Kernel::NotYetImplementedException( __FILE__, __LINE__, __FUNCTION__, "The method is not implemented."); }
    virtual ITransmissionGroups* GetTransmissionGroups() const                                                 override { throw Kernel::NotYetImplementedException( __FILE__, __LINE__, __FUNCTION__, "The method is not implemented."); }
    virtual void Update(float)                                                                                 override { throw Kernel::NotYetImplementedException( __FILE__, __LINE__, __FUNCTION__, "The method is not implemented."); }
    virtual IIndividualHuman* processImmigratingIndividual(IIndividualHuman*)                                  override { throw Kernel::NotYetImplementedException( __FILE__, __LINE__, __FUNCTION__, "The method is not implemented."); }
    virtual void SortHumans()                                                                                  override { throw Kernel::NotYetImplementedException( __FILE__, __LINE__, __FUNCTION__, "The method is not implemented."); }
    virtual const std::vector<IIndividualHuman*>& GetHumans()                                            const override { throw Kernel::NotYetImplementedException( __FILE__, __LINE__, __FUNCTION__, "The method is not implemented."); }
    virtual std::map<std::string, int>& GetStrainClades()                                                const override { throw Kernel::NotYetImplementedException( __FILE__, __LINE__, __FUNCTION__, "The method is not implemented."); }
    virtual const std::map<std::string, int>& GetStrainGenomes()                                         const override { throw Kernel::NotYetImplementedException( __FILE__, __LINE__, __FUNCTION__, "The method is not implemented."); }
    virtual std::map<std::string, std::vector<float>>& GetStrainData()                                   const override { throw Kernel::NotYetImplementedException( __FILE__, __LINE__, __FUNCTION__, "The method is not implemented."); }

    virtual float GetBasePopulationScaleFactor() const
    {
        return 1.0;
    }

    virtual ProbabilityNumber GetProbMaternalTransmission() const
    {
        return 1.0;
    }
    virtual float GetMaxInfectionProb( TransmissionRoute::Enum route ) const
    {
        return 0.0f;
    }


    virtual float GetNonDiseaseMortalityRateByAgeAndSex( float age, Gender::Enum sex ) const override
    {
        throw Kernel::NotYetImplementedException( __FILE__, __LINE__, __FUNCTION__, "The method or operation is not implemented.");
    }

    virtual float initiatePregnancyForIndividual( int individual_id, float dt ) override
    {
        throw Kernel::NotYetImplementedException( __FILE__, __LINE__, __FUNCTION__, "The method or operation is not implemented.");
    }

    virtual bool updatePregnancyForIndividual( int individual_id, float duration ) override
    {
        throw Kernel::NotYetImplementedException( __FILE__, __LINE__, __FUNCTION__, "The method or operation is not implemented.");
    }

    virtual void populateNewIndividualsByBirth(int count_new_individuals = 100) override
    {
        throw Kernel::NotYetImplementedException( __FILE__, __LINE__, __FUNCTION__, "The method or operation is not implemented.");
    }

};
