
#pragma once

#include "ISupports.h"
#include "SimulationEnums.h"
#include "IContagionPopulation.h"
#include "IInfection.h"

namespace Kernel
{
    struct IIndividualHuman;

    struct IInfectable : ISupports
    {
        virtual void Expose( const IContagionPopulation* cp, float dt, TransmissionRoute::Enum tx_route=TransmissionRoute::CONTACT ) = 0;
        virtual const infection_list_t& GetInfections() const = 0;

        virtual IIndividualHuman* GetIndividual() = 0;

        virtual ~IInfectable() {}
    };
}
