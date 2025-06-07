
#pragma once

#include "BinnedReport.h"

namespace Kernel {
    class BinnedReportPy : public BinnedReport
    {
        public:
            static IReport* CreateReport();
            virtual ~BinnedReportPy();

            virtual void LogIndividualData( IIndividualHuman * individual );
            virtual void EndTimestep( float currentTime, float dt );

            virtual void postProcessAccumulatedData();

        protected:
            BinnedReportPy();

            virtual void initChannelBins();
            void clearChannelsBins();

            // channels specific to this particular report-type
    };
}
