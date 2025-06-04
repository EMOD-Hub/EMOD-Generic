
#pragma once

#include "Types.h"
#include "InterventionFactory.h"
#include "Interventions.h"

namespace Kernel
{
    struct IHIVInterventionsContainer;

    class IDMAPI PMTCT : public BaseIntervention
    {
        IMPLEMENT_DEFAULT_REFERENCE_COUNTING()
        DECLARE_FACTORY_REGISTERED(IndividualIVFactory, PMTCT, IDistributableIntervention)

    public: 
        PMTCT();
        PMTCT( const PMTCT& );
        ~PMTCT();
        virtual bool Configure( const Configuration* pConfig ) override;

        // IDistributingDistributableIntervention
        virtual void SetContextTo(IIndividualHumanContext *context) override;
        virtual void Update(float dt) override;
        virtual bool Distribute(IIndividualHumanInterventionsContext *context, ICampaignCostObserver* const pEC) override;

    protected:
        IHIVInterventionsContainer* ivc;
        NonNegativeFloat timer;
        float efficacy;

#pragma warning( push )
#pragma warning( disable: 4251 ) // See IdmApi.h for details
        DECLARE_SERIALIZABLE(PMTCT);
#pragma warning( pop )
    };
}
