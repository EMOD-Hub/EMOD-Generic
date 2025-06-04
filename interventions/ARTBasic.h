
#pragma once

#include "Interventions.h"
#include "InterventionFactory.h"    // macros that 'auto'-register classes

namespace Kernel
{
    class ARTBasic : public BaseIntervention
    {
        DECLARE_FACTORY_REGISTERED(IndividualIVFactory, ARTBasic, IDistributableIntervention);

    public:
        ARTBasic();
        virtual ~ARTBasic();

        virtual bool Configure( const Configuration * ) override;

        // IDistributableIntervention
        virtual bool Distribute(IIndividualHumanInterventionsContext *context, ICampaignCostObserver * const pCCO ) override;
        virtual void Update( float dt ) override;

    protected:
        // These have same names as analogous methods on container but are internal for the drug itself.

        bool viral_suppression;
        float days_to_achieve_suppression ;

        DECLARE_SERIALIZABLE(ARTBasic);
    };
}
