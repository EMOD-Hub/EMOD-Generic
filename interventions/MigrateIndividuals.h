/***************************************************************************************************

Copyright (c) 2018 Intellectual Ventures Property Holdings, LLC (IVPH) All rights reserved.

EMOD is licensed under the Creative Commons Attribution-Noncommercial-ShareAlike 4.0 License.
To view a copy of this license, visit https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode

***************************************************************************************************/

#pragma once

#include <string>

#include "Configuration.h"
#include "Configure.h"
#include "InterventionFactory.h"
#include "Interventions.h"
#include "IDistribution.h"
#include "INodeContext.h"

namespace Kernel
{
    class IDMAPI MigrateIndividuals :  public BaseIntervention
    {
        IMPLEMENT_DEFAULT_REFERENCE_COUNTING()
        DECLARE_FACTORY_REGISTERED(IndividualIVFactory, MigrateIndividuals, IDistributableIntervention)

    public: 
        MigrateIndividuals();
        MigrateIndividuals( const MigrateIndividuals& master );
        virtual ~MigrateIndividuals();
        virtual bool Configure( const Configuration* pConfig ) override;

        virtual bool Distribute(IIndividualHumanInterventionsContext *context, ICampaignCostObserver * const pICCO) override;
        virtual void Update(float dt) override;

    protected:

#pragma warning( push )
#pragma warning( disable: 4251 ) // See IdmApi.h for details
        ExternalNodeId_t destination_external_node_id;
        IDistribution* duration_before_leaving;
        IDistribution* duration_at_node;
        bool is_moving;

        DECLARE_SERIALIZABLE(MigrateIndividuals);
#pragma warning( pop )
    };
}
