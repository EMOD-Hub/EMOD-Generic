/***************************************************************************************************

Copyright (c) 2018 Intellectual Ventures Property Holdings, LLC (IVPH) All rights reserved.

EMOD is licensed under the Creative Commons Attribution-Noncommercial-ShareAlike 4.0 License.
To view a copy of this license, visit https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode

***************************************************************************************************/

#include "stdafx.h"

#include "ReportRelationshipCensus.h"
#include "DllInterfaceHelper.h"
#include "FactorySupport.h"

#include "NodeEventContext.h"
#include "IIndividualHuman.h"
#include "IIndividualHumanSTI.h"
#include "IdmDateTime.h"
#include "INodeContext.h"

//******************************************************************************

#define THREE_MONTHS  ( 91) // ~3 months
#define SIX_MONTHS    (182) // ~6 months
#define NINE_MONTHS   (274) // ~9 months
#define TWELVE_MONTHS (365) // ~12 months
#define DEFAULT_NAME ("ReportRelationshipCensus.csv")

//******************************************************************************

SETUP_LOGGING( "ReportRelationshipCensus" )

static const char* _sim_types[]  = { "STI_SIM", "HIV_SIM", nullptr };
static const float PERIODS[]     = { THREE_MONTHS, SIX_MONTHS, NINE_MONTHS, TWELVE_MONTHS };
static std::vector<float> UNIQUE_PARTNER_TIME_PERIODS( PERIODS, PERIODS + sizeof( PERIODS ) / sizeof( PERIODS[ 0 ] ) );

Kernel::DllInterfaceHelper DLL_HELPER( _module, _sim_types );

//******************************************************************************
// DLL Methods
//******************************************************************************

#ifdef __cplusplus
extern "C" {
#endif

DTK_DLLEXPORT char*
__cdecl GetEModuleVersion(char* sVer, const Environment* pEnv)
{
    return DLL_HELPER.GetEModuleVersion( sVer, pEnv );
}

DTK_DLLEXPORT void
__cdecl GetSupportedSimTypes(char* simTypes[])
{
    DLL_HELPER.GetSupportedSimTypes( simTypes );
}

DTK_DLLEXPORT const char*
__cdecl GetType()
{
    return DLL_HELPER.GetType();
}

DTK_DLLEXPORT Kernel::IReport*
__cdecl GetReportInstantiator()
{
    return new Kernel::ReportRelationshipCensus();
}

#ifdef __cplusplus
}
#endif

//******************************************************************************
// ----------------------------------------
// --- ReportRelationshipCensus Methods
// ----------------------------------------

namespace Kernel
{
    ReportRelationshipCensus::ReportRelationshipCensus()
        : BaseTextReportEvents( DEFAULT_NAME )
        , m_ReportName( DEFAULT_NAME )
        , m_StartYear(0.0f)
        , m_EndYear( FLT_MAX )
        , m_ReportingIntervalYears(1.0f)
        , m_IntervalTimerDays(0.0f)
        , m_IsCollectingData(false)
        , m_FirstDataCollection(true)
    {
        // ------------------------------------------------------------------------------------------------
        // --- Since this report will be listening for events, it needs to increment its reference count
        // --- so that it is 1.  Other objects will be AddRef'ing and Release'ing this report/observer
        // --- so it needs to start with a refcount of 1.
        // ------------------------------------------------------------------------------------------------
        AddRef();
    }

    ReportRelationshipCensus::~ReportRelationshipCensus()
    {
    }

    bool ReportRelationshipCensus::Configure( const Configuration * inputJson )
    {
        // If you want to configure stuff, you can use initConfig() parameters and a custom_reports.json

        initConfigTypeMap( "Report_File_Name",         &m_ReportName,             "Name of the file to be written", DEFAULT_NAME );
        initConfigTypeMap( "Start_Year",               &m_StartYear,              "Year to start collecting data",  0.0f, FLT_MAX, 0.0f );
        initConfigTypeMap( "End_Year",                 &m_EndYear,                "Year to stop collecting data",   0.0f, FLT_MAX, FLT_MAX );
        initConfigTypeMap( "Reporting_Interval_Years", &m_ReportingIntervalYears, "Years between census",           0.0f, 100.0f, 1.0f );

        bool ret = JsonConfigurable::Configure( inputJson );
        if( ret )
        {
            if( m_StartYear >= m_EndYear )
            {
                throw IncoherentConfigurationException( __FILE__, __LINE__, __FUNCTION__, "Start_Year", m_StartYear, "End_Year", m_EndYear, "Start_Year must be < End_Year" );
            }
            if( m_ReportingIntervalYears == 0.0 )
            {
                throw InvalidInputDataException( __FILE__, __LINE__, __FUNCTION__, "Reporting_Interval_Years must be > 0." );
            }
        }

        // Add events that you want to collect incidence data on
        //eventTriggerList.push_back( IndividualEventTriggerType::pairs::lookup_key( IndividualEventTriggerType::Emigrating      ) );
        
        return ret;
    }

    void ReportRelationshipCensus::UpdateEventRegistration( float currentTime, 
                                                            float dt, 
                                                            std::vector<INodeEventContext*>& rNodeEventContextList,
                                                            ISimulationEventContext* pSimEventContext )
    {
        release_assert( rNodeEventContextList.size() > 0 );
        float year = rNodeEventContextList[ 0 ]->GetTime().Year();
        m_IsCollectingData = false;
        if( (year < m_StartYear) || ((m_EndYear+dt) < year) )
        {
            return;
        }
        m_IntervalTimerDays -= dt;
        if( m_IntervalTimerDays <= 0.0 )
        {
            if( m_FirstDataCollection )
            {
                m_IntervalTimerDays = 0.0;
                m_FirstDataCollection = false;
            }
            m_IntervalTimerDays += m_ReportingIntervalYears*DAYSPERYEAR;
            m_IsCollectingData = true;
        }

        BaseTextReportEvents::UpdateEventRegistration( currentTime, dt, rNodeEventContextList, pSimEventContext );
    }

    std::string ReportRelationshipCensus::GetReportName() const
    {
        return m_ReportName;
    }

    std::string ReportRelationshipCensus::GetHeader() const
    {
        std::stringstream header ;
        header      << "Year"
            << ", " << "NodeID"
            << ", " << "IndividualID"
            << ", " << "Gender"
            << ", " << "AgeYears"
            << ", " << "IndividualProperties"
            ;
        // last 6 months by type
        for( int i = 0; i < RelationshipType::COUNT; ++i )
        {
            header << ',' << "NumRels_Last6Months_" << RelationshipType::pairs::get_keys()[ i ];
        }

        // active by type
        for( int i = 0; i < RelationshipType::COUNT; ++i )
        {
            header << ',' << "NumRels_Active_" << RelationshipType::pairs::get_keys()[ i ];
        }

        // last 12 months by type
        for( int i = 0; i < RelationshipType::COUNT; ++i )
        {
            header << ',' << "NumRels_Last12Months_" << RelationshipType::pairs::get_keys()[ i ];
        }

        for( int itp = 0; itp < UNIQUE_PARTNER_TIME_PERIODS.size(); ++itp )
        {
            for( int irel = 0; irel < RelationshipType::COUNT; ++irel )
            {
                int num_months = int( UNIQUE_PARTNER_TIME_PERIODS[itp] / IDEALDAYSPERMONTH );
                header << ',' << "NumUniquePartners_Last-" << num_months << "-Months_" << RelationshipType::pairs::get_keys()[ irel ];
            }
        }

        return header.str();
    }

    bool ReportRelationshipCensus::notifyOnEvent( IIndividualHumanEventContext *context, 
                                                  const EventTrigger::Enum &trigger )
    {
        return true;
    }

    void ReportRelationshipCensus::LogIndividualData( IIndividualHuman* individual ) 
    {
        if( !m_IsCollectingData ) return;

        IIndividualHumanSTI* p_sti = nullptr;
        if( s_OK != individual->QueryInterface( GET_IID( IIndividualHumanSTI ), (void**)&p_sti ) )
        {
            throw QueryInterfaceException( __FILE__, __LINE__, __FUNCTION__, "individual", "IIndividualHumanSTI", "IIndividualHuman" );
        }

        uint32_t relationship_count[ RelationshipType::COUNT ];
        memset( relationship_count, 0, sizeof( uint32_t )*RelationshipType::COUNT );

        for( auto relationship : p_sti->GetRelationships() )
        {
            relationship_count[ int( relationship->GetType() ) ]++;
        }

        // strip out Relationship properties
        tProperties props = const_cast<IIndividualHuman*>(individual)->GetProperties()->GetOldVersion();
        std::string prop_str = "" ;
        for( auto entry : props )
        {
            std::string key = entry.first;
            std::string value = entry.second;
            if( key.find( std::string( "Relationship" ) ) == string::npos )
            {
                prop_str += key+":"+value+";";
            }
        }
        prop_str = prop_str.substr( 0, prop_str.size()-1 );

        GetOutputStream()
                   << individual->GetParent()->GetTime().Year()
            << "," << individual->GetParent()->GetExternalID()
            << "," << individual->GetSuid().data
            << "," << ((individual->GetGender() == Gender::MALE) ? "M" : "F")
            << "," << (individual->GetAge() / DAYSPERYEAR)
            << "," << prop_str;

        // last 6 months by type
        for( int i = 0; i < RelationshipType::COUNT; ++i )
        {
            GetOutputStream() << ',' << p_sti->GetLast6MonthRels( RelationshipType::Enum(i) );
        }

        // active relationships per type
        for( int i = 0; i < RelationshipType::COUNT; ++i )
        {
            GetOutputStream() << ',' << relationship_count[ i ];
        }

        // last 12 months by type
        for( int i = 0; i < RelationshipType::COUNT; ++i )
        {
            GetOutputStream() << ',' << p_sti->GetLast12MonthRels( RelationshipType::Enum( i ) );
        }

        for( int itp = 0; itp < UNIQUE_PARTNER_TIME_PERIODS.size(); ++itp )
        {
            for( int irel = 0; irel < RelationshipType::COUNT; ++irel )
            {
                GetOutputStream() << ',' << p_sti->GetNumUniquePartners( itp, irel );
            }
        }

        GetOutputStream() << endl;
    }

    bool ReportRelationshipCensus::IsCollectingIndividualData( float currentTime, float dt ) const
    {
        return true;
    }

    void ReportRelationshipCensus::EndTimestep( float currentTime, float dt )
    {
        BaseTextReportEvents::EndTimestep( currentTime, dt );
    }

    void ReportRelationshipCensus::Reduce()
    {
        BaseTextReportEvents::Reduce();
    }
}
