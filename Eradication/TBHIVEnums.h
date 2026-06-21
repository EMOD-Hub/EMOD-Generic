
#pragma once

#include "EnumSupport.h"

namespace Kernel
{
    #define IDM_ENUMSPEC_CD4_Stage                                                       \
        ENUM_VALUE_SPEC(NA                                                  , 0)         \
        ENUM_VALUE_SPEC(HIV_NEGATIVE                                        , 1)         \
        ENUM_VALUE_SPEC(CD4_UNDER_200                                       , 2)         \
        ENUM_VALUE_SPEC(CD4_200_TO_350                                      , 3)         \
        ENUM_VALUE_SPEC(CD4_350_TO_500                                      , 4)         \
        ENUM_VALUE_SPEC(CD4_ABOVE_500                                       , 5)         \
        ENUM_VALUE_SPEC(COUNT                                               , 6)
    ENUM_DECLARE(CD4_Stage, IDM_ENUMSPEC_CD4_Stage)


    #define IDM_ENUMSPEC_ARTStatusLocal                                                  \
        ENUM_VALUE_SPEC(NA                                                  , 0)         \
        ENUM_VALUE_SPEC(OFFART                                              , 1)         \
        ENUM_VALUE_SPEC(ONART                                               , 2)         \
        ENUM_VALUE_SPEC(COUNT                                               , 3)
    ENUM_DECLARE(ARTStatusLocal, IDM_ENUMSPEC_ARTStatusLocal)


    #define IDM_ENUMSPEC_TB_State                                                        \
        ENUM_VALUE_SPEC(NA                                                  , 0)         \
        ENUM_VALUE_SPEC(Negative                                            , 1)         \
        ENUM_VALUE_SPEC(Latent                                              , 2)         \
        ENUM_VALUE_SPEC(ActivePreSymptomatic                                , 3)         \
        ENUM_VALUE_SPEC(ActiveSmearPos                                      , 4)         \
        ENUM_VALUE_SPEC(ActiveSmearNeg                                      , 5)         \
        ENUM_VALUE_SPEC(ActiveExtraPulm                                     , 6)         \
        ENUM_VALUE_SPEC(COUNT                                               , 7)
    ENUM_DECLARE(TB_State, IDM_ENUMSPEC_TB_State)


    #define IDM_ENUMSPEC_MDR_State                                                       \
        ENUM_VALUE_SPEC(NA                                                  , 0)         \
        ENUM_VALUE_SPEC(Negative                                            , 1)         \
        ENUM_VALUE_SPEC(MDR                                                 , 2)         \
        ENUM_VALUE_SPEC(COUNT                                               , 3)
    ENUM_DECLARE(MDR_State, IDM_ENUMSPEC_MDR_State)


    #define IDM_ENUMSPEC_Report_Age                                                      \
        ENUM_VALUE_SPEC(LESS_1                                              , 0)         \
        ENUM_VALUE_SPEC(LESS_5                                              , 1)         \
        ENUM_VALUE_SPEC(LESS_10                                             , 2)         \
        ENUM_VALUE_SPEC(LESS_15                                             , 3)         \
        ENUM_VALUE_SPEC(LESS_20                                             , 4)         \
        ENUM_VALUE_SPEC(LESS_25                                             , 5)         \
        ENUM_VALUE_SPEC(LESS_30                                             , 6)         \
        ENUM_VALUE_SPEC(LESS_35                                             , 7)         \
        ENUM_VALUE_SPEC(LESS_40                                             , 8)         \
        ENUM_VALUE_SPEC(LESS_45                                             , 9)         \
        ENUM_VALUE_SPEC(LESS_50                                             , 10)        \
        ENUM_VALUE_SPEC(LESS_55                                             , 11)        \
        ENUM_VALUE_SPEC(LESS_60                                             , 12)        \
        ENUM_VALUE_SPEC(LESS_65                                             , 13)        \
        ENUM_VALUE_SPEC(LESS_70                                             , 14)        \
        ENUM_VALUE_SPEC(LESS_75                                             , 15)        \
        ENUM_VALUE_SPEC(LESS_80                                             , 16)        \
        ENUM_VALUE_SPEC(LESS_85                                             , 17)        \
        ENUM_VALUE_SPEC(LESS_90                                             , 18)        \
        ENUM_VALUE_SPEC(LESS_95                                             , 19)        \
        ENUM_VALUE_SPEC(GREAT_95                                            , 20)        \
        ENUM_VALUE_SPEC(COUNT                                               , 21)
    ENUM_DECLARE(Report_Age, IDM_ENUMSPEC_Report_Age)
}
