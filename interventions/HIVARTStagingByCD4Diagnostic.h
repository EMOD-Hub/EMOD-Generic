
#pragma once

#include "InterpolatedValueMap.h"
#include "HIVARTStagingAbstract.h"

namespace Kernel
{
    class IDMAPI HIVARTStagingByCD4Diagnostic : public HIVARTStagingAbstract
    {
        DECLARE_FACTORY_REGISTERED(IndividualIVFactory, HIVARTStagingByCD4Diagnostic, IDistributableIntervention)

    public: 
        HIVARTStagingByCD4Diagnostic();
        HIVARTStagingByCD4Diagnostic( const HIVARTStagingByCD4Diagnostic& );

        virtual bool Configure( const Configuration * inputJson ) override;

    protected:
        virtual bool positiveTestResult( IIndividualHumanHIV *pHIV, 
                                         float year, 
                                         float CD4count, 
                                         bool hasActiveTB, 
                                         bool isPregnant ) override;

#pragma warning( push )
#pragma warning( disable: 4251 ) // See IdmApi.h for details
        InterpolatedValueMap threshold;
        InterpolatedValueMap ifActiveTB;
        InterpolatedValueMap ifPregnant;

        DECLARE_SERIALIZABLE(HIVARTStagingByCD4Diagnostic);
#pragma warning( pop )
    };
}
