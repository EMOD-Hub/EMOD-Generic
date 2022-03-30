/***************************************************************************************************

Copyright (c) 2018 Intellectual Ventures Property Holdings, LLC (IVPH) All rights reserved.

EMOD is licensed under the Creative Commons Attribution-Noncommercial-ShareAlike 4.0 License.
To view a copy of this license, visit https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode

***************************************************************************************************/

#pragma once

#include <vector>

#include "BaseReportEventRecorder.h"

namespace Kernel
{
    class ICoordinatorEventContext;
    class EventTriggerCoordinatorFactory;

    class ReportEventRecorderCoordinator : public BaseReportEventRecorder< ICoordinatorEventBroadcaster,
                                                                           ICoordinatorEventObserver,
                                                                           IEventCoordinatorEventContext,
                                                                           EventTriggerCoordinator,
                                                                           EventTriggerCoordinatorFactory >
    {
    public:
        GET_SCHEMA_STATIC_WRAPPER( ReportEventRecorderCoordinator )

    public:
        static IReport* CreateReport();

    public:
        ReportEventRecorderCoordinator();
        ReportEventRecorderCoordinator( const std::string& rReportName );
        virtual ~ReportEventRecorderCoordinator();

        // ------------
        // --- IReport
        // ------------
        virtual void UpdateEventRegistration( float currentTime,
                                              float dt,
                                              std::vector<INodeEventContext*>& rNodeEventContextList,
                                              ISimulationEventContext* pSimEventContext ) override;

        virtual std::string GetHeader() const override;

    protected:
        virtual std::string GetOtherData( IEventCoordinatorEventContext *context, const EventTriggerCoordinator& trigger ) override;
        virtual float GetTime( IEventCoordinatorEventContext* pEntity ) const override;
    };
}
