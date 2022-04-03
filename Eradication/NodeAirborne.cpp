/***************************************************************************************************

Copyright (c) 2018 Intellectual Ventures Property Holdings, LLC (IVPH) All rights reserved.

EMOD is licensed under the Creative Commons Attribution-Noncommercial-ShareAlike 4.0 License.
To view a copy of this license, visit https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode

***************************************************************************************************/

#include "stdafx.h"

#ifndef DISABLE_AIRBORNE

#include "NodeAirborne.h"

#include "IndividualAirborne.h"

SETUP_LOGGING( "NodeAirborne" )

namespace Kernel
{
    NodeAirborne::NodeAirborne(ISimulationContext *_parent_sim, ExternalNodeId_t externalNodeId, suids::suid node_suid)
    : Node(_parent_sim, externalNodeId, node_suid)
    {
    }

    NodeAirborne::NodeAirborne() : Node()
    {
    }

    NodeAirborne::~NodeAirborne(void)
    {
    }

    NodeAirborne *NodeAirborne::CreateNode(ISimulationContext *_parent_sim, ExternalNodeId_t externalNodeId, suids::suid node_suid)
    {
        NodeAirborne *newnode = _new_ NodeAirborne(_parent_sim, externalNodeId, node_suid);
        newnode->Initialize();

        return newnode;
    }

    IIndividualHuman* NodeAirborne::createHuman(suids::suid suid, float monte_carlo_weight, float initial_age, int gender)
    {
        return IndividualHumanAirborne::CreateHuman(this, suid, monte_carlo_weight, initial_age, gender);
    }

    float NodeAirborne::getClimateCorrection() const
    {
        // Airborne infectivity depends on relative humidity.
        // TODO: make more configurable to accommodate different modalities:
        //       - primarily indoor transmission with controlled temperatures
        //       - relative importance of settling, ventilation, inactivation
        //       - temperature/humidity effects on contagion viability

        if ( localWeather == nullptr )
        {
            throw NullPointerException( __FILE__, __LINE__, __FUNCTION__, "localWeather", "Climate");
        }

        float humidity = localWeather->humidity();

        // The following is a sigmoidal form that drops from 1 to 0.5 mostly in the range from 30% to 70% relative humidity
        float correction = 0.75 - 0.2 * atan( 6 * humidity - 3 ); // linux build breaker?
        LOG_DEBUG_F( "Infectivity scale factor = %f at relative humidity = %f.\n", correction, humidity );

        return correction;
    }

    REGISTER_SERIALIZABLE(NodeAirborne);

    void NodeAirborne::serialize(IArchive& ar, NodeAirborne* obj)
    {
        Node::serialize(ar, obj);
    }
}

#endif // DISABLE_AIRBORNE
