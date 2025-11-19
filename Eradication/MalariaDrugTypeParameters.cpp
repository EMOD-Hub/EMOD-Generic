
#include "stdafx.h"
#include "MalariaDrugTypeParameters.h"
#include "IStrainIdentity.h"
#include "MalariaEnums.h"
#include "IGenomeMarkers.h"
#include "Debug.h"
#include "Common.h"
#include "Exceptions.h"
#include "Log.h"

SETUP_LOGGING( "MalariaDrugTypeParameters" )

namespace Kernel
{
    // ------------------------------------------------------------------------
    // --- DoseFractionByAge
    // ------------------------------------------------------------------------

    DoseFractionByAge::DoseFractionByAge()
        : JsonConfigurable()
        , m_AgeDays( 0.0 )
        , m_DoseFraction( 1.0 )
    {
    }

    DoseFractionByAge::~DoseFractionByAge()
    {
    }

    bool DoseFractionByAge::Configure( const Configuration * inputJson )
    {
        float years = 0.0;

        initConfigTypeMap( "Upper_Age_In_Years",     &years,          Upper_Age_In_Years_DESC_TEXT,     0.0f, MAX_HUMAN_AGE, 0.0f );
        initConfigTypeMap( "Fraction_Of_Adult_Dose", &m_DoseFraction, Fraction_Of_Adult_Dose_DESC_TEXT, 0.0f,           1.0, 1.0f );

        bool is_configured = JsonConfigurable::Configure( inputJson );
        if( is_configured && !JsonConfigurable::_dryrun )
        {
            m_AgeDays = years * DAYSPERYEAR;
        }
        return is_configured;
    }

    float DoseFractionByAge::GetAgeDays() const
    {
        return m_AgeDays;
    }

    float DoseFractionByAge::GetDoseFraction() const
    {
        return m_DoseFraction;
    }

    // ------------------------------------------------------------------------
    // --- DoseMap
    // ------------------------------------------------------------------------

    DoseMap::DoseMap()
        : JsonConfigurableCollection( "Fractional_Dose_By_Upper_Age" )
    {
    }

    DoseMap::~DoseMap()
    {
    }

    bool CheckAge( const DoseFractionByAge* pLeft, const DoseFractionByAge* pRight )
    {
        return (pLeft->GetAgeDays() < pRight->GetAgeDays());
    }

    bool FindAge( const float leftAgeDays, const DoseFractionByAge* pRight )
    {
        return (leftAgeDays < pRight->GetAgeDays());
    }

    void DoseMap::CheckConfiguration()
    {
        std::sort( m_Collection.begin(), m_Collection.end(), CheckAge );
    }

    float DoseMap::GetFractionalDose( float ageInDays ) const
    {
        auto it = std::upper_bound( m_Collection.begin(), m_Collection.end(), ageInDays, FindAge );

        float fractional_dose = 1.0;
        if( it != m_Collection.end() )
        {
            fractional_dose = (*it)->GetDoseFraction();
        }
        return fractional_dose;
    }

    DoseFractionByAge* DoseMap::CreateObject()
    {
        return new DoseFractionByAge();
    }

    // ------------------------------------------------------------------------
    // --- DrugResistanceModifiers
    // ------------------------------------------------------------------------

#define GMM_PKPD_C50_Modifier_DESC_TEXT "TBD"
#define GMM_Max_IRBC_Kill_Modifier_DESC_TEXT "TBD"

    GenomeMarkerModifiers::GenomeMarkerModifiers( const std::string& rMarkerName, uint64_t genomeBitMask )
        : JsonConfigurable()
        , m_MarkerName( rMarkerName )
        , m_C50(1.0f)
        , m_MaxKilling(1.0f)
        , m_GenomeBitMask( genomeBitMask )
    {
    }

    GenomeMarkerModifiers::~GenomeMarkerModifiers()
    {
    }

    bool GenomeMarkerModifiers::Configure( const Configuration * inputJson )
    {
        initConfigTypeMap( "PKPD_C50_Modifier",      &m_C50,        GMM_PKPD_C50_Modifier_DESC_TEXT,      0.0f, 1000.0f, 1.0f );
        initConfigTypeMap( "Max_IRBC_Kill_Modifier", &m_MaxKilling, GMM_Max_IRBC_Kill_Modifier_DESC_TEXT, 0.0f, 1000.0f, 1.0f );

        return JsonConfigurable::Configure( inputJson );
    }

    // ------------------------------------------------------------------------
    // --- DrugResistanceModifiers
    // ------------------------------------------------------------------------

    DrugResistantModifiers::DrugResistantModifiers( const IGenomeMarkers& rGenomeMarkers )
        : JsonConfigurable()
        , m_ModifierCollection()
    {
        const std::set<std::string>& r_marker_names = rGenomeMarkers.GetNameSet();
        for( auto& r_name : r_marker_names )
        {
            uint64_t bits = rGenomeMarkers.GetBits( r_name );
            m_ModifierCollection.push_back( GenomeMarkerModifiers( r_name,  bits ) );
        }
    }

    DrugResistantModifiers::~DrugResistantModifiers()
    {
    }

    json::QuickBuilder DrugResistantModifiers::GetSchema()
    {
        GenomeMarkerModifiers mods(std::string(""),0);
        if( JsonConfigurable::_dryrun )
        {
            mods.Configure( nullptr );
        }

        json::QuickBuilder schema( GetSchemaBase() );
        auto tn = JsonConfigurable::_typename_label();
        auto ts = JsonConfigurable::_typeschema_label();
        schema[ tn ] = json::String( "idmType:DrugResistantModifiers" );

        schema[ ts ] = json::Array();
        schema[ ts ][ 0 ] = json::Object();
        schema[ ts ][ 0 ][ "<genome marker name>" ] = json::Object();
        schema[ ts ][ 0 ][ "<genome marker name>" ][ "type" ] = json::String( "String" );
        schema[ ts ][ 0 ][ "<genome marker name>" ][ "constraints" ] = json::String( "Genome_Markers" );
        schema[ ts ][ 0 ][ "<GenomeMarkerModifiers>" ] = mods.GetSchema().As<Object>();

        return schema;
    }

    void DrugResistantModifiers::ConfigureFromJsonAndKey( const Configuration* inputJson, const std::string& key )
    {
        // Temporary object created so we can 'operate' on json with the desired tools
        auto p_config = Configuration::CopyFromElement( (*inputJson)[ key ], inputJson->GetDataLocation() );

        for( GenomeMarkerModifiers& r_mods : m_ModifierCollection )
        {
            if( !p_config->Exist( r_mods.GetMarkerName() ) )
            {
                std::stringstream ss;
                ss << "Cannot find GenomeMarkerModifiers for genome marker = " << r_mods.GetMarkerName();
                throw InvalidInputDataException( __FILE__, __LINE__, __FUNCTION__, ss.str().c_str() );
            }
            Configuration* p_element_config = Configuration::CopyFromElement( (*p_config)[ r_mods.GetMarkerName().c_str() ], inputJson->GetDataLocation() );

            r_mods.Configure( p_element_config );

            delete p_element_config;
        }
        delete p_config;

    }

    int DrugResistantModifiers::Size() const
    {
        return m_ModifierCollection.size();
    }

    const GenomeMarkerModifiers& DrugResistantModifiers::operator[]( int index ) const
    {
        return m_ModifierCollection[ index ];
    }

    float DrugResistantModifiers::GetC50( const IStrainIdentity& rStrain ) const
    {
        float c50 = 1.0;
        for( const GenomeMarkerModifiers& r_mods : m_ModifierCollection )
        {
            if( r_mods.HasGenomeBit( rStrain.GetGeneticID() )  )
            {
                c50 *= r_mods.GetC50();
            }
        }
        return c50;
    }

    float DrugResistantModifiers::GetMaxKilling( const IStrainIdentity& rStrain ) const
    {
        float max_killing = 1.0;
        for( const GenomeMarkerModifiers& r_mods : m_ModifierCollection )
        {
            if( r_mods.HasGenomeBit( rStrain.GetGeneticID() ) )
            {
                max_killing *= r_mods.GetMaxKilling();
            }
        }
        return max_killing;
    }

    // ------------------------------------------------------------------------
    // --- MalariaDrugTypeParameters
    // ------------------------------------------------------------------------

    MalariaDrugTypeParameters::MalariaDrugTypeParameters( const std::string &drugType, const IGenomeMarkers& rGenomeMarkers )
        : max_drug_IRBC_kill(5.0f)
        , drug_hepatocyte_killrate(0.0f)
        , drug_gametocyte02_killrate( 0.0f )
        , drug_gametocyte34_killrate( 0.0f )
        , drug_gametocyteM_killrate( 0.0f )
        , drug_pkpd_c50(100.0f)
        , drug_Cmax(1000.0f)
        , drug_Vd(10.0f)
        , drug_decay_T1(1.0f)
        , drug_decay_T2(1.0f)
        , drug_fulltreatment_doses(3)
        , drug_dose_interval(1.0f)
        , bodyweight_exponent(0.0f)
        , dose_map()
        , m_Modifiers( rGenomeMarkers )
        , _drugType( drugType )
    {
        LOG_DEBUG_F( "ctor: drugType = %s\n", drugType.c_str() );
        initConfigTypeMap( "Max_Drug_IRBC_Kill",         &max_drug_IRBC_kill,         Max_Drug_IRBC_Kill_DESC_TEXT,         0.0f, 100000.0f, 5.0f );
        initConfigTypeMap( "Drug_Hepatocyte_Killrate",   &drug_hepatocyte_killrate,   Drug_Hepatocyte_Killrate_DESC_TEXT,   0.0f, 100000.0f, 0.0f );
        initConfigTypeMap( "Drug_Gametocyte02_Killrate", &drug_gametocyte02_killrate, Drug_Gametocyte02_Killrate_DESC_TEXT, 0.0f, 100000.0f, 0.0f );
        initConfigTypeMap( "Drug_Gametocyte34_Killrate", &drug_gametocyte34_killrate, Drug_Gametocyte34_Killrate_DESC_TEXT, 0.0f, 100000.0f, 0.0f );
        initConfigTypeMap( "Drug_GametocyteM_Killrate",  &drug_gametocyteM_killrate,  Drug_GametocyteM_Killrate_DESC_TEXT,  0.0f, 100000.0f, 0.0f );
        initConfigTypeMap( "Drug_PKPD_C50",              &drug_pkpd_c50,              Drug_PKPD_C50_DESC_TEXT,              0.0f, 100000.0f, 100.0f );
        initConfigTypeMap( "Drug_Cmax",                  &drug_Cmax,                  Drug_Cmax_DESC_TEXT,                  0.0f, 100000.0f, 1000.0f );
        initConfigTypeMap( "Drug_Vd",                    &drug_Vd,                    Drug_Vd_DESC_TEXT,                    0.0f, 100000.0f, 10.0f );
        initConfigTypeMap( "Drug_Decay_T1",              &drug_decay_T1,              Drug_Decay_T1_DESC_TEXT,              0.0f, 100000.0f, 1.0f );
        initConfigTypeMap( "Drug_Decay_T2",              &drug_decay_T2,              Drug_Decay_T2_DESC_TEXT,              0.0f, 100000.0f, 1.0f );
        initConfigTypeMap( "Drug_Fulltreatment_Doses",   &drug_fulltreatment_doses,   Drug_Fulltreatment_Doses_DESC_TEXT,   1,    100000,    3 );
        initConfigTypeMap( "Drug_Dose_Interval",         &drug_dose_interval,         Drug_Dose_Interval_DESC_TEXT,         0.0f, 100000.0f, 1.0f );

        // was optionally parsed, but that was a little heterodox: age-dependent dosing specifications
        initConfigTypeMap( "Bodyweight_Exponent",        &bodyweight_exponent,        DRUG_Bodyweight_Exponent_DESC_TEXT,   0.0f, 100000.0f, 0.0f );
    }

    MalariaDrugTypeParameters::~MalariaDrugTypeParameters()
    {
    }

    MalariaDrugTypeParameters* MalariaDrugTypeParameters::CreateMalariaDrugTypeParameters(
        const Configuration* inputJson,
        const std::string &drugType,
        const IGenomeMarkers& rGenomeMarkers
    )
    { 
        LOG_DEBUG( "Create\n" );

        // The MDTP construct exists to cache once-per-simulation values of one or more drugs.
        // Until there can be different parameters for the same drug in a single simulation,
        // at which point we would probably just make those configurable properties of AntimalarialDrug itself,
        // there is no need to keep leaking memory here.

        // There used to be code to check for and retrieve the existing instance for a given drugType from the map
        // but there was no use case for that and function name suggests creation.
        auto * params = _new_ MalariaDrugTypeParameters( drugType, rGenomeMarkers );
        release_assert( params );
        params->Initialize(drugType);
        if( !JsonConfigurable::_dryrun )
        {
            try
            {
                Configuration* drug_config = Configuration::CopyFromElement( (*inputJson)["Malaria_Drug_Params"][drugType], inputJson->GetDataLocation() );
                params->Configure( drug_config );
                delete drug_config;
                drug_config = nullptr;

                // Check validity of dosing regimen
                float sim_tstep = (*inputJson)["Simulation_Timestep"].As<Number>();
                float updates_per_tstep = (*inputJson)["Infection_Updates_Per_Timestep"].As<Number>();
                if ( params->drug_dose_interval < sim_tstep/updates_per_tstep )
                {
                    std::ostringstream oss;
                    oss << "time_between_doses (" << params->drug_dose_interval << ") is less than dt (" << sim_tstep/updates_per_tstep << ")" << std::endl;
                    throw GeneralConfigurationException( __FILE__, __LINE__, __FUNCTION__, oss.str().c_str() );
                }

            }
            catch( const json::Exception &e )
            {
                // Exception getting parameter block for drug of type "drugType" from config.json
                throw GeneralConfigurationException( __FILE__, __LINE__, __FUNCTION__, e.what() ); 
            }
        }
        return params;
    }

    bool
    MalariaDrugTypeParameters::Configure(
        const ::Configuration *config
    )
    {
        LOG_DEBUG( "Configure\n" );

        if( config->Exist( "Resistance" ) || JsonConfigurable::_dryrun )
        {
            initConfigComplexType( "Resistance", &m_Modifiers, Resistance_DESC_TEXT );
        }
        if( config->Exist( "Fractional_Dose_By_Upper_Age" ) || JsonConfigurable::_dryrun ) // :(
        {
            initConfigComplexType( "Fractional_Dose_By_Upper_Age", &dose_map, Fraction_Of_Adult_Dose_DESC_TEXT ); // not sure this is right desc_text
        }

        return JsonConfigurable::Configure( config );
    }

    void MalariaDrugTypeParameters::Initialize(const std::string &drugType)
    {
        LOG_DEBUG_F( "MalariaDrugTypeParameters::Initialize: drug type = %s\n", drugType.c_str() );

        int eDrug = MalariaDrugType::pairs::lookup_value(drugType.c_str());

        if ( (eDrug < 0) && !JsonConfigurable::_dryrun )
        {
            LOG_WARN_F("Anti-malarial drug name in Malaria_Drug_Params block (%s) is not one of the standard cases.\n", drugType.c_str());
        }

        _drugType = drugType;
    }

    float MalariaDrugTypeParameters::GetMaxDrugIRBCKill() const
    {
        return max_drug_IRBC_kill;
    }

    float MalariaDrugTypeParameters::GetKillRateHepatocyte() const
    {
        return drug_hepatocyte_killrate;
    }

    float MalariaDrugTypeParameters::GetKillRateGametocyte02() const
    {
        return drug_gametocyte02_killrate;
    }

    float MalariaDrugTypeParameters::GetKillRateGametocyte34() const
    {
        return drug_gametocyte34_killrate;
    }

    float MalariaDrugTypeParameters::GetKillRateGametocyteM() const
    {
        return drug_gametocyteM_killrate;
    }

    float MalariaDrugTypeParameters::GetPkpdC50() const
    {
        return drug_pkpd_c50;
    }

    float MalariaDrugTypeParameters::GetCMax() const
    {
        return drug_Cmax;
    }

    float MalariaDrugTypeParameters::GetVd() const
    {
        return drug_Vd;
    }

    float MalariaDrugTypeParameters::GetDecayT1() const
    {
        return drug_decay_T1;
    }

    float MalariaDrugTypeParameters::GetDecayT2() const
    {
        return drug_decay_T2;
    }

    int MalariaDrugTypeParameters::GetFullTreatmentDoses() const
    {
        return drug_fulltreatment_doses;
    }

    float MalariaDrugTypeParameters::GetDoseInterval() const
    {
        return drug_dose_interval;
    }

    float MalariaDrugTypeParameters::GetBodyWeightExponent() const
    {
        return bodyweight_exponent;
    }

    const DoseMap& MalariaDrugTypeParameters::GetDoseMap() const
    {
        return dose_map;
    }

    const DrugResistantModifiers& MalariaDrugTypeParameters::GetResistantModifiers() const
    {
        return m_Modifiers;
    }
}
