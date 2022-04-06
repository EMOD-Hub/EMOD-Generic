/***************************************************************************************************

Copyright (c) 2018 Intellectual Ventures Property Holdings, LLC (IVPH) All rights reserved.

EMOD is licensed under the Creative Commons Attribution-Noncommercial-ShareAlike 4.0 License.
To view a copy of this license, visit https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode

***************************************************************************************************/

#pragma once

#include "EnumSupport.h"

namespace Kernel
{

    // ENUM defs for Gender_Type
    ENUM_DEFINE(Gender,
        ENUM_VALUE_SPEC(MALE                                                , 0)        // previously: #define MALE 1
        ENUM_VALUE_SPEC(FEMALE                                              , 1)        // previously: #define FEMALE 2
        ENUM_VALUE_SPEC(COUNT                                               , 2) )

    // ENUM defs for CLIMATE_STRUCTURE
    ENUM_DEFINE(ClimateStructure, 
        ENUM_VALUE_SPEC(CLIMATE_OFF                                         , 0)
        ENUM_VALUE_SPEC(CLIMATE_CONSTANT                                    , 1)
        ENUM_VALUE_SPEC(CLIMATE_KOPPEN                                      , 2)
        ENUM_VALUE_SPEC(CLIMATE_BY_DATA                                     , 3))

    // ENUM defs for CLIMATE_UPDATE_RESOLUTION
    ENUM_DEFINE(ClimateUpdateResolution, 
        ENUM_VALUE_SPEC(CLIMATE_UPDATE_YEAR                                 , 0)
        ENUM_VALUE_SPEC(CLIMATE_UPDATE_MONTH                                , 1)
        ENUM_VALUE_SPEC(CLIMATE_UPDATE_WEEK                                 , 2)
        ENUM_VALUE_SPEC(CLIMATE_UPDATE_DAY                                  , 3)
        ENUM_VALUE_SPEC(CLIMATE_UPDATE_HOUR                                 , 4))

    // ENUM defs for Distribution types (e.g. AgeInitializationDistributionType)
    ENUM_DEFINE(DistributionType,
        ENUM_VALUE_SPEC(DISTRIBUTION_OFF                                    , 0)
        ENUM_VALUE_SPEC(DISTRIBUTION_SIMPLE                                 , 1)
        ENUM_VALUE_SPEC(DISTRIBUTION_COMPLEX                                , 2))

    // ENUM defs for IND_SAMPLING_TYPE
    ENUM_DEFINE(IndSamplingType, 
        ENUM_VALUE_SPEC(TRACK_ALL                                           , 0)
        ENUM_VALUE_SPEC(FIXED_SAMPLING                                      , 1)
        ENUM_VALUE_SPEC(ADAPTED_SAMPLING_BY_POPULATION_SIZE                 , 2)
        ENUM_VALUE_SPEC(ADAPTED_SAMPLING_BY_AGE_GROUP                       , 3)
        ENUM_VALUE_SPEC(ADAPTED_SAMPLING_BY_AGE_GROUP_AND_POP_SIZE          , 4)
        ENUM_VALUE_SPEC(ADAPTED_SAMPLING_BY_IMMUNE_STATE                    , 5))

    // ENUM defs for MIGRATION_PATTERN
    ENUM_DEFINE(MigrationPattern, 
        ENUM_VALUE_SPEC(RANDOM_WALK_DIFFUSION                               , 0)
        ENUM_VALUE_SPEC(SINGLE_ROUND_TRIPS                                  , 1)
        ENUM_VALUE_SPEC(WAYPOINTS_HOME                                      , 2))

    // ENUM defs for MIGRATION_STRUCTURE
    ENUM_DEFINE(MigrationStructure, 
        ENUM_VALUE_SPEC(NO_MIGRATION                                        , 0)
        ENUM_VALUE_SPEC(FIXED_RATE_MIGRATION                                , 1))

    ENUM_DEFINE(MigrationType,
        ENUM_VALUE_SPEC(NO_MIGRATION                                        , 0)
        ENUM_VALUE_SPEC(LOCAL_MIGRATION                                     , 1)
        ENUM_VALUE_SPEC(AIR_MIGRATION                                       , 2)
        ENUM_VALUE_SPEC(REGIONAL_MIGRATION                                  , 3)
        ENUM_VALUE_SPEC(SEA_MIGRATION                                       , 4)
        ENUM_VALUE_SPEC(FAMILY_MIGRATION                                    , 5)
        ENUM_VALUE_SPEC(INTERVENTION_MIGRATION                              , 6))

    // ENUM defs for Vector_Migration_Modifier_Equation
    ENUM_DEFINE(ModiferEquationType,
        ENUM_VALUE_SPEC(NOT_DEFINED                                         , 0)
        ENUM_VALUE_SPEC(LINEAR                                              , 1)
        ENUM_VALUE_SPEC(EXPONENTIAL                                         , 2))

    // ENUM defs for MORTALITY_TIME_COURSE
    ENUM_DEFINE(MortalityTimeCourse,
        ENUM_VALUE_SPEC(DAILY_MORTALITY                                     , 0)
        ENUM_VALUE_SPEC(MORTALITY_AFTER_INFECTIOUS                          , 1))

    // ENUM defs for PKPD_MODEL
    ENUM_DEFINE(PKPDModel, 
        ENUM_VALUE_SPEC(FIXED_DURATION_CONSTANT_EFFECT                      , 0)
        ENUM_VALUE_SPEC(CONCENTRATION_VERSUS_TIME                           , 1))

    // ENUM defs for RANDOM_TYPE
    ENUM_DEFINE(RandomType, 
        ENUM_VALUE_SPEC(USE_PSEUDO_DES                                      , 0)
        ENUM_VALUE_SPEC(USE_LINEAR_CONGRUENTIAL                             , 1)
        ENUM_VALUE_SPEC(USE_AES_COUNTER                                     , 2))

     // ENUM defs for Maternal_Protection_Type (Susceptibility variation with agent age)
     ENUM_DEFINE(MaternalProtectionType,
        ENUM_VALUE_SPEC(SIGMOID                                             , 0)
        ENUM_VALUE_SPEC(LINEAR                                              , 1))

     // ENUM defs for Susceptibility_Type
     ENUM_DEFINE(SusceptibilityType,
        ENUM_VALUE_SPEC(FRACTIONAL                                          , 0)
        ENUM_VALUE_SPEC(BINARY                                              , 1))

    // ENUM defs for Sim_Type
    ENUM_DEFINE(SimType, 
        ENUM_VALUE_SPEC(GENERIC_SIM                                         , 0)
        ENUM_VALUE_SPEC(VECTOR_SIM                                          , 1)
        ENUM_VALUE_SPEC(MALARIA_SIM                                         , 2)
        ENUM_VALUE_SPEC(ENVIRONMENTAL_SIM                                   , 3)
        ENUM_VALUE_SPEC(POLIO_SIM                                           , 4)
        ENUM_VALUE_SPEC(AIRBORNE_SIM                                        , 5) 
        ENUM_VALUE_SPEC(TBHIV_SIM                                           , 6)
        ENUM_VALUE_SPEC(STI_SIM                                             , 7)
        ENUM_VALUE_SPEC(HIV_SIM                                             , 8)
        ENUM_VALUE_SPEC(PY_SIM                                              , 9) 
        ENUM_VALUE_SPEC(TYPHOID_SIM                                         , 10)
        ENUM_VALUE_SPEC(DENGUE_SIM                                          , 11))

    // ENUM defs for VITAL_BIRTH_DEPENDENCE
    ENUM_DEFINE(VitalBirthDependence, 
        ENUM_VALUE_SPEC(FIXED_BIRTH_RATE                                    , 0)
        ENUM_VALUE_SPEC(POPULATION_DEP_RATE                                 , 1)
        ENUM_VALUE_SPEC(DEMOGRAPHIC_DEP_RATE                                , 2)
        ENUM_VALUE_SPEC(INDIVIDUAL_PREGNANCIES                              , 3)
        ENUM_VALUE_SPEC(INDIVIDUAL_PREGNANCIES_BY_AGE_AND_YEAR              , 4))

    // ENUM defs for VITAL_DEATH_DEPENDENCE
    ENUM_DEFINE(VitalDeathDependence,                                               // TODO: FIXED_DEATH_RATE (Makeham), Gompertz�Makeham, Lifetable, Heligman-Pollard, Siler (5-Component Competing Hazard), 
        ENUM_VALUE_SPEC(NONDISEASE_MORTALITY_BY_AGE_AND_GENDER              , 0)
        ENUM_VALUE_SPEC(NONDISEASE_MORTALITY_BY_YEAR_AND_AGE_FOR_EACH_GENDER, 1))

    ENUM_DEFINE(PolioSerotypes,
        ENUM_VALUE_SPEC(PV1                                                 , 0)
        ENUM_VALUE_SPEC(PV2                                                 , 1)
        ENUM_VALUE_SPEC(PV3                                                 , 2))

    ENUM_DEFINE(PolioVirusTypes,
        ENUM_VALUE_SPEC(WPV1                                                , 0)
        ENUM_VALUE_SPEC(WPV2                                                , 1)
        ENUM_VALUE_SPEC(WPV3                                                , 2)
        ENUM_VALUE_SPEC(VRPV1                                               , 3)
        ENUM_VALUE_SPEC(VRPV2                                               , 4)
        ENUM_VALUE_SPEC(VRPV3                                               , 5))

    ENUM_DEFINE(PolioVaccines,
        ENUM_VALUE_SPEC(TOPV                                                , 0)
        ENUM_VALUE_SPEC(BOPV                                                , 1)
        ENUM_VALUE_SPEC(MOPV1                                               , 2)
        ENUM_VALUE_SPEC(MOPV2                                               , 3)
        ENUM_VALUE_SPEC(MOPV3                                               , 4)
        ENUM_VALUE_SPEC(IPV                                                 , 5)) 

    // ENUM defs for RiskGroup
    ENUM_DEFINE(RiskGroup,
        ENUM_VALUE_SPEC(LOW                                                 , 0)
        ENUM_VALUE_SPEC(HIGH                                                , 1)
        ENUM_VALUE_SPEC(COUNT                                               , 2) )

    ENUM_DEFINE(TBFastProgressorType,
        ENUM_VALUE_SPEC(AGE                                                 , 0)
        ENUM_VALUE_SPEC(POVERTY                                             , 1)
        ENUM_VALUE_SPEC(POVERTY_SUSCEPTIBILITY_TO_INFECTION                 , 2)
        ENUM_VALUE_SPEC(POVERTY_AND_SUSCEPTIBILITY                          , 3))

    // ENUM defs for evolution_polio_clock_type
    ENUM_DEFINE(EvolutionPolioClockType, 
        ENUM_VALUE_SPEC(POLIO_EVOCLOCK_NONE                                 , 0)
        ENUM_VALUE_SPEC(POLIO_EVOCLOCK_LINEAR                               , 1)
        ENUM_VALUE_SPEC(POLIO_EVOCLOCK_IMMUNITY                             , 2)
        ENUM_VALUE_SPEC(POLIO_EVOCLOCK_REVERSION_AND_IMMUNITY               , 3)
        ENUM_VALUE_SPEC(POLIO_EVOCLOCK_REVERSION                            , 4)
        ENUM_VALUE_SPEC(POLIO_EVOCLOCK_POISSONSITES                         , 5))

    // ENUM defs for VDPV_virulence_model_type
    ENUM_DEFINE(VDPVVirulenceModelType, 
        ENUM_VALUE_SPEC(POLIO_VDPV_NONVIRULENT                              , 0)
        ENUM_VALUE_SPEC(POLIO_VDPV_PARALYSIS                                , 1)
        ENUM_VALUE_SPEC(POLIO_VDPV_PARALYSIS_AND_LOG_INFECTIVITY            , 2)
        ENUM_VALUE_SPEC(POLIO_VDPV_LOG_PARALYSIS_AND_LOG_INFECTIVITY        , 3))
}
