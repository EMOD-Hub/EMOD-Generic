/***************************************************************************************************

Copyright (c) 2018 Intellectual Ventures Property Holdings, LLC (IVPH) All rights reserved.

EMOD is licensed under the Creative Commons Attribution-Noncommercial-ShareAlike 4.0 License.
To view a copy of this license, visit https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode

***************************************************************************************************/

#include "stdafx.h"
#include "PMTCT.h"
#include "Common.h"
#include "IHIVInterventionsContainer.h"
#include "IIndividualHumanContext.h"

SETUP_LOGGING( "PMTCT" )

namespace Kernel
{
    BEGIN_QUERY_INTERFACE_BODY(PMTCT)
        HANDLE_INTERFACE(IConfigurable)
        HANDLE_INTERFACE(IDistributableIntervention)
        HANDLE_ISUPPORTS_VIA(IDistributableIntervention)
    END_QUERY_INTERFACE_BODY(PMTCT)

    IMPLEMENT_FACTORY_REGISTERED(PMTCT)

    PMTCT::PMTCT()
        : BaseIntervention(  )
        , timer( DAYSPERWEEK * WEEKS_FOR_GESTATION )
        , ivc( nullptr )
        , efficacy( 0.0 )
    {
        initSimTypes( 1, "HIV_SIM" );
    }

    PMTCT::PMTCT( const PMTCT& master )
        : BaseIntervention( master )
        , timer( DAYSPERWEEK * WEEKS_FOR_GESTATION ) // best not to give this out on day 0 of pregnancy to avoid "perfect timing" questions
        , ivc( nullptr )
    {
        efficacy = master.efficacy;
    }

    PMTCT::~PMTCT()
    {
    }

    bool PMTCT::Configure( const Configuration* inputJson )
    {
        initConfigTypeMap("Efficacy", &efficacy, PMTCT_Efficacy_DESC_TEXT, 0.0, 1.0, 0.5 );
        return BaseIntervention::Configure( inputJson );
    }

    bool PMTCT::Distribute(IIndividualHumanInterventionsContext* context, ICampaignCostObserver* const pEC)
    {
        bool success = BaseIntervention::Distribute( context, pEC );

        if( success )
        {
            // Apply effect (update PMTCT) on distribute. On expiration, eliminate effect 
            LOG_DEBUG("Distributing Prevention of Mother-To-Child Transmission drug.\n");
            ivc = context->GetContainerHIV();
            release_assert( ivc );
            ivc->ApplyProbMaternalTransmissionModifier( efficacy );
        }
        return success;
    }

    void PMTCT::SetContextTo(IIndividualHumanContext* context)
    {
        BaseIntervention::SetContextTo( context );

        ivc = context->GetInterventionsContext()->GetContainerHIV();
    }

    void PMTCT::Update( float dt )
    {
        if( !BaseIntervention::UpdateIndividualsInterventionStatus() )
        {
            return;
        }

        if( timer > dt )
        {
            timer -= dt;
        }
        else
        {
            if( ivc )
            {
                LOG_DEBUG("PMTCT has expired (after 9 months). Restore modifier to 0.0.\n" );
                ivc->ApplyProbMaternalTransmissionModifier( 0.0 );
            }
            expired = true;
        }
    }

    REGISTER_SERIALIZABLE(PMTCT);

    void PMTCT::serialize(IArchive& ar, PMTCT* obj)
    {
        BaseIntervention::serialize( ar, obj );
        PMTCT& pmtct = *obj;

        ar.labelElement("timer"   ) & pmtct.timer;
        ar.labelElement("efficacy") & pmtct.efficacy;
    }
}
