
#include "ReferenceTrackingEventCoordinator.h"

namespace Kernel
{
    class ReferenceTrackingEventCoordinatorHIV : public ReferenceTrackingEventCoordinator 
    {
        DECLARE_FACTORY_REGISTERED(EventCoordinatorFactory, ReferenceTrackingEventCoordinatorHIV, IEventCoordinator)    

    public:
        IMPLEMENT_DEFAULT_REFERENCE_COUNTING()

        ReferenceTrackingEventCoordinatorHIV();
        virtual ~ReferenceTrackingEventCoordinatorHIV() { } 

        virtual bool Configure(const Configuration* config) override;
        virtual bool qualifiesDemographically( IIndividualHumanEventContext* pIndividual );

    protected:
        TargetDiseaseStateType::Enum target_disease_state;
    };
}
