/***************************************************************************************************

Copyright (c) 2018 Intellectual Ventures Property Holdings, LLC (IVPH) All rights reserved.

EMOD is licensed under the Creative Commons Attribution-Noncommercial-ShareAlike 4.0 License.
To view a copy of this license, visit https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode

***************************************************************************************************/

#include "stdafx.h"
#include "HIVDrawBlood.h"

#include "InterventionEnums.h"
#include "InterventionFactory.h"
#include "NodeEventContext.h"  // for INodeEventContext (ICampaignCostObserver)
#include "IHIVInterventionsContainer.h" // for time-date util function
#include "IIndividualHumanHIV.h"
#include "IIndividualHumanContext.h"
#include "ISusceptibilityHIV.h"

SETUP_LOGGING( "HIVDrawBlood" )

namespace Kernel
{
    BEGIN_QUERY_INTERFACE_DERIVED(HIVDrawBlood, HIVSimpleDiagnostic)
    END_QUERY_INTERFACE_DERIVED(HIVDrawBlood, HIVSimpleDiagnostic)

    IMPLEMENT_FACTORY_REGISTERED(HIVDrawBlood)

    HIVDrawBlood::HIVDrawBlood()
        : HIVSimpleDiagnostic()
    {
    }

    HIVDrawBlood::HIVDrawBlood( const HIVDrawBlood& master )
        : HIVSimpleDiagnostic( master )
    {
    }

    bool HIVDrawBlood::Configure(const Configuration* inputJson)
    {
        bool ret = HIVSimpleDiagnostic::Configure( inputJson );
        if( negative_diagnosis_event != EventTrigger::NoTrigger )
        {
            throw GeneralConfigurationException( __FILE__, __LINE__, __FUNCTION__, "HIVDrawBlood can't have a Negative_Diagnosis_Event." );
        }

        return ret;
    }

    bool HIVDrawBlood::positiveTestResult()
    {
        return true;
    }

    void HIVDrawBlood::positiveTestDistribute()
    {
        LOG_DEBUG_F( "HIVDrawBlood: %s\n", __FUNCTION__ );
        IIndividualHumanHIV* hiv_parent = parent->GetIndividualHIV();
        release_assert(hiv_parent);

        IHIVInterventionsContainer* ihiv_iv = parent->GetInterventionsContext()->GetContainerHIV();
        release_assert(ihiv_iv);

        float cd4count = hiv_parent->GetHIVSusceptibility()->GetCD4count();
        ihiv_iv->GetHIVMedicalHistory()->OnTestCD4(cd4count);

        expired = true;

        HIVSimpleDiagnostic::positiveTestDistribute();
    }

    REGISTER_SERIALIZABLE(HIVDrawBlood);

    void HIVDrawBlood::serialize(IArchive& ar, HIVDrawBlood* obj)
    {
        HIVSimpleDiagnostic::serialize( ar, obj );
        HIVDrawBlood& blood = *obj;
    }
}
