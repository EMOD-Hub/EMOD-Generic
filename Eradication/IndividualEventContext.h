/***************************************************************************************************

Copyright (c) 2018 Intellectual Ventures Property Holdings, LLC (IVPH) All rights reserved.

EMOD is licensed under the Creative Commons Attribution-Noncommercial-ShareAlike 4.0 License.
To view a copy of this license, visit https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode

***************************************************************************************************/

#pragma once

#include <string>
#include <list>
#include <vector>
#include "ISupports.h"

#include "Common.h" // for HumanStateChange enum at least
#include "suids.hpp"
#include "PropertiesString.h"

namespace Kernel
{
    struct IIndividualHumanInterventionsContext;
    struct IIndividualHuman;
    struct INodeEventContext;
    class IPKeyValueContainer;

    struct IDMAPI IIndividualHumanEventContext : public ISupports 
    {
        // for example, individual attributes...
        virtual bool   IsPregnant()                    const = 0;

        virtual float  GetAge()              const = 0;
        virtual int    GetGender()           const = 0;
        virtual float  GetMonteCarloWeight() const = 0;
        virtual bool   IsPossibleMother()    const = 0;
        virtual bool   IsInfected()          const = 0;
        virtual bool   IsSymptomatic()       const = 0;
        virtual float  GetInfectiousness()   const = 0;

        virtual HumanStateChange GetStateChange(void) const = 0;
        virtual void   Die( HumanStateChange ) = 0; // for testing, quick results
        virtual suids::suid GetSuid() const = 0;

        // useful sub contexts
        virtual IIndividualHumanInterventionsContext*  GetInterventionsContext() const  = 0;
        virtual IIndividualHuman*                      GetIndividual()                  = 0;
        virtual INodeEventContext*                     GetNodeEventContext()            = 0;
        virtual IPKeyValueContainer*                   GetProperties()                  = 0;

        // TODO: add more methods here to extend programmatic capability of event coordinators w.r.t. to individual state!
    };
}
