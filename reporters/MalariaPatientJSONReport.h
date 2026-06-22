
#pragma once

#include <vector>
#include <map>

#include "IReport.h"
#include "Configuration.h"
#include "ReportFactory.h"

namespace Kernel
{
    struct MalariaPatient
    {
        MalariaPatient(int id_=0, float age_=0.0f, float birthday_=0.0f);
        ~MalariaPatient();

        int id;
        float initial_age;
        float birthday;
        std::vector<float> true_asexual_density;
        std::vector<float> true_gametocyte_density;
        std::vector<float> asexual_parasite_density;
        std::vector<float> gametocyte_density;
        std::vector<float> infectiousness;
        std::vector<float> hemoglobin;
        std::vector<float> fever;
        std::vector<float> pos_fields_of_view;
        std::vector<float> gametocyte_pos_fields_of_view;

        int n_drug_treatments;
        std::vector<std::string> drug_treatments;

        virtual void JSerialize( json::Object& );

    protected:
        void SerializeChannel( std::string channel_name, std::vector<float> &channel_data, json::Object& root);
        void SerializeChannel( std::string channel_name, std::vector<std::string> &channel_data, json::Object& root );
    };

    class MalariaPatientJSONReport : public BaseReport
    {
        DECLARE_FACTORY_REGISTERED(ReportFactory, MalariaPatientJSONReport, IReport)
    public:
        static IReport* CreateReport();
        MalariaPatientJSONReport();
        virtual ~MalariaPatientJSONReport();

        virtual void Initialize( unsigned int nrmSize ) override; // public because Simulation::Populate will call this function, passing in NodeRankMap size

        virtual void BeginTimestep() override;
        virtual void LogNodeData( INodeContext * pNC ) override;
        virtual bool IsCollectingIndividualData( float currentTime, float dt ) const override;
        virtual void LogIndividualData( IIndividualHuman* individual ) override;
        virtual void EndTimestep( float currentTime, float dt ) override;

        // TODO: are we ever going to want to use this on multi-core?  Lot's of data output!
        virtual void Reduce() override;

        virtual std::string GetReportName() const override;
        virtual void Finalize() override;

    protected:
        std::string report_name;
        float simtime;
        int ntsteps;

        typedef std::map<int, MalariaPatient*> patient_map_t;
        patient_map_t patient_map;
    };
}
