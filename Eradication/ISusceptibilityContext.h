
#pragma once
#include "ISerializable.h"

namespace Kernel
{
    class  ISusceptibilityHIV;
    class  ISusceptibilityTB;
    struct ISusceptibilityVector;
    class  ISusceptibilityDengue;
    struct IMalariaSusceptibility;
    class  ISusceptibilityPolio;

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
        virtual ISusceptibilityDengue*    GetSusceptibilityDengue()  = 0;
        virtual IMalariaSusceptibility*   GetSusceptibilityMalaria() = 0;
        virtual ISusceptibilityPolio*     GetSusceptibilityPolio()   = 0;
    };
}
