/***************************************************************************************************

Copyright (c) 2018 Intellectual Ventures Property Holdings, LLC (IVPH) All rights reserved.

EMOD is licensed under the Creative Commons Attribution-Noncommercial-ShareAlike 4.0 License.
To view a copy of this license, visit https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode

***************************************************************************************************/

#include "stdafx.h"
#include "BitingRisk.h"
#include "VectorContexts.h"
#include "IIndividualHumanContext.h"
#include "Distributions.h"
#include "DistributionFactory.h"

SETUP_LOGGING( "BitingRisk" )

namespace Kernel
{
    IMPLEMENT_FACTORY_REGISTERED( BitingRisk )

        BitingRisk::BitingRisk()
        : BaseIntervention()
        , m_IBitingRisk( nullptr )
        , m_Distribution( nullptr )
    {
        // ??????????????
        // ??? DENGUE_SIM
        // ??????????????
        initSimTypes( 2, "VECTOR_SIM", "MALARIA_SIM" );
    }

    BitingRisk::BitingRisk( const BitingRisk& master )
        : BaseIntervention( master )
        , m_IBitingRisk( nullptr )
        , m_Distribution( master.m_Distribution->Clone() )
    {
    }

    BitingRisk::~BitingRisk()
    {
        delete m_Distribution;
    }

    bool BitingRisk::Configure( const Configuration * inputJson )
    {
        float param1_risk = 0.0, param2_risk = 0.0;
        DistributionFunction::Enum risk_distribution_function(DistributionFunction::NOT_INITIALIZED);
        initConfig("Risk_Distribution", risk_distribution_function, inputJson, MetadataDescriptor::Enum("Risk_Distribution_Type", BR_Risk_Distribution_Type_DESC_TEXT, MDD_ENUM_ARGS(DistributionFunction)));
        m_Distribution = DistributionFactory::CreateDistribution( this, risk_distribution_function, "Risk", inputJson );
       
        return BaseIntervention::Configure( inputJson );
    }

    bool BitingRisk::Distribute( IIndividualHumanInterventionsContext *context,
                                 ICampaignCostObserver * const pCCO )
    {
        if( AbortDueToDisqualifyingInterventionStatus( context->GetParent() ) )
        {
            return false;
        }

        bool distributed = BaseIntervention::Distribute( context, pCCO );
        if( distributed )
        {
            m_IBitingRisk = context->GetContainerVector();
        }
        return distributed;
    }

    void BitingRisk::SetContextTo( IIndividualHumanContext *context )
    {
        BaseIntervention::SetContextTo( context );

        m_IBitingRisk = context->GetInterventionsContext()->GetContainerVector();
    }

    void BitingRisk::Update( float dt )
    {
        if( !BaseIntervention::UpdateIndividualsInterventionStatus() ) return;

        float rate = m_Distribution->Calculate( parent->GetRng() );
        release_assert(m_IBitingRisk);
        m_IBitingRisk->UpdateRelativeBitingRate( rate );
        expired = true;
    }

    REGISTER_SERIALIZABLE( BitingRisk );

    void BitingRisk::serialize( IArchive& ar, BitingRisk* obj )
    {
        BaseIntervention::serialize( ar, obj );
        BitingRisk& biting_risk = *obj;
        ar.labelElement( "m_Distribution" ) & biting_risk.m_Distribution;
    }
}
