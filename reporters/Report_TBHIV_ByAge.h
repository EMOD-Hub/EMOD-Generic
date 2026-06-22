
#pragma once

#include "BaseTextReportEvents.h"
#include "TBHIVEnums.h"
#include "Properties.h"
#include "Configuration.h"
#include "ReportFactory.h"

namespace Kernel
{
    class Report_TBHIV_ByAge : public BaseTextReportEvents
    {
        DECLARE_FACTORY_REGISTERED(ReportFactory, Report_TBHIV_ByAge, IReport)

    public:
        Report_TBHIV_ByAge();
        virtual ~Report_TBHIV_ByAge();

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
        float min_age_yrs;
        float max_age_yrs;

        Report_Age::Enum ComputeAgeBin(float age);

        float Population[Report_Age::Enum::COUNT];
        float DiseaseDeaths[Report_Age::Enum::COUNT];
        float NonDiseaseDeaths[Report_Age::Enum::COUNT];
        float OnART[Report_Age::Enum::COUNT];     
        float New_Activations[Report_Age::Enum::COUNT];
        float Active_Prevalence[Report_Age::Enum::COUNT];
        float Active_Sx_Prevalence[Report_Age::Enum::COUNT];
        float Active_PreSymptomatic[Report_Age::Enum::COUNT];
        float Active_Smear_Positive[Report_Age::Enum::COUNT];
        float Latent[Report_Age::Enum::COUNT];
        float HIVstatus[Report_Age::Enum::COUNT];
        float Births;
        float HIVDeaths[Report_Age::Enum::COUNT];
        float TBStartTreatment[Report_Age::Enum::COUNT];
        
        float TBFailedTreatment[Report_Age::Enum::COUNT];
        float PotentialNotifications[Report_Age::Enum::COUNT];
        float Retreatments[Report_Age::Enum::COUNT];
        float PrevalentMDR[Report_Age::Enum::COUNT];
        float IncidentMDR[Report_Age::Enum::COUNT];
        float NewInfections[Report_Age::Enum::COUNT];
        float HIVPosNewActivations[Report_Age::Enum::COUNT];
        float HIVPosTBDeaths[Report_Age::Enum::COUNT];
        float HIVPosNotifications[Report_Age::Enum::COUNT];
        float TBTests[Report_Age::Enum::COUNT];
        float HIVDeathsActiveTB[Report_Age::Enum::COUNT];

        vector <EventTrigger::Enum> Additional_Event_Names;
        float DynamicEvents[100][Report_Age::Enum::COUNT];
    };
}
