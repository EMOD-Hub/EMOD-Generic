
#include "Interventions.h"

namespace Kernel
{
    struct IHealthSeekingBehavior : public ISupports
    {
        virtual void UpdateProbabilityofSeeking(float new_probability_of_seeking) = 0;
        virtual IHealthSeekingBehavior* GetHSB() = 0;
    };
}
