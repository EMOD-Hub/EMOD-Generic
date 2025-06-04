
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
