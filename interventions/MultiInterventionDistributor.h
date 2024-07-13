/***************************************************************************************************

Copyright (c) 2018 Intellectual Ventures Property Holdings, LLC (IVPH) All rights reserved.

EMOD is licensed under the Creative Commons Attribution-Noncommercial-ShareAlike 4.0 License.
To view a copy of this license, visit https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode

***************************************************************************************************/

#pragma once

#include "Interventions.h"
#include "InterventionFactory.h"

namespace Kernel
{
    class MultiInterventionDistributor : public BaseIntervention
    {
        DECLARE_FACTORY_REGISTERED(IndividualIVFactory, MultiInterventionDistributor, IDistributableIntervention)
    
    public: 
        MultiInterventionDistributor();
        MultiInterventionDistributor( const MultiInterventionDistributor& rMaster );
        virtual ~MultiInterventionDistributor();

        bool Configure( const Configuration* config );

        // IDistributableIntervention
        virtual QueryResult QueryInterface(iid_t iid, void **ppvObject) override;
        virtual void Update(float dt) override;
        virtual bool Distribute(IIndividualHumanInterventionsContext *context, ICampaignCostObserver * const pICCO ) override;

    protected:
        virtual void Expire();

        std::vector<IDistributableIntervention*> m_Interventions;
    };
}
