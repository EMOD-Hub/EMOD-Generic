
#pragma once

#include "BaseTextReport.h"
#include "ISimulation.h"
#include "RelationshipReporting.h"

namespace Kernel
{
    struct  IIndividualHumanSTI ;
    struct  IIndividualHumanEventContext ;

    class StiRelationshipStartReporter : public BaseTextReport
    {
    public:
        static IReport* Create(ISimulation* simulation);

        // IReport
        virtual bool Configure( const Configuration* inputJson ) override;
        virtual void Initialize( unsigned int nrmSize ) override;
        virtual void BeginTimestep() override;
        virtual void EndTimestep( float currentTime, float dt ) override;

    protected:
        StiRelationshipStartReporter(ISimulation* simulation);
        virtual ~StiRelationshipStartReporter();

        // BaseTextReport
        virtual std::string GetHeader() const override;

        void onNewNode(INodeContext* node);
        void onNewRelationship(IRelationship* relationship);

        virtual void ClearData();
        virtual void CollectOtherData( unsigned int relationshipID,
                                       IIndividualHumanSTI* pPartnerA,
                                       IIndividualHumanSTI* pPartnerB ) {} ;
        virtual std::string GetOtherData( unsigned int relationshipID ) { return ""; };

        ISimulation* simulation;
        std::vector<RelationshipStartInfo> report_data;
        jsonConfigurable::tDynamicStringSet properties_to_report;
        std::vector<IPKey> keys_to_report;
    };
}
