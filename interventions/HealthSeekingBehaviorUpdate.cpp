/***************************************************************************************************

Copyright (c) 2018 Intellectual Ventures Property Holdings, LLC (IVPH) All rights reserved.

EMOD is licensed under the Creative Commons Attribution-Noncommercial-ShareAlike 4.0 License.
To view a copy of this license, visit https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode

***************************************************************************************************/

#include "stdafx.h"
#include "HealthSeekingBehaviorUpdate.h"
#include "Common.h"             // for INFINITE_TIME
#include "TBInterventionsContainer.h"  // for IHealthSeekingBehaviorEffectsUpdate
#include "IIndividualHumanContext.h"

SETUP_LOGGING( "HealthSeekingBehaviorUpdate" )

namespace Kernel
{
    IMPLEMENT_FACTORY_REGISTERED(HealthSeekingBehaviorUpdate)

    HealthSeekingBehaviorUpdate::HealthSeekingBehaviorUpdate()
        : new_probability_of_seeking(0.0f)
        , ihsbuea(nullptr)
    {
        initSimTypes( 1, "TBHIV_SIM" );
    }

    bool HealthSeekingBehaviorUpdate::Configure(const Configuration* inputJson)
    {
        initConfigTypeMap("New_Tendency", &new_probability_of_seeking, HSB_Update_New_Tendency_DESC_TEXT );
        return BaseIntervention::Configure( inputJson );
    }

    bool HealthSeekingBehaviorUpdate::Distribute(IIndividualHumanInterventionsContext* context, ICampaignCostObserver* const pCCO)
    {
        ihsbuea = context->GetContainerTB();
        return BaseIntervention::Distribute( context, pCCO );
    }

    void HealthSeekingBehaviorUpdate::Update( float dt )
    {
        if( !BaseIntervention::UpdateIndividualsInterventionStatus() ) return;

        release_assert(ihsbuea);
        ihsbuea->UpdateHealthSeekingBehaviors( new_probability_of_seeking );
        LOG_DEBUG_F( "Update the HSB tendency with value %f\n", new_probability_of_seeking );
        expired = true;
    }

    void HealthSeekingBehaviorUpdate::SetContextTo(IIndividualHumanContext* context)
    {
        BaseIntervention::SetContextTo( context );
        ihsbuea = context->GetInterventionsContext()->GetContainerTB();
    }
}