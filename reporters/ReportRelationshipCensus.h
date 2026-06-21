
#pragma once

#include "BaseTextReportEvents.h"
#include "ReportFactory.h"

namespace Kernel
{
    class ReportRelationshipCensus : public BaseTextReportEvents
    {
    public:
        DECLARE_FACTORY_REGISTERED(ReportFactory, ReportRelationshipCensus, IReport)

        ReportRelationshipCensus();
        virtual ~ReportRelationshipCensus();

        // BaseEventReport
        virtual bool Configure( const Configuration* ) override;
        virtual bool Validate(const ISimulationContext*) override;

        virtual void UpdateEventRegistration( float currentTime, 
                                              float dt, 
                                              std::vector<INodeEventContext*>& rNodeEventContextList,
                                              ISimulationEventContext* pSimEventContext ) override;

        virtual std::string GetHeader() const override;
        virtual bool notifyOnEvent( IIndividualHumanEventContext *context, 
                                    const EventTrigger::Enum &trigger ) override;
        virtual void LogIndividualData( IIndividualHuman* individual ) override;
        virtual bool IsCollectingIndividualData( float currentTime, float dt ) const override;
        virtual void EndTimestep( float currentTime, float dt ) override;
        virtual void Reduce() override;
        virtual std::string GetReportName() const override;

    private:
        std::string m_ReportName;
        float m_StartYear;
        float m_EndYear;
        float m_ReportingIntervalYears;
        float m_IntervalTimerDays;
        bool m_IsCollectingData;
        bool m_FirstDataCollection;
        std::vector<std::string> m_RelationshipTypes;
    };
}
