
#pragma once

#include <list>
#include <map>
#include <vector>
#include <string>
#include <fstream>

#include "SpatialReport.h"

namespace Kernel {

    class SpatialReportPy : public SpatialReport
    {
        GET_SCHEMA_STATIC_WRAPPER(SpatialReportPy)

        public:
            static IReport* CreateReport();
            virtual ~SpatialReportPy() { }

            virtual void LogIndividualData( IIndividualHuman * individual );
            virtual void LogNodeData( INodeContext * pNC );

        protected:
            SpatialReportPy();

            virtual void postProcessAccumulatedData();

            virtual void populateChannelInfos(tChanInfoMap &channel_infos);

            // counters for LogIndividualData stuff 

        private:
    };
}
