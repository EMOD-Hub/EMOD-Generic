
#pragma once

#include "PropertyReport.h"

namespace Kernel {

    class PropertyReportTyphoid : public PropertyReport
    {
        GET_SCHEMA_STATIC_WRAPPER( PropertyReportTyphoid )
    public:
        static IReport* CreateReport();
        virtual ~PropertyReportTyphoid() { }

        virtual bool Configure( const Configuration* inputJson ) override;
        virtual bool Validate( const ISimulationContext* parent_sim ) override;
        virtual void UpdateEventRegistration( float currentTime,
                                              float dt,
                                              std::vector<INodeEventContext*>& rNodeEventContextList,
                                              ISimulationEventContext* pSimEventContext ) override;
        virtual void BeginTimestep() override;
        virtual bool IsCollectingIndividualData( float currentTime, float dt ) const override;
        virtual void LogIndividualData( Kernel::IIndividualHuman* individual ) override;
        virtual void LogNodeData( Kernel::INodeContext * pNC ) override;

    protected:
        PropertyReportTyphoid();
        float start_year; // Year to start collecting data
        float stop_year;  // Year to stop  collecting data 
        bool is_collecting_data;
    };
}