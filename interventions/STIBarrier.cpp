/***************************************************************************************************

Copyright (c) 2018 Intellectual Ventures Property Holdings, LLC (IVPH) All rights reserved.

EMOD is licensed under the Creative Commons Attribution-Noncommercial-ShareAlike 4.0 License.
To view a copy of this license, visit https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode

***************************************************************************************************/

#include "stdafx.h"
#include "STIBarrier.h"

#include <typeinfo>

#include "IIndividualHumanContext.h"
#include "InterventionEnums.h"
#include "STIInterventionsContainer.h"
#include "IRelationship.h"
#include "Sigmoid.h"
#include "Common.h"

SETUP_LOGGING( "STIBarrier" )

namespace Kernel
{
    BEGIN_QUERY_INTERFACE_BODY(STIBarrier)
        HANDLE_INTERFACE(IConfigurable)
        HANDLE_INTERFACE(IDistributableIntervention)
        HANDLE_ISUPPORTS_VIA(IDistributableIntervention)
    END_QUERY_INTERFACE_BODY(STIBarrier)

    IMPLEMENT_FACTORY_REGISTERED(STIBarrier)
    
    STIBarrier::STIBarrier()
    {
        LOG_DEBUG_F( "STIBarrier\n" );
        initSimTypes( 2, "STI_SIM", "HIV_SIM" );
        initConfigTypeMap( "Cost_To_Consumer", &cost_per_unit, STI_Barrier_Cost_DESC_TEXT, 0, 999999, 10.0);
        initConfigTypeMap( "Early", &early, STI_Barrier_Early_DESC_TEXT, 0.0, 1.0, 1.0 );
        initConfigTypeMap( "Late", &late, STI_Barrier_Late_DESC_TEXT, 0.0, 1.0, 1.0 );
        initConfigTypeMap( "MidYear", &midyear, STI_Barrier_MidYear_DESC_TEXT, MIN_YEAR, MAX_YEAR, 2000 );
        initConfigTypeMap( "Rate", &rate, STI_Barrier_Rate_DESC_TEXT, -100.0, 100.0, 1.0 );
    }

    bool STIBarrier::Configure( const Configuration* inputJson )
    {
        LOG_DEBUG_F( "%s\n", __FUNCTION__ );
        initConfig( "Relationship_Type", rel_type, inputJson, MetadataDescriptor::Enum("Relationship_Type", STI_Barrier_Relationship_Type_DESC_TEXT, MDD_ENUM_ARGS(RelationshipType)) );
        return BaseIntervention::Configure( inputJson );
    }

    bool STIBarrier::Distribute( IIndividualHumanInterventionsContext* context, ICampaignCostObserver* const pCCO )
    {
        isticontainer = context->GetContainerSTI();

        return BaseIntervention::Distribute( context, pCCO );
    }

    void STIBarrier::Update( float dt )
    {
        if( !BaseIntervention::UpdateIndividualsInterventionStatus() ) return;

        LOG_DEBUG_F( "%s\n", __FUNCTION__ );
        Sigmoid probs( early, late, midyear, rate );

        release_assert(isticontainer);
        isticontainer->UpdateSTIBarrierProbabilitiesByType( rel_type, probs );

        expired = true;
    }

    void STIBarrier::SetContextTo( IIndividualHumanContext* context )
    {
        BaseIntervention::SetContextTo( context );

        isticontainer = context->GetInterventionsContext()->GetContainerSTI();
    }

    REGISTER_SERIALIZABLE(STIBarrier);

    void STIBarrier::serialize(IArchive& ar, STIBarrier* obj)
    {
        BaseIntervention::serialize( ar, obj );
        STIBarrier& barrier = *obj;
        ar.labelElement("early"   ) & barrier.early;
        ar.labelElement("late"    ) & barrier.late;
        ar.labelElement("midyear" ) & barrier.midyear;
        ar.labelElement("rate"    ) & barrier.rate;
        ar.labelElement("rel_type") & (uint32_t&)barrier.rel_type;
    }
}
