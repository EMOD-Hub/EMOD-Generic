/***************************************************************************************************

Copyright (c) 2018 Intellectual Ventures Property Holdings, LLC (IVPH) All rights reserved.

EMOD is licensed under the Creative Commons Attribution-Noncommercial-ShareAlike 4.0 License.
To view a copy of this license, visit https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode

***************************************************************************************************/

#include "stdafx.h"
#include "StatusReporter.h"
#include <errno.h>
#include <iostream>
#include "Environment.h"
#include "FileSystem.h"
#include "Exceptions.h"
#include "Configuration.h"
#include "Configure.h"
#include "Log.h"
#include "IdmMpi.h"
#include "EventTrigger.h"
#include "IdmString.h"

using namespace std;

SETUP_LOGGING( "Environment" )

#pragma warning(disable: 4996) // for suppressing strcpy caused security warnings

Environment* Environment::localEnv = nullptr;

Environment::Environment()
    : MPI()
    , Log( nullptr )
    , Config(nullptr)
    , SimConfig(nullptr)
    , pIPFactory( nullptr )
    , pNPFactory( nullptr )
    , pRngFactory( nullptr )
    , Status_Reporter(nullptr)
    , InputPaths()
    , OutputPath()
    , StatePath()
    , DllPath()
{
    MPI.NumTasks  = 1;
    MPI.Rank      = 0;
    MPI.p_idm_mpi = nullptr;
}

bool Environment::Initialize(
    IdmMpi::MessageInterface* pMpi,
    string configFileName, 
    string inputPath, string outputPath, string dllPath,
    bool get_schema)
{
    release_assert( pMpi );
    if( localEnv == nullptr )
    {
        throw Kernel::IllegalOperationException( __FILE__, __LINE__, __FUNCTION__, "Environment has not been created." );
    }

    localEnv->MPI.p_idm_mpi = pMpi;
    localEnv->MPI.NumTasks  = pMpi->GetNumTasks();
    localEnv->MPI.Rank      = pMpi->GetRank();

    std::list< std::string > inputPaths;
    std::size_t start_pos    = 0;
    std::size_t end_pos      = 0;
    std::string sep_chars    = ";,";
    std::string in_path_list = FileSystem::RemoveTrailingChars( inputPath );

    // Split in_path_list into list based on sep_chars
    while((start_pos = in_path_list.find_first_not_of(sep_chars, end_pos)) != std::string::npos)
    {
        end_pos = in_path_list.find_first_of(sep_chars, start_pos);
        inputPaths.push_back(in_path_list.substr(start_pos, end_pos-start_pos));
    }

    // Always check current working directory, but do it last
    inputPaths.push_back(".");

    outputPath = FileSystem::RemoveTrailingChars( outputPath );

    if( !get_schema )
    {
        if( FileSystem::FileExists( outputPath ) )
        {
            std::string msg = "Output Path ("+outputPath+") exists and is not a directory." ;
            throw Kernel::GeneralConfigurationException( __FILE__, __LINE__, __FUNCTION__, msg.c_str() );
        }

        if( localEnv->MPI.Rank != 0 )
        {
            // -------------------------------------------------------
            // --- Wait for Rank=0 process to create output directory
            // -------------------------------------------------------
            localEnv->MPI.p_idm_mpi->Barrier();
        }
        else
        {
            // ------------------------------------------------------------------------------
            // --- The Process with Rank=0 is responsible for creating the output directory
            // ------------------------------------------------------------------------------
            if( !FileSystem::DirectoryExists(outputPath) )
            {
                FileSystem::MakeDirectory( outputPath ) ;
            }

            // ----------------------------------------------------------------------
            // --- Synchronize with other process after creating output directory
            // ----------------------------------------------------------------------
            localEnv->MPI.p_idm_mpi->Barrier();

            if( !FileSystem::DirectoryExists(outputPath) )
            {
                LOG_ERR_F( "Rank=%d: Failed to create new output directory '%s' with error %s\n", localEnv->MPI.Rank, outputPath.c_str(), strerror(errno) );
                throw Kernel::FileNotFoundException( __FILE__, __LINE__, __FUNCTION__, outputPath.c_str() );
            }
        }
    }

    localEnv->OutputPath  = outputPath;
    localEnv->InputPaths  = inputPaths;
    localEnv->DllPath     = dllPath;

    if( get_schema )
    {
        return true;
    }

    Configuration *config = Configuration::Load(configFileName);

    if (!config) 
    {
        delete localEnv;
        localEnv = nullptr;
        throw Kernel::InitializationException( __FILE__, __LINE__, __FUNCTION__, configFileName.c_str() );
    }

    localEnv->Config = Configuration::CopyFromElement( (*config)["parameters"], config->GetDataLocation() );

    localEnv->Status_Reporter = StatusReporter::getInstance();

    return true;
}

Environment::~Environment()
{
    if (Config)
        delete Config;

    delete pIPFactory ;
    pIPFactory = nullptr ;

    delete pNPFactory ;
    pNPFactory = nullptr ;

    for( auto factory : event_trigger_factories )
    {
        delete factory;
    }
    event_trigger_factories.clear();
}

void Environment::Finalize()
{
    delete localEnv;
    localEnv = nullptr;
}

std::string Environment::FindFileOnPath( const std::string& rFilename )
{
    if( rFilename == "" )
    {
        return "";
    }

    if( localEnv == nullptr )
    {
        throw Kernel::IllegalOperationException( __FILE__, __LINE__, __FUNCTION__, "Environment has not been created." );
    }
    std::string error = "";
    for( auto path : localEnv->InputPaths )
    {
        std::string filepath = FileSystem::Concat( path, rFilename );
        error += filepath;
        if( FileSystem::FileExists( filepath ) )
        {
            return filepath;
        }
        else
        {
            error += " or here ";
        }
    }
        
    throw Kernel::FileNotFoundException( __FILE__, __LINE__, __FUNCTION__, error.c_str() );
}

const Configuration* Environment::getConfiguration()
{ 
    if( localEnv == nullptr )
    {
        throw Kernel::IllegalOperationException( __FILE__, __LINE__, __FUNCTION__, "Environment has not been created." );
    }
    return getInstance()->Config ;
}


Configuration* Environment::CopyFromElement( const json::Element& rElement, const std::string& rDataLocation )
{
    return Configuration::CopyFromElement( rElement, rDataLocation );
}

Configuration* Environment::LoadConfigurationFile( const std::string& rFileName )
{
    return Configuration::Load( rFileName );
}

Environment* Environment::getInstance()
{ 
    if( localEnv == nullptr )
    {
        localEnv = new Environment();
    }
    return localEnv;
}

void Environment::setInstance(Environment* env)
{
    if( localEnv != env )
    {
        delete localEnv ;
    }
    localEnv = env ;
}

void Environment::setLogger(SimpleLogger* log)
{ 
    getInstance()->Log = log; 
}

void Environment::initLogger()
{
    getInstance()->Log->Init();
}

void Environment::setSimulationConfig(void* SimConfig)
{ 
    getInstance()->SimConfig = SimConfig;
}

const void* Environment::getSimulationConfig()
{ 
    if( localEnv == nullptr )
    {
        throw Kernel::IllegalOperationException( __FILE__, __LINE__, __FUNCTION__, "Environment has not been created." );
    }
    return localEnv->SimConfig;
}

StatusReporter * Environment::getStatusReporter() 
{ 
    if( localEnv == nullptr )
    {
        throw Kernel::IllegalOperationException( __FILE__, __LINE__, __FUNCTION__, "Environment has not been created." );
    }
    return localEnv->Status_Reporter ;
}

void Environment::setIPFactory( void* pipf )
{ 
    getInstance()->pIPFactory = pipf; 
}

void* Environment::getIPFactory()
{
    if (localEnv == nullptr)
    {
        throw Kernel::IllegalOperationException(__FILE__, __LINE__, __FUNCTION__, "Environment has not been created.");
    }
    return localEnv->pIPFactory;
}

void Environment::setNPFactory( void* pnpf )
{
    getInstance()->pNPFactory = pnpf;
}

void* Environment::getNPFactory()
{
    if( localEnv == nullptr )
    {
        throw Kernel::IllegalOperationException( __FILE__, __LINE__, __FUNCTION__, "Environment has not been created." );
    }
    return localEnv->pNPFactory;
}

const void* Environment::getEventTriggerFactory( int event_type )
{
    return getInstance()->event_trigger_factories[ event_type ];
}

void Environment::setEventTriggerFactory( int event_type, void* pFactory )
{
    release_assert( localEnv );
    localEnv->event_trigger_factories[ event_type ] = pFactory;
}
