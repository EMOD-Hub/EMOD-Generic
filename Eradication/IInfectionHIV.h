
#include "HIVEnums.h"

namespace Kernel
{
    class IInfectionHIV : public ISupports
    {
    public:
        virtual float GetWHOStage() const = 0;
        virtual NaturalNumber GetViralLoad() const = 0;
        virtual float GetPrognosis() const = 0;
        virtual float GetTimeInfected() const = 0;
        virtual float GetDaysTillDeath() const = 0;
        virtual const HIVInfectionStage::Enum& GetStage() const = 0;
        virtual void SetupSuppressedDiseaseTimers() = 0;
        virtual void ApplySuppressionDropout() = 0;
        virtual void ApplySuppressionFailure() = 0;
    };
}
