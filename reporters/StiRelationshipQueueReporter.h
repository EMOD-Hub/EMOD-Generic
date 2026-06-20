
#pragma once

#include "BaseTextReport.h"
#include "ReportFactory.h"
#include "IIndividualHuman.h"

namespace Kernel
{
    class INodeSTI;
    struct ISociety;

    class StiRelationshipQueueReporter : public BaseTextReport
    {
        DECLARE_FACTORY_REGISTERED(ReportFactory, StiRelationshipQueueReporter, IReport)

    public:
        StiRelationshipQueueReporter();
        StiRelationshipQueueReporter(const StiRelationshipQueueReporter&);
        virtual ~StiRelationshipQueueReporter();

        // IReport
        virtual void UpdateEventRegistration( float currentTime, 
                                              float dt, 
                                              std::vector<INodeEventContext*>& rNodeEventContextList,
                                              ISimulationEventContext* pSimEventContext ) override;
        virtual void LogNodeData( INodeContext* pNC ) override;

    protected:
        // BaseTextReport
        virtual std::string GetHeader() const override;

    private:
        std::string CreateLengthsString( const map<int, vector<int>>& rQueueLengthMap );
        std::string CreateBinString( const std::vector<int>& rQueue, int* pTotalCount );

        bool m_FirstTime ;
    };
}
