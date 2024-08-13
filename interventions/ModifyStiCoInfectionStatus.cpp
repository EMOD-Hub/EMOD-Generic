/***************************************************************************************************

Copyright (c) 2018 Intellectual Ventures Property Holdings, LLC (IVPH) All rights reserved.

EMOD is licensed under the Creative Commons Attribution-Noncommercial-ShareAlike 4.0 License.
To view a copy of this license, visit https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode

***************************************************************************************************/

#include "stdafx.h"

#include "ModifyStiCoInfectionStatus.h"
#include "Exceptions.h"
#include "ISTIInterventionsContainer.h"

SETUP_LOGGING( "ModifyStiCoInfectionStatus" )

namespace Kernel
{
    IMPLEMENT_FACTORY_REGISTERED(ModifyStiCoInfectionStatus)

    ModifyStiCoInfectionStatus::ModifyStiCoInfectionStatus()
        : BaseIntervention()
        , set_flag_to( false )
    {
        initSimTypes( 2, "STI_SIM", "HIV_SIM" );
        initConfigTypeMap( "New_STI_CoInfection_Status", &set_flag_to, MSCIS_New_STI_Co_Status_DESC_TEXT, false );
    }

    bool ModifyStiCoInfectionStatus::Configure( const Configuration* inputJson )
    {
        // -----------------------------------------------------------
        // --- Do not use the BaseIntervention::Configure()
        // --- because we don't want to support those attributes here
        // -----------------------------------------------------------
        return JsonConfigurable::Configure( inputJson );
    }

    bool ModifyStiCoInfectionStatus::Distribute( IIndividualHumanInterventionsContext* context, ICampaignCostObserver* pCCO )
    {
        bool success = true;
        ISTIInterventionsContainer* interventions_container = context->GetContainerSTI();
        release_assert(interventions_container);

        LOG_DEBUG( "Setting/clearing HIV individual's (non-HIV) STI Co-Infection status (flag).\n" );
        if( set_flag_to == true )
        {
            interventions_container->SpreadStiCoInfection();
        }
        else
        {
            interventions_container->CureStiCoInfection();
        }

        return success;
    }

    void ModifyStiCoInfectionStatus::Update( float dt )
    {
        expired = true;
    }

    REGISTER_SERIALIZABLE(ModifyStiCoInfectionStatus);

    void ModifyStiCoInfectionStatus::serialize(IArchive& ar, ModifyStiCoInfectionStatus* obj)
    {
        BaseIntervention::serialize( ar, obj );
        ModifyStiCoInfectionStatus& modify = *obj;
        ar.labelElement("set_flag_to") & modify.set_flag_to;
    }
}
