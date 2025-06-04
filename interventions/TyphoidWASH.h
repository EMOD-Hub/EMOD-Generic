
#pragma once

#include "NodeTyphoidEventContext.h"
#include "IWaningEffect.h"
#include "Interventions.h"
#include "InterventionFactory.h"

namespace Kernel
{
    class TyphoidWASH : public BaseNodeIntervention
    {
        DECLARE_FACTORY_REGISTERED(NodeIVFactory, TyphoidWASH, INodeDistributableIntervention)
        IMPLEMENT_DEFAULT_REFERENCE_COUNTING()

    public:
        TyphoidWASH ();
        TyphoidWASH( const TyphoidWASH& );
        virtual ~TyphoidWASH();
        
        // INodeDistributableIntervention 
        virtual bool Configure(const Configuration* config) override;
        virtual void SetContextTo(INodeEventContext *context) override;
        virtual bool Distribute(INodeEventContext *context, IEventCoordinator2* pEC = nullptr ) override;

        // IDistributableIntervention
        virtual void Update(float dt) override;

        TyphoidVaccineMode::Enum vaccine_mode;
        float effect;
        INodeTyphoidInterventionEffects* itvc; // interventions container
        IWaningEffect* changing_effect;

    protected:
        std::string targeted_individual_properties;
        bool use_property_targeting;
    };
}
