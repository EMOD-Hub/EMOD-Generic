/***************************************************************************************************

Copyright (c) 2018 Intellectual Ventures Property Holdings, LLC (IVPH) All rights reserved.

EMOD is licensed under the Creative Commons Attribution-Noncommercial-ShareAlike 4.0 License.
To view a copy of this license, visit https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode

***************************************************************************************************/

#pragma once

#include <map>
#include <vector>

#include "IdmApi.h"
#include "BoostLibWrapper.h"
#include "Configure.h"
#include "Climate.h"
#include "Common.h"
#include "Environment.h"

#include "NodeDemographics.h"
#include "ITransmissionGroups.h"
#include "suids.hpp"
#include "IInfectable.h"
#include "MathFunctions.h"
#include "Serialization.h"
#include "NodeProperties.h"
#include "INodeContext.h"
#include "StrainIdentity.h"

class Report;
class ReportVector;
class DemographicsReport;
class BaseChannelReport;

namespace Kernel
{
    class SimulationConfig;
    class RANDOMBASE;
    struct INodeEventContext;
    //typedef 
    class  NodeEventContextHost;
    struct ISimulation;
    struct IMigrationInfoFactory;
    struct IDistribution;

    class IDMAPI Node : public INodeContext, public JsonConfigurable
    {
        GET_SCHEMA_STATIC_WRAPPER(Node)
        IMPLEMENT_DEFAULT_REFERENCE_COUNTING()
        DECLARE_QUERY_INTERFACE()

        // This needs to go once we COM-ify and/or make accessors
        // BTW, the scope operators are needed by MSVC, not GCC (or is it the other way around?)
        friend class ::Report;
        friend class ::ReportVector;
        friend class ::DemographicsReport;

    public:
        static Node *CreateNode(ISimulationContext *_parent_sim, ExternalNodeId_t externalNodeId, suids::suid node_suid);

        Node(ISimulationContext *_parent_sim, ExternalNodeId_t externalNodeId, suids::suid _suid);
        Node(); // constructor for serialization use
        virtual ~Node();

        // INodeContext
        virtual void PreUpdate();
        virtual void Update(float dt) override;
        virtual ISimulationContext* GetParent() override;
        virtual suids::suid   GetSuid() const override;
        virtual suids::suid   GetNextInfectionSuid() override; 
        virtual RANDOMBASE* GetRng() override;
        virtual void SetRng( RANDOMBASE* prng ) override; 
        virtual void AddEventsFromOtherNodes( const std::vector<EventTrigger::Enum>& rTriggerList ) override;


        virtual IMigrationInfo*   GetMigrationInfo() override;
        virtual const NodeDemographics* GetDemographics()  const override;
        virtual std::vector<bool> GetMigrationTypeEnabledFromDemographics() const override;
        virtual NPKeyValueContainer& GetNodeProperties() override;


        // Migration
        virtual void SetupMigration( IMigrationInfoFactory * migration_factory, 
                                     MigrationStructure::Enum ms,
                                     const boost::bimap<ExternalNodeId_t, suids::suid>& rNodeIdSuidMap ) override;
        virtual IIndividualHuman* processImmigratingIndividual( IIndividualHuman* ) override;
        virtual void SortHumans() override;
        virtual const std::vector<IIndividualHuman*>& GetHumans() const override;

        // Strain tracking
        virtual const std::map<std::string, int>&                 GetStrainClades()   const override { return strain_map_clade;   }
        virtual const std::map<std::string, int>&                 GetStrainGenomes()  const override { return strain_map_genome;  }
        virtual const std::map<std::string, std::vector<float>>&  GetStrainData()     const override { return strain_map_data;    }

        // Initialization
        virtual void SetContextTo(ISimulationContext* context) override;
        virtual void SetParameters( NodeDemographicsFactory *demographics_factory, ClimateFactory *climate_factory, bool white_list_enabled ) override;
        virtual void PopulateFromDemographics() override;
        virtual void InitializeTransmissionGroupPopulations() override;

        // Campaign event-related
        bool IsInPolygon(float* vertex_coords, int numcoords); // might want to create a real polygon object at some point
        bool IsInPolygon( const json::Array &poly );
        bool IsInExternalIdSet( const std::list<ExternalNodeId_t>& nodelist );

        // Reporting to higher levels (intermediate form)
        // Possible TODO: refactor into common interfaces if there is demand
        virtual       INodeEventContext*  GetEventContext()                     override;
        virtual       ExternalNodeId_t    GetExternalID() const override;
        virtual const IdmDateTime& GetTime()     const override;
        virtual float GetInfected()              const override;
        virtual float GetSymptomatic()           const override;
        virtual float GetNewlySymptomatic()      const override;
        virtual float GetStatPop()               const override;
        virtual float GetBirths()                const override;
        virtual float GetCampaignCost()          const override;
        virtual float GetInfectivity()           const override;
        virtual float GetInfectionRate()         const override;
        virtual float GetSusceptDynamicScaling() const override;
        virtual const Climate* GetLocalWeather() const override;
        virtual long int GetPossibleMothers()    const override;

        virtual float GetMeanAgeInfection()      const override;
        virtual float GetBasePopulationScaleFactor() const override;

        virtual float GetNonDiseaseMortalityRateByAgeAndSex( float age, Gender::Enum sex ) const override;


        // Heterogeneous intra-node transmission
        virtual void ExposeIndividual(IInfectable* candidate, TransmissionGroupMembership_t individual, float dt) override;
        virtual void DepositFromIndividual( const IStrainIdentity& strain_IDs, float contagion_quantity, TransmissionGroupMembership_t individual, TransmissionRoute::Enum route = TransmissionRoute::TRANSMISSIONROUTE_CONTACT) override;
        virtual void GetGroupMembershipForIndividual(const RouteList_t& route, const tProperties& properties, TransmissionGroupMembership_t& membershipOut) override;
        virtual void UpdateTransmissionGroupPopulation(const tProperties& properties, float size_changes,float mc_weight) override;
        virtual void SetupIntranodeTransmission();
        virtual ITransmissionGroups* CreateTransmissionGroups();
        virtual ITransmissionGroups* GetTransmissionGroups() const override;
        virtual void AddDefaultRoute( void );
        virtual void AddRoute( const std::string& rRouteName );
        virtual void BuildTransmissionRoutes( float contagionDecayRate );
        virtual bool IsValidTransmissionRoute( const string& transmissionRoute );

        virtual act_prob_vec_t DiscreteGetTotalContagion( void ) override;

        virtual void ValidateIntranodeTransmissionConfiguration();

        virtual float GetTotalContagion( void ) override;
        virtual std::map< std::string, float > GetContagionByRoute() const;
        virtual const RouteList_t& GetTransmissionRoutes() const override;

        //Methods for implementing time dependence in various quantities; infectivity, birth rate, migration rate
        virtual float getBoxcarCorrection(float boxcar_amplitude, float boxcar_start_time, float boxcar_end_time)  const;
        virtual float getClimateCorrection()                                                                       const;
        virtual float getSinusoidalCorrection(float sinusoidal_amplitude, float sinusoidal_phase)                  const;

        virtual float GetContagionByRouteAndProperty( const std::string& route, const IPKeyValue& property_value ) override;

        // These methods are not const because they will extract the value from the demographics
        // if it has not been done yet.
        virtual float GetLatitudeDegrees()override;
        virtual float GetLongitudeDegrees() override;

        virtual bool IsEveryoneHome() const override;
        virtual void SetWaitingForFamilyTrip( suids::suid migrationDestination,
                                              MigrationType::Enum migrationType,
                                              float timeUntilTrip,
                                              float timeAtDestination,
                                              bool isDestinationNewHome ) override;

        virtual ProbabilityNumber GetProbMaternalTransmission() const override;

        virtual const NodeDemographicsDistribution* GetImmunityDistribution()        const override { return SusceptibilityDistribution; }
        virtual const NodeDemographicsDistribution* GetFertilityDistribution()       const override { return FertilityDistribution; }
        virtual const NodeDemographicsDistribution* GetMortalityDistribution()       const override { return MortalityDistribution; }
        virtual const NodeDemographicsDistribution* GetMortalityDistributionMale()   const override { return MortalityDistributionMale; }
        virtual const NodeDemographicsDistribution* GetMortalityDistributionFemale() const override { return MortalityDistributionFemale; }
        virtual const NodeDemographicsDistribution* GetAgeDistribution()             const override { return AgeDistribution; }

        virtual void ManageFamilyTrip( float currentTime, float dt );

    protected:
        float area;

#pragma warning( push )
#pragma warning( disable: 4251 ) // See IdmApi.h for details
        
        SerializationFlags serializationMask;

        NodeDemographicsDistribution* SusceptibilityDistribution;
        NodeDemographicsDistribution* FertilityDistribution;
        NodeDemographicsDistribution* MortalityDistribution;
        NodeDemographicsDistribution* MortalityDistributionMale;
        NodeDemographicsDistribution* MortalityDistributionFemale;
        NodeDemographicsDistribution* AgeDistribution;

        // Do not access these directly but use the access methods above.
        float _latitude;
        float _longitude;

        // Standard distributions for SIMPLE initialization
        IDistribution* distribution_age;
        IDistribution* distribution_migration;
        IDistribution* distribution_demographic_risk;
        IDistribution* distribution_susceptibility;

        // Enum type name                            Enum variable name                         Name in config.json
        IndSamplingType::Enum                        ind_sampling_type;                         // Individual_Sampling_Type
        DistributionType::Enum                       age_initialization_distribution_type;      // Age_Initialization_Distribution_Type
        PopulationScaling::Enum                      population_scaling;                        // POPULATION_SCALING
        float susceptibility_dynamic_scaling;

        // Node properties
        suids::suid suid;
        float birthrate;

        // ----------------------------------------------------------------------------------------
        // --- DMB 9-16-2014 Through comparison, it was determined that using a vector (and moving
        // --- the last human in the list to the location being removed) was faster and used less
        // --- memory than using a std::list (even with a variety of allocators).
        // --- Please the IDM Wiki for more details.
        // --- http://ivlabsdvapp50:8090/pages/viewpage.action?pageId=30015603
        // ----------------------------------------------------------------------------------------
        std::vector<IIndividualHuman*> individualHumans;
        std::map<int,suids::suid> home_individual_ids; // people who call this node home

        bool                family_waiting_to_migrate;
        suids::suid         family_migration_destination;
        MigrationType::Enum family_migration_type;
        float               family_time_until_trip;
        float               family_time_at_destination;
        bool                family_is_destination_new_home;

        // Heterogeneous intra-node transmission
        ITransmissionGroups *transmissionGroups;
        
        // Climate and demographics
        Climate *localWeather;
        IMigrationInfo *migration_info;
        NodeDemographics demographics;
        ExternalNodeId_t externalId; // DON'T USE THIS EXCEPT FOR INPUT/OUTPUT PURPOSES!
        NPKeyValueContainer node_properties;

        // Infectivity modifications
        bool  enable_infectivity_reservoir;
        bool  enable_infectivity_scaling;
        bool  enable_infectivity_scaling_boxcar;
        bool  enable_infectivity_scaling_climate;
        bool  enable_infectivity_scaling_density;
        bool  enable_infectivity_scaling_exponential;
        bool  enable_infectivity_scaling_sinusoid;

        float infectivity_boxcar_amplitude;
        float infectivity_boxcar_start_time;
        float infectivity_boxcar_end_time;
        float infectivity_exponential_baseline;
        float infectivity_exponential_rate;
        float infectivity_exponential_delay;
        float infectivity_multiplier;
        float infectivity_population_density_halfmax;
        float infectivity_reservoir_end_time;
        float infectivity_reservoir_size;
        float infectivity_reservoir_start_time;
        float infectivity_sinusoidal_amplitude;
        float infectivity_sinusoidal_phase;

        // Event handling
        friend class NodeEventContextHost;
        friend class Simulation; // so migration can call configureAndAdd?????
        NodeEventContextHost *event_context_host;
        std::vector<EventTrigger::Enum> events_from_other_nodes ;

        //  Counters (some for reporting, others also for internal calculations)
        float statPop;
        float Infected;
        float Births;
        float Disease_Deaths;
        float new_infections;
        float new_reportedinfections;
        float Cumulative_Infections;
        float Cumulative_Reported_Infections;
        float Campaign_Cost;
        long int Possible_Mothers;
        float symptomatic;
        float newly_symptomatic;

        float mean_age_infection;      // (years)
        float newInfectedPeopleAgeProduct;
        static const int infection_averaging_window = 1;   // = 30 time steps
        std::list<float> infected_people_prior; // [infection_averaging_window];
        std::list<float> infected_age_people_prior; // [infection_averaging_window];

        float infectionrate; // TODO: this looks like its only a reporting counter now and possibly not accurately updated in all cases
        float mInfectivity;

        // Reporting container for strain tracking
        std::map<std::string, int>                  strain_map_clade;
        std::map<std::string, int>                  strain_map_genome;
        std::map<std::string, std::vector<float>>   strain_map_data;

        ISimulationContext *parent;     // Access back to simulation methods
        ISimulation* parent_sim; //reduce access to RNG

        bool demographics_birth;
        bool enable_demographics_risk;

        float base_sample_rate;        // Fraction of individuals in each node to sample;
        float max_sampling_cell_pop;
        float sample_rate_birth;
        float sample_rate_0_18mo;
        float sample_rate_18mo_4yr;
        float sample_rate_5_9;
        float sample_rate_10_14;
        float sample_rate_15_19;
        float sample_rate_20_plus;
        float rel_sample_rate_immune;
        float immune_threshold_for_downsampling;
        float prob_maternal_infection_transmission;
        float population_scaling_factor;
        bool vital_dynamics;
        bool enable_natural_mortality;
        bool enable_maternal_infection_transmission;
        bool enable_initial_prevalence;

        std::vector<float> init_prev_fraction;
        std::vector<int>   init_prev_clade;
        std::vector<int>   init_prev_genome;

        bool vital_birth;
        VitalBirthDependence::Enum                           vital_birth_dependence;                           // Vital_Birth_Dependence
        VitalBirthTimeDependence::Enum                       vital_birth_time_dependence;                      //Time dependence in Birth Rate
        float x_birth;
        float x_othermortality;

        RouteList_t routes;

        virtual void Initialize();
        virtual void setupEventContextHost();
        virtual bool Configure( const Configuration* config ) override;
        void ExtractDataFromDemographics();
        virtual void LoadImmunityDemographicsDistribution();
        virtual void LoadOtherDiseaseSpecificDistributions() {};

        // Updates
        virtual void updateInfectivity(float dt = 0.0f);
        virtual void updatePopulationStatistics(float=1.0);     // called by updateinfectivity to gather population statistics
        virtual void accumulateIndividualPopulationStatistics(float dt, IIndividualHuman* individual);

    public: 
        void  updateVitalDynamics(float dt = 1.0f);             // handles births and non-disease mortality
        virtual void considerPregnancyForIndividual( bool bPossibleMother, bool bIsPregnant, float age, int individual_id, float dt, IIndividualHuman* pIndividual = nullptr ); 
        virtual float initiatePregnancyForIndividual( int individual_id, float dt ) override;
        virtual bool updatePregnancyForIndividual( int individual_id, float duration ) override;
        virtual void accumulateIndividualPopStatsByValue(float mcw, float infectiousness, bool poss_mom, bool is_infected, bool is_symptomatic, bool is_newly_symptomatic);
        virtual void resetNodeStateCounters(void);
    protected:

        // Population Initialization
        virtual void populateNewIndividualsFromDemographics(int count_new_individuals = 100);
        virtual void populateNewIndividualsByBirth(int count_new_individuals = 100) override;
        virtual void populateNewIndividualFromMotherId( unsigned int temp_mother_id );
        virtual void populateNewIndividualFromMotherPointer( IIndividualHuman* mother );
        virtual unsigned int populateNewIndividualFromMotherParams( float mcw, unsigned int child_infections );
        void  conditionallyInitializePregnancy( IIndividualHuman* temp_mother );
        float getPrevalenceInPossibleMothers();
        virtual float drawInitialSusceptibility(float ind_init_age);

        virtual IIndividualHuman *createHuman( suids::suid id, float MCweight, float init_age, int gender);
        IIndividualHuman* configureAndAddNewIndividual(float ind_MCweight=1.0f, float ind_init_age=(20*DAYSPERYEAR), float comm_init_prev=0.0f, float comm_female_ratio=0.5f, float init_mod_acquire=1.0f);
        virtual IIndividualHuman* addNewIndividual(
            float monte_carlo_weight = 1.0,
            float initial_age = 0,
            int gender = 0,
            int initial_infections = 0,
            float susceptibility_parameter = 1.0,
            float risk_parameter = 1.0,
            float migration_heterogeneity = 1.0);

        virtual void RemoveHuman( int index );

        virtual IIndividualHuman* addNewIndividualFromSerialization();

        double calculateInitialAge( double temp_age );
        Fraction adjustSamplingRateByImmuneState( Fraction sampling_rate, bool is_immune ) const;
        Fraction adjustSamplingRateByAge( Fraction sampling_rate, double age ) const;

        // Reporting
        virtual void updateNodeStateCounters(IIndividualHuman *ih);
        virtual void finalizeNodeStateCounters(void);
        virtual void reportNewInfection(IIndividualHuman *ih);
        virtual void reportDetectedInfection(IIndividualHuman *ih);

        // Migration
        virtual void processEmigratingIndividual(IIndividualHuman* i); // not a public method since decision to emigrate is an internal one;
        virtual void postIndividualMigration(IIndividualHuman* ind);
        void resolveEmigration(IIndividualHuman *tempind);

        // Fix up child object pointers after deserializing
        virtual INodeContext *getContextPointer();
        virtual void propagateContextToDependents();


        const SimulationConfig* params() const;

        float birth_rate_sinusoidal_forcing_amplitude;  // Only for Birth_Rate_Time_Dependence = SINUSOIDAL_FUNCTION_OF_TIME
        float birth_rate_sinusoidal_forcing_phase;      // Only for Birth_Rate_Time_Dependence = SINUSOIDAL_FUNCTION_OF_TIME
        float birth_rate_boxcar_forcing_amplitude;      // Only for Birth_Rate_Time_Dependence = ANNUAL_BOXCAR_FUNCTION
        float birth_rate_boxcar_start_time;             // Only for Birth_Rate_Time_Dependence = ANNUAL_BOXCAR_FUNCTION
        float birth_rate_boxcar_end_time;               // Only for Birth_Rate_Time_Dependence = ANNUAL_BOXCAR_FUNCTION

        // Skipping functions & variables
        virtual int calcGap();
        bool bSkipping; // for skip exposure
        std::map< TransmissionRoute::Enum, float > maxInfectionProb; // set to 1.0 if not defined
        int gap;
        virtual void computeMaxInfectionProb( float dt );
        // TBD: Put below in cpp
        virtual float GetMaxInfectionProb( TransmissionRoute::Enum route ) const
        {
            // Note that in GENERIC there's on ly one route. Can get tricky b/w CONTACT and ALL.
            return maxInfectionProb.at( route );
        }
        VitalDeathDependence::Enum                           vital_death_dependence;                           // Vital_Death_Dependence

        RANDOMBASE* m_pRng;
        suids::distributed_generator m_IndividualHumanSuidGenerator;

        DECLARE_SERIALIZABLE(Node);

#pragma warning( pop )
    };
}

#define CONTACT "contact"
