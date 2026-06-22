
#pragma once

#include <map>

#include "BaseEventReport.h"
#include "ChannelDataMap.h"
#include "ReportFactory.h"

namespace Kernel
{
    class ReportEventCounter : public BaseEventReport
    {
        DECLARE_FACTORY_REGISTERED(ReportFactory, ReportEventCounter, IReport)

    public:
        ReportEventCounter();
        virtual ~ReportEventCounter();

        // BaseEventReport
        virtual bool Configure( const Configuration* ) override;
        virtual void BeginTimestep() override;
        virtual void Reduce() override;
        virtual void Finalize() override;
        virtual bool notifyOnEvent( IIndividualHumanEventContext *context, 
                                    const EventTrigger::Enum& trigger ) override;
    private:
        ChannelDataMap channelDataMap ;
        std::map<std::string, std::string> unitsMap;
    };
}
