/***************************************************************************************************

Copyright (c) 2018 Intellectual Ventures Property Holdings, LLC (IVPH) All rights reserved.

EMOD is licensed under the Creative Commons Attribution-Noncommercial-ShareAlike 4.0 License.
To view a copy of this license, visit https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode

***************************************************************************************************/

#include "stdafx.h"

#include "ARTDropout.h"

#include "IIndividualHumanContext.h"
#include "Debug.h"                  // for IIndividualHumanContext, IIndividualHumanInterventionsContext
#include "IHIVInterventionsContainer.h"  // for IHIVDrugEffectsApply methods

SETUP_LOGGING( "ARTDropout" )

namespace Kernel
{
    IMPLEMENT_FACTORY_REGISTERED(ARTDropout)

    ARTDropout::ARTDropout()
        : BaseIntervention()
    {
        initSimTypes( 2, "HIV_SIM", "TBHIV_SIM" );
    }

    ARTDropout::~ARTDropout()
    {
    }

    bool ARTDropout::Configure(const Configuration* inputJson)
    {
        initConfigTypeMap("Cost_To_Consumer", &cost_per_unit, DRUG_Cost_To_Consumer_DESC_TEXT, 0, 99999);
        // Skip GenericDrug (base class) Configure (to avoid picking up all those parameters). Connection with GenericDrug is fairly loose.
        return BaseIntervention::Configure( inputJson );
    }

    bool ARTDropout::Distribute(IIndividualHumanInterventionsContext* context, ICampaignCostObserver* pCCO)
    {
        bool distributed = BaseIntervention::Distribute( context, pCCO );
        if( distributed )
        {
            LOG_DEBUG_F( "ARTDropout distributed to individual %d.\n", context->GetParent()->GetSuid().data );

            IHIVInterventionsContainer* ihiv_iv = context->GetContainerHIV();
            release_assert(ihiv_iv);
            ihiv_iv->GetHIVDrugEffectApply()->GoOffART();
        }
        return distributed;
    }

    void ARTDropout::Update( float dt )
    {
        Expired();
    }

    REGISTER_SERIALIZABLE(ARTDropout);

    void ARTDropout::serialize(IArchive& ar, ARTDropout* obj)
    {
        BaseIntervention::serialize( ar, obj );
        ARTDropout& art = *obj;
    }
}
