/***************************************************************************************************

Copyright (c) 2018 Intellectual Ventures Property Holdings, LLC (IVPH) All rights reserved.

EMOD is licensed under the Creative Commons Attribution-Noncommercial-ShareAlike 4.0 License.
To view a copy of this license, visit https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode

***************************************************************************************************/

#include "stdafx.h"
#include "Outbreak.h"
#include "Exceptions.h"
#include "NodeEventContext.h"
#include "StrainIdentity.h"
#include "ISimulationContext.h"
#include "RANDOM.h"

SETUP_LOGGING( "Outbreak" )

namespace Kernel
{
    BEGIN_QUERY_INTERFACE_BODY(Outbreak)
        HANDLE_INTERFACE(IConfigurable)
        HANDLE_INTERFACE(IBaseIntervention)
        HANDLE_INTERFACE(INodeDistributableIntervention)
        HANDLE_INTERFACE(IOutbreak)
        HANDLE_ISUPPORTS_VIA(INodeDistributableIntervention)
    END_QUERY_INTERFACE_BODY(Outbreak)


    IMPLEMENT_FACTORY_REGISTERED(Outbreak)

    Outbreak::Outbreak()
        : clade(0)
        , genome(0)
        , num_cases_per_node(0)
        , import_age(0.0f)
        , inf_prob(0.0f)
        , female_prob(0.0f)
        , mc_weight(0.0f)
    {
        // Schema documentation
        initSimTypes( 12, "GENERIC_SIM", "VECTOR_SIM", "MALARIA_SIM", "AIRBORNE_SIM", "POLIO_SIM", "TBHIV_SIM", "STI_SIM", "HIV_SIM", "PY_SIM", "TYPHOID_SIM", "ENVIRONMENTAL_SIM", "DENGUE_SIM" );
    }

    bool Outbreak::Configure(const Configuration * inputJson)
    {
        initConfigTypeMap( "Clade",                 &clade,               Clade_DESC_TEXT,                         0,         9,   0 );
        initConfigTypeMap( "Genome",                &genome,              Genome_DESC_TEXT,                        0, MAX_24BIT,   0 );
        initConfigTypeMap( "Number_Cases_Per_Node", &num_cases_per_node,  Num_Import_Cases_Per_Node_DESC_TEXT,     0,   INT_MAX,   1 );

        initConfigTypeMap( "Import_Age",              &import_age,        Import_Age_DESC_TEXT,                 0.0f,  MAX_HUMAN_AGE*DAYSPERYEAR, DAYSPERYEAR );
        initConfigTypeMap( "Import_Female_Prob",      &female_prob,       Import_Female_Prob_DESC_TEXT,         0.0f,                       1.0f,        0.5f );
        initConfigTypeMap( "Import_Agent_MC_Weight",  &mc_weight,         Import_Agent_MC_Weight_DESC_TEXT,  FLT_MIN,                    FLT_MAX,        1.0f );

        initConfigTypeMap( "Probability_of_Infection", &inf_prob,         Probability_of_Infection_DESC_TEXT,   0.0f,                       1.0f,        1.0f );


        // --------------------------------------------------------------
        // --- Don't call BaseIntervention::Configure() because we don't
        // --- want to inherit those parameters.
        // --------------------------------------------------------------
        return JsonConfigurable::Configure( inputJson );;
    }

    bool Outbreak::Distribute(INodeEventContext* p_nec, IEventCoordinator2* pEC)
    {
        bool wasDistributed = false;

        const StrainIdentity outbreak_strain(clade,genome);
        p_nec->AddImportCases(&outbreak_strain, import_age, num_cases_per_node, inf_prob, female_prob, mc_weight);
        wasDistributed = true;

        return wasDistributed;
    }

    void Outbreak::Update( float dt )
    {
        // Distribute() doesn't call GiveIntervention() for this intervention, so it isn't added to the NodeEventContext's list of NDI
        release_assert(false);
    }
}
