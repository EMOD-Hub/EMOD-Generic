
#include "stdafx.h"
#include "ReportEventCounter.h"
#include "Environment.h"

SETUP_LOGGING( "ReportEventCounter" )

namespace Kernel
{
    IMPLEMENT_FACTORY_REGISTERED(ReportEventCounter)

    ReportEventCounter::ReportEventCounter()
        : BaseEventReport( "ReportEventCounter" )
        , channelDataMap()
        , unitsMap()
    {
        // ------------------------------------------------------------------------------------------------
        // --- Since this report will be listening for events, it needs to increment its reference count
        // --- so that it is 1.  Other objects will be AddRef'ing and Release'ing this report/observer
        // --- so it needs to start with a refcount of 1.
        // ------------------------------------------------------------------------------------------------
        AddRef();
    }

    ReportEventCounter::~ReportEventCounter()
    {
    }

    bool ReportEventCounter::Configure( const Configuration * inputJson )
    {
        bool ret = BaseEventReport::Configure( inputJson );

        if( ret )
        {
            const std::vector< EventTrigger::Enum >& trigger_list = GetEventTriggerList();
            for( auto trigger : trigger_list )
            {
                auto event_string = EventTrigger::pairs::lookup_key( trigger );
                unitsMap[ event_string ] = "" ;
                channelDataMap.AddChannel( event_string );
            }
        }
        return ret;
    }

    void ReportEventCounter::BeginTimestep()
    {
        channelDataMap.IncreaseChannelLength( 1 );
    }

    void ReportEventCounter::Reduce()
    {
        channelDataMap.Reduce();
        BaseEventReport::Reduce();
    }

    void ReportEventCounter::Finalize()
    {
        std::string output_fn = GetBaseOutputFilename() + ".json" ;
        channelDataMap.WriteOutput( output_fn, unitsMap );
        BaseEventReport::Finalize();
    }

    bool ReportEventCounter::notifyOnEvent( IIndividualHumanEventContext *context, 
                                            const EventTrigger::Enum& trigger )
    {
        if( HaveUnregisteredAllEvents() )
        {
            return false ;
        }

        channelDataMap.Accumulate( EventTrigger::pairs::lookup_key( trigger ), 1.0 );

        return true ;
    }

}
