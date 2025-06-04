
#pragma once
#include "Infection.h"

namespace Kernel
{
    class InfectionAirborneConfig : public InfectionConfig
    {
    public:
        virtual bool Configure( const Configuration* config ) override;

    protected:
        friend class InfectionAirborne;
    };

    class InfectionAirborne : public Infection
    {
    public:
        virtual ~InfectionAirborne(void);
        static InfectionAirborne *CreateInfection(IIndividualHumanContext *context, suids::suid _suid);

    protected:
        InfectionAirborne();
        InfectionAirborne(IIndividualHumanContext *context);

        DECLARE_SERIALIZABLE(InfectionAirborne);
    };
}
