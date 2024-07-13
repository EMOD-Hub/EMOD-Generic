/***************************************************************************************************

Copyright (c) 2018 Intellectual Ventures Property Holdings, LLC (IVPH) All rights reserved.

EMOD is licensed under the Creative Commons Attribution-Noncommercial-ShareAlike 4.0 License.
To view a copy of this license, visit https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode

***************************************************************************************************/

#include "stdafx.h"
#include "CD4Diagnostic.h"
#include "InterventionEnums.h"
#include "NodeEventContext.h"  // for INodeEventContext (ICampaignCostObserver)
#include "IIndividualHumanHIV.h"
#include "IIndividualHumanContext.h"
#include "ISusceptibilityHIV.h"

SETUP_LOGGING( "CD4Diagnostic" )

namespace Kernel
{
    BEGIN_QUERY_INTERFACE_DERIVED(CD4Diagnostic, AgeDiagnostic)
    END_QUERY_INTERFACE_DERIVED(CD4Diagnostic, AgeDiagnostic)

    IMPLEMENT_FACTORY_REGISTERED(CD4Diagnostic)

    CD4Diagnostic::CD4Diagnostic()
        : AgeDiagnostic( HIV_CD4_Diagnostic_Low_DESC_TEXT,
                         HIV_CD4_Diagnostic_High_DESC_TEXT,
                         HIV_CD4_Diagnostic_Event_Name_DESC_TEXT )
    {
        initSimTypes( 2, "HIV_SIM","TBHIV_SIM" );
    }

    CD4Diagnostic::CD4Diagnostic( const CD4Diagnostic& master )
        : AgeDiagnostic( master )
    { }

    CD4Diagnostic::~CD4Diagnostic()
    {
        LOG_DEBUG("Destructing CD4 Diagnostic \n");
    }

    void CD4Diagnostic::ConfigureRangeThresholds( const Configuration* inputJson )
    {
        initConfigComplexCollectionType("CD4_Thresholds", &range_thresholds, HIV_CD4_Thresholds_DESC_TEXT);
    }

    float CD4Diagnostic::GetValue() const
    {
        IIndividualHumanHIV* hiv_ind = parent->GetIndividualHIV();
        release_assert(hiv_ind);

        auto cd4count = hiv_ind->GetHIVSusceptibility()->GetCD4count();
        LOG_DEBUG_F( "cd4count measured at %f. %d thresholds configured.\n", cd4count, range_thresholds.Size() );

        return cd4count;
    }

    REGISTER_SERIALIZABLE(CD4Diagnostic);

    void CD4Diagnostic::serialize(IArchive& ar, CD4Diagnostic* obj)
    {
        AgeDiagnostic::serialize( ar, obj );
        CD4Diagnostic& cd4 = *obj;
    }
}
