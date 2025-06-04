
#include "stdafx.h"

#include "BroadcasterImpl.h"
#include "EventTrigger.h"
#include "Log.h"

SETUP_LOGGING( "BroadcasterImpl" )

// moved after SETUP_LOGGING to make Linux Debug happy with LOG_DEBUG statements
#include "BroadcasterImplTemplate.h"

namespace Kernel
{
    template class BroadcasterImpl< ICoordinatorEventObserver,
                                    IEventCoordinatorEventContext >;

    template class BroadcasterImpl< INodeEventObserver,
                                    INodeEventContext >;

    template class BroadcasterImpl< IIndividualEventObserver,
                                    IIndividualHumanEventContext
                                     >;
}


