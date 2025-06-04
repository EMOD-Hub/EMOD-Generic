
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
        virtual void SetContextTo( IIndividualHumanContext *context ) override;
        virtual void Update( float dt ) override;

    protected:
        IVectorInterventionsEffects* m_IBitingRisk;
        IDistribution* m_Distribution;

        DECLARE_SERIALIZABLE( BitingRisk );
    };
}
