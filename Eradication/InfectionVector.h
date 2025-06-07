
#pragma once

#include "VectorContexts.h"
#include "Infection.h"

namespace Kernel
{
    class InfectionVectorConfig : public InfectionConfig
    {
    public:
        virtual bool Configure( const Configuration* config ) override;

    protected:
        friend class InfectionVector;
    };

    class InfectionVector : public Infection
    {
    public:
        static InfectionVector *CreateInfection(IIndividualHumanContext *context, suids::suid _suid);
        virtual ~InfectionVector(void);

    protected:
        InfectionVector();
        InfectionVector(IIndividualHumanContext *context);
        virtual void Initialize(suids::suid _suid) override;

        DECLARE_SERIALIZABLE(InfectionVector);
    };
}
