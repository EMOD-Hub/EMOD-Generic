
#pragma once
#include "Infection.h"

namespace Kernel
{
    class InfectionSTIConfig : public InfectionConfig
    {
    public:
        virtual bool Configure( const Configuration* config ) override;

    protected:
        friend class InfectionSTI;
    };

    class InfectionSTI : public Infection
    {
    public:
        virtual ~InfectionSTI(void);
        static InfectionSTI *CreateInfection(IIndividualHumanContext *context, suids::suid _suid);
        virtual void Update(float dt, ISusceptibilityContext* immunity = nullptr) override;

    protected:
        InfectionSTI();
        InfectionSTI(IIndividualHumanContext *context);

        DECLARE_SERIALIZABLE(InfectionSTI);
    };
}
