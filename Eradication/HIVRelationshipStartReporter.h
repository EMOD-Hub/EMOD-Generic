
#pragma once

#include <map>
#include "StiRelationshipStartReporter.h"

namespace Kernel
{
    struct IHIVMedicalHistory ;
    struct IIndividualHumanHIV ;

    class HIVRelationshipStartReporter : public StiRelationshipStartReporter
    {
    public:
        static IReport* Create(ISimulation* simulation);

    protected:
        HIVRelationshipStartReporter(ISimulation* simulation);
        virtual ~HIVRelationshipStartReporter();

        // BaseTextReport
        virtual std::string GetHeader() const ;

        virtual void ClearData();
        virtual void CollectOtherData( unsigned int relationshipID,
                                       IIndividualHumanSTI* pPartnerA,
                                       IIndividualHumanSTI* pPartnerB ) ;
        virtual std::string GetOtherData( unsigned int relationshipID );

        IIndividualHumanHIV* GetIndividualHumanHIV( IIndividualHumanSTI* pPartner );
        IHIVMedicalHistory* GetMedicalHistory( IIndividualHumanHIV* pPartner );
        int GetHivTestedPositive( IIndividualHumanHIV* pPartner );
        std::string GetReceivedTestResultForHIV( IIndividualHumanHIV* pPartner );
    private:
        std::map<unsigned int, std::string> hiv_report_data ;
    };
}
