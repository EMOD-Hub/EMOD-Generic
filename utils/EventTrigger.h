/***************************************************************************************************

Copyright (c) 2016 Intellectual Ventures Property Holdings, LLC (IVPH) All rights reserved.

EMOD is licensed under the Creative Commons Attribution-Noncommercial-ShareAlike 4.0 License.
To view a copy of this license, visit https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode

***************************************************************************************************/

#pragma once

#include "EnumSupport.h"

namespace Kernel
{
    #define IDM_ENUMSPEC_EventType                                                       \
        ENUM_VALUE_SPEC(INDIVIDUAL                    ,   0)                             \
        ENUM_VALUE_SPEC(NODE                          ,   1)                             \
        ENUM_VALUE_SPEC(COORDINATOR                   ,   2)
    ENUM_DECLARE(EventType, IDM_ENUMSPEC_EventType)


    // An EventTrigger is the name of an event that occurs to an individual (node in future).
    // EventTriggers can be built-in or defined by the user.  They are broadcasted by
    // a INodeTriggeredInterventionConsumer (which is implemented by NodeEventContextHost)
    // to objects that have registered to be notified of these events.
    // This implementation of EventTrigger attempts to reduce string processing and
    // improve performance.

    // At the end of the regular event section is a 'bank' of 100 'GPIO' style events.
    // These are used where a suitable event does not exist, usually for campaigns. Your campaign
    // can specify arbitrary events and the adhoc_event EP4 scripts will map them to GP_EVENTs internally.
    // You'll only need to think about the GP_EVENT internal representation if you're debugging or using
    // logging for SFTs.

    // Or you can explicitly use GP_EVENTs. Don't do a mix of adhoc and explicit GP_EVENTs.

    #define IDM_ENUMSPEC_EventTrigger                                                    \
        ENUM_VALUE_SPEC(NoTrigger                     ,   0)                             \
        ENUM_VALUE_SPEC(Births                        ,   1)                             \
        ENUM_VALUE_SPEC(EveryUpdate                   ,   2)                             \
        ENUM_VALUE_SPEC(EveryTimeStep                 ,   3)                             \
        ENUM_VALUE_SPEC(NewInfection                  ,   4)                             \
        ENUM_VALUE_SPEC(TBActivation                  ,   5)                             \
        ENUM_VALUE_SPEC(NewClinicalCase               ,   6)                             \
        ENUM_VALUE_SPEC(NewSevereCase                 ,   7)                             \
        ENUM_VALUE_SPEC(DiseaseDeaths                 ,   8)                             \
        ENUM_VALUE_SPEC(OpportunisticInfectionDeath   ,   9)                             \
        ENUM_VALUE_SPEC(NonDiseaseDeaths              ,  10)                             \
        ENUM_VALUE_SPEC(TBActivationSmearPos          ,  11)                             \
        ENUM_VALUE_SPEC(TBActivationSmearNeg          ,  12)                             \
        ENUM_VALUE_SPEC(TBActivationExtrapulm         ,  13)                             \
        ENUM_VALUE_SPEC(TBActivationPostRelapse       ,  14)                             \
        ENUM_VALUE_SPEC(TBPendingRelapse              ,  15)                             \
        ENUM_VALUE_SPEC(TBActivationPresymptomatic    ,  16)                             \
        ENUM_VALUE_SPEC(TestPositiveOnSmear           ,  17)                             \
        ENUM_VALUE_SPEC(ProviderOrdersTBTest          ,  18)                             \
        ENUM_VALUE_SPEC(TBTestPositive                ,  19)                             \
        ENUM_VALUE_SPEC(TBTestNegative                ,  20)                             \
        ENUM_VALUE_SPEC(TBTestDefault                 ,  21)                             \
        ENUM_VALUE_SPEC(TBRestartHSB                  ,  22)                             \
        ENUM_VALUE_SPEC(TBMDRTestPositive             ,  23)                             \
        ENUM_VALUE_SPEC(TBMDRTestNegative             ,  24)                             \
        ENUM_VALUE_SPEC(TBMDRTestDefault              ,  25)                             \
        ENUM_VALUE_SPEC(TBFailedDrugRegimen           ,  26)                             \
        ENUM_VALUE_SPEC(TBRelapseAfterDrugRegimen     ,  27)                             \
        ENUM_VALUE_SPEC(TBStartDrugRegimen            ,  28)                             \
        ENUM_VALUE_SPEC(TBStopDrugRegimen             ,  29)                             \
        ENUM_VALUE_SPEC(PropertyChange                ,  30)                             \
        ENUM_VALUE_SPEC(STIDebut                      ,  31)                             \
        ENUM_VALUE_SPEC(StartedART                    ,  32)                             \
        ENUM_VALUE_SPEC(StoppedART                    ,  33)                             \
        ENUM_VALUE_SPEC(InterventionDisqualified      ,  34)                             \
        ENUM_VALUE_SPEC(HIVNewlyDiagnosed             ,  35)                             \
        ENUM_VALUE_SPEC(GaveBirth                     ,  36)                             \
        ENUM_VALUE_SPEC(Pregnant                      ,  37)                             \
        ENUM_VALUE_SPEC(Emigrating                    ,  38)                             \
        ENUM_VALUE_SPEC(Immigrating                   ,  39)                             \
        ENUM_VALUE_SPEC(HIVTestedNegative             ,  40)                             \
        ENUM_VALUE_SPEC(HIVTestedPositive             ,  41)                             \
        ENUM_VALUE_SPEC(HIVSymptomatic                ,  42)                             \
        ENUM_VALUE_SPEC(HIVPreARTToART                ,  43)                             \
        ENUM_VALUE_SPEC(HIVNonPreARTToART             ,  44)                             \
        ENUM_VALUE_SPEC(TwelveWeeksPregnant           ,  45)                             \
        ENUM_VALUE_SPEC(FourteenWeeksPregnant         ,  46)                             \
        ENUM_VALUE_SPEC(SixWeeksOld                   ,  47)                             \
        ENUM_VALUE_SPEC(EighteenMonthsOld             ,  48)                             \
        ENUM_VALUE_SPEC(STIPreEmigrating              ,  49)                             \
        ENUM_VALUE_SPEC(STIPostImmigrating            ,  50)                             \
        ENUM_VALUE_SPEC(STINewInfection               ,  51)                             \
        ENUM_VALUE_SPEC(NewExternalHIVInfection       ,  52)                             \
        ENUM_VALUE_SPEC(NodePropertyChange            ,  53)                             \
        ENUM_VALUE_SPEC(HappyBirthday                 ,  54)                             \
        ENUM_VALUE_SPEC(EnteredRelationship           ,  55)                             \
        ENUM_VALUE_SPEC(ExitedRelationship            ,  56)                             \
        ENUM_VALUE_SPEC(FirstCoitalAct                ,  57)                             \
        ENUM_VALUE_SPEC(NewlySymptomatic              ,  58)                             \
        ENUM_VALUE_SPEC(SymptomaticCleared            ,  59)                             \
        ENUM_VALUE_SPEC(ExposureComplete              ,  60)                             \
        ENUM_VALUE_SPEC(SheddingComplete              ,  61)                             \
        ENUM_VALUE_SPEC(PositiveResult                ,  62)                             \
        ENUM_VALUE_SPEC(NegativeResult                ,  63)                             \
        ENUM_VALUE_SPEC(Blackout                      ,  64)                             \
        ENUM_VALUE_SPEC(ReceivedTreatment             ,  65)                             \
        ENUM_VALUE_SPEC(SixMonthsOld                  ,  66)                             \
        ENUM_VALUE_SPEC(OneYearOld                    ,  67)                             \
        ENUM_VALUE_SPEC(MonteCarloDeaths              ,  68)                             \
        ENUM_VALUE_SPEC(InfectionCleared              ,  69)                             \
        ENUM_VALUE_SPEC(GP_EVENT_000                  , 100)                             \
        ENUM_VALUE_SPEC(GP_EVENT_001                  , 101)                             \
        ENUM_VALUE_SPEC(GP_EVENT_002                  , 102)                             \
        ENUM_VALUE_SPEC(GP_EVENT_003                  , 103)                             \
        ENUM_VALUE_SPEC(GP_EVENT_004                  , 104)                             \
        ENUM_VALUE_SPEC(GP_EVENT_005                  , 105)                             \
        ENUM_VALUE_SPEC(GP_EVENT_006                  , 106)                             \
        ENUM_VALUE_SPEC(GP_EVENT_007                  , 107)                             \
        ENUM_VALUE_SPEC(GP_EVENT_008                  , 108)                             \
        ENUM_VALUE_SPEC(GP_EVENT_009                  , 109)                             \
        ENUM_VALUE_SPEC(GP_EVENT_010                  , 110)                             \
        ENUM_VALUE_SPEC(GP_EVENT_011                  , 111)                             \
        ENUM_VALUE_SPEC(GP_EVENT_012                  , 112)                             \
        ENUM_VALUE_SPEC(GP_EVENT_013                  , 113)                             \
        ENUM_VALUE_SPEC(GP_EVENT_014                  , 114)                             \
        ENUM_VALUE_SPEC(GP_EVENT_015                  , 115)                             \
        ENUM_VALUE_SPEC(GP_EVENT_016                  , 116)                             \
        ENUM_VALUE_SPEC(GP_EVENT_017                  , 117)                             \
        ENUM_VALUE_SPEC(GP_EVENT_018                  , 118)                             \
        ENUM_VALUE_SPEC(GP_EVENT_019                  , 119)                             \
        ENUM_VALUE_SPEC(GP_EVENT_020                  , 120)                             \
        ENUM_VALUE_SPEC(GP_EVENT_021                  , 121)                             \
        ENUM_VALUE_SPEC(GP_EVENT_022                  , 122)                             \
        ENUM_VALUE_SPEC(GP_EVENT_023                  , 123)                             \
        ENUM_VALUE_SPEC(GP_EVENT_024                  , 124)                             \
        ENUM_VALUE_SPEC(GP_EVENT_025                  , 125)                             \
        ENUM_VALUE_SPEC(GP_EVENT_026                  , 126)                             \
        ENUM_VALUE_SPEC(GP_EVENT_027                  , 127)                             \
        ENUM_VALUE_SPEC(GP_EVENT_028                  , 128)                             \
        ENUM_VALUE_SPEC(GP_EVENT_029                  , 129)                             \
        ENUM_VALUE_SPEC(GP_EVENT_030                  , 130)                             \
        ENUM_VALUE_SPEC(GP_EVENT_031                  , 131)                             \
        ENUM_VALUE_SPEC(GP_EVENT_032                  , 132)                             \
        ENUM_VALUE_SPEC(GP_EVENT_033                  , 133)                             \
        ENUM_VALUE_SPEC(GP_EVENT_034                  , 134)                             \
        ENUM_VALUE_SPEC(GP_EVENT_035                  , 135)                             \
        ENUM_VALUE_SPEC(GP_EVENT_036                  , 136)                             \
        ENUM_VALUE_SPEC(GP_EVENT_037                  , 137)                             \
        ENUM_VALUE_SPEC(GP_EVENT_038                  , 138)                             \
        ENUM_VALUE_SPEC(GP_EVENT_039                  , 139)                             \
        ENUM_VALUE_SPEC(GP_EVENT_040                  , 140)                             \
        ENUM_VALUE_SPEC(GP_EVENT_041                  , 141)                             \
        ENUM_VALUE_SPEC(GP_EVENT_042                  , 142)                             \
        ENUM_VALUE_SPEC(GP_EVENT_043                  , 143)                             \
        ENUM_VALUE_SPEC(GP_EVENT_044                  , 144)                             \
        ENUM_VALUE_SPEC(GP_EVENT_045                  , 145)                             \
        ENUM_VALUE_SPEC(GP_EVENT_046                  , 146)                             \
        ENUM_VALUE_SPEC(GP_EVENT_047                  , 147)                             \
        ENUM_VALUE_SPEC(GP_EVENT_048                  , 148)                             \
        ENUM_VALUE_SPEC(GP_EVENT_049                  , 149)                             \
        ENUM_VALUE_SPEC(GP_EVENT_050                  , 150)                             \
        ENUM_VALUE_SPEC(GP_EVENT_051                  , 151)                             \
        ENUM_VALUE_SPEC(GP_EVENT_052                  , 152)                             \
        ENUM_VALUE_SPEC(GP_EVENT_053                  , 153)                             \
        ENUM_VALUE_SPEC(GP_EVENT_054                  , 154)                             \
        ENUM_VALUE_SPEC(GP_EVENT_055                  , 155)                             \
        ENUM_VALUE_SPEC(GP_EVENT_056                  , 156)                             \
        ENUM_VALUE_SPEC(GP_EVENT_057                  , 157)                             \
        ENUM_VALUE_SPEC(GP_EVENT_058                  , 158)                             \
        ENUM_VALUE_SPEC(GP_EVENT_059                  , 159)                             \
        ENUM_VALUE_SPEC(GP_EVENT_060                  , 160)                             \
        ENUM_VALUE_SPEC(GP_EVENT_061                  , 161)                             \
        ENUM_VALUE_SPEC(GP_EVENT_062                  , 162)                             \
        ENUM_VALUE_SPEC(GP_EVENT_063                  , 163)                             \
        ENUM_VALUE_SPEC(GP_EVENT_064                  , 164)                             \
        ENUM_VALUE_SPEC(GP_EVENT_065                  , 165)                             \
        ENUM_VALUE_SPEC(GP_EVENT_066                  , 166)                             \
        ENUM_VALUE_SPEC(GP_EVENT_067                  , 167)                             \
        ENUM_VALUE_SPEC(GP_EVENT_068                  , 168)                             \
        ENUM_VALUE_SPEC(GP_EVENT_069                  , 169)                             \
        ENUM_VALUE_SPEC(GP_EVENT_070                  , 170)                             \
        ENUM_VALUE_SPEC(GP_EVENT_071                  , 171)                             \
        ENUM_VALUE_SPEC(GP_EVENT_072                  , 172)                             \
        ENUM_VALUE_SPEC(GP_EVENT_073                  , 173)                             \
        ENUM_VALUE_SPEC(GP_EVENT_074                  , 174)                             \
        ENUM_VALUE_SPEC(GP_EVENT_075                  , 175)                             \
        ENUM_VALUE_SPEC(GP_EVENT_076                  , 176)                             \
        ENUM_VALUE_SPEC(GP_EVENT_077                  , 177)                             \
        ENUM_VALUE_SPEC(GP_EVENT_078                  , 178)                             \
        ENUM_VALUE_SPEC(GP_EVENT_079                  , 179)                             \
        ENUM_VALUE_SPEC(GP_EVENT_080                  , 180)                             \
        ENUM_VALUE_SPEC(GP_EVENT_081                  , 181)                             \
        ENUM_VALUE_SPEC(GP_EVENT_082                  , 182)                             \
        ENUM_VALUE_SPEC(GP_EVENT_083                  , 183)                             \
        ENUM_VALUE_SPEC(GP_EVENT_084                  , 184)                             \
        ENUM_VALUE_SPEC(GP_EVENT_085                  , 185)                             \
        ENUM_VALUE_SPEC(GP_EVENT_086                  , 186)                             \
        ENUM_VALUE_SPEC(GP_EVENT_087                  , 187)                             \
        ENUM_VALUE_SPEC(GP_EVENT_088                  , 188)                             \
        ENUM_VALUE_SPEC(GP_EVENT_089                  , 189)                             \
        ENUM_VALUE_SPEC(GP_EVENT_090                  , 190)                             \
        ENUM_VALUE_SPEC(GP_EVENT_091                  , 191)                             \
        ENUM_VALUE_SPEC(GP_EVENT_092                  , 192)                             \
        ENUM_VALUE_SPEC(GP_EVENT_093                  , 193)                             \
        ENUM_VALUE_SPEC(GP_EVENT_094                  , 194)                             \
        ENUM_VALUE_SPEC(GP_EVENT_095                  , 195)                             \
        ENUM_VALUE_SPEC(GP_EVENT_096                  , 196)                             \
        ENUM_VALUE_SPEC(GP_EVENT_097                  , 197)                             \
        ENUM_VALUE_SPEC(GP_EVENT_098                  , 198)                             \
        ENUM_VALUE_SPEC(GP_EVENT_099                  , 199)                             \
        ENUM_VALUE_SPEC(NUM_EVENT_TRIGGERS            , 200)
    ENUM_DECLARE(EventTrigger, IDM_ENUMSPEC_EventTrigger)

}
