/***************************************************************************************************

Copyright (c) 2018 Intellectual Ventures Property Holdings, LLC (IVPH) All rights reserved.

EMOD is licensed under the Creative Commons Attribution-Noncommercial-ShareAlike 4.0 License.
To view a copy of this license, visit https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode

***************************************************************************************************/

#include "stdafx.h"
#include "HousingModification.h"

#include <typeinfo>

#include "IIndividualHumanContext.h"
#include "InterventionEnums.h"
#include "VectorContexts.h"
#include "Log.h"

SETUP_LOGGING( "SimpleHousingModification" )

namespace Kernel
{
    BEGIN_QUERY_INTERFACE_BODY(SimpleHousingModification)
        HANDLE_INTERFACE(IConfigurable)
        HANDLE_INTERFACE(IDistributableIntervention)
        HANDLE_ISUPPORTS_VIA(IDistributableIntervention)
    END_QUERY_INTERFACE_BODY(SimpleHousingModification)

    IMPLEMENT_FACTORY_REGISTERED(SimpleHousingModification)
    IMPLEMENT_FACTORY_REGISTERED(IRSHousingModification)
    IMPLEMENT_FACTORY_REGISTERED(ScreeningHousingModification)
    IMPLEMENT_FACTORY_REGISTERED(SpatialRepellentHousingModification)

    REGISTER_SERIALIZABLE(SimpleHousingModification);
    REGISTER_SERIALIZABLE(IRSHousingModification);
    REGISTER_SERIALIZABLE(ScreeningHousingModification);
    REGISTER_SERIALIZABLE(SpatialRepellentHousingModification);

    // ------------------------------------------------------------------------
    // --- SimpleHousingModification
    // ------------------------------------------------------------------------

    SimpleHousingModification::SimpleHousingModification()
        : BaseIntervention()
        , blocking_effect(nullptr)
        , killing_effect(nullptr)
        , m_pIHMC(nullptr)
    {
        initSimTypes( 2, "VECTOR_SIM", "MALARIA_SIM" );
    }

    SimpleHousingModification::SimpleHousingModification( const SimpleHousingModification& master )
        : BaseIntervention( master )
        , blocking_effect( nullptr )
        , killing_effect( nullptr )
        , m_pIHMC( nullptr )
    {
        if(master.blocking_effect)
        {
            blocking_effect = master.blocking_effect->Clone();
        }
        if(master.killing_effect)
        {
            killing_effect = master.killing_effect->Clone();
        }
    }

    SimpleHousingModification::~SimpleHousingModification()
    {
        delete blocking_effect;
        delete killing_effect;

        blocking_effect = nullptr;
        killing_effect  = nullptr;
    }

    bool SimpleHousingModification::Configure( const Configuration * inputJson )
    {
        initConfigTypeMap("Cost_To_Consumer", &cost_per_unit, HM_Cost_To_Consumer_DESC_TEXT, 0, 999999, 8.0);

        initConfigRepelling();
        initConfigKilling();

        bool configured = BaseIntervention::Configure( inputJson );

        return configured;
    }

    void SimpleHousingModification::initConfigRepelling()
    {
        blocking_effect = WaningEffectFactory::CreateInstance();
        initConfigTypeMap("Blocking_Config",  blocking_effect->GetConfigurable(),  HM_Blocking_Config_DESC_TEXT);
    }

    void SimpleHousingModification::initConfigKilling()
    {
        killing_effect = WaningEffectFactory::CreateInstance();
        initConfigTypeMap("Killing_Config",   killing_effect->GetConfigurable(),   HM_Killing_Config_DESC_TEXT);
    }

    bool SimpleHousingModification::Distribute(IIndividualHumanInterventionsContext* context, ICampaignCostObserver* const pCCO)
    {
        if( AbortDueToDisqualifyingInterventionStatus( context->GetParent() ) )
        {
            return false;
        }

        m_pIHMC = context->GetContainerVector();

        context->PurgeExisting( typeid(*this).name() );

        return BaseIntervention::Distribute( context, pCCO );
    }

    void SimpleHousingModification::SetContextTo(IIndividualHumanContext* context)
    {
        BaseIntervention::SetContextTo( context );

        if(blocking_effect)
        {
            blocking_effect->SetContextTo( context );
        }
        if(killing_effect)
        {
            killing_effect->SetContextTo( context );
        }

        LOG_DEBUG("SimpleHousingModification::SetContextTo (probably deserializing)\n");
        m_pIHMC = context->GetInterventionsContext()->GetContainerVector();
    }

    void SimpleHousingModification::Update( float dt )
    {
        if( !BaseIntervention::UpdateIndividualsInterventionStatus() ) return;

        if(blocking_effect)
        {
            blocking_effect->Update(dt);
            if(blocking_effect->Expired())
            {
                delete blocking_effect;
                blocking_effect = nullptr;
            }
            else
            {
                ApplyEffectsRepelling( dt );
            }
        }

        if(killing_effect)
        {
            killing_effect->Update(dt);
            if(killing_effect->Expired())
            {
                delete killing_effect;
                killing_effect = nullptr;
            }
            else
            {
                ApplyEffectsKilling( dt );
            }
        }
    }

    void SimpleHousingModification::ApplyEffectsRepelling( float dt )
    {
        release_assert(m_pIHMC);
        release_assert(blocking_effect);
        m_pIHMC->ApplyHouseBlockingProbability( blocking_effect->Current() );
    }

    void SimpleHousingModification::ApplyEffectsKilling( float dt )
    {
        release_assert(m_pIHMC);
        release_assert(killing_effect);
        m_pIHMC->UpdateProbabilityOfScreenKilling( killing_effect->Current() );
    }

    void SimpleHousingModification::serialize(IArchive& ar, SimpleHousingModification* obj)
    {
        BaseIntervention::serialize( ar, obj );
        SimpleHousingModification& mod = *obj;
        ar.labelElement("blocking_effect") & mod.blocking_effect;
        ar.labelElement("killing_effect") & mod.killing_effect;
    }

    // ------------------------------------------------------------------------
    // --- IRSHousingModification
    // ------------------------------------------------------------------------

    void IRSHousingModification::serialize(IArchive& ar, IRSHousingModification* obj)
    {
        SimpleHousingModification::serialize(ar, obj);
    }

    // ------------------------------------------------------------------------
    // --- ScreeningHousingModification
    // ------------------------------------------------------------------------

    void ScreeningHousingModification::serialize(IArchive& ar, ScreeningHousingModification* obj)
    {
        SimpleHousingModification::serialize(ar, obj);
    }

    // ------------------------------------------------------------------------
    // --- SpatialRepellentHousingModification
    // ------------------------------------------------------------------------

    void SpatialRepellentHousingModification::initConfigKilling()
    {
        // do not include killing
    }

    void SpatialRepellentHousingModification::ApplyEffectsKilling( float dt )
    {
        // no killing
    }

    void SpatialRepellentHousingModification::serialize(IArchive& ar, SpatialRepellentHousingModification* obj)
    {
        SimpleHousingModification::serialize(ar, obj);
    }
}

