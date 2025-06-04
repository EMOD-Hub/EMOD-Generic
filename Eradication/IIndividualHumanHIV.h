
#pragma once

#include <set>
#include <string>

#include "ISupports.h"

namespace Kernel {
    class IInfectionHIV;
    class ISusceptibilityHIV;
    struct IHIVInterventionsContainer;

    struct IIndividualHumanHIV : public ISupports
    {
        virtual bool HasHIV() const = 0;
        virtual IInfectionHIV* GetHIVInfection() const = 0;
        virtual ISusceptibilityHIV* GetHIVSusceptibility() const = 0;
        virtual IHIVInterventionsContainer* GetHIVInterventionsContainer() const = 0;
        virtual std::string toString() const = 0; // serialization, for logging
    };

}
