/***************************************************************************************************

Copyright (c) 2018 Intellectual Ventures Property Holdings, LLC (IVPH) All rights reserved.

EMOD is licensed under the Creative Commons Attribution-Noncommercial-ShareAlike 4.0 License.
To view a copy of this license, visit https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode

***************************************************************************************************/

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
