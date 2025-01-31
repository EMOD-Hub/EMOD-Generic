/***************************************************************************************************

Copyright (c) 2018 Intellectual Ventures Property Holdings, LLC (IVPH) All rights reserved.

EMOD is licensed under the Creative Commons Attribution-Noncommercial-ShareAlike 4.0 License.
To view a copy of this license, visit https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode

***************************************************************************************************/

#pragma once

#include <string>
#include <list>
#include <vector>

#include "Configuration.h"
#include "Configure.h"
#include "InterventionEnums.h"
#include "InterventionFactory.h"
#include "Interventions.h"
#include "EventTrigger.h"

namespace Kernel
{
    class IDMAPI BroadcastEvent :  public BaseIntervention
    {
        IMPLEMENT_DEFAULT_REFERENCE_COUNTING()
        DECLARE_FACTORY_REGISTERED(IndividualIVFactory, BroadcastEvent, IDistributableIntervention)

    public:
        BroadcastEvent();
        BroadcastEvent( const BroadcastEvent& master );
        virtual ~BroadcastEvent() {  }
        bool Configure( const Configuration* pConfig ) override;

        virtual void Update(float dt) override;

    protected:

#pragma warning( push )
#pragma warning( disable: 4251 ) // See IdmApi.h for details
        EventTrigger::Enum broadcast_event;

        DECLARE_SERIALIZABLE(BroadcastEvent);
#pragma warning( pop )
    };
}
