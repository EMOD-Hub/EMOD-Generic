
#pragma once

#include "BaseTextReportEvents.h"
#include "TBHIVEnums.h"
#include "ReportFactory.h"

namespace Kernel
{
    class Report_TBHIV_Basic : public BaseTextReportEvents
    {
        DECLARE_FACTORY_REGISTERED(ReportFactory, Report_TBHIV_Basic, IReport)

    public:
        Report_TBHIV_Basic();
        virtual ~Report_TBHIV_Basic();

        // BaseEventReport
        virtual bool Configure( const Configuration* ) override;
        virtual void Initialize( unsigned int nrmSize ) override;

        virtual void UpdateEventRegistration( float currentTime, 
                                              float dt, 
                                              std::vector<INodeEventContext*>& rNodeEventContextList,
                                              ISimulationEventContext* pSimEventContext ) override;

        virtual std::string GetHeader() const override;
        virtual bool IsCollectingIndividualData( float currentTime, float dt ) const override;
        virtual void LogIndividualData( Kernel::IIndividualHuman* individual ) override;
        virtual void LogNodeData( Kernel::INodeContext * pNC ) override;
        virtual bool notifyOnEvent( IIndividualHumanEventContext *context, 
                                    const EventTrigger::Enum& trigger ) override;

    private:
        const float report_tbhiv_half_period;
        float next_report_time;
        bool doReport;
        float startYear;
        float stopYear;
        bool is_collecting_data;

        CD4_Stage::Enum ComputeCD4Stage(IIndividualHumanEventContext *context);
        TB_State::Enum ComputeTBState(IIndividualHumanEventContext *context);
        MDR_State::Enum ComputeMDRState(IIndividualHumanEventContext* context);
        ARTStatusLocal::Enum ComputeARTStatus(IIndividualHumanEventContext* context);

        float Population[TB_State::Enum::COUNT][CD4_Stage::Enum::COUNT][MDR_State::COUNT][ARTStatusLocal::COUNT];
        float DiseaseDeaths[TB_State::Enum::COUNT][CD4_Stage::Enum::COUNT][MDR_State::COUNT][ARTStatusLocal::COUNT];
        float NonDiseaseDeaths;
        float ARTDropouts[CD4_Stage::Enum::COUNT];
        float ART_Initiations[CD4_Stage::Enum::COUNT];
        float New_Activations[CD4_Stage::Enum::COUNT][MDR_State::COUNT][ARTStatusLocal::COUNT];
        float New_TBDiagnoses[CD4_Stage::Enum::COUNT][MDR_State::COUNT][ARTStatusLocal::COUNT];  
    };
}
