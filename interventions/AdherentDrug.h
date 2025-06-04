
#pragma once

#include "AntiMalarialDrug.h"
#include "EventTrigger.h"
#include "InterpolatedValueMap.h"
#include "InterventionEnums.h"

namespace Kernel
{
    struct IWaningEffect;

    class AdherentDrug : public AntimalarialDrug
    {
        DECLARE_FACTORY_REGISTERED( IndividualIVFactory, AdherentDrug, IDistributableIntervention )

    public:
        AdherentDrug();
        AdherentDrug( const AdherentDrug& rOrig );
        virtual ~AdherentDrug();

        virtual bool Configure( const Configuration * ) override;

        // GenericDrug method
        virtual void Update( float dt ) override;
        virtual void SetContextTo( IIndividualHumanContext *context ) override;
        virtual bool IsTakingDose( float dt ) override;

    protected:
        virtual void  ConfigureDrugTreatment( IIndividualHumanInterventionsContext * ivc ) override;
        NonAdherenceOptionsType::Enum AdherentDrug::SelectNonAdherenceOption();

        IWaningEffect* m_pAdherenceEffect;
        InterpolatedValueMap m_DoseEffects;

        std::vector<NonAdherenceOptionsType::Enum> m_NonAdherenceOptions;
        std::vector<float>                         m_NonAdherenceCdf;

        EventTrigger::Enum m_TookDoseEvent;
        float m_MaxDuration;
        float m_CurrentDuration;
        int m_CurrentDose;
        int m_TotalDoses;

        DECLARE_SERIALIZABLE( AdherentDrug );
    };
}
