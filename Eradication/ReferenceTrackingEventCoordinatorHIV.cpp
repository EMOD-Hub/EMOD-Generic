/***************************************************************************************************

Copyright (c) 2018 Intellectual Ventures Property Holdings, LLC (IVPH) All rights reserved.

EMOD is licensed under the Creative Commons Attribution-Noncommercial-ShareAlike 4.0 License.
To view a copy of this license, visit https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode

***************************************************************************************************/

#include "stdafx.h"

#include "ReferenceTrackingEventCoordinatorHIV.h"
#include "IIndividualHuman.h"
#include "IIndividualHumanContext.h"
#include "IIndividualHumanHIV.h"
#include "IHIVInterventionsContainer.h"

SETUP_LOGGING( "ReferenceTrackingEventCoordinatorHIV" )

namespace Kernel
{
    IMPLEMENT_FACTORY_REGISTERED(ReferenceTrackingEventCoordinatorHIV)
    IMPL_QUERY_INTERFACE2(ReferenceTrackingEventCoordinatorHIV, IEventCoordinator, IConfigurable)

    ReferenceTrackingEventCoordinatorHIV::ReferenceTrackingEventCoordinatorHIV()
        : ReferenceTrackingEventCoordinator()
        , target_disease_state(TargetDiseaseStateType::Everyone)
    {
    }

    bool ReferenceTrackingEventCoordinatorHIV::Configure(const Configuration* inputJson)
    {
        initConfig("Target_Disease_State", target_disease_state, inputJson, MetadataDescriptor::Enum("Target_Disease_State", RTEC_HIV_Target_Disease_State_DESC_TEXT, MDD_ENUM_ARGS(TargetDiseaseStateType))) ;

        auto ret = ReferenceTrackingEventCoordinator::Configure( inputJson );
        return ret;
    }

    bool ReferenceTrackingEventCoordinatorHIV::qualifiesDemographically( IIndividualHumanEventContext* pIndividual )
    {
        bool qualifies = ReferenceTrackingEventCoordinator::qualifiesDemographically( pIndividual );
        if( qualifies )
        {
            IIndividualHumanHIV* p_hiv_individual = pIndividual->GetIndividual()->GetIndividualContext()->GetIndividualHIV();
            release_assert(p_hiv_individual);
            IHIVInterventionsContainer* p_hiv_container = pIndividual->GetInterventionsContext()->GetContainerHIV();
            release_assert(p_hiv_container);
            IHIVMedicalHistory* p_med_history = p_hiv_container->GetHIVMedicalHistory();

            switch( target_disease_state )
            {
                case TargetDiseaseStateType::Everyone:
                    qualifies = true;
                    break;
                case TargetDiseaseStateType::HIV_Positive:
                    qualifies = p_hiv_individual->HasHIV();
                    break;
                case TargetDiseaseStateType::HIV_Negative:
                    qualifies = !p_hiv_individual->HasHIV();
                    break;
                case TargetDiseaseStateType::Tested_Positive:
                    qualifies = p_med_history->EverTestedHIVPositive();
                    break;
                case TargetDiseaseStateType::Tested_Negative:
                    qualifies = p_med_history->EverTested() && !p_med_history->EverTestedHIVPositive();
                    break;
                case TargetDiseaseStateType::Not_Tested_Or_Tested_Negative:
                    qualifies = !p_med_history->EverTested() || !p_med_history->EverTestedHIVPositive();
                    break;
                default:
                    throw BadEnumInSwitchStatementException( __FILE__, __LINE__, __FUNCTION__, "target_disease_state", target_disease_state, TargetDiseaseStateType::pairs::lookup_key( target_disease_state ) );
            }
        }
        return qualifies;
    }
}

