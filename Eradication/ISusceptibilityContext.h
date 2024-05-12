/***************************************************************************************************

Copyright (c) 2018 Intellectual Ventures Property Holdings, LLC (IVPH) All rights reserved.

EMOD is licensed under the Creative Commons Attribution-Noncommercial-ShareAlike 4.0 License.
To view a copy of this license, visit https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode

***************************************************************************************************/

#pragma once
#include "ISerializable.h"

namespace Kernel
{
    class  ISusceptibilityHIV;
    class  ISusceptibilityTB;
    struct ISusceptibilityVector;

    struct ISusceptibilityContext : ISerializable
    {
        virtual float getModAcquire() const = 0;
        virtual float getModTransmit() const = 0;
        virtual float getModMortality() const = 0;
        virtual float getModRisk() const = 0;

        virtual bool  HasMaternalImmunity() const = 0;

        virtual void  InitNewInfection() = 0;
        virtual bool  IsImmune() const = 0;

        virtual ISusceptibilityHIV*       GetSusceptibilityHIV()     = 0;
        virtual ISusceptibilityTB*        GetSusceptibilityTB()      = 0;
        virtual ISusceptibilityVector*    GetSusceptibilityVector()  = 0;
    };
}
