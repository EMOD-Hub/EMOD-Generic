
#pragma once

#include "ReportVectorStats.h"

namespace Kernel
{
    class ReportVectorStatsMalaria : public ReportVectorStats
    {
    public:
        ReportVectorStatsMalaria();
        virtual ~ReportVectorStatsMalaria();

        // BaseEventReport
        virtual bool Configure( const Configuration* ) override;

        virtual std::string GetHeader() const override;

    protected:
        virtual void ResetOtherCounters() override;
        virtual void CollectOtherData( IVectorPopulation* pIVPR ) override;
        virtual void WriteOtherData() override;

    private:
        std::vector<ReportUtilitiesMalaria::GenomeMarkerColumn> genome_marker_columns;
    };
}
