/***************************************************************************************************

Copyright (c) 2018 Intellectual Ventures Property Holdings, LLC (IVPH) All rights reserved.

EMOD is licensed under the Creative Commons Attribution-Noncommercial-ShareAlike 4.0 License.
To view a copy of this license, visit https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode

***************************************************************************************************/

#include "stdafx.h"
#include "Simulation.h"

#include <iomanip>      // std::setprecision

#include "FileSystem.h"
#include "Debug.h"
#include "Report.h"
#include "BinnedReport.h"
#include "DemographicsReport.h"
#include "SpatialReport.h"
#include "PropertyReport.h"
#include "Exceptions.h"
#include "Instrumentation.h"
#include "Memory.h"
#include "IMigrationInfo.h"
#include "Node.h"
#include "NodeDemographics.h"
#include "RANDOM.h"
#include "SimulationConfig.h"
#include "SimulationEventContext.h"
#include "NodeInfo.h"
#include "ReportEventRecorder.h"
#include "ReportEventRecorderNode.h"
#include "ReportEventRecorderCoordinator.h"
#include "ReportSurveillanceEventRecorder.h"
#include "Individual.h"
#include "LoadBalanceScheme.h"
#include "EventTrigger.h"
#include "RandomNumberGeneratorFactory.h"

#include "DllLoader.h"

#include "JsonRawWriter.h"
#include "JsonRawReader.h"
#include "MpiDataExchanger.h"
#include "IdmMpi.h"
#include "Properties.h"
#include "NodeProperties.h"
#include "PythonSupport.h"

#ifdef _DEBUG
#include "BinaryArchiveReader.h"
#include "BinaryArchiveWriter.h"
#endif

#include <chrono>
typedef std::chrono::high_resolution_clock _clock;

using namespace std;

SETUP_LOGGING( "Simulation" )


#define RUN_ALL_CUSTOM_REPORTS "RunAllCustomReports"
#define NO_CUSTOM_REPORTS "NoCustomReports"

namespace Kernel
{
    // Enable querying of interfaces from Simulation objects
    GET_SCHEMA_STATIC_WRAPPER_IMPL(Simulation,Simulation)
    BEGIN_QUERY_INTERFACE_BODY(Simulation)
        HANDLE_INTERFACE(IGlobalContext)
        HANDLE_INTERFACE(ISimulation)
        HANDLE_INTERFACE(ISimulationContext)
        HANDLE_ISUPPORTS_VIA(ISimulationContext)
    END_QUERY_INTERFACE_BODY(Simulation)

    float Simulation::base_year = 0.0f;

    //------------------------------------------------------------------
    //   Initialization methods
    //------------------------------------------------------------------

    Simulation::Simulation()
        : serializationMask(SerializationFlags(uint32_t(SerializationFlags::Population) | uint32_t(SerializationFlags::Parameters)))
        , nodes()
        , nodeRankMap()
        , node_events_added()
        , node_events_to_be_processed()
        , node_event_context_list()
        , nodeid_suid_map()
        , migratingIndividualQueues()
        , m_simConfigObj((const SimulationConfig*)Environment::getSimulationConfig())
        , m_interventionFactoryObj(nullptr)
        , demographicsContext(nullptr)
        , infectionSuidGenerator(EnvPtr->MPI.Rank, EnvPtr->MPI.NumTasks)
        , nodeSuidGenerator(EnvPtr->MPI.Rank, EnvPtr->MPI.NumTasks)
        , campaignFilename()
        , loadBalanceFilename()
        , rng(nullptr)
        , reports()
        , individual_data_reports()
        , reportClassCreator(nullptr)
        , binnedReportClassCreator(nullptr)
        , spatialReportClassCreator(nullptr)
        , propertiesReportClassCreator(nullptr)
        , demographicsReportClassCreator(nullptr)
        , eventReportClassCreator( nullptr )
        , nodeEventReportClassCreator( nullptr )
        , coordinatorEventReportClassCreator( nullptr )
        , surveillanceEventReportClassCreator( nullptr )
        , event_coordinators()
        , campaign_events()
        , event_context_host(nullptr)
        , currentTime()
        , sim_type(SimType::GENERIC_SIM)
        , demographic_tracking(false)
        , enable_spatial_output(false)
        , enable_property_output(false)
        , enable_default_report(false)
        , enable_event_report( false )
        , enable_node_event_report( false )
        , enable_coordinator_event_report( false )
        , enable_surveillance_event_report( false )
        , enable_termination_on_zero_total_infectivity(false)
        , campaign_filename()
        , custom_reports_filename( RUN_ALL_CUSTOM_REPORTS )
        , loadbalance_filename()
        , can_support_family_trips( false )
        , m_IPWhiteListEnabled(true)
        , demographics_factory(nullptr)
        , m_pRngFactory( new RandomNumberGeneratorFactory() )
        , min_sim_endtime(0.0f)
        , new_node_observers()
    {
        LOG_DEBUG( "CTOR\n" );

        reportClassCreator                  = Report::CreateReport;
        binnedReportClassCreator            = BinnedReport::CreateReport;
        spatialReportClassCreator           = SpatialReport::CreateReport;
        propertiesReportClassCreator        = PropertyReport::CreateReport;
        demographicsReportClassCreator      = DemographicsReport::CreateReport;
        eventReportClassCreator             = ReportEventRecorder::CreateReport;
        nodeEventReportClassCreator         = ReportEventRecorderNode::CreateReport;
        coordinatorEventReportClassCreator  = ReportEventRecorderCoordinator::CreateReport;
        surveillanceEventReportClassCreator = ReportSurveillanceEventRecorder::CreateReport;

        nodeRankMap.SetNodeInfoFactory( this );

        if( (EnvPtr != nullptr) &&
            (EnvPtr->Config != nullptr) &&
            EnvPtr->Config->Exist( "Disable_IP_Whitelist" ) &&
            (*(EnvPtr->Config))[ "Disable_IP_Whitelist" ].As<json::Number>() == 1 )
        {
            m_IPWhiteListEnabled = false;
        }

        // Initialize node demographics from file
        if( !JsonConfigurable::_dryrun )
        {
            release_assert( m_simConfigObj );

            demographics_factory = NodeDemographicsFactory::CreateNodeDemographicsFactory( &nodeid_suid_map,
                EnvPtr->Config,
                m_simConfigObj->demographics_initial,
                m_simConfigObj->default_torus_size,
                m_simConfigObj->default_node_population
            );
            if( demographics_factory == nullptr )
            {
                throw InitializationException( __FILE__, __LINE__, __FUNCTION__, "Failed to create NodeDemographicsFactory" );
            }
            demographicsContext = demographics_factory->CreateDemographicsContext();

            ExternalNodeId_t first_node_id = demographics_factory->GetNodeIDs()[ 0 ];
            JsonObjectDemog json_for_first_node = demographics_factory->GetJsonForNode( first_node_id );
            IPFactory::GetInstance()->Initialize( first_node_id, json_for_first_node, m_IPWhiteListEnabled );

            NPFactory::GetInstance()->Initialize( demographics_factory->GetNodePropertiesJson(), m_IPWhiteListEnabled );
        }
    }

    Simulation::~Simulation()
    {
/* maybe #ifdef _DEBUG?
        LOG_DEBUG( "DTOR\n" );
        for (auto& entry : nodes)
        {
            delete entry.second;
        }
        nodes.clear();

        delete demographics_factory;
        demographics_factory = nullptr;

        if (rng) delete rng;

        delete event_context_host;
        event_context_host = nullptr;

        for (auto report : reports)
        {
            LOG_DEBUG_F( "About to delete report = %s\n", report->GetReportName().c_str() );
            delete report;
        }
        reports.clear();
*/        
    }

    bool Simulation::Configure( const Configuration * inputJson )
    {
        LOG_DEBUG("Configure\n");

        // Temporary configuration values
        float sim_delta_time = 0.0f;

        initConfig( "Simulation_Type", sim_type, inputJson, MetadataDescriptor::Enum("sim_type", Simulation_Type_DESC_TEXT, MDD_ENUM_ARGS(SimType)) ); // simulation only (???move)

        initConfigTypeMap( "Simulation_Timestep", &sim_delta_time,  Simulation_Timestep_DESC_TEXT, 0.0f, FLT_MAX, 1.0f );

        initConfigTypeMap( "Enable_Termination_On_Zero_Total_Infectivity",  &enable_termination_on_zero_total_infectivity,  Enable_Termination_On_Zero_Total_Infectivity_DESC_TEXT,  false );
        initConfigTypeMap( "Enable_Default_Reporting",       &enable_default_report,          Enable_Default_Reporting_DESC_TEXT,       true  );
        initConfigTypeMap( "Enable_Demographics_Reporting",  &demographic_tracking,           Enable_Demographics_Reporting_DESC_TEXT,  true  );
        initConfigTypeMap( "Enable_Property_Output",         &enable_property_output,         Enable_Property_Output_DESC_TEXT,         false );
        initConfigTypeMap( "Enable_Spatial_Output",          &enable_spatial_output,          Enable_Spatial_Output_DESC_TEXT,          false );
        initConfigTypeMap( "Report_Event_Recorder",          &enable_event_report,            Report_Event_Recorder_DESC_TEXT,          false );

        initConfigTypeMap( ReportEventRecorder::GetEnableParameterName().c_str(),             &enable_event_report,              Report_Event_Recorder_DESC_TEXT,             false );
        initConfigTypeMap( ReportEventRecorderNode::GetEnableParameterName().c_str(),         &enable_node_event_report,         Report_Node_Event_Recorder_DESC_TEXT,        false );
        initConfigTypeMap( ReportEventRecorderCoordinator::GetEnableParameterName().c_str(),  &enable_coordinator_event_report,  Report_Coordinator_Event_Recorder_DESC_TEXT, false );
        initConfigTypeMap( ReportSurveillanceEventRecorder::GetEnableParameterName().c_str(), &enable_surveillance_event_report, Report_Coordinator_Event_Recorder_DESC_TEXT, false );        
        
        initConfigTypeMap( "Campaign_Filename",       &campaign_filename,      Campaign_Filename_DESC_TEXT, "", "Enable_Interventions" );
        initConfigTypeMap( "Load_Balance_Filename",   &loadbalance_filename,   Load_Balance_Filename_DESC_TEXT ); 
        initConfigTypeMap( "Minimum_End_Time",        &min_sim_endtime,        Minimum_End_Time_DESC_TEXT, 0.0f, 1000000.0f, 0.0f, "Enable_Termination_On_Zero_Total_Infectivity" );
        std::vector< int > py_inproc_tsteps_tmp_vec;
        initConfigTypeMap( "Python_Inprocessing_Tsteps",        &py_inproc_tsteps_tmp_vec,        "Array of timesteps when you want to call out to python" );

        if( JsonConfigurable::_dryrun || EnvPtr->Config->Exist( "Custom_Reports_Filename" ) )
        {
            initConfigTypeMap( "Custom_Reports_Filename", &custom_reports_filename, Custom_Reports_Filename_DESC_TEXT, RUN_ALL_CUSTOM_REPORTS );
        }

        bool create_rng_from_serialized_data = false;
        if( JsonConfigurable::_dryrun || EnvPtr->Config->Exist( "Enable_Random_Generator_From_Serialized_Population" ) )
        {
            initConfigTypeMap( "Enable_Random_Generator_From_Serialized_Population", &create_rng_from_serialized_data, Enable_Random_Generator_From_Serialized_Population_DESC_TEXT, false );
        }

        bool ret = JsonConfigurable::Configure( inputJson );

        if( ret || JsonConfigurable::_dryrun )
        {
            if( create_rng_from_serialized_data && !inputJson->Exist( "Serialized_Population_Filenames" ) )
            {
                throw IncoherentConfigurationException( __FILE__, __LINE__, __FUNCTION__,
                                                        "Serialized_Population_Filenames", "<not exist>",
                                                        "Enable_Random_Generator_From_Serialized_Population", "1",
                                                        "'Enable_Random_Generator_From_Serialized_Population' can only be enabled if using a serialized population.");
            }
            m_pRngFactory->CreateFromSerializeData( create_rng_from_serialized_data );
            if( JsonConfigurable::_dryrun || !create_rng_from_serialized_data )
            {
                m_pRngFactory->Configure( inputJson );
            }
        }

        if( !JsonConfigurable::_dryrun && 
            ((GET_CONFIGURABLE(SimulationConfig)->starttime + GET_CONFIGURABLE(SimulationConfig)->Sim_Duration) < min_sim_endtime) )
        {
            throw IncoherentConfigurationException(__FILE__, __LINE__, __FUNCTION__, "Minimum_End_Time", min_sim_endtime, "maximum simulation time step",
                                                    GET_CONFIGURABLE(SimulationConfig)->starttime + GET_CONFIGURABLE(SimulationConfig)->Sim_Duration,
                                                   "Start_Time + Simulation_Duration must be greater than Minimum_End_Time." );
        }

        // Set configuration from temporary values
        currentTime.SetTimeDelta(sim_delta_time);

        // convert vector into queue
        for( auto elem : py_inproc_tsteps_tmp_vec )
        {
            py_inproc_tsteps.push( elem );
        }

        return ret;
    }

    // Check simulation abort conditions
    bool Simulation::TimeToStop()
    {
        bool abortSim = false;

        if(enable_termination_on_zero_total_infectivity && currentTime.time > min_sim_endtime)
        {
            // Accumulate node infectivity
            float totInfVal         = 0.0f;
            float totInfVal_unified = 0.0f;

            for (auto iterator = nodes.rbegin(); iterator != nodes.rend(); ++iterator)
            {
                INodeContext* n = iterator->second;
                totInfVal += n->GetInfectivity();
            }

            // Accumulate infectivity from all cores
            EnvPtr->MPI.p_idm_mpi->Allreduce_SUM(&totInfVal, &totInfVal_unified, 1);

            // Abort on zero infectivity
            if(totInfVal_unified == 0.0f)
            {
                LOG_INFO("Zero infectivity at current time-step; simulation aborting.\n");
                abortSim = true;
            }
        }

        return abortSim;
    }

    Simulation *Simulation::CreateSimulation()
    {
        Simulation *newsimulation = _new_ Simulation();
        newsimulation->Initialize();

        return newsimulation;
    }

    Simulation *Simulation::CreateSimulation(const ::Configuration *config)
    {
        Simulation *newsimulation = _new_ Simulation();

        if (newsimulation)
        {
            // This sequence is important: first
            // Creation-->Initialization-->Validation
            newsimulation->Initialize(config);
            if(!ValidateConfiguration(config))
            {
                delete newsimulation;
                throw GeneralConfigurationException( __FILE__, __LINE__, __FUNCTION__, "GENERIC_SIM requested with invalid configuration." );
            }
        }

        return newsimulation;
    }

    bool Simulation::ValidateConfiguration(const ::Configuration *config)
    {
        // NOTE: ClimateFactory::climate_structure, configured from the parameter "Climate_Model",
        //       is only available after the factory is created in Simulation::populateFromDemographics
        //       SimulationConfig and the various static individual/infection/susceptibility parameters
        //       should be initialized by this point (see Simulation::Initialize).
        //       However, this function still has access to the complete set of configurable parameters
        //       from the  json::Configuration argument that is passed in by the SimulationFactory.

        return true;
    }

    void Simulation::Initialize()
    {
        LOG_DEBUG( "Initialize\n" );
    }

    void Simulation::Initialize(const ::Configuration *config)
    {
        Configure( config );
        IndividualHuman::InitializeStatics( config );
        MemoryGauge mg;
        mg.Configure( config );

        m_interventionFactoryObj = InterventionFactory::getInstance();

        setupMigrationQueues();
        setupEventContextHost();
        setupRng();
        setParams(config);
        initSimulationState();
        Reports_CreateBuiltIn();
        Reports_ConfigureBuiltIn();
        Reports_CreateCustom();
        Reports_FindReportsCollectingIndividualData();
    }

    INodeInfo* Simulation::CreateNodeInfo()
    {
        INodeInfo* pin = new NodeInfo();
        return pin ;
    }

    INodeInfo* Simulation::CreateNodeInfo( int rank, INodeContext* pNC )
    {
        INodeInfo* pin = new NodeInfo( rank, pNC );
        return pin ;
    }

    void Simulation::setupMigrationQueues()
    {
        migratingIndividualQueues.resize(EnvPtr->MPI.NumTasks); // elements are instances not pointers
    }

    void Simulation::setupEventContextHost()
    {
        event_context_host = _new_ SimulationEventContextHost(this);
    }

    void Simulation::setupRng()
    {
        RANDOMBASE* tmp_rng = m_pRngFactory->CreateRng();
        // don't change pointer in case created from serialization
        if( tmp_rng != nullptr )
        {
            rng = tmp_rng;
        }
    }

    void Simulation::setParams(const ::Configuration *config)
    {
        // Seems to me that if enable_interventions is 0, we shouldn't have this exception here.
        if( m_simConfigObj->interventions )
        {
            if( campaign_filename.empty() )
            {
                throw InvalidInputDataException( __FILE__, __LINE__, __FUNCTION__, "'Campaign_Filename' is empty.  You must have a file." );
            }
            campaignFilename = campaign_filename;
        }

        loadBalanceFilename  = Environment::FindFileOnPath( loadbalance_filename  );
        currentTime.time     = m_simConfigObj->starttime;
    }

    void Simulation::initSimulationState()
    {
        // right now this function absorbs whatever was left from the original constructor semantics
        // the intent is that this sets any member variables not set up already that are part of the 
        // initial simulation state

        currentTime.timestep = 0;
    }

    //------------------------------------------------------------------
    //   Reporting methods
    //------------------------------------------------------------------

    void Simulation::Reports_CreateBuiltIn()
    {
        // Check reporter_plugins directory and use report classes from there if any exists

        LOG_DEBUG( "Reports_CreateBuiltIn()\n" );

        if( enable_default_report )
        {
            // Default report
            IReport * report = (*reportClassCreator)();
            release_assert(report);
            reports.push_back(report);
        }

        if(enable_property_output)
        {
            IReport * prop_report = (*propertiesReportClassCreator)();
            release_assert(prop_report);
            reports.push_back(prop_report);
        }

        if(enable_spatial_output)
        {
            IReport * spatial_report = (*spatialReportClassCreator)();
            release_assert(spatial_report);
            reports.push_back(spatial_report);
        }

        if( enable_event_report )
        {
            IReport * event_report = (*eventReportClassCreator)();
            release_assert(event_report);
            reports.push_back(event_report);
        }

        if( enable_node_event_report )
        {
            IReport * node_event_report = (*nodeEventReportClassCreator)();
            release_assert( node_event_report );
            reports.push_back( node_event_report );
        }

        if( enable_coordinator_event_report )
        {
            IReport * coordinator_event_report = (*coordinatorEventReportClassCreator)();
            release_assert( coordinator_event_report );
            reports.push_back( coordinator_event_report );
        }

        if( enable_surveillance_event_report )
        {
            IReport * surveillance_event_report = (*surveillanceEventReportClassCreator)();
            release_assert( surveillance_event_report );
            reports.push_back( surveillance_event_report );
        }

        if(demographic_tracking)
        {
            IReport * binned_report = (*binnedReportClassCreator)();
            release_assert(binned_report);
            reports.push_back(binned_report);

            IReport* demo_report = (*demographicsReportClassCreator)();
            release_assert(demo_report);
            reports.push_back(demo_report);
        }
    }

    void Simulation::RegisterNewNodeObserver(void* id, Kernel::ISimulation::callback_t observer)
    {
        new_node_observers[id] = observer;
    }

    void Simulation::UnregisterNewNodeObserver(void* id)
    {
        if (new_node_observers.erase(id) == 0)
        {
            LOG_WARN_F("%s: Didn't find entry for id %08X in observer map.", __FUNCTION__, id);
        }
    }

    void Simulation::DistributeEventToOtherNodes( const EventTrigger::Enum& rEventTrigger, INodeQualifier* pQualifier )
    {
        release_assert( pQualifier );

        for( auto entry : nodeRankMap.GetRankMap() )
        {
            INodeInfo* pni = entry.second;
            if( pQualifier->Qualifies( *pni ) )
            {
                // -------------------------------------------------------------------------------------
                // --- One could use a map to keep a node from getting more than one event per timestep
                // --- but I think the logic for that belongs in the object processing the event
                // -------------------------------------------------------------------------------------
                node_events_added[ nodeRankMap.GetRankFromNodeSuid( pni->GetSuid() ) ].Add( pni->GetSuid(), rEventTrigger ) ;
            }
        }
    }


    void Simulation::UpdateNodeEvents()
    {
        WithSelfFunc to_self_func = [this](int myRank) 
        { 
            //do nothing
        }; 

        // -------------------------------------------------------------
        // --- Send Event Triggers destined for nodes not on this processor
        // -------------------------------------------------------------
        SendToOthersFunc to_others_func = [this](IArchive* writer, int toRank)
        {
            *writer & node_events_added[toRank];
        };

        // -----------------------------------------------------------------
        // --- Receive the Event Triggers destined for the nodes on this processor
        // -----------------------------------------------------------------
        ReceiveFromOthersFunc from_others_func = [this](IArchive* reader, int fromRank)
        {
            EventsForOtherNodes efon ;
            *reader & efon;
            node_events_added[ EnvPtr->MPI.Rank ].Update( efon );
        };

        ClearDataFunc clear_data_func = [this](int rank)
        {
        };

        MpiDataExchanger exchanger( "EventsForOtherNodes", to_self_func, to_others_func, from_others_func, clear_data_func );
        exchanger.ExchangeData( this->currentTime );

        // ---------------------------------------------------
        // --- Update the events to be processed during the 
        // --- next time step for the nodes on this processor
        // ---------------------------------------------------
        node_events_to_be_processed.clear();
        for( auto entry : node_events_added[ EnvPtr->MPI.Rank ].GetMap() )
        {
            suids::suid node_id = entry.first;
            auto& trigger_list = entry.second;
            for( auto trigger : trigger_list )
            {
                node_events_to_be_processed[ node_id ].push_back( trigger );
            }
        }
        node_events_added.clear();

    }

    ISimulationEventContext* Simulation::GetSimulationEventContext()
    {
        return event_context_host;
    }

    void Simulation::Reports_ConfigureBuiltIn()
    {
        for( auto report : reports )
        {
            report->Configure( EnvPtr->Config );
        }
    }

    void Simulation::Reports_CreateCustom()
    {
        // -------------------------------------------------------------
        // --- Allow the user to indicate that they do not want to use
        // --- any custom reports even if DLL's are present.
        // -------------------------------------------------------------
        if( custom_reports_filename.empty() || (custom_reports_filename == NO_CUSTOM_REPORTS) )
        {
            return ;
        }

        ReportInstantiatorMap report_instantiator_map ;
        SimType::Enum st_enum = m_simConfigObj->sim_type;
        DllLoader dllLoader(SimType::pairs::lookup_key(st_enum));
        if( !dllLoader.LoadReportDlls( report_instantiator_map ) )
        {
            LOG_WARN_F("Failed to load reporter emodules for SimType: %s from path: %s\n" , SimType::pairs::lookup_key(st_enum), dllLoader.GetEModulePath(REPORTER_EMODULES).c_str());
        }
        Reports_Instantiate( report_instantiator_map );
    }

    void Simulation::Reports_FindReportsCollectingIndividualData()
    {
        // ---------------------------------------------------------------------
        // --- Get the subset of reports that are collecting individual data
        // --- This allows us to avoid calling LogIndividualData() for reports
        // --- that just have no-ops on every individual.
        // ---------------------------------------------------------------------
        individual_data_reports.clear();
        for( auto report : reports )
        {
            if( report->IsCollectingIndividualData( currentTime.time, currentTime.GetTimeDelta() ) )
            {
                individual_data_reports.push_back( report );
            }
        }
    }

    Configuration* Simulation::Reports_GetCustomReportConfiguration()
    {
        Configuration* p_cr_config = nullptr ;

        // ------------------------------------------------------------------------------
        // --- If the user does not define the custom_reports_filename input parameter,
        // --- then they want to run all reports.  Returning null will do this.
        // ------------------------------------------------------------------------------
        if( !custom_reports_filename.empty() && (custom_reports_filename != RUN_ALL_CUSTOM_REPORTS) )
        {
            LOG_INFO_F("Looking for custom reports file = %s\n", custom_reports_filename.c_str());
            if( FileSystem::FileExists( custom_reports_filename ) )
            {
                LOG_INFO_F("Found custom reports file = %s\n", custom_reports_filename.c_str());
                // it is extremely unlikely that this will return null.  It will throw an exception if an error occurs.
                Configuration* p_config = Configuration::Load( custom_reports_filename );
                if( !p_config ) 
                {
                    throw Kernel::InitializationException( __FILE__, __LINE__, __FUNCTION__, custom_reports_filename.c_str() );
                }
                p_cr_config = Configuration::CopyFromElement( (*p_config)["Custom_Reports"], p_config->GetDataLocation() );
                delete p_config ;
            }
            else
            {
                throw Kernel::FileNotFoundException(__FILE__, __LINE__, __FUNCTION__, custom_reports_filename.c_str());
            }
        }

        return p_cr_config ;
    }

    void Simulation::Reports_Instantiate( ReportInstantiatorMap& rReportInstantiatorMap )
    {
        auto cachedValue = JsonConfigurable::_useDefaults;
        JsonConfigurable::_useDefaults = true;
        Configuration* p_cr_config = Reports_GetCustomReportConfiguration();

        bool load_all_reports = (p_cr_config == nullptr) ||
                                !p_cr_config->Exist( "Use_Explicit_Dlls" ) ||
                                (int(p_cr_config->operator[]( "Use_Explicit_Dlls" ).As<json::Number>()) != 1) ;

        LOG_INFO_F("Found %d Custom Report DLL's to consider loading, load_all_reports=%d\n", rReportInstantiatorMap.size(), load_all_reports );

        // Verify that a DLL exists for each report defined in the custom reports file
        if( p_cr_config )
        {
            auto custom_reports_config = p_cr_config->As<json::Object>();
            for( auto it = custom_reports_config.Begin(); it != custom_reports_config.End(); ++it )
            {
                std::string reportname(it->name);
                if( reportname != "Use_Explicit_Dlls" &&  rReportInstantiatorMap.find( reportname ) == rReportInstantiatorMap.end() )
                {
                    //check if report is enabled
                    json::QuickInterpreter dll_data = p_cr_config->operator[]( reportname ).As<json::Object>();
                    if( int( dll_data["Enabled"].As<json::Number>() ) != 0 )
                    {
                        //Dll not found
                        std::stringstream ss;
                        ss << reportname << " (dll)";
                        throw Kernel::FileNotFoundException( __FILE__, __LINE__, __FUNCTION__, ss.str().c_str() );
                    }                    
                }
            }
        }

        for( auto ri_entry : rReportInstantiatorMap )
        {
            std::string class_name = ri_entry.first ;
            try
            {
                if( (p_cr_config != nullptr) && p_cr_config->Exist( class_name ) )
                {
                    LOG_INFO_F("Found custom report data for %s\n", class_name.c_str());
                    json::QuickInterpreter dll_data = p_cr_config->operator[]( class_name ).As<json::Object>() ;
                    if( int(dll_data["Enabled"].As<json::Number>()) != 0 )
                    {
                        json::Array report_data = dll_data["Reports"].As<json::Array>() ;
                        for( int i = 0 ; i < report_data.Size() ; i++ )
                        {
                            LOG_INFO_F( "Created instance #%d of %s\n", (i+1),class_name.c_str() );
                            Configuration* p_cfg = Configuration::CopyFromElement( report_data[i], p_cr_config->GetDataLocation() );

                            IReport* p_cr = ri_entry.second(); // creates report object
                            p_cr->Configure( p_cfg );
                            reports.push_back( p_cr );
                            delete p_cfg ;
                            p_cfg = nullptr;
                        }
                    }
                }
                else if( load_all_reports )
                {
                    LOG_WARN_F("Did not find report configuration for report DLL %s.  Creating report with defaults.\n", class_name.c_str());

                    json::Object empty_json_obj ;
                    Configuration* p_cfg = Configuration::CopyFromElement( empty_json_obj, "no file" );

                    IReport* p_cr = ri_entry.second();  // creates report object
                    p_cr->Configure( p_cfg );
                    reports.push_back( p_cr );
                    delete p_cfg ;
                    p_cfg = nullptr;
                }
            }
            catch( json::Exception& e )
            {
                std::stringstream ss ;
                ss << "Error occured reading report data for " << class_name << ".  Error: " << e.what() << std::endl ;
                throw InitializationException( __FILE__, __LINE__, __FUNCTION__, ss.str().c_str() );
            }
        }
        delete p_cr_config;
        p_cr_config = nullptr;
        JsonConfigurable::_useDefaults = cachedValue;
    }

    void Simulation::Reports_UpdateEventRegistration()
    {
        for (auto report : reports)
        {
            report->UpdateEventRegistration( currentTime.time, currentTime.GetTimeDelta(), node_event_context_list, event_context_host );
        }
    }

    void Simulation::Reports_BeginTimestep()
    {
        for (auto report : reports)
        {
            release_assert(report);
            report->BeginTimestep();
        }
    }

    void Simulation::Reports_EndTimestep()
    {
        for (auto report : reports)
        {
            release_assert(report);
            report->EndTimestep( currentTime.time, currentTime.GetTimeDelta() );
        }
    }

    void Simulation::Reports_LogNodeData( INodeContext* n )
    {
        for (auto report : reports)
        {
            report->LogNodeData( n );
        }
    }

    void Simulation::PrintTimeAndPopulation()
    {
        // print out infections and population out
        int stat_pop = 0, infected = 0;
        for (auto& entry : nodes)
        {
            INodeContext* n = entry.second;
            stat_pop += n->GetStatPop();
            infected += n->GetInfected();
        }

        std::ostringstream oss;
        oss << std::fixed << std::setprecision(1) << "Update(): Time: " << float(currentTime.time);
        if( currentTime.getBaseYear() > 0 )
        {
            oss << std::fixed << " Year: " << currentTime.Year();
        }
        oss << std::fixed << " Rank: " << EnvPtr->MPI.Rank << " StatPop: " << stat_pop << " Infected: " << infected << std::endl;
        LOG_INFO( oss.str().c_str() );
    }

    void Simulation::WriteReportsData()
    {
        //std::cout << "There are " << reports.size() << " reports to finalize." << std::endl;
        for (auto report : reports)
        {
            report->Reduce();

            // the rest only make sense on rank 0
            if (EnvPtr->MPI.Rank == 0)
            {
                LOG_INFO_F( "Finalizing '%s' reporter.\n", report->GetReportName().c_str() );
                report->Finalize();
                LOG_INFO_F( "Finalized  '%s' reporter.\n", report->GetReportName().c_str() );
            }
        }
    }

    //------------------------------------------------------------------
    //   Every timestep Update() method
    //------------------------------------------------------------------

    void Simulation::Update()
    {
        Reports_UpdateEventRegistration();
        Reports_FindReportsCollectingIndividualData();

        // -----------------
        // --- Update Events
        // -----------------
        release_assert(event_context_host);
        event_context_host->Update(currentTime.GetTimeDelta());

        Reports_BeginTimestep();

        // -----------------
        // --- Update Nodes
        // -----------------
        for (auto iterator = nodes.rbegin(); iterator != nodes.rend(); ++iterator)
        {
            INodeContext* n = iterator->second;
            release_assert(n);
            n->AddEventsFromOtherNodes(node_events_to_be_processed[n->GetSuid()]);
            n->Update(currentTime.GetTimeDelta());

            Reports_LogNodeData(n);
        }

        // -----------------------
        // --- Resolve Migration
        // -----------------------
        REPORT_TIME( ENABLE_DEBUG_MPI_TIMING, "resolveMigration", resolveMigration() );

        for (auto iterator = nodes.rbegin(); iterator != nodes.rend(); ++iterator)
        {
            INodeContext* n = iterator->second;
            nodeRankMap.Update(n);
        }
        nodeRankMap.Sync( currentTime );

        UpdateNodeEvents();

        // -------------------
        // --- Increment Time
        // -------------------
        currentTime.Update();

        // ----------------------------------------------------------
        // --- Output Information for the end of the update/timestep
        // ----------------------------------------------------------
        PrintTimeAndPopulation();

        Reports_EndTimestep();

        // Call out to Embedded Python IN-processing script if any
        // Check if time now elapsed is greater than next timestep in 
        // Python_Inprocessing_Tsteps array.
        if( py_inproc_tsteps.size() > 0 && currentTime.timestep > py_inproc_tsteps.front() )
        {
            auto newCampaignFilename = Kernel::PythonSupport::RunPyFunction( std::to_string( currentTime.time ), Kernel::PythonSupport::SCRIPT_IN_PROCESS );
            py_inproc_tsteps.pop();
            const vector<ExternalNodeId_t>& nodeIDs = demographics_factory->GetNodeIDs();
            loadCampaignFromFile(newCampaignFilename, nodeIDs);
        }

        // Unconditionally checking against potential memory blowup with minimum cost
        MemoryGauge::CheckMemoryFailure( false );
    }

    //------------------------------------------------------------------
    //   First timestep Populate() methods
    //------------------------------------------------------------------

    bool Simulation::Populate()
    {
        LOG_DEBUG("Calling populateFromDemographics()\n");

        // Populate nodes
        LOG_INFO_F("Campaign file name identified as: %s\n", campaignFilename.c_str());
        int node_count = populateFromDemographics(campaignFilename.c_str(), loadBalanceFilename.c_str());
        LOG_INFO_F("populateFromDemographics() generated %d nodes.\n", node_count);

        LOG_INFO_F("Rank %d contributes %d nodes...\n", EnvPtr->MPI.Rank, nodeRankMap.Size());
        EnvPtr->Log->Flush();
        LOG_INFO_F("Merging node rank maps...\n");
        nodeRankMap.MergeMaps(); // merge rank maps across all processors
        LOG_INFO_F("Merged rank %d map now has %d nodes.\n", EnvPtr->MPI.Rank, nodeRankMap.Size());

//        if (nodeRankMap.Size() < 500)
//            LOG_INFO_F("Rank %d map contents:\n%s\n", EnvPtr->MPI.Rank, nodeRankMap.ToString().c_str());
//        else 
//            LOG_INFO("(Rank map contents not displayed due to large (> 500) number of entries.)\n");
        LOG_INFO("Rank map contents not displayed until NodeRankMap::ToString() (re)implemented.\n");

        // We'd like to be able to run even if a processor has no nodes, but there are other issues.
        // So for now just bail...
        if(node_count <= 0)
        {
            LOG_WARN_F("Rank %d wasn't assigned any nodes! (# of procs is too big for simulation?)\n", EnvPtr->MPI.Rank);
            return false;
        }

        for (auto report : reports)
        {
            LOG_DEBUG( "Initializing report...\n" );
            report->Initialize( nodeRankMap.Size());
            report->CheckForValidNodeIDs(demographics_factory->GetNodeIDs());
            LOG_INFO_F( "Initialized '%s' reporter\n", report->GetReportName().c_str() );
        }

        return true;
    }

    void Simulation::MergeNodeIdSuidBimaps(nodeid_suid_map_t& local_map, nodeid_suid_map_t& merged_map)
    {
        merged_map = local_map;

        if (EnvPtr->MPI.NumTasks > 1)
        {
            auto json_writer = new JsonRawWriter();
            IArchive& writer_archive = *static_cast<IArchive*>(json_writer);
            size_t count = local_map.size();
            writer_archive.startArray( count );
            LOG_VALID_F( "Serializing %d id-suid bimap entries.\n", count );
            for (auto& entry : local_map)
            {
                writer_archive.startObject();
                    writer_archive.labelElement( "id" ) & (uint32_t&)entry.left;
                    // entry.right is const which doesn't play well with IArchive operator '&'
                    suids::suid suid(entry.right);
                    writer_archive.labelElement( "suid" ) & suid;
                writer_archive.endObject();
            }
            writer_archive.endArray();

            for (int rank = 0; rank < EnvPtr->MPI.NumTasks; ++rank)
            {
                if (rank == EnvPtr->MPI.Rank)
                {
                    const char* buffer = writer_archive.GetBuffer();
                    size_t byte_count = writer_archive.GetBufferSize();
                    LOG_VALID_F( "Broadcasting serialized bimap (%d bytes)\n", byte_count );
                    EnvPtr->MPI.p_idm_mpi->PostChars( const_cast<char*>(buffer), byte_count, rank );
                }
                else
                {
                    std::vector<char> received;
                    EnvPtr->MPI.p_idm_mpi->GetChars( received, rank );
                    auto json_reader = new JsonRawReader( received.data() );
                    IArchive& reader_archive = *static_cast<IArchive*>(json_reader);
                    size_t entry_count;
                    reader_archive.startArray( entry_count );
                        LOG_VALID_F( "Merging %d id-suid bimap entries from rank %d\n", entry_count, rank );
                        for (size_t i = 0; i < entry_count; ++i)
                        {
                            uint32_t id;
                            suids::suid suid;
                            reader_archive.startObject();
                                reader_archive.labelElement( "id" ) & id;
                                reader_archive.labelElement( "suid" ) & suid;
                            reader_archive.endObject();
                            merged_map.insert(nodeid_suid_pair(id, suid));
                        }
                    reader_archive.endArray();
                    delete json_reader;
                    //delete [] buffer;
                }
            }

            delete json_writer;
        }
    }

    IMigrationInfoFactory* Simulation::CreateMigrationInfoFactory ( const std::string& idreference,
                                                                    MigrationStructure::Enum ms,
                                                                    int torusSize )
    {
        IMigrationInfoFactory* pmf = MigrationFactory::ConstructMigrationInfoFactory( EnvPtr->Config, 
                                                                                      idreference,
                                                                                      m_simConfigObj->sim_type,
                                                                                      ms,
                                                                                      !(m_simConfigObj->demographics_initial),
                                                                                      torusSize );
        return pmf ;
    }

    void Simulation::LoadInterventions(const char* campaignfilename, const std::vector<ExternalNodeId_t>& demographic_node_ids)
    {
        // Set up campaign interventions from file
        release_assert( event_context_host );
        event_context_host->campaign_filename = campaignfilename;
        release_assert( m_simConfigObj );
        if (m_simConfigObj->interventions)
        {
            LOG_INFO_F( "Looking for campaign file %s\n", campaignfilename );

            if ( !FileSystem::FileExists( campaignfilename ) )
            {
                throw FileNotFoundException( __FILE__, __LINE__, __FUNCTION__, campaignfilename );
            }
            else 
            {
                LOG_INFO("Found campaign file successfully.\n");
            }

#ifdef WIN32
            DllLoader dllLoader;
            if (!dllLoader.LoadInterventionDlls())
            {
                SimType::Enum st_enum = m_simConfigObj->sim_type;
                LOG_WARN_F("Failed to load intervention emodules for SimType: %s from path: %s\n", SimType::pairs::lookup_key(st_enum), dllLoader.GetEModulePath(INTERVENTION_EMODULES).c_str());
            }
#endif

            JsonConfigurable::_track_missing = false;

            loadCampaignFromFile( campaignfilename, demographic_node_ids);

            JsonConfigurable::_track_missing = true;

            // ------------------------------------------
            // --- Setup Individual Property Transitions
            // ------------------------------------------
            if( IPFactory::GetInstance()->HasIPs() )
            {
                std::string transitions_file_path = FileSystem::Concat( Environment::getInstance()->OutputPath, std::string( IPFactory::transitions_dot_json_filename ) );

                if (EnvPtr->MPI.Rank == 0)
                {
                    // Delete any existing transitions.json file
                    LOG_DEBUG_F( "Deleting any existing %s file.\n", transitions_file_path.c_str() );
                    FileSystem::RemoveFile( transitions_file_path );

                    // Write the new transitions.json file
                    IPFactory::GetInstance()->WriteTransitionsFile();
                }

                // Everyone waits until the Rank=0 process is done deleting and creating the transitions file
                EnvPtr->MPI.p_idm_mpi->Barrier();

                if ( !FileSystem::FileExists( transitions_file_path ) )
                {
                    throw FileNotFoundException( __FILE__, __LINE__, __FUNCTION__, transitions_file_path.c_str() );
                }

                // Load the Individual Property Transitions
                JsonConfigurable::_track_missing = false;

                loadCampaignFromFile( transitions_file_path.c_str(), demographic_node_ids);

                JsonConfigurable::_track_missing = true;
            }
        }
    }

    int Simulation::populateFromDemographics(const char* campaignfilename, const char* loadbalancefilename)
    {
        string idreference  = demographics_factory->GetIdReference();
        const vector<ExternalNodeId_t>& nodeIDs = demographics_factory->GetNodeIDs();
        ClimateFactory * climate_factory = nullptr;
#ifndef DISABLE_CLIMATE
        // Initialize climate from file
        climate_factory = ClimateFactory::CreateClimateFactory(&nodeid_suid_map, EnvPtr->Config, idreference);
        if (climate_factory == nullptr)
        {
            throw InitializationException( __FILE__, __LINE__, __FUNCTION__, "Failed to create ClimateFactory" );
        }
        // We can validate climate structure against sim_type now.
#endif

        m_pRngFactory->SetNodeIds( nodeIDs );

        // Initialize load-balancing scheme from file
        IInitialLoadBalanceScheme* p_lbs = LoadBalanceSchemeFactory::Create( loadbalancefilename, nodeIDs.size(), EnvPtr->MPI.NumTasks );
        nodeRankMap.SetInitialLoadBalanceScheme( p_lbs );

        // Delete any existing transitions.json file
        // Anyone could delete the file, but we�ll delegate to rank 0
        if (EnvPtr->MPI.Rank == 0)
        {
            std::string transitions_file_path = FileSystem::Concat( Environment::getInstance()->OutputPath, std::string( IPFactory::transitions_dot_json_filename ) );
            LOG_DEBUG_F( "Deleting any existing %s file.\n", transitions_file_path.c_str() );
            FileSystem::RemoveFile( transitions_file_path );
        }
        EnvPtr->MPI.p_idm_mpi->Barrier();

        if (nodes.size() == 0)   // "Standard" initialization path
        {
            // Add nodes according to demographics-and climate file specifications
            uint32_t node_index = 0;
            for (auto external_node_id : nodeIDs)
            {
                if (getInitialRankFromNodeId( external_node_id ) == EnvPtr->MPI.Rank) // inclusion criteria to be added to this processor's shared memory space
                {
                    suids::suid node_suid;
                    node_suid.data = node_index + 1;

                    LOG_DEBUG_F( "Creating/adding new node: external_node_id = %lu, node_suid = %lu\n", external_node_id, node_suid.data );
                    nodeid_suid_map.insert( nodeid_suid_pair( external_node_id, node_suid ) );

                    addNewNodeFromDemographics( external_node_id, node_suid, demographics_factory, climate_factory, m_IPWhiteListEnabled );
                }
                ++node_index;
            }
        }
        else    // We already have nodes... must have loaded a serialized population.
        {
            for (auto& entry : nodes)
            {
                auto& suid = entry.first;
                auto node = entry.second;
                nodeid_suid_map.insert( nodeid_suid_pair( node->GetExternalID(), suid ) );
                node->SetContextTo(this);
                initializeNode( node, demographics_factory, climate_factory, m_IPWhiteListEnabled );
            }
        }

        if( enable_property_output && !IPFactory::GetInstance()->HasIPs() )
        {
            throw IncoherentConfigurationException( __FILE__, __LINE__, __FUNCTION__, "<Number of Individual Properties>", "0", "Enable_Property_Output", "1" );
        }

        // Merge nodeid<->suid bimaps
        nodeid_suid_map_t merged_map;
        MergeNodeIdSuidBimaps( nodeid_suid_map, merged_map );

        // Initialize migration structure from file
        IMigrationInfoFactory * migration_factory = CreateMigrationInfoFactory( idreference, 
                                                                                m_simConfigObj->migration_structure, 
                                                                                m_simConfigObj->default_torus_size );

        if (migration_factory == nullptr)
        {
            throw InitializationException( __FILE__, __LINE__, __FUNCTION__, "IMigrationInfoFactory" );
        }

        can_support_family_trips = IndividualHumanConfig::CanSupportFamilyTrips( migration_factory );

        release_assert( m_simConfigObj );
        if( (m_simConfigObj->migration_structure != MigrationStructure::NO_MIGRATION) &&
            m_simConfigObj->demographics_initial &&
            !migration_factory->IsAtLeastOneTypeConfiguredForIndividuals() )
        {
            LOG_WARN("Enable_Demographics_Builtin is set to false,  Migration_Model != NO_MIGRATION, and no migration file names have been defined and enabled.  No file-based migration will occur.\n");
        }

        for (auto& entry : nodes)
        {
            release_assert(entry.second);
            (entry.second)->SetupMigration( migration_factory, m_simConfigObj->migration_structure, merged_map );
        } 

        LoadInterventions(campaignfilename, nodeIDs);

#ifndef DISABLE_CLIMATE
        // Clean up
        delete climate_factory; 
        climate_factory = nullptr;
#endif

        delete migration_factory;
        migration_factory = nullptr;

        LOG_INFO_F( "populateFromDemographics() created %d nodes\n", nodes.size() );
        return int(nodes.size());
    }

    void Kernel::Simulation::addNewNodeFromDemographics( ExternalNodeId_t externalNodeId,
                                                         suids::suid node_suid, 
                                                         NodeDemographicsFactory *nodedemographics_factory, 
                                                         ClimateFactory *climate_factory,
                                                         bool white_list_enabled )
    {
        Node *node = Node::CreateNode(this, externalNodeId, node_suid);
        addNode_internal( node, nodedemographics_factory, climate_factory, white_list_enabled );
    }

    void Kernel::Simulation::addNode_internal( INodeContext *node,
                                               NodeDemographicsFactory *nodedemographics_factory,
                                               ClimateFactory *climate_factory,
                                               bool white_list_enabled )
    {

        release_assert(node);
        release_assert(nodedemographics_factory);
#ifndef DISABLE_CLIMATE
        release_assert(climate_factory);
#endif

        node->SetRng( m_pRngFactory->CreateRng( node->GetExternalID() ) );

        // Node initialization 
        node->SetParameters( nodedemographics_factory, climate_factory, white_list_enabled );

        // Populate node
        node->PopulateFromDemographics();

        // Add node to the map
        nodes.insert( std::pair<suids::suid, INodeContext*>(node->GetSuid(), node) );
        node_event_context_list.push_back( node->GetEventContext() );
        nodeRankMap.Add( EnvPtr->MPI.Rank, node );

        notifyNewNodeObservers(node);
    }

    void Kernel::Simulation::initializeNode( INodeContext* node, 
                                             NodeDemographicsFactory* nodedemographics_factory, 
                                             ClimateFactory* climate_factory,
                                             bool white_list_enabled )
    {
        release_assert( node );
        release_assert( nodedemographics_factory );
#ifndef DISABLE_CLIMATE
        release_assert( climate_factory );
#endif

        node->SetParameters( nodedemographics_factory, climate_factory, white_list_enabled );

        // node->PopulateFromDemographics();    // Skip this, node already is populated.
        node->InitializeTransmissionGroupPopulations();

        node_event_context_list.push_back( node->GetEventContext() );
        nodeRankMap.Add( EnvPtr->MPI.Rank, node );

        notifyNewNodeObservers( node );
    }

    void Simulation::loadCampaignFromFile( const std::string& campaignfilename, const std::vector<ExternalNodeId_t>& demographic_node_ids)
    {
        // load in the configuration
        // parse the DM creation events, create them, and add them to an event queue
        event_context_host->LoadCampaignFromFile( campaignfilename, demographic_node_ids);
    }

    void Simulation::notifyNewNodeObservers(INodeContext* node)
    {
        if (new_node_observers.size() > 0)
        {
            for (const auto& entry : new_node_observers)
            {
                entry.second(node);
            }
        }
    }

    //------------------------------------------------------------------
    //   Individual migration methods
    //------------------------------------------------------------------

    void Simulation::resolveMigration()
    {
        LOG_DEBUG("resolveMigration\n");

        WithSelfFunc to_self_func = [this](int myRank) 
        { 
#ifndef _DEBUG
            // Don't bother to serialize locally
            // for (auto individual : migratingIndividualQueues[destination_rank]) // Note the direction of iteration below!
            for (auto iterator = migratingIndividualQueues[myRank].rbegin(); iterator != migratingIndividualQueues[myRank].rend(); ++iterator)
            {
                auto individual = *iterator;
                IMigrate* emigre = individual->GetIMigrate();
                emigre->ImmigrateTo( nodes[emigre->GetMigrationDestination()] );
                if( individual->IsDead() )
                {
                    // we want the individual to migrate home and then finish dieing
                    delete individual;
                }
            }
#else
            if ( migratingIndividualQueues[myRank].size() > 0 )
            {
                auto writer = make_shared<BinaryArchiveWriter>();
                (*static_cast<IArchive*>(writer.get())) & migratingIndividualQueues[myRank];

                for (auto& individual : migratingIndividualQueues[myRank])
                    delete individual; // individual->Recycle();

                migratingIndividualQueues[myRank].clear();

                auto reader = make_shared<BinaryArchiveReader>(static_cast<IArchive*>(writer.get())->GetBuffer(), static_cast<IArchive*>(writer.get())->GetBufferSize());
                (*static_cast<IArchive*>(reader.get())) & migratingIndividualQueues[myRank];
                for (auto individual : migratingIndividualQueues[myRank])
                {
                    IMigrate* immigrant = individual->GetIMigrate();
                    immigrant->ImmigrateTo( nodes[immigrant->GetMigrationDestination()] );
                }
            }
#endif
        }; 

        SendToOthersFunc to_others_func = [this](IArchive* writer, int toRank)
        {
            *writer & migratingIndividualQueues[toRank];
            for (auto& individual : migratingIndividualQueues[toRank])
                individual->Recycle();  // delete individual
        };

        ClearDataFunc clear_data_func = [this](int rank)
        {
            migratingIndividualQueues[rank].clear();
        };

        ReceiveFromOthersFunc from_others_func = [this](IArchive* reader, int fromRank)
        {
            *reader & migratingIndividualQueues[fromRank];
            for (auto individual : migratingIndividualQueues[fromRank])
            {
                IMigrate* immigrant = individual->GetIMigrate();
                immigrant->ImmigrateTo( nodes[immigrant->GetMigrationDestination()] );
                if( individual->IsDead() )
                {
                    // we want the individual to migrate home and then finish dieing
                    delete individual;
                }
            }
        };

        MpiDataExchanger exchanger( "HumanMigration", to_self_func, to_others_func, from_others_func, clear_data_func );
        exchanger.ExchangeData( this->currentTime );

        // -----------------------------------------------------------
        // --- We sort the humans so that they are in a known order.
        // --- This solves the problem where individuals will get in
        // --- different orders depending on how many cores are used.
        // -----------------------------------------------------------
        if( m_pRngFactory->GetPolicy() == RandomNumberGeneratorPolicy::ONE_PER_NODE )
        {
            for( auto node : nodes )
            {
                node.second->SortHumans();
            }
        }
    }

    void Simulation::PostMigratingIndividualHuman(IIndividualHuman *i)
    {
        migratingIndividualQueues[nodeRankMap.GetRankFromNodeSuid(i->GetMigrationDestination())].push_back(i);
    }

    bool Simulation::CanSupportFamilyTrips() const
    {
        return can_support_family_trips;
    }

    //------------------------------------------------------------------
    //   Assorted getters and setters
    //-----------------------------------------------------------------

    const DemographicsContext* Simulation::GetDemographicsContext() const
    {
        return demographicsContext;
    }

    const IdmDateTime& Simulation::GetSimulationTime() const
    {
        return currentTime;
    }

    int Simulation::GetSimulationTimestep() const
    {
        return currentTime.timestep;
    }

    suids::suid Simulation::GetNextInfectionSuid()
    {
        return infectionSuidGenerator();
    }

    suids::suid Simulation::GetNodeSuid( uint32_t external_node_id )
    {
        return nodeRankMap.GetSuidFromExternalID( external_node_id );
    }

    ExternalNodeId_t Simulation::GetNodeExternalID( const suids::suid& rNodeSuid )
    {
        return nodeRankMap.GetNodeInfo( rNodeSuid ).GetExternalID();
    }

    uint32_t Simulation::GetNodeRank( const suids::suid& rNodeSuid )
    {
        return nodeRankMap.GetRankFromNodeSuid( rNodeSuid );
    }

    // Should only be accessed by Node
    RANDOMBASE* Simulation::GetRng()
    {
        return rng;
    }

    std::vector<IReport*>& Simulation::GetReports()
    {
        return reports;
    }

    std::vector<IReport*>& Simulation::GetReportsNeedingIndividualData()
    {
        return individual_data_reports ;
    }

    int Simulation::getInitialRankFromNodeId( ExternalNodeId_t node_id )
    {
        return nodeRankMap.GetInitialRankFromNodeId(node_id); // R: leave as a wrapper call to nodeRankMap.GetInitialRankFromNodeId()
    }
    
    ISimulationContext * Simulation::GetContextPointer() 
    { 
        return this; 
    }

    void Simulation::PropagateContextToDependents()
    {
        ISimulationContext *context = GetContextPointer();
        for (auto& entry : nodes)
        {
            entry.second->SetContextTo(context);
        }
    }

    // IGlobalContext inferface for all the other components 
    const SimulationConfig* Simulation::GetSimulationConfigObj() const
    {
        return m_simConfigObj;
    }

    const IInterventionFactory* Simulation::GetInterventionFactory() const
    {
        return m_interventionFactoryObj;
    }

    REGISTER_SERIALIZABLE(Simulation);

    void Simulation::serialize(IArchive& ar, Simulation* obj)
    {
        Simulation& sim = *obj;
        ar.labelElement("serializationMask") & (uint32_t&)sim.serializationMask;

        if (((sim.serializationMask & SerializationFlags::Population) != 0) ||
            ((sim.serializationMask & SerializationFlags::Properties) != 0))
        {
            ar.labelElement("infectionSuidGenerator") & sim.infectionSuidGenerator;
            ar.labelElement("m_RngFactory") & sim.m_pRngFactory;
            ar.labelElement( "rng" ) & sim.rng;
        }

        if ((sim.serializationMask & SerializationFlags::Population) != 0) {
            if (ar.IsReader()) {
                // Read the nodes element in case it's a version 1 serialized file which includes
                // the nodes in the nodes element.
                // If it's a version 2+ serialized file, the nodes element will be empty and this
                // will be harmless.
                ar.labelElement("nodes"); serialize(ar, sim.nodes);
            }
            else {
                // Write an empty element as the nodes will be serialized separately.
                NodeMap_t empty;
                ar.labelElement("nodes"); serialize(ar, empty);
            }
        }

        if ((sim.serializationMask & SerializationFlags::Parameters) != 0) {
            ar.labelElement( "campaignFilename" ) & sim.campaignFilename;
            ar.labelElement( "custom_reports_filename" ) & sim.custom_reports_filename;

            ar.labelElement("sim_type") & (uint32_t&)sim.sim_type;
            ar.labelElement("demographic_tracking") & sim.demographic_tracking;
            ar.labelElement("enable_spatial_output") & sim.enable_spatial_output;
            ar.labelElement("enable_property_output") & sim.enable_property_output;
            ar.labelElement("enable_default_report") & sim.enable_default_report;
            ar.labelElement( "enable_event_report" ) & sim.enable_event_report;
            ar.labelElement( "enable_node_event_report" ) & sim.enable_event_report;
            ar.labelElement( "enable_coordinator_event_report" ) & sim.enable_coordinator_event_report;
            ar.labelElement( "enable_surveillance_event_report" ) & sim.enable_surveillance_event_report;

            ar.labelElement("loadbalance_filename") & sim.loadbalance_filename;
        }

        if ((sim.serializationMask & SerializationFlags::Properties) != 0) {
            ar.labelElement("loadBalanceFilename") & sim.loadBalanceFilename;
            ar.labelElement("campaign_filename") & sim.campaign_filename;
        }
    }

    void Simulation::serialize(IArchive& ar, NodeMap_t& node_map)
    {
        size_t count = (ar.IsWriter() ? node_map.size() : -1);
        ar.startArray(count);
        if (ar.IsWriter())
        {
            for (auto& entry : node_map)
            {
                ar.startObject();
                // entry.first is const which doesn't play well with IArchive operator '&'
                suids::suid suid(entry.first);
                ar.labelElement("suid") & suid;
                ar.labelElement("node") & entry.second;
                ar.endObject();
            }
        }
        else
        {
            for (size_t i = 0; i < count; ++i)
            {
                ar.startObject();
                suids::suid suid;
                ISerializable* obj;
                ar.labelElement("suid") & suid;
                ar.labelElement("node") & obj;
                ar.endObject();
                node_map[suid] = static_cast<Node*>(obj);
            }
        }
        ar.endArray();
    }
}
