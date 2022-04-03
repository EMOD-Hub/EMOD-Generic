/***************************************************************************************************

Copyright (c) 2018 Intellectual Ventures Property Holdings, LLC (IVPH) All rights reserved.

EMOD is licensed under the Creative Commons Attribution-Noncommercial-ShareAlike 4.0 License.
To view a copy of this license, visit https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode

***************************************************************************************************/

#include "stdafx.h"
#include "Environment.h"
#include "Debug.h"
#include "Infection.h"
#include "InterventionsContainer.h"
#include "ISusceptibilityContext.h"
#include "RANDOM.h"
#include "SimulationConfig.h"
#include "MathFunctions.h"
#include "StrainIdentity.h"
#include "IIndividualHumanContext.h"
#include "IDistribution.h"
#include "DistributionFactory.h"
#include "IndividualEventContext.h"
#include "NodeEventContext.h"

SETUP_LOGGING( "Infection" )

namespace Kernel
{
    // static initializers for config base class
    MortalityTimeCourse::Enum  InfectionConfig::mortality_time_course   =  MortalityTimeCourse::DAILY_MORTALITY;
    IDistribution* InfectionConfig::infectious_distribution = nullptr;
    IDistribution* InfectionConfig::incubation_distribution = nullptr;
    IDistribution* InfectionConfig::infectivity_distribution = nullptr;
    float InfectionConfig::base_mortality = 1.0f;
    bool  InfectionConfig::enable_disease_mortality = false;
    bool  InfectionConfig::enable_strain_tracking   = false;
    unsigned int InfectionConfig::log2genomes     = 0;
    unsigned int InfectionConfig::number_clades   = 1;
    unsigned int InfectionConfig::number_genomes  = 1;
    
    // symptomatic
    float InfectionConfig::symptomatic_infectious_offset = FLT_MAX; //disabled

    GET_SCHEMA_STATIC_WRAPPER_IMPL(Infection,InfectionConfig)
    BEGIN_QUERY_INTERFACE_BODY(InfectionConfig)
    END_QUERY_INTERFACE_BODY(InfectionConfig)


    bool InfectionConfig::Configure(const Configuration* config)
    {
        LOG_DEBUG("Configure\n");

        initConfigTypeMap("Enable_Disease_Mortality", &enable_disease_mortality, Enable_Disease_Mortality_DESC_TEXT, true, "Simulation_Type", "GENERIC_SIM,VECTOR_SIM,STI_SIM,ENVIRONMENTAL_SIM,MALARIA_SIM,TBHIV_SIM,TYPHOID_SIM,PY_SIM,HIV_SIM");
        initConfig( "Mortality_Time_Course", mortality_time_course, config, MetadataDescriptor::Enum("mortality_time_course", Mortality_Time_Course_DESC_TEXT, MDD_ENUM_ARGS(MortalityTimeCourse)), "Enable_Disease_Mortality" );
        initConfigTypeMap("Base_Mortality", &base_mortality, Base_Mortality_DESC_TEXT, 0.0f, 1000.0f, 0.001f, "Enable_Disease_Mortality"); // should default change depending on disease?

        // Configure incubation duration
        DistributionFunction::Enum incubation_period_function( DistributionFunction::NOT_INITIALIZED );
        initConfig("Incubation_Period_Distribution", incubation_period_function, config, MetadataDescriptor::Enum("Incubation_Period_Distribution", Incubation_Period_Distribution_DESC_TEXT, MDD_ENUM_ARGS(DistributionFunction)), "Simulation_Type", "GENERIC_SIM,STI_SIM,VECTOR_SIM,DENGUE_SIM,MALARIA_SIM,AIRBORNE_SIM,ENVIRONMENTAL_SIM,POLIO_SIM,TYPHOID_SIM,PY_SIM");
        if( incubation_period_function != DistributionFunction::NOT_INITIALIZED || JsonConfigurable::_dryrun )
        {
            incubation_distribution = DistributionFactory::CreateDistribution( this, incubation_period_function, "Incubation_Period", config );
        }

        // Configure infectious duration
        DistributionFunction::Enum infectious_distribution_function( DistributionFunction::NOT_INITIALIZED );
        initConfig("Infectious_Period_Distribution", infectious_distribution_function, config, MetadataDescriptor::Enum("Infectious_Period_Distribution", Infectious_Period_Distribution_DESC_TEXT, MDD_ENUM_ARGS(DistributionFunction)), "Simulation_Type", "GENERIC_SIM,STI_SIM,VECTOR_SIM,AIRBORNE_SIM,ENVIRONMENTAL_SIM,POLIO_SIM,PY_SIM");
        if( infectious_distribution_function != DistributionFunction::NOT_INITIALIZED || JsonConfigurable::_dryrun )
        {
            infectious_distribution = DistributionFactory::CreateDistribution( this, infectious_distribution_function, "Infectious_Period", config );
        } 

        // Configure infectivity
        DistributionFunction::Enum infectivity_distribution_function( DistributionFunction::NOT_INITIALIZED );
        initConfig("Base_Infectivity_Distribution", infectivity_distribution_function, config, MetadataDescriptor::Enum("Base_Infectivity_Distribution", Base_Infectivity_Distribution_DESC_TEXT, MDD_ENUM_ARGS(DistributionFunction)), "Simulation_Type", "GENERIC_SIM,STI_SIM,HIV_SIM,VECTOR_SIM,AIRBORNE_SIM,TBHIV_SIM,ENVIRONMENTAL_SIM,PY_SIM");
        if( infectivity_distribution_function != DistributionFunction::NOT_INITIALIZED || JsonConfigurable::_dryrun )
        {
            infectivity_distribution = DistributionFactory::CreateDistribution( this, infectivity_distribution_function, "Base_Infectivity", config );
        }

        // Symptomatic; 0.0 = Symptomatic when infectious, FLT_MAX = Individual never symptomatic
        initConfigTypeMap( "Symptomatic_Infectious_Offset", &symptomatic_infectious_offset, Symptomatic_Infectious_Offset_DESC_TEXT, -FLT_MAX, FLT_MAX, 0.0f, "Simulation_Type", "GENERIC_SIM" );

        // Strain tracking
        initConfigTypeMap("Enable_Strain_Tracking",            &enable_strain_tracking,  Enable_Strain_Tracking_DESC_TEXT, false);

        const std::map<std::string, std::string> depends_set_number_clades  {{"Enable_Strain_Tracking", "1"}, {"Simulation_Type", "GENERIC_SIM,VECTOR_SIM,MALARIA_SIM,ENVIRONMENTAL_SIM,TYPHOID_SIM,STI_SIM,HIV_SIM,AIRBORNE_SIM,TBHIV_SIM,PY_SIM"}};
        const std::map<std::string, std::string> depends_set_log2genomes    {{"Enable_Strain_Tracking", "1"}, {"Simulation_Type", "GENERIC_SIM,VECTOR_SIM,DENGUE_SIM,STI_SIM,HIV_SIM,AIRBORNE_SIM,TBHIV_SIM,PY_SIM"}};
        initConfigTypeMap("Number_of_Clades",                  &number_clades,  Number_of_Clades_DESC_TEXT,                   1,  10,   1, nullptr, nullptr, &depends_set_number_clades);
        initConfigTypeMap("Log2_Number_of_Genomes_per_Clade",  &log2genomes,    Log2_Number_of_Genomes_per_Clade_DESC_TEXT,   0,  24,   0, nullptr, nullptr, &depends_set_log2genomes );

        // Evaluate configuration
        bool bRet = JsonConfigurable::Configure( config );

        // Post-process values
        number_genomes = number_genomes << log2genomes;

        return bRet;
    }

    Infection::Infection()
        : parent(nullptr)
        , suid(suids::nil_suid())
        , duration(0.0f)
        , total_duration(0.0f)
        , incubation_timer(0.0f)
        , infectious_timer(0.0f)
        , infectiousness(0.0f)
        , infectiousnessByRoute()
        , StateChange(InfectionStateChange::None)
        , infection_strain(nullptr)
        , m_is_symptomatic( false )
        , m_is_newly_symptomatic( false )
    {
    }

    BEGIN_QUERY_INTERFACE_BODY(Infection)
        HANDLE_INTERFACE(IInfection)
        HANDLE_ISUPPORTS_VIA(IInfection)
    END_QUERY_INTERFACE_BODY(Infection)

    Infection::Infection(IIndividualHumanContext *context)
        : parent(context)
        , suid(suids::nil_suid())
        , duration(0.0f)
        , total_duration(0.0f)
        , incubation_timer(0.0f)
        , infectious_timer(0.0f)
        , infectiousness(0.0f)
        , infectiousnessByRoute()
        , StateChange(InfectionStateChange::None)
        , infection_strain(nullptr)
        , m_is_symptomatic( false )
        , m_is_newly_symptomatic( false )
    {
    }

    void Infection::Initialize(suids::suid _suid)
    {
        suid = _suid;
    }

    Infection *Infection::CreateInfection(IIndividualHumanContext *context, suids::suid _suid)
    {
        Infection *newinfection = _new_ Infection(context);
        newinfection->Initialize(_suid);

        return newinfection;
    }

    Infection::~Infection()
    {
        delete infection_strain;
    }

    void Infection::SetParameters( IStrainIdentity* infstrain, float incubation_period_override ) // or something
    {
        // Set up infection strain
        CreateInfectionStrain(infstrain);

        LOG_VALID_F("New Infection: Individual: %d Node: %d Clade: %d Genome: %d .\n",
            GetParent()->GetSuid().data,
            GetParent()->GetEventContext()->GetNodeEventContext()->GetExternalId(),
            infection_strain->GetCladeID(),
            infection_strain->GetGeneticID());  // TODO: Test code from cwiswell in case this breaks

        // Incubation duration; subtracting FLT_MIN ensures infectious when incubation_time = 0.0f 
        if( incubation_period_override >= 0.0 )
        {
            incubation_timer = incubation_period_override - FLT_MIN;
        }
        else if(InfectionConfig::incubation_distribution)
        {
            incubation_timer = InfectionConfig::incubation_distribution->Calculate( GetParent()->GetRng() ) - FLT_MIN;
        }
        else
        {
            // Should only get here if NOT_INITIALIZED is used as distribution type.
            throw GeneralConfigurationException( __FILE__, __LINE__, __FUNCTION__, "Incubation_Period_Distribution must be initialized.");
        }
        LOG_DEBUG_F( "incubation_timer initialized to %f for individual %d\n", incubation_timer, GetParent()->GetSuid().data );

        // Infection duration
        if(InfectionConfig::infectious_distribution)
        {
            infectious_timer = InfectionConfig::infectious_distribution->Calculate( GetParent()->GetRng() );
        }
        else
        {
            // Should only get here if NOT_INITIALIZED is used as distribution type.
            throw GeneralConfigurationException( __FILE__, __LINE__, __FUNCTION__, "Infectious_Period_Distribution must be initialized.");
        }
        LOG_DEBUG_F( "infectious_timer = %f\n", infectious_timer );

        // Infectiousness
        if(InfectionConfig::infectivity_distribution)
        {
            infectiousness = InfectionConfig::infectivity_distribution->Calculate( GetParent()->GetRng() );
        }
        else
        {
            // Should only get here if NOT_INITIALIZED is used as distribution type.
            throw GeneralConfigurationException( __FILE__, __LINE__, __FUNCTION__, "Base_Infectivity_Distribution must be initialized.");
        }
        LOG_DEBUG_F( "infectiousness initialized to %f for individual %d\n", infectiousness, GetParent()->GetSuid().data );

        total_duration = incubation_timer + infectious_timer;
        StateChange    = InfectionStateChange::None;
    }

    void Infection::InitInfectionImmunology(ISusceptibilityContext* _immunity)
    {
    }

    // TODO future : grant access to the susceptibility object by way of the host context and keep the update call neutral
    void Infection::Update(float dt, ISusceptibilityContext* immunity)
    {
        StateChange = InfectionStateChange::None;
        duration += dt;

        // To query for mortality-reducing effects of drugs or vaccines
        IDrugVaccineInterventionEffects* idvie = nullptr;

        // if disease has a daily mortality rate, and disease mortality is on, then check for death. mortality_time_course depends-on enable_disease_mortality BUT DAILY_MORTALITY is default
        if (InfectionConfig::enable_disease_mortality && (InfectionConfig::mortality_time_course == MortalityTimeCourse::DAILY_MORTALITY) && (duration > incubation_timer))
        {
            if ( s_OK != GetParent()->GetInterventionsContext()->QueryInterface(GET_IID(IDrugVaccineInterventionEffects), (void**)&idvie) )
            {
                throw QueryInterfaceException( __FILE__, __LINE__, __FUNCTION__, "GetParent()->GetInterventionsContext()", "IDrugVaccineInterventionEffects", "IIndividualHumanInterventionsContext" );
            }
            float prob = InfectionConfig::base_mortality * dt * immunity->getModMortality() * idvie->GetInterventionReducedMortality();
            if( GetParent()->GetRng()->SmartDraw( prob ) )
            { 
                StateChange = InfectionStateChange::Fatal; 
            }
        }

        if (duration > total_duration)
        {
            // disease mortality active and is accounted for at end of infectious period. mortality_time_course depends-on enable_disease_mortality
            if (InfectionConfig::enable_disease_mortality && InfectionConfig::mortality_time_course == MortalityTimeCourse::MORTALITY_AFTER_INFECTIOUS )
            {
                if ( s_OK != GetParent()->GetInterventionsContext()->QueryInterface(GET_IID(IDrugVaccineInterventionEffects), (void**)&idvie) )
                {
                    throw QueryInterfaceException( __FILE__, __LINE__, __FUNCTION__, "GetParent()->GetInterventionsContext()", "IDrugVaccineInterventionEffects", "IIndividualHumanInterventionsContext" );
                }
                float prob = InfectionConfig::base_mortality * immunity->getModMortality() * idvie->GetInterventionReducedMortality();
                if( GetParent()->GetRng()->SmartDraw( prob ) )
                {
                    StateChange = InfectionStateChange::Fatal;
                }
                else
                {
                    StateChange = InfectionStateChange::Cleared;
                }//  For debugging only  (1-base_mortality) recover, rest chance die at end of infection, modified by mortality immunity
            }
            else
            {
                StateChange = InfectionStateChange::Cleared;
            }
        }

        UpdateSymptomatic( duration, incubation_timer );

        EvolveStrain(immunity, dt); // genomic modifications
    }

    void Infection::CreateInfectionStrain(IStrainIdentity* infstrain)
    {
        if (infection_strain == nullptr)
        {
            // this infection is new, not passed from another processor, so need to initialize the strain object
            infection_strain = _new_ Kernel::StrainIdentity;
        }

        if (infstrain != nullptr)
        {
            infection_strain->SetCladeID( infstrain->GetCladeID() );
            infection_strain->SetGeneticID( infstrain->GetGeneticID() );
        }
    }

    void Infection::EvolveStrain(ISusceptibilityContext* immunity, float dt)
    {
        // genetic evolution happens here.
        // infection_strain
    }

    void Infection::GetInfectiousStrainID( IStrainIdentity* infstrain ) 
    {
        // Really want to make this cloning an internal StrainIdentity function
        infstrain->SetCladeID( infection_strain->GetCladeID() );
        infstrain->SetGeneticID( infection_strain->GetGeneticID() );
    }

    const IStrainIdentity* Infection::GetStrain() const
    {
        return static_cast<IStrainIdentity*>(infection_strain);
    }

    bool Infection::StrainMatches( IStrainIdentity * pStrain )
    {
        return( infection_strain->GetCladeID() == pStrain->GetCladeID() );
    }

    void Infection::SetContextTo(IIndividualHumanContext* context)
    {
        parent = context;
    }

    IIndividualHumanContext* Infection::GetParent()
    {
        return parent;
    }

    suids::suid Infection::GetSuid() const 
    {
        return suid;
    }

    // overridden in derived classes but with different return types to hide the casting operation
    const SimulationConfig* Infection::params()
    {
        return GET_CONFIGURABLE(SimulationConfig);
    }

    InfectionStateChange::_enum Infection::GetStateChange() const
    {
        return StateChange;
    }

    float Infection::GetInfectiousness() const
    {
        return duration > incubation_timer ? infectiousness : 0.0f;
    }

    float Infection::GetInfectiousnessByRoute( const string& route ) const
    {
        if( infectiousnessByRoute.find( route ) == infectiousnessByRoute.end() )
        {
            throw BadMapKeyException( __FILE__, __LINE__, __FUNCTION__, "infectiousnesssByRoute", route.c_str() );
        }
        return infectiousnessByRoute.at(route); 
    }

    // Created for TB, but makes sense to be in base class, but no-one else is using yet, placeholder functionality
    bool Infection::IsActive() const
    {
        return false;
    }

    NonNegativeFloat Infection::GetDuration() const
    {
        return duration;
    }

    bool Infection::IsNewlySymptomatic() const
    {
        return  m_is_newly_symptomatic;
    }

    bool Infection::IsSymptomatic() const
    {
        return m_is_symptomatic;
    }

    void Infection::UpdateSymptomatic( float const duration, float const incubation_timer )
    {
        bool prev_symptomatic = m_is_symptomatic;
        m_is_symptomatic = DetermineSymptomatology( duration, incubation_timer );
        m_is_newly_symptomatic = ( m_is_symptomatic && !prev_symptomatic );
    }

    bool Infection::DetermineSymptomatology( float const duration, float const incubation_timer )
    {
        return ( ( duration - incubation_timer ) > InfectionConfig::symptomatic_infectious_offset );
    }

    REGISTER_SERIALIZABLE(Infection);

    void Infection::serialize(IArchive& ar, Infection* obj)
    {
        Infection& infection = *obj;
        ar.labelElement( "suid" )                          & infection.suid;
        ar.labelElement( "duration" )                      & infection.duration;
        ar.labelElement( "total_duration" )                & infection.total_duration;
        ar.labelElement( "incubation_timer" )              & infection.incubation_timer;
        ar.labelElement( "infectious_timer" )              & infection.infectious_timer;
        ar.labelElement( "infectiousness" )                & infection.infectiousness;
        ar.labelElement( "infectiousnessByRoute" )         & infection.infectiousnessByRoute;

        ar.labelElement( "StateChange" )                   & (uint32_t&)infection.StateChange;

        ar.labelElement( "infection_strain" );            StrainIdentity::serialize( ar, infection.infection_strain );

        ar.labelElement( "m_is_symptomatic" )              & infection.m_is_symptomatic;
        ar.labelElement( "m_is_newly_symptomatic" )        & infection.m_is_newly_symptomatic;
    }
}
