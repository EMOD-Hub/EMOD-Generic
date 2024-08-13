/***************************************************************************************************

Copyright (c) 2018 Intellectual Ventures Property Holdings, LLC (IVPH) All rights reserved.

EMOD is licensed under the Creative Commons Attribution-Noncommercial-ShareAlike 4.0 License.
To view a copy of this license, visit https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode

***************************************************************************************************/

#include "stdafx.h"
#include "IndividualRepellent.h"
#include "IIndividualHumanContext.h"
#include "InterventionEnums.h"
#include "VectorContexts.h"

SETUP_LOGGING( "SimpleIndividualRepellent" )

namespace Kernel
{
    IMPLEMENT_FACTORY_REGISTERED(SimpleIndividualRepellent)

    SimpleIndividualRepellent::SimpleIndividualRepellent()
        : BaseIntervention()
        , blocking_effect(nullptr)
        , m_pIRC(nullptr)
    {
        initSimTypes( 2, "VECTOR_SIM", "MALARIA_SIM" );
    }

    SimpleIndividualRepellent::SimpleIndividualRepellent( const SimpleIndividualRepellent& master )
        : BaseIntervention( master )
        , blocking_effect( nullptr )
        , m_pIRC( nullptr )
    {
        if( master.blocking_effect != nullptr )
        {
            blocking_effect = master.blocking_effect->Clone();
        }
    }

    SimpleIndividualRepellent::~SimpleIndividualRepellent()
    {
        delete blocking_effect;
        blocking_effect = nullptr;
    }

    bool SimpleIndividualRepellent::Configure(const Configuration* inputJson)
    {
        initConfigTypeMap("Cost_To_Consumer", &cost_per_unit, SIR_Cost_To_Consumer_DESC_TEXT, 0, 999999, 8.0);

        blocking_effect = WaningEffectFactory::CreateInstance();
        initConfigTypeMap("Blocking_Config",  blocking_effect->GetConfigurable(),  SIR_Blocking_Config_DESC_TEXT);

        bool configured = BaseIntervention::Configure( inputJson );

        return configured;
    }

    bool SimpleIndividualRepellent::Distribute(IIndividualHumanInterventionsContext* context, ICampaignCostObserver* const pCCO)
    {
        m_pIRC = context->GetContainerVector();

        return BaseIntervention::Distribute( context, pCCO );
    }

    void SimpleIndividualRepellent::SetContextTo(IIndividualHumanContext* context)
    {
        BaseIntervention::SetContextTo( context );

        if(blocking_effect)
        {
            blocking_effect->SetContextTo( context );
        }

        LOG_DEBUG("SimpleIndividualRepellent::SetContextTo (probably deserializing)\n");
        m_pIRC = context->GetInterventionsContext()->GetContainerVector();
    }

    void SimpleIndividualRepellent::Update( float dt )
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
                release_assert(m_pIRC);
                m_pIRC->UpdateProbabilityOfIndRepBlocking( blocking_effect->Current() );
            }
        }
    }

    REGISTER_SERIALIZABLE(SimpleIndividualRepellent);

    void SimpleIndividualRepellent::serialize(IArchive& ar, SimpleIndividualRepellent* obj)
    {
        BaseIntervention::serialize( ar, obj );
        SimpleIndividualRepellent& repellent = *obj;
        ar.labelElement("blocking_effect") & repellent.blocking_effect;
    }
}
