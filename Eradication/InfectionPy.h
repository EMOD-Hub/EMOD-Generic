
#pragma once
#include "Infection.h"

namespace Kernel
{
    class InfectionPyConfig : public InfectionConfig
    {
        friend class IndividualPy;
        GET_SCHEMA_STATIC_WRAPPER(InfectionPyConfig)
        IMPLEMENT_DEFAULT_REFERENCE_COUNTING()

    public:
        virtual bool Configure( const Configuration* config ) override;

    protected:
        friend class InfectionPy;
    };

    class IInfectionPy : public ISupports
    {
        public:
        virtual void Clear() = 0;
    };

    class InfectionPy
        : public Infection
        , public IInfectionPy 
    {
        IMPLEMENT_DEFAULT_REFERENCE_COUNTING()

    public:
        static InfectionPy *CreateInfection(IIndividualHumanContext *context, suids::suid _suid);
        virtual ~InfectionPy(void);

        virtual IInfectionPy* GetInfectionPy() override;

        virtual void InitInfectionImmunology(Susceptibility* _immunity);
        virtual void Update(float dt, ISusceptibilityContext* _immunity = nullptr) override;
        void SetMCWeightOfHost(float ind_mc_weight);
        virtual void Clear();

        // InfectionPyReportable methods
    protected:
        InfectionPy(); 

        InfectionPy(IIndividualHumanContext *context);
        void Initialize(suids::suid _suid);

    private:
    };
}

