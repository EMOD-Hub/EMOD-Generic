
#pragma once

#include "Infection.h"

namespace Kernel
{
    class InfectionEnvironmental : public Infection
    {
    public:
        static InfectionEnvironmental *CreateInfection(IIndividualHumanContext *context, suids::suid _suid);
        virtual ~InfectionEnvironmental(void);

    protected:
        InfectionEnvironmental(IIndividualHumanContext *context);
        InfectionEnvironmental();

        DECLARE_SERIALIZABLE(InfectionEnvironmental);
    };
}
