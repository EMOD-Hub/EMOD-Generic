
#pragma once

#include <map>

#include "BaseTextReport.h"
#include "SpatialReportMalaria.h"
#include "ReportFactory.h"

namespace Kernel
{
    class SpatialReportMalariaFiltered : public SpatialReportMalaria
    {
        DECLARE_FACTORY_REGISTERED(ReportFactory, SpatialReportMalariaFiltered, IReport)

    public:
        SpatialReportMalariaFiltered();
        virtual ~SpatialReportMalariaFiltered();

        // SpatialReportMalaria
        virtual bool Configure( const Configuration* ) override;
        virtual void Initialize( unsigned int nrmSize ) override;

        virtual void UpdateEventRegistration( float currentTime, 
                                              float dt, 
                                              std::vector<INodeEventContext*>& rNodeEventContextList,
                                              ISimulationEventContext* pSimEventContext ) override;
        virtual void BeginTimestep() override;
        virtual void LogIndividualData( IIndividualHuman* individual ) override;
        virtual bool IsCollectingIndividualData( float currentTime, float dt ) const override;
        virtual void LogNodeData( INodeContext* pNC ) override;
        virtual void EndTimestep( float currentTime, float dt ) override;
        virtual void Reduce() override;

    private:
        // SpatialReport methods
        virtual void WriteHeaderParameters( std::ofstream* file ) override;
        virtual void postProcessAccumulatedData() override;
        virtual void WriteData( ChannelDataMap& rChannelDataMap ) override;
        virtual void ClearData() override;

        bool IsValidNode( uint32_t externalNodeID ) const;

        std::map<uint32_t,bool> m_NodesToInclude;
        float m_StartDay;
        float m_EndDay;
        float m_ReportingInterval;
        float m_IntervalTimer;
        float m_TimeElapsed;
        float m_Dt;
        bool m_IsValidDay;
        ChannelDataMap m_FilteredChannelDataMap;
    };
}
