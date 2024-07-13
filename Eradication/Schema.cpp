/***************************************************************************************************

Copyright (c) 2018 Intellectual Ventures Property Holdings, LLC (IVPH) All rights reserved.

EMOD is licensed under the Creative Commons Attribution-Noncommercial-ShareAlike 4.0 License.
To view a copy of this license, visit https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode

***************************************************************************************************/

#include "stdafx.h"

#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream> // ostringstream

#ifdef WIN32
#include <windows.h>
#endif

#include "Schema.h"
#include "Log.h"
#include "ProgVersion.h"
#include "DllLoader.h"
#include "EventTrigger.h"
#include "InterventionFactory.h"
#include "SimulationConfig.h"
#include "CampaignEvent.h"
#include "EventCoordinator.h"
#include "PythonSupport.h"
#include "Memory.h"
#include "FileSystem.h"
#include "iv_params.rc"

SETUP_LOGGING( "Schema" )

#define CAMP_Use_Defaults_DESC_TEXT "Set to true (1) if you don't want to have to specify all params for event coordinators and interventions. Use at own risk." 

const std::vector<std::string> getSimTypeList()
{
    const char * simTypeListC[] = { "GENERIC_SIM"
#ifndef DISABLE_VECTOR
        , "VECTOR_SIM"
#endif
#ifndef DISABLE_MALARIA
        , "MALARIA_SIM"
#endif
#ifndef DISABLE_AIRBORNE
        , "AIRBORNE_SIM"
#endif
#ifdef ENABLE_POLIO
        , "POLIO_SIM"
#endif
#ifndef DISABLE_TBHIV
        , "TBHIV_SIM"
#endif
#ifndef DISABLE_STI
        , "STI_SIM"
#endif
#ifndef DISABLE_HIV
        , "HIV_SIM"
#endif
#ifdef ENABLE_DENGUE
        , "DENGUE_SIM"
#endif
#ifdef ENABLE_PYTHON_FEVER
        , "PY_SIM"
#endif
#ifdef ENABLE_TYPHOID
        , "TYPHOID_SIM"
#endif
#ifdef ENABLE_ENVIRONMENTAL
        , "ENVIRONMENTAL_SIM"
#endif
    };

#define KNOWN_SIM_COUNT (sizeof(simTypeListC)/sizeof(simTypeListC[0]))
    std::vector<std::string> simTypeList( simTypeListC, simTypeListC + KNOWN_SIM_COUNT );
    return simTypeList;
}

void writeInputSchemas( const char* dll_path, const char* output_path, const char* exe_name )
{
    json::Object jsonRoot;
    json::QuickBuilder total_schema( jsonRoot );

    Kernel::JsonConfigurable::_dryrun = true;

    // --------------------------
    // --- Create Metadata Schema
    // --------------------------
    json::Object vsRoot;
    json::QuickBuilder versionSchema( vsRoot );
    ProgDllVersion pv;
    versionSchema["DTK_Version"]    = json::String( pv.getVersion() );
    versionSchema["DTK_Branch"]     = json::String( pv.getSccsBranch() );
    versionSchema["DTK_Build_Date"] = json::String( pv.getBuildDate() );
    versionSchema["Creator"]        = json::String( exe_name );

    total_schema["Version"]         = versionSchema.As<json::Object>();

    // ---------------------
    // --- Create DLL Schema
    // ---------------------
    DllLoader dllLoader;
    std::map< std::string, createSim > createSimFuncPtrMap;

    if( dllLoader.LoadDiseaseDlls(createSimFuncPtrMap) )
    {
        json::Object diseaseSchemas = dllLoader.GetDiseaseDllSchemas();
        total_schema[ "config:emodules" ] = diseaseSchemas;
    }

    // ------------------------
    // --- Create Config Schema
    // ------------------------
    json::Object configSchemaAll;

    for (auto& sim_type : getSimTypeList())
    {
        json::Object fakeSimTypeConfigJson;
        fakeSimTypeConfigJson["Simulation_Type"] = json::String(sim_type);
        Configuration* fakeConfig = Configuration::CopyFromElement( fakeSimTypeConfigJson );
        Kernel::SimulationConfig* pConfig = Kernel::SimulationConfigFactory::CreateInstance(fakeConfig);
        release_assert( pConfig );

        json::QuickBuilder config_schema = pConfig->GetSchema();
        configSchemaAll[sim_type] = config_schema;

        delete fakeConfig;
        fakeConfig = nullptr;
    }

    for (auto& entry : Kernel::JsonConfigurable::get_registration_map())
    {
        const std::string& classname = entry.first;
        json::QuickBuilder config_schema = ((*(entry.second))());
        configSchemaAll[classname] = config_schema;
    }

    total_schema[ "config" ] = configSchemaAll;

    // --------------------------
    // --- Create Campaign Schema
    // --------------------------
    json::Object useDefaultsRoot;
    json::QuickBuilder udSchema( useDefaultsRoot );

    udSchema["type"]        = json::String( "bool" );
    udSchema["default"]     = json::Number( 0 );
    udSchema["description"] = json::String( CAMP_Use_Defaults_DESC_TEXT );

    json::Object objRoot;
    json::QuickBuilder camp_schema( objRoot );

    camp_schema["Events"][0]    = json::String( "idmType:CampaignEvent" );
    camp_schema["Use_Defaults"] = udSchema.As<json::Object>();

    total_schema[ "interventions" ] = camp_schema.As<json::Object>();

    // --------------------------
    // --- Create idmTypes Schema
    // --------------------------
    json::Object ivtypes_root;
    json::QuickBuilder ivt_schema( ivtypes_root );

    json::QuickBuilder ivs_schema = Kernel::IndividualIVFactory::getInstance()->GetSchema();
    json::QuickBuilder nvs_schema = Kernel::NodeIVFactory::getInstance()->GetSchema();

    ivt_schema["idmAbstractType:IndividualIntervention"] = ivs_schema.As<json::Object>();
    ivt_schema["idmAbstractType:NodeIntervention"]       = nvs_schema.As<json::Object>();

    json::Object idmtypes_root;
    json::QuickBuilder idmtypes_schema( idmtypes_root );
    
    json::QuickBuilder ces_schema = Kernel::CampaignEventFactory::getInstance()->GetSchema();
    json::QuickBuilder ecs_schema = Kernel::EventCoordinatorFactory::getInstance()->GetSchema();
    json::QuickBuilder nds_schema  = Kernel::NodeSetFactory::getInstance()->GetSchema();

    idmtypes_schema["idmAbstractType:CampaignEvent"]    = ces_schema.As<json::Object>();
    idmtypes_schema["idmAbstractType:Intervention"]     = ivt_schema.As<json::Object>();
    idmtypes_schema["idmAbstractType:EventCoordinator"] = ecs_schema.As<json::Object>();
    idmtypes_schema["idmAbstractType:NodeSet"]          = nds_schema.As<json::Object>();

    total_schema[ "idmTypes" ] = idmtypes_schema.As<json::Object>();

    // --------------------------
    // --- Post-processing schema
    // --------------------------
    json::Object idmtypes_addl;
    json::SchemaUpdater updater(idmtypes_addl);
    jsonRoot.Accept(updater);

    json::Object::iterator it(idmtypes_addl.Begin());
    json::Object::iterator itEnd(idmtypes_addl.End());

    while (it != itEnd)
    {
        if( it->element.Type() == json::ElementType::OBJECT_ELEMENT &&
            (json::QuickInterpreter(it->element)).As<json::Object>().Exist("base") )
        {
            // Don't add element
        }
        else
        {
            total_schema["idmTypes"][it->name] = it->element;
        }
        it++;
    }

    // --------------------------
    // --- Write Schema to output
    // --------------------------
    std::ofstream schema_ostream_file;
    
    std::string szOutputPath = std::string( output_path );
    if( szOutputPath != "stdout" )
    {
        // Attempt to open output path
        FileSystem::OpenFileForWriting( schema_ostream_file, output_path );
    }
    std::ostream &schema_ostream = ( ( szOutputPath == "stdout" ) ? std::cout : schema_ostream_file );
    schema_ostream << std::setprecision(10);
    
    json::Writer::Write( total_schema, schema_ostream, "    ", true, true );
    schema_ostream_file.close();

    // PythonSupportPtr can be null during componentTests
    if( szOutputPath != "stdout" )
    {
        std::cout << "Successfully created schema in file " << output_path << ". Attempting to post-process." << std::endl;
        Kernel::PythonSupport::RunPyFunction( output_path, Kernel::PythonSupport::SCRIPT_POST_PROCESS_SCHEMA );
    }
}


namespace json
{
    void SchemaUpdater::Visit(Array& array)
    {
        Array::iterator it(array.Begin());
        Array::iterator itEnd(array.End());

        // Iterate over elements
        while (it != itEnd)
        {
            // No action; 
            it->Accept(*this); 
            it++;
        }
    }

    void SchemaUpdater::Visit(Object& object)
    {
        Object::iterator it(object.Begin());
        Object::iterator itEnd(object.End());

        // Iterate over elements
        while (it != itEnd)
        {
            it->element.Accept(*this);

            // Aggregate idmTypes to separate object
            std::string key_str = it->name;
            if(key_str.rfind("idmType:", 0) == 0)
            {
                if(!m_idmt_schema.Exist(it->name))
                {
                    Object::Member obj_copy(key_str, Element(it->element));
                    m_idmt_schema.Insert(obj_copy);
                }
                it = object.Erase(it);
            }
            else
            {
                it++;
            }
        }
    }

    void SchemaUpdater::Visit(String& stringElement)
    {
        std::string in_val = stringElement;

        // Replace "Type" with "AbstractType" as needed
        if (in_val == "idmType:Intervention")
        {
            json::QuickBuilder type_str(stringElement);
            type_str = json::String("idmAbstractType:Intervention");
        }
        else if (in_val == "idmType:IndividualIntervention")
        {
            json::QuickBuilder type_str(stringElement);
            type_str = json::String("idmAbstractType:IndividualIntervention");
        }
        else if (in_val == "idmType:NodeIntervention")
        {
            json::QuickBuilder type_str(stringElement);
            type_str = json::String("idmAbstractType:NodeIntervention");
        }
        else if (in_val == "idmType:EventCoordinator")
        {
            json::QuickBuilder type_str(stringElement);
            type_str = json::String("idmAbstractType:EventCoordinator");
        }
        else if (in_val == "idmType:CampaignEvent")
        {
            json::QuickBuilder type_str(stringElement);
            type_str = json::String("idmAbstractType:CampaignEvent");
        }
        else if (in_val == "idmType:NodeSet")
        {
            json::QuickBuilder type_str(stringElement);
            type_str = json::String("idmAbstractType:NodeSet");
        }
    }

    void SchemaUpdater::Visit(Number& number)
    {
        // No action;
    }

    void SchemaUpdater::Visit(Uint64& number)
    {
        // No action;
    }

    void SchemaUpdater::Visit(Boolean& booleanElement)
    {
        // No action;
    }

    void SchemaUpdater::Visit(Null& nullElement)
    {
        // No action;
    }
}
