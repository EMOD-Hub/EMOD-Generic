/***************************************************************************************************

Copyright (c) 2018 Intellectual Ventures Property Holdings, LLC (IVPH) All rights reserved.

EMOD is licensed under the Creative Commons Attribution-Noncommercial-ShareAlike 4.0 License.
To view a copy of this license, visit https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode

***************************************************************************************************/

#pragma once

#include "BaseTextReport.h"

namespace Kernel
{
    class ReportStrainTracking : public BaseTextReport
    {
        DECLARE_FACTORY_REGISTERED(ReportFactory, ReportStrainTracking, IReport)

        public:
            ReportStrainTracking();
            ReportStrainTracking(const ReportStrainTracking&);
            ~ReportStrainTracking();

            bool Configure(const Configuration* inputJson) override;
            void LogNodeData(INodeContext* node) override;
            void EndTimestep(float currentTime, float dt) override;

            std::string GetHeader() const override;

        private:
            bool m_all_done;
            float m_time_start;
            float m_time_end;
    };
}
