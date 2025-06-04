
#pragma once

#include "OutbreakIndividual.h"
#include "StrainIdentity.h"

namespace Kernel
{
    class IndividualHumanCoInfection;

    class OutbreakIndividualTBorHIV : public OutbreakIndividual
    {
        DECLARE_FACTORY_REGISTERED(IndividualIVFactory, OutbreakIndividualTBorHIV, IDistributableIntervention)

    public:
        OutbreakIndividualTBorHIV();
        virtual ~OutbreakIndividualTBorHIV() { }

        virtual bool Configure(const Configuration* config) override;
        QuickBuilder GetSchema();
        virtual bool Distribute(IIndividualHumanInterventionsContext *context, ICampaignCostObserver * const pCCO);

    protected:
        int infection_type;
    };
}
