
#pragma once

#include "Individual.h"

namespace Kernel
{
    class IndividualHumanAirborneConfig : public IndividualHumanConfig
    {
        friend class IndividualHumanAirborne;

    public:
        virtual bool Configure( const Configuration* config ) override;
    };

    class IndividualHumanAirborne : public IndividualHuman
    {
        friend class SimulationAirborne;

        DECLARE_SERIALIZABLE( IndividualHumanAirborne )

    public:
        static IndividualHumanAirborne *CreateHuman(INodeContext *context, suids::suid _suid, float monte_carlo_weight = 1.0f, float initial_age = 0.0f, int gender = 0);
        virtual ~IndividualHumanAirborne(void) { }

        // Infections and Susceptibility
        virtual void CreateSusceptibility(float=1.0, float=1.0) override;

    protected:
        IndividualHumanAirborne(suids::suid id = suids::nil_suid(), float monte_carlo_weight = 1.0f, float initial_age = 0.0f, int gender = 0);

        virtual IInfection* createInfection(suids::suid _suid) override;

    private:
    };
}
