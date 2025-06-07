
#pragma once

#include <string>
#include <list>
#include <vector>

#include "HealthSeekingBehavior.h"
#include "IHealthSeekingBehavior.h"

namespace Kernel
{
    class HealthSeekingBehaviorUpdateable :  public SimpleHealthSeekingBehavior, public IHealthSeekingBehavior
    {
        IMPLEMENT_DEFAULT_REFERENCE_COUNTING()
        DECLARE_FACTORY_REGISTERED(IndividualIVFactory, HealthSeekingBehaviorUpdateable, IDistributableIntervention)
    
    public: 
        HealthSeekingBehaviorUpdateable();
        virtual ~HealthSeekingBehaviorUpdateable() {};

        //IHealthSeekingBehavior
        virtual void UpdateProbabilityofSeeking(float new_probability_of_seeking) override;
        virtual IHealthSeekingBehavior* GetHSB() override;

    protected:
        DECLARE_SERIALIZABLE(HealthSeekingBehaviorUpdateable);
    };
}
