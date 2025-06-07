
#pragma once

#include "HIVSimpleDiagnostic.h"

namespace Kernel
{
    class IDMAPI HIVDrawBlood : public HIVSimpleDiagnostic
    {
        IMPLEMENT_DEFAULT_REFERENCE_COUNTING()
        DECLARE_FACTORY_REGISTERED(IndividualIVFactory, HIVDrawBlood, IDistributableIntervention)

    public: 
        HIVDrawBlood();
        HIVDrawBlood( const HIVDrawBlood& );

        virtual bool Configure(const Configuration* inputJson) override;

        // HIVSimpleDiagnostic
        virtual bool positiveTestResult() override;
        virtual void positiveTestDistribute() override;

#pragma warning( push )
#pragma warning( disable: 4251 ) // See IdmApi.h for details
        DECLARE_SERIALIZABLE(HIVDrawBlood);
#pragma warning( pop )
    };
}
