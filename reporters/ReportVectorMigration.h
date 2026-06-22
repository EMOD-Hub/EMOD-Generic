
#pragma once

#include "BaseTextReport.h"
#include "IVectorMigrationReporting.h"
#include "ReportFactory.h"

namespace Kernel
{
    class ReportVectorMigration : public BaseTextReport, public IVectorMigrationReporting
    {
        DECLARE_FACTORY_REGISTERED(ReportFactory, ReportVectorMigration, IReport)

    public:
        ReportVectorMigration();
        virtual ~ReportVectorMigration();

        virtual std::string GetHeader() const override;
        virtual void LogVectorMigration( ISimulationContext* pSim,
                                         float currentTime, 
                                         const suids::suid& nodeSuid, 
                                         IVectorCohort* pivc ) override;
    };
}
