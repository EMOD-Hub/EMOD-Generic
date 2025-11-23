
#include "stdafx.h"
#include <memory> // unique_ptr
#include "UnitTest++.h"
#include "componentTests.h"

#include "MalariaDrugTypeParameters.h"
#include "StrainIdentity.h"
#include "GenomeMarkers.h"
#include "RANDOM.h"

using namespace std;
using namespace Kernel;

SUITE( MalariaDrugTypeParametersTest )
{
    struct DrugFixture
    {
        DrugFixture()
        {
            Environment::Finalize();
            Environment::setLogger( new SimpleLogger( Logger::tLevel::WARNING ) );
            JsonConfigurable::missing_parameters_set.clear();
        }

        ~DrugFixture()
        {
            MalariaDrugTypeCollection::DeleteInstance();
            Environment::Finalize();

            JsonConfigurable::missing_parameters_set.clear();
        }
    };

    TEST_FIXTURE( DrugFixture, TestInitialize )
    {
        EnvPtr->Config = Environment::LoadConfigurationFile( "testdata/MalariaDrugTypeParametersTest/TestInitialize.json" );

        MalariaDrugTypeCollection::GetInstanceNonConst()->ConfigureFromJsonAndKey( EnvPtr->Config, "Malaria_Drug_Params" );
        MalariaDrugTypeCollection::GetInstanceNonConst()->CheckConfiguration();

        const MalariaDrugTypeParameters& r_drug = MalariaDrugTypeCollection::GetInstance()->GetDrug( "Chloroquine" );

        CHECK_EQUAL(  1.0f, r_drug.GetBodyWeightExponent() );
        CHECK_EQUAL(  2.0f, r_drug.GetCMax() );
        CHECK_EQUAL(  3.0f, r_drug.GetDecayT1() );
        CHECK_EQUAL(  4.0f, r_drug.GetDecayT2() );
        CHECK_EQUAL(  5.0f, r_drug.GetDoseInterval() );
        CHECK_EQUAL(  6,    r_drug.GetFullTreatmentDoses() );
        CHECK_EQUAL(  7.0f, r_drug.GetKillRateGametocyte02() );
        CHECK_EQUAL(  8.0f, r_drug.GetKillRateGametocyte34() );
        CHECK_EQUAL(  9.0f, r_drug.GetKillRateGametocyteM() );
        CHECK_EQUAL( 10.0f, r_drug.GetKillRateHepatocyte() );
        CHECK_EQUAL( 11.0f, r_drug.GetPkpdC50() );
        CHECK_EQUAL( 12.0f, r_drug.GetVd() );
        CHECK_EQUAL( 13.0f, r_drug.GetMaxDrugIRBCKill() );
    }

    void TestHelper_InitializeException( int lineNumber, const std::string& rFilename, const std::string& rExpMsg )
    {
        JsonConfigurable::_useDefaults = false;
        JsonConfigurable::_track_missing = false;
        try
        {
            EnvPtr->Config = Environment::LoadConfigurationFile( rFilename );

            MalariaDrugTypeCollection::GetInstanceNonConst()->ConfigureFromJsonAndKey( EnvPtr->Config, "Malaria_Drug_Params" );
            MalariaDrugTypeCollection::GetInstanceNonConst()->CheckConfiguration();

            const MalariaDrugTypeParameters& r_drug = MalariaDrugTypeCollection::GetInstance()->GetDrug( "Chloroquine" );

            CHECK_LN( false, lineNumber ); // should not get here
        }
        catch( DetailedException& re )
        {
            std::string msg = re.GetMsg();
            if( msg.find( rExpMsg ) == string::npos )
            {
                cerr << msg << endl <<endl;
                cerr << rExpMsg << endl;
                PrintDebug(           std::string( "\n" ) );
                PrintDebug( rExpMsg + std::string( "\n" ) );
                PrintDebug( msg     + std::string( "\n" ) );
                CHECK_LN( false, lineNumber );
            }
        }
    }

    TEST_FIXTURE( DrugFixture, TestBadJsonDoseMapAge )
    {
        TestHelper_InitializeException( __LINE__, "testdata/MalariaDrugTypeParametersTest/TestBadJsonDoseMapAge.json",
                                        "Parameter 'Upper_Age_In_Years of DoseFractionByAge' not found in input file 'testdata/MalariaDrugTypeParametersTest/TestBadJsonDoseMapAge.json'." );
    }

    TEST_FIXTURE( DrugFixture, TestBadJsonDoseMapFraction )
    {
        TestHelper_InitializeException( __LINE__, "testdata/MalariaDrugTypeParametersTest/TestBadJsonDoseMapFraction.json",
                                        "Parameter 'Fraction_Of_Adult_Dose of DoseFractionByAge' not found in input file 'testdata/MalariaDrugTypeParametersTest/TestBadJsonDoseMapFraction.json'." );
    }

    TEST_FIXTURE( DrugFixture, TestOutOfRangeAgeLow )
    {
        TestHelper_InitializeException( __LINE__, "testdata/MalariaDrugTypeParametersTest/TestOutOfRangeAgeLow.json",
                                        "Configuration variable 'Upper_Age_In_Years' with value -16 out of range: less than 0." );
    }

    TEST_FIXTURE( DrugFixture, TestOutOfRangeAgeHigh )
    {
        TestHelper_InitializeException( __LINE__, "testdata/MalariaDrugTypeParametersTest/TestOutOfRangeAgeHigh.json",
                                        "Configuration variable 'Upper_Age_In_Years' with value 999 out of range: greater than 125." );
    }

    TEST_FIXTURE( DrugFixture, TestOutOfRangeFractionLow )
    {
        TestHelper_InitializeException( __LINE__, "testdata/MalariaDrugTypeParametersTest/TestOutOfRangeFractionLow.json",
                                        "Configuration variable 'Fraction_Of_Adult_Dose' with value -1 out of range: less than 0." );
    }

    TEST_FIXTURE( DrugFixture, TestOutOfRangeFractionHigh )
    {
        TestHelper_InitializeException( __LINE__, "testdata/MalariaDrugTypeParametersTest/TestOutOfRangeFractionHigh.json",
                                        "Configuration variable 'Fraction_Of_Adult_Dose' with value 77 out of range: greater than 1." );
    }

    TEST_FIXTURE( DrugFixture, TestMissingDrug )
    {
        TestHelper_InitializeException( __LINE__, "testdata/MalariaDrugTypeParametersTest/TestMissingDrug.json",
                                        "'Chloroquine' is an unknown drug.\nValid drug names are:\nBAD" );
    }
}
