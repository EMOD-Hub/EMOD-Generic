/***************************************************************************************************

Copyright (c) 2018 Intellectual Ventures Property Holdings, LLC (IVPH) All rights reserved.

EMOD is licensed under the Creative Commons Attribution-Noncommercial-ShareAlike 4.0 License.
To view a copy of this license, visit https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode

***************************************************************************************************/

#pragma once

#include "stdafx.h"
#include "NodeMalariaEventContext.h"
#include "InterventionsContainer.h"
#include "MalariaContexts.h" // for IMalariaHumanInfectable
#include "IInfectable.h"     // for IInfectionAcquirable
#include "IIndividualHumanContext.h"
#include "RANDOM.h"

SETUP_LOGGING( "NodeMalariaEventContext" )

namespace Kernel
{
    BEGIN_QUERY_INTERFACE_DERIVED(NodeMalariaEventContextHost, NodeVectorEventContextHost)
        HANDLE_INTERFACE(ISporozoiteChallengeConsumer)
    END_QUERY_INTERFACE_DERIVED(NodeMalariaEventContextHost, NodeVectorEventContextHost)

    NodeMalariaEventContextHost::NodeMalariaEventContextHost(Node* _node) : NodeVectorEventContextHost(_node)
    { 
    }

    NodeMalariaEventContextHost::~NodeMalariaEventContextHost()
    {
    }

    void NodeMalariaEventContextHost::ChallengeWithSporozoites(int n_sporozoites, float coverage, tAgeBitingFunction risk )
    {
        INodeEventContext::individual_visit_function_t sporozoite_challenge_func = 
            [this, n_sporozoites, coverage, risk](IIndividualHumanEventContext *ihec)
        {
            float relative_risk=1.0f;
            if(risk != nullptr)
            {
                relative_risk = risk(ihec->GetAge());
            }

            // Allow the bite challenge rate to be modified the individual factors that are usually handled in the Expose logic
            // GetRelativeBitingRate is already handled by the flexibility to pass in an age-risk function
            // Susceptibility::getModAcquire is only used in GENERIC and VECTOR immunity but not MALARIA
            // InterventionsContainer::GetInterventionReducedAcquire can be modified by a Vaccine intervention used in MALARIA_SIM
            // Adding route aware IVs required querying based on route of infection; malaria sims only use CONTACT route (default)
            relative_risk *= ihec->GetInterventionsContext()->GetParent()->GetVaccineContext()->GetInterventionReducedAcquire(TransmissionRoute::CONTACT);

            IMalariaHumanInfectable* imhi = nullptr;
            if ( s_OK !=  ihec->QueryInterface(GET_IID(IMalariaHumanInfectable), (void**)&imhi) )
            {
                throw QueryInterfaceException( __FILE__, __LINE__, __FUNCTION__, "ihec", "IMalariaHumanInfectable", "IIndividualHumanEventContext" );
            }

            if( GetRng()->SmartDraw( coverage*relative_risk ) && imhi->ChallengeWithSporozoites( n_sporozoites ) )
            {
                IInfectionAcquirable* iia = nullptr;
                if ( s_OK !=  ihec->QueryInterface(GET_IID(IInfectionAcquirable), (void**)&iia) )
                {
                    throw QueryInterfaceException( __FILE__, __LINE__, __FUNCTION__, "ihec", "IInfectionAcquirable", "IIndividualHumanEventContext" );
                }
                iia->AcquireNewInfection();
            }
        };

        VisitIndividuals(sporozoite_challenge_func);
    }

    void NodeMalariaEventContextHost::ChallengeWithInfectiousBites(int n_bites, float coverage, tAgeBitingFunction risk)
    {
        INodeEventContext::individual_visit_function_t infectious_bite_challenge_func = 
            [this, n_bites, coverage, risk](IIndividualHumanEventContext *ihec)
        {
            float relative_risk=1.0f;
            if(risk != nullptr)
            {
                relative_risk = risk(ihec->GetAge());
            }

            // Allow the bite challenge rate to be modified the individual factors that are usually handled in the Expose logic
            // GetRelativeBitingRate is already handled by the flexibility to pass in an age-risk function
            // Susceptibility::getModAcquire is only used in GENERIC and VECTOR immunity but not MALARIA
            // InterventionsContainer::GetInterventionReducedAcquire can be modified by a Vaccine intervention used in MALARIA_SIM
            // Adding route aware IVs required querying based on route of infection; malaria sims only use CONTACT route (default)
            relative_risk *= ihec->GetInterventionsContext()->GetParent()->GetVaccineContext()->GetInterventionReducedAcquire(TransmissionRoute::CONTACT);

            IMalariaHumanInfectable* imhi = nullptr;
            if ( s_OK !=  ihec->QueryInterface(GET_IID(IMalariaHumanInfectable), (void**)&imhi) )
            {
                throw QueryInterfaceException( __FILE__, __LINE__, __FUNCTION__, "ihec", "IMalariaHumanInfectable", "IIndividualHumanEventContext" );
            }

            if( GetRng()->SmartDraw( coverage*relative_risk ) && imhi->ChallengeWithBites( n_bites ) )
            {
                IInfectionAcquirable* iia = nullptr;
                if ( s_OK !=  ihec->QueryInterface(GET_IID(IInfectionAcquirable), (void**)&iia) )
                {
                    throw QueryInterfaceException( __FILE__, __LINE__, __FUNCTION__, "ihec", "IInfectionAcquirable", "IIndividualHumanEventContext" );
                }
                iia->AcquireNewInfection();
            }
        };

        VisitIndividuals(infectious_bite_challenge_func);
    }
}