/***************************************************************************************************

Copyright (c) 2018 Intellectual Ventures Property Holdings, LLC (IVPH) All rights reserved.

EMOD is licensed under the Creative Commons Attribution-Noncommercial-ShareAlike 4.0 License.
To view a copy of this license, visit https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode

***************************************************************************************************/

#include "stdafx.h"
#ifdef ENABLE_TYPHOID

#include "Sugar.h"
#include "Debug.h"
#include "Environment.h"

#include "IIndividualHumanContext.h"
#include "InterventionFactory.h"
#include "TyphoidInterventionsContainer.h"
#include "IndividualTyphoid.h"

SETUP_LOGGING( "TyphoidInterventionsContainer" )

namespace Kernel
{

    BEGIN_QUERY_INTERFACE_DERIVED(TyphoidInterventionsContainer, InterventionsContainer)
        HANDLE_INTERFACE(ITyphoidVaccineEffectsApply)
    END_QUERY_INTERFACE_DERIVED(TyphoidInterventionsContainer, InterventionsContainer)

    TyphoidInterventionsContainer::TyphoidInterventionsContainer()
        : current_shedding_attenuation_contact( 1.0f )
        , current_dose_attenuation_contact( 1.0f )
        , current_exposures_attenuation_contact( 1.0f )
        , current_shedding_attenuation_environment( 1.0f )
        , current_dose_attenuation_environment( 1.0f )
        , current_exposures_attenuation_environment( 1.0f )
    {
    }

    TyphoidInterventionsContainer::~TyphoidInterventionsContainer()
    {
    }

    ITyphoidVaccineEffectsApply* TyphoidInterventionsContainer::GetContainerTyphoid()
    {
        return static_cast<ITyphoidVaccineEffectsApply*>(this);
    }

    void TyphoidInterventionsContainer::Update(float dt)
    {
        InterventionsContainer::Update(dt);
    }

    void TyphoidInterventionsContainer::ApplyReducedSheddingEffect( float rate )
    {
        
        LOG_VALID_F( "%s: Set current_shedding_attenuation_contact to %f for individual %d.\n", __FUNCTION__, rate, parent->GetSuid().data );
        current_shedding_attenuation_contact = rate; 
    }

    void TyphoidInterventionsContainer::ApplyReducedDoseEffect( float rate )
    {
        LOG_VALID_F( "%s: Set current_dose_attenuation_contact to %f for individual %d.\n", __FUNCTION__, rate, parent->GetSuid().data );
        current_dose_attenuation_contact = rate;
    }

    void TyphoidInterventionsContainer::ApplyReducedNumberExposuresEffect( float rate )
    {
        
        LOG_VALID_F( "%s: Set current_exposures_attenuation_contact to %f for individual %d.\n", __FUNCTION__, rate, parent->GetSuid().data );
        current_exposures_attenuation_contact = rate;
    }

    float TyphoidInterventionsContainer::GetContactDepositAttenuation() const
    {
        return current_shedding_attenuation_contact;
    }

    float TyphoidInterventionsContainer::GetEnviroDepositAttenuation() const
    {
        return current_shedding_attenuation_environment;
    }

    float TyphoidInterventionsContainer::GetContactDoseAttenuation() const
    {
        return current_dose_attenuation_contact;
    }

    float TyphoidInterventionsContainer::GetEnviroDoseAttenuation() const
    {
        return current_dose_attenuation_environment;
    }

    float TyphoidInterventionsContainer::GetContactExposuresAttenuation() const
    {
        return current_exposures_attenuation_contact;
    }

    float TyphoidInterventionsContainer::GetEnviroExposuresAttenuation() const
    {
        return current_exposures_attenuation_environment;
    }

    // Infection Clearance
    void TyphoidInterventionsContainer::ApplyClearance( ProbabilityNumber clearanceRate /*not used yet*/ )
    {
        IIndividualHumanTyphoid* p_iht = parent->GetIndividualTyphoid();
        release_assert(p_iht);
        p_iht->ForceClearInfection();
    }
}

#endif // ENABLE_TYPHOID
