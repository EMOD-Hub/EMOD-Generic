
#pragma once

#include <string>

#include "Configuration.h"
#include "InterventionFactory.h"
#include "Interventions.h"

namespace Kernel
{
    class BroadcastNodeEvent : public BaseNodeIntervention
    {
        IMPLEMENT_DEFAULT_REFERENCE_COUNTING()
        DECLARE_FACTORY_REGISTERED( NodeIVFactory, BroadcastNodeEvent, INodeDistributableIntervention )

    public:
        BroadcastNodeEvent();
        BroadcastNodeEvent( const BroadcastNodeEvent& master );
        virtual ~BroadcastNodeEvent();

        virtual bool Configure( const Configuration* pConfig ) override;

        virtual void Update( float dt ) override;

    protected:
        EventTrigger::Enum m_EventToBroadcast;

        // TODO - can't do until BaseNodeIntervention is serializable
        //DECLARE_SERIALIZABLE(BroadcastNodeEvent);
    };
}
