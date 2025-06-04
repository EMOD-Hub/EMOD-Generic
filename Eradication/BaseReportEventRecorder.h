
#pragma once

#include <vector>

#include "BaseTextReportEvents.h"
#include "BroadcasterObserver.h"

namespace Kernel
{
    struct IdmDateTime;

    template<class Broadcaster, class Observer, class Entity>
    class BaseReportEventRecorder : public BaseTextReportEventsTemplate<Broadcaster, Observer, Entity>
    {
    public:

    protected:
        BaseReportEventRecorder( const std::string& rReportName );
        virtual ~BaseReportEventRecorder();

        // -----------------------------
        // --- BaseTextReportEvents
        // -----------------------------
        virtual bool Configure( const Configuration* inputJson ) override;
        virtual std::string GetHeader() const;

        // IObserver
        bool notifyOnEvent( Entity *pEntity, const EventTrigger::Enum& trigger );

    protected:
        virtual std::string GetOtherData( Entity *pEntity, const EventTrigger::Enum& trigger );
        virtual std::string GetTimeHeader() const;

        virtual float GetTime( Entity* pEntity ) const = 0;

        bool ignore_events_in_list;
        std::vector<EventTrigger::Enum> event_trigger_list;
    };
}
