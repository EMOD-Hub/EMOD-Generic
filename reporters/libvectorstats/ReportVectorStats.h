/***************************************************************************************************

Copyright (c) 2018 Intellectual Ventures Property Holdings, LLC (IVPH) All rights reserved.

EMOD is licensed under the Creative Commons Attribution-Noncommercial-ShareAlike 4.0 License.
To view a copy of this license, visit https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode

***************************************************************************************************/

#pragma once

#include "BaseTextReportEvents.h"
#include "IVectorMigrationReporting.h"
#include "ReportUtilitiesMalaria.h"

namespace Kernel
{
    struct IVectorPopulationReporting;

    class ReportVectorStats : public BaseTextReportEvents, public IVectorMigrationReporting
    {
    public:
        ReportVectorStats();
        virtual ~ReportVectorStats();

        // BaseEventReport
        virtual bool Configure( const Configuration* ) override;
        //virtual void BeginTimestep() override;

        virtual void UpdateEventRegistration( float currentTime, 
                                              float dt, 
                                              std::vector<INodeEventContext*>& rNodeEventContextList,
                                              ISimulationEventContext* pSimEventContext ) override;

        virtual std::string GetHeader() const override;
        virtual void LogNodeData( Kernel::INodeContext * pNC ) override;
        virtual bool notifyOnEvent( IIndividualHumanEventContext *context, 
                                    const EventTrigger& trigger ) override;

        // IVectorMigrationReporting
        virtual void LogVectorMigration( ISimulationContext* pSim, 
                                         float currentTime, 
                                         const suids::suid& nodeSuid, 
                                         IVectorCohort* pivc ) override;
    protected:
        ReportVectorStats( const std::string& rReportName );
        virtual void ResetOtherCounters() {};
        virtual void CollectOtherData( IVectorPopulationReporting* pIVPR ) {};
        virtual void WriteOtherData() {};

    private:
        std::map<uint32_t, std::map<std::string, int>> migration_count_local;
        std::map<uint32_t, std::map<std::string, int>> migration_count_regional;
        std::vector<std::string> species_list ;
        bool stratify_by_species;
    };
}
