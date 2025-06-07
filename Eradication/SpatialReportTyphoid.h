
#include "stdafx.h"
#pragma once

#include <list>
#include <map>
#include <vector>
#include <string>
#include <fstream>

#include "SpatialReport.h"

namespace Kernel {

    class SpatialReportTyphoid : public SpatialReport
    {
        GET_SCHEMA_STATIC_WRAPPER(SpatialReportTyphoid)

        public:
            static IReport* CreateReport();
            virtual ~SpatialReportTyphoid() { }

            virtual void LogIndividualData( IIndividualHuman * individual );
            virtual void LogNodeData( INodeContext * pNC );

        protected:
            SpatialReportTyphoid();

            virtual void postProcessAccumulatedData();

            virtual void populateChannelInfos(tChanInfoMap &channel_infos);

            // counters for LogIndividualData stuff 

        private: 
    };
}
