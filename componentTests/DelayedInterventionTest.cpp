/***************************************************************************************************

Copyright (c) 2018 Intellectual Ventures Property Holdings, LLC (IVPH) All rights reserved.

EMOD is licensed under the Creative Commons Attribution-Noncommercial-ShareAlike 4.0 License.
To view a copy of this license, visit https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode

***************************************************************************************************/

#include "stdafx.h"
#include "UnitTest++.h"
#include <string>
#include <memory>
#include "componentTests.h"

#include "DelayedIntervention.h"
#include "IdmMpi.h"
#include "SimulationConfig.h"

using namespace Kernel;

SUITE( DelayedInterventionTest )
{
    struct DelayedFixture
    {
        IdmMpi::MessageInterface* m_pMpi;
        SimulationConfig*         m_pSimulationConfig;

        DelayedFixture()
            : m_pMpi(nullptr)
            , m_pSimulationConfig( new SimulationConfig() )
        {
            Environment::Finalize();
            JsonConfigurable::ClearMissingParameters();
            JsonConfigurable::_useDefaults = true;

            m_pMpi = IdmMpi::MessageInterface::CreateNull();

            Environment::Finalize();
            Environment::setLogger( new SimpleLogger( Logger::tLevel::WARNING ) );
            int argc = 1;
            char* exeName = "componentTests.exe";
            char** argv = &exeName;
            string configFilename( "testdata/DelayedInterventionTest/config.json" );
            string inputPath( "testdata/DelayedInterventionTest" );
            string outputPath( "testdata/DelayedInterventionTest" );
            string statePath( "testdata/DelayedInterventionTest" );
            string dllPath( "" );
            Environment::Initialize( m_pMpi, configFilename, inputPath, outputPath, dllPath, false );

            Environment::setSimulationConfig( m_pSimulationConfig );
        }

        ~DelayedFixture()
        {
            Environment::Finalize();
            JsonConfigurable::_useDefaults = false;
        }
    };

    void TestHelper_Exception( int lineNumber, const std::string& rFilename, const std::string& rExpMsg )
    {
        try
        {
            unique_ptr<Configuration> p_config( Environment::LoadConfigurationFile( rFilename ) );

            DelayedIntervention di;

            di.Configure( p_config.get() );

            CHECK_LN( false, lineNumber ); // should not get here
        }
        catch( DetailedException& re )
        {
            std::string msg = re.GetMsg();
            if( msg.find( rExpMsg ) == string::npos )
            {
                PrintDebug( msg );
                CHECK_LN( msg.find( rExpMsg ) != string::npos, lineNumber );
            }
        }
    }

    TEST_FIXTURE( DelayedFixture, TestIllegalNodeLevelIntervention )
    {
        m_pSimulationConfig->migration_structure = MigrationStructure::FIXED_RATE_MIGRATION;

        TestHelper_Exception( __LINE__, "testdata/DelayedInterventionTest/TestIllegalNodeLevelIntervention.json",
                              "Invalid Intervention Type in 'DelayedIntervention'.\n'MigrateFamily' is a(n) NODE-level intervention.\nA(n) INDIVIDUAL-level intervention is required." );
    }
}
