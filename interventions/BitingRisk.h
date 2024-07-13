/***************************************************************************************************

Copyright (c) 2018 Intellectual Ventures Property Holdings, LLC (IVPH) All rights reserved.

EMOD is licensed under the Creative Commons Attribution-Noncommercial-ShareAlike 4.0 License.
To view a copy of this license, visit https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode

***************************************************************************************************/

#pragma once

#include <map>

#include "Interventions.h"
#include "InterventionFactory.h"
#include "Configuration.h"
#include "Configure.h"
#include "IDistribution.h"

namespace Kernel
{
    struct IVectorInterventionsEffects;

    class BitingRisk : public BaseIntervention
    {
        DECLARE_FACTORY_REGISTERED( IndividualIVFactory, BitingRisk, IDistributableIntervention )

    public:
        BitingRisk();
        BitingRisk( const BitingRisk& );
        virtual ~BitingRisk();

        virtual bool Configure( const Configuration * config ) override;

        // IDistributableIntervention
        virtual bool Distribute( IIndividualHumanInterventionsContext *context, ICampaignCostObserver  * const pCCO ) override;
        virtual QueryResult QueryInterface( iid_t iid, void **ppvObject ) override;
        virtual void SetContextTo( IIndividualHumanContext *context ) override;
        virtual void Update( float dt ) override;

    protected:
        IVectorInterventionsEffects* m_IBitingRisk;
        IDistribution* m_Distribution;

        DECLARE_SERIALIZABLE( BitingRisk );
    };
}
