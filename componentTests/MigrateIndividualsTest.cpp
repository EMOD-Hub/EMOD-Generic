

#include "stdafx.h"
#include "UnitTest++.h"
#include <string>
#include <memory>

#include "componentTests.h"
#include "MigrateIndividuals.h"
#include "Individual.h"

#include "FileSystem.h"
#include "Configuration.h"
#include "ConfigParams.h"

using namespace Kernel;

SUITE(MigrateIndividualsTest)
{
    struct MigrateFixture
    {
        MigrateIndividuals  m_MigrateIndividuals;

        MigrateFixture()
            : m_MigrateIndividuals()
        {
            JsonConfigurable::_useDefaults = true;
            Environment::Finalize();
            Environment::setLogger( new SimpleLogger( Logger::tLevel::WARNING ) );
        }

        ~MigrateFixture()
        {
            Environment::Finalize();
        }
    };

    TEST_FIXTURE(MigrateFixture, TestPositive)
    {
        // -------------------------------------------------------------
        // --- Test that the intervention Configures() ok when
        // --- Migration Model is valid.
        // -------------------------------------------------------------
        ConfigParams gen_config_obj;
        std::unique_ptr<Configuration> mig_config_file( Configuration_Load( "testdata/MigrationTest/config.json" ) );
        std::unique_ptr<Configuration> mig_config( Environment::CopyFromElement( (*mig_config_file)["parameters"] ) );
        gen_config_obj.Configure( mig_config.get() );

        std::unique_ptr<Configuration> p_config( Configuration_Load( "testdata/MigrateIndividualsTest.json" ) );
        m_MigrateIndividuals.Configure( p_config.get() );
    }
}
