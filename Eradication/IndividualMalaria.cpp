/***************************************************************************************************

Copyright (c) 2018 Intellectual Ventures Property Holdings, LLC (IVPH) All rights reserved.

EMOD is licensed under the Creative Commons Attribution-Noncommercial-ShareAlike 4.0 License.
To view a copy of this license, visit https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode

***************************************************************************************************/

#include "stdafx.h"

#include "IndividualMalaria.h"
#include "NodeVector.h"
#include "SusceptibilityMalaria.h"
#include "InfectionMalaria.h"
#include "Debug.h"
#include "Sigmoid.h"
#include "TransmissionGroupMembership.h"

#include "MalariaInterventionsContainer.h"
#include "SimulationConfig.h"
#include "MathFunctions.h"
#include "GenomeMarkers.h"
#include "MalariaParameters.h"
#include "EventTrigger.h"
#include "Vector.h"
#include "RANDOM.h"
#include "Exceptions.h"

SETUP_LOGGING( "IndividualMalaria" )

#ifndef WIN32
#define max(a,b) ( (a>b) ? a : b )
#endif

namespace Kernel
{
    GET_SCHEMA_STATIC_WRAPPER_IMPL(Malaria.Individual,IndividualHumanMalariaConfig)

    // ----------------- IndividualHumanMalariaConfig ---------------
    MalariaModel::Enum    IndividualHumanMalariaConfig::malaria_model                     = MalariaModel::MALARIA_MECHANISTIC_MODEL;
    float                 IndividualHumanMalariaConfig::mean_sporozoites_per_bite         = 0.0f;
    float                 IndividualHumanMalariaConfig::base_sporozoite_survival_fraction = 0.0f;
    float                 IndividualHumanMalariaConfig::antibody_csp_killing_threshold    = 0.0f;
    float                 IndividualHumanMalariaConfig::antibody_csp_killing_invwidth     = 0.0f;

    BEGIN_QUERY_INTERFACE_BODY(IndividualHumanMalariaConfig)
    END_QUERY_INTERFACE_BODY(IndividualHumanMalariaConfig)

    bool IndividualHumanMalariaConfig::Configure( const Configuration * config )
    {
        LOG_DEBUG( "Configure\n" );
        IndividualHumanConfig::enable_immunity = true;

        initConfig( "Malaria_Model", malaria_model, config, MetadataDescriptor::Enum("malaria_model", Malaria_Model_DESC_TEXT, MDD_ENUM_ARGS(MalariaModel)) );
        
        if (malaria_model != MalariaModel::MALARIA_MECHANISTIC_MODEL)
        {
            std::ostringstream errMsg;
            errMsg << "Malaria model " << malaria_model << " not yet implemented.";
            throw NotYetImplementedException(  __FILE__, __LINE__, __FUNCTION__, errMsg.str().c_str() );
        }

        initConfigTypeMap( "Mean_Sporozoites_Per_Bite",          &mean_sporozoites_per_bite,         Mean_Sporozoites_Per_Bite_DESC_TEXT,          0.0f,  1000.0f, DEFAULT_SPOROZOITES_PER_BITE );
        initConfigTypeMap( "Base_Sporozoite_Survival_Fraction",  &base_sporozoite_survival_fraction, Base_Sporozoite_Survival_Fraction_DESC_TEXT,  0.0f,  1.0f,    DEFAULT_SPOROZOITE_SURVIVAL_FRACTION );
        initConfigTypeMap( "Antibody_CSP_Killing_Threshold",     &antibody_csp_killing_threshold,    Antibody_CSP_Killing_Threshold_DESC_TEXT,     1e-6f, 1e6f,    DEFAULT_ANTIBODY_CSP_KILLING_THRESHOLD );
        initConfigTypeMap( "Antibody_CSP_Killing_Inverse_Width", &antibody_csp_killing_invwidth,     Antibody_CSP_Killing_Inverse_Width_DESC_TEXT, 1e-6f, 1e6f,    DEFAULT_ANTIBODY_CSP_KILLING_INVWIDTH );

        return JsonConfigurable::Configure( config );
    }

    void IndividualHumanMalaria::InitializeStaticsMalaria( const Configuration * config )
    {
        SusceptibilityMalariaConfig immunity_config;
        immunity_config.Configure( config );
        InfectionMalariaConfig infection_config;
        infection_config.Configure( config );
        IndividualHumanMalariaConfig individual_config;
        individual_config.Configure( config );
    }

    // ----------------- IndividualHumanMalaria ---------------
    BEGIN_QUERY_INTERFACE_DERIVED(IndividualHumanMalaria, IndividualHumanVector)
        HANDLE_INTERFACE(IMalariaHumanContext)
        HANDLE_INTERFACE(IMalariaHumanInfectable)
    END_QUERY_INTERFACE_DERIVED(IndividualHumanMalaria, IndividualHumanVector)

    IndividualHumanMalaria::IndividualHumanMalaria(suids::suid _suid, double monte_carlo_weight, double initial_age, int gender)
    : IndividualHumanVector(_suid, monte_carlo_weight, initial_age, gender)
    , malaria_susceptibility(nullptr)
    , m_inv_microliters_blood(INV_MICROLITERS_BLOOD_ADULT)
    , m_male_gametocytes(0)
    , m_female_gametocytes(0)
    , m_female_gametocytes_by_strain()
    , m_parasites_detected_by_blood_smear(0.0)
    , m_parasites_detected_by_new_diagnostic(0.0)
    , m_gametocytes_detected(0.0)
    //, m_clinical_symptoms()
    , m_CSP_antibody(nullptr)
    , m_initial_infected_hepatocytes(0)
    {
        ResetClinicalSymptoms();
    }

    IndividualHumanMalaria::IndividualHumanMalaria(INodeContext *context)
    : IndividualHumanVector(context)
    , malaria_susceptibility(nullptr)
    , m_inv_microliters_blood(INV_MICROLITERS_BLOOD_ADULT)
    , m_male_gametocytes(0)
    , m_female_gametocytes(0)
    , m_female_gametocytes_by_strain()
    , m_parasites_detected_by_blood_smear(0.0)
    , m_parasites_detected_by_new_diagnostic(0.0)
    , m_gametocytes_detected(0.0)
    //, m_clinical_symptoms()
    , m_CSP_antibody(nullptr)
    , m_initial_infected_hepatocytes(0)
    {
        ResetClinicalSymptoms();
    }

    IndividualHumanMalaria *IndividualHumanMalaria::CreateHuman(INodeContext *context, suids::suid id, double weight, double initial_age, int gender)
    {
        IndividualHumanMalaria *newhuman = _new_ IndividualHumanMalaria(id, weight, initial_age, gender);

        newhuman->SetContextTo(context);
        LOG_DEBUG_F( "Created human with age=%f\n", newhuman->m_age );

        return newhuman;
    }

    IndividualHumanMalaria::~IndividualHumanMalaria()
    {
        // deletion of infections handled by ~Individual
        // deletion of susceptibility handled by ~Individual
    }

    void IndividualHumanMalaria::PropagateContextToDependents()
    {
        IndividualHumanVector::PropagateContextToDependents();

        if( malaria_susceptibility == nullptr && susceptibility != nullptr)
        {
            if ( s_OK != susceptibility->QueryInterface(GET_IID(IMalariaSusceptibility), (void**)&malaria_susceptibility) )
            {
                throw QueryInterfaceException( __FILE__, __LINE__, __FUNCTION__, "susceptibility", "IMalariaSusceptibility", "Susceptibility" );
            }
        }
    }

    void IndividualHumanMalaria::setupInterventionsContainer()
    {
        vector_interventions = _new_ MalariaInterventionsContainer();
        interventions = vector_interventions;
    }

    void IndividualHumanMalaria::CreateSusceptibility(float imm_mod, float risk_mod)
    {
        SusceptibilityMalaria *newsusceptibility = SusceptibilityMalaria::CreateSusceptibility(dynamic_cast<IIndividualHumanContext*>(this), m_age, imm_mod, risk_mod);
        malaria_susceptibility = newsusceptibility;
        vector_susceptibility = newsusceptibility;
        susceptibility = newsusceptibility;

        // initialize CSP antibody pointer
        m_CSP_antibody = newsusceptibility->RegisterAntibody(MalariaAntibodyType::CSP, 0); // only type 0 for now
    }

    void IndividualHumanMalaria::setupMaternalAntibodies(IIndividualHumanContext* mother, INodeContext* node)
    {
        if ( mother )
        {
            IMalariaHumanContext* malaria_mother;
            if( s_OK != mother->QueryInterface(GET_IID(IMalariaHumanContext), (void**)&malaria_mother) )
            {
                throw QueryInterfaceException( __FILE__, __LINE__, __FUNCTION__, "mother", "IMalariaHumanContext", "IIndividualHumanContext" );
            }

            // If we are using individual pregnancies, initialize maternal antibodies dependent on mother's antibody history
            malaria_susceptibility->init_maternal_antibodies(malaria_mother->GetMalariaSusceptibilityContext()->get_fraction_of_variants_with_antibodies(MalariaAntibodyType::PfEMP1_major));
        }
        else
        {
            INodeMalaria* malaria_node;
            if( s_OK != node->QueryInterface(GET_IID(INodeMalaria), (void**)&malaria_node) )
            {
                throw QueryInterfaceException( __FILE__, __LINE__, __FUNCTION__, "node", "INodeMalaria", "INodeContext" );
            }

            // If we are doing births at population level, use the average antibody history of all possible mothers
            malaria_susceptibility->init_maternal_antibodies(malaria_node->GetMaternalAntibodyFraction());
        }
    }


    IInfection* IndividualHumanMalaria::createInfection(suids::suid _suid)
    {
        // If m_initial_infected_hepatocytes=0, this function is being called from initial infections at t=0 or an Outbreak intervention.
        // In that case, default to the mean number of infected hepatocytes.
        int initial_hepatocytes = int(IndividualHumanMalariaConfig::mean_sporozoites_per_bite * IndividualHumanMalariaConfig::base_sporozoite_survival_fraction);

        // A non-zero value for m_initial_infected_hepatocytes means
        // that the Poisson draw in ApplyTotalBitingExposure initiated the AcquireNewInfection function call.
        // (Or that a MalariaChallenge intervention has been called and ChallengeWithSporozoites returned true)
        if(m_initial_infected_hepatocytes > 0)
        {
            initial_hepatocytes = m_initial_infected_hepatocytes;
        }

        // Reset initial infected hepatocyte variable for next time steps
        m_initial_infected_hepatocytes = 0;

        return InfectionMalaria::CreateInfection(dynamic_cast<IIndividualHumanContext*>(this), _suid, initial_hepatocytes);
    }


    void IndividualHumanMalaria::ExposeToInfectivity(float dt, TransmissionGroupMembership_t transmissionGroupMembership)
    {
        // Here we track exposure in the antibody_capacity to CSP.  This is not used at the moment to reduce bite probability of success, but it could in the future as knowledge improves

        // Is there any exposure at all from mosquitoes?
        double infectivity = parent->GetTotalContagion();

        // If so, mark its presence and provide a slow increase in antibodies to CSP over time and exposure.
        // This is just a proxy for exposure with an approximately 3-year time constant.
        if ( infectivity > 0 )
        {
            m_CSP_antibody->UpdateAntibodyCapacityByRate( dt, infectivity * 0.001);  // 0.001 ~= 1 / (3*DAYSPERYEAR)
            m_CSP_antibody->SetAntigenicPresence(true);
        }
        else
        {
            m_CSP_antibody->SetAntigenicPresence(false);
        }

        // Now decide whether the individual's exposure results in an infection
        TransmissionGroupMembership_t dummy( -1 );
        IndividualHumanVector::ExposeToInfectivity( dt, dummy );
    }

    void IndividualHumanMalaria::ApplyTotalBitingExposure()
    {
        // Unlike for vector sims, don't do the draw on probability of *any* infectious bites, i.e. EXPCDF(-m_total_exposure)
        // We will instead do a Poisson draw on how many infectious bites, with the equivalent behavior for zero bites.
        // Downstream (in createInfection), we will use the number of infected hepatocytes to initialize new infections.

        // First calculate number of infectious bites
        int n_infectious_bites = CalculateInfectiousBites();
        if ( n_infectious_bites == 0 ) return;

        // Then do sporozoite challenge (caching inital infected hepatocyte count for createInfection)
        if ( ChallengeWithBites( n_infectious_bites ) )
        {
            // If there is a non-zero number of initial infected hepatocytes,
            // choose a strain based on a weighted draw over values from all vector-to-human pools and acquire infection
            float strain_cdf_draw = GetRng()->e() * m_total_exposure;
            std::vector<strain_exposure_t>::iterator it = std::lower_bound( m_strain_exposure.begin(), m_strain_exposure.end(), strain_cdf_draw, compare_strain_exposure_float_less()); 
            IStrainIdentity * pSI = &(it->first);
            AcquireNewInfection( pSI );
        }
    }

    int IndividualHumanMalaria::CalculateInfectiousBites()
    {
        return GetRng()->Poisson( m_total_exposure );
    }

    bool IndividualHumanMalaria::ChallengeWithBites( int n_infectious_bites )
    {
        int n_sporozoites = n_infectious_bites * IndividualHumanMalariaConfig::mean_sporozoites_per_bite;
        return ChallengeWithSporozoites( n_sporozoites );
    }

    bool IndividualHumanMalaria::ChallengeWithSporozoites( int n_sporozoites )
    {
        // TODO: make the survival probability some function of m_CSP_antibody->GetAntibodyConcentration();
        //       - compare to challenge studies with naive individuals (0) and with African adults (1)
        //       - compare to RTS,S phase IIa trial for no protection (NP), delayed patent parasitemia (DL), and protected (PR) population mean anti-CSP concentration
        //       - compare to RTS,S phase IIb and phase III trials for decay profile of anti-CSP concentration and protection for those with 20-2000 times higher antibody levels
        //       - verify the decay is not structurally problematic (need to specialize MalariaAntibodyCSP::UpdateAntibodyConcentration to deal with >1 concentrations)
        //       - verify the decay is scientifically accurate based on RTS,S trial numbers (again feed into if concentration>1 block of UpdateAntibodyConcentration)

        float sporozoite_survival_prob = IndividualHumanMalariaConfig::base_sporozoite_survival_fraction;

        float anti_csp_concentration = m_CSP_antibody->GetAntibodyConcentration();
        if ( anti_csp_concentration > 0 )
        {
            // TODO: is this an adequate functional form?
            sporozoite_survival_prob *= ( 1.0f - Sigmoid::variableWidthSigmoid( log10(anti_csp_concentration), log10(IndividualHumanMalariaConfig::antibody_csp_killing_threshold), IndividualHumanMalariaConfig::antibody_csp_killing_invwidth ) ); 
        }

        m_initial_infected_hepatocytes = GetRng()->Poisson( n_sporozoites * sporozoite_survival_prob );
        return ( m_initial_infected_hepatocytes > 0 ) ? true : false;
    }

    void IndividualHumanMalaria::UpdateInfectiousness(float dt)
    {
        if (dt > 0) // N.B. the old pattern where dt=0 was for retrieval has been superceded by GetInfectiousness.  at t=0, though, UpdateInfectivity is called with dt=0.
        {
            UpdateGametocyteCounts(dt);
            infectiousness = 0;
        }
        if (m_female_gametocytes <= 0)
        {
            // this individual has no gametocytes and is not infectious.
            infectiousness = 0;
        }
        else
        {
            // infectiousness calculated based on total gametocytes
            // contagion deposited by outcrossing of strains proportional to relative concentrations
            DepositInfectiousnessFromGametocytes();
        }
    }

    void IndividualHumanMalaria::UpdateGametocyteCounts(float dt)
    {
        // Check for mature gametocyte drug killing
        IMalariaDrugEffects* imde = nullptr;
        if (s_OK != GetInterventionsContext()->QueryInterface(GET_IID(IMalariaDrugEffects), (void **)&imde))
        {
            throw QueryInterfaceException( __FILE__, __LINE__, __FUNCTION__, "GetInterventionsContext()", "IMalariaDrugEffects", "IIndividualHumanInterventionsContext" );
        }

        m_male_gametocytes = 0;
        m_female_gametocytes = 0;
        m_female_gametocytes_by_strain.clear();

        // Loop over infections and add newly matured male and female gametocytes by strain
        for (auto infection : infections)
        {
            // Cast from Infection --> InfectionMalaria
            IInfectionMalaria *tempinf = nullptr;
            if( infection->QueryInterface( GET_IID(IInfectionMalaria), (void**)&tempinf ) != s_OK )
            {
                throw QueryInterfaceException( __FILE__, __LINE__, __FUNCTION__, "Infection", "IInfectionMalaria", "tempinf" );
            }

            // Get mature gametocytes at this time step
            // N.B. malariaCycleGametocytes is called once per asexual cycle (several days),
            int64_t tmp_male_gametocytes   = tempinf->get_MaleGametocytes(   GametocyteStages::Mature );
            int64_t tmp_female_gametocytes = tempinf->get_FemaleGametocytes( GametocyteStages::Mature );

            // Decay half-life--Sinden, R. E., G. A. Butcher, et al. (1996). "Regulation of Infectivity of Plasmodium to the Mosquito Vector." Advances in Parasitology 38: 53-117.
            // Smalley, M. E. and R. E. Sinden (1977). "Plasmodium falciparum gametocytes: their longevity and infectivity." Parasitology 74(01): 1-8.
            StrainIdentity strain;
            infection->GetInfectiousStrainID( &strain );
            float drugGametocyteKill = imde->get_drug_gametocyteM( strain );
            double pkill = EXPCDF( -dt * (0.277 + drugGametocyteKill) ); // half-life of 2.5 days corresponds to a decay time constant of 3.6 days, 0.277 = 1/3.6

            // apply kill probability to mature gametocytes, including effect of gametocytocidal drugs
            tempinf->apply_MatureGametocyteKillProbability( pkill );

            // No mature gametocytes
            if ( (tmp_male_gametocytes == 0) && (tmp_female_gametocytes == 0) ) continue;

            // Add new gametocytes to those carried over (and not killed) from the previous time steps
            StrainIdentity tmp_strainIDs;
            infection->GetInfectiousStrainID( &tmp_strainIDs );

            m_female_gametocytes_by_strain[tmp_strainIDs] += tmp_female_gametocytes;
            m_female_gametocytes += tmp_female_gametocytes;

            m_male_gametocytes += tmp_male_gametocytes;
        }
    }

    void IndividualHumanMalaria::DepositInfectiousnessFromGametocytes()
    {
        release_assert( malaria_susceptibility );
        m_inv_microliters_blood = malaria_susceptibility->get_inv_microliters_blood();

        // Now add a factor to limit inactivation of gametocytes by inflammatory cytokines
        // Important for slope of infectivity v. gametocyte counts
        double fever_effect = malaria_susceptibility->get_cytokines();
        fever_effect = Sigmoid::basic_sigmoid(SusceptibilityMalariaConfig::cytokine_gametocyte_inactivation, float(fever_effect));
        // fever_effect*=0.95;

        // Infectivity is reviewed by Sinden, R. E., G. A. Butcher, et al. (1996). "Regulation of Infectivity of Plasmodium to the Mosquito Vector." Advances in Parasitology 38: 53-117.
        // model based on data from Jeffery, G. M. and D. E. Eyles (1955). "Infectivity to Mosquitoes of Plasmodium Falciparum as Related to Gametocyte Density and Duration of Infection." Am J Trop Med Hyg 4(5): 781-789.
        // and Schneider, P., J. T. Bousema, et al. (2007). "Submicroscopic Plasmodium falciparum gametocyte densities frequently result in mosquito infection." Am J Trop Med Hyg 76(3): 470-474.
        // 2 due to bloodmeal and other factor due to conservative estimate for macrogametocyte ookinete transition, can be a higher reduction due to immune response
        // that factor also includes effect of successful fertilization with male gametocytes
        infectiousness = float(EXPCDF(-double(m_female_gametocytes) * m_inv_microliters_blood * MICROLITERS_PER_BLOODMEAL * SusceptibilityMalariaConfig::base_gametocyte_mosquito_survival * (1.0 - fever_effect))); //temp function, see vector_parameter_scratch.xlsx

        LOG_DEBUG_F("Gametocytes: %lld (male) %lld (female).  Infectiousness=%0.2g\n", m_male_gametocytes, m_female_gametocytes, infectiousness);

        // Effects of transmission-reducing immunity.  N.B. interventions on vector success are not here, since they depend on vector-population-specific behavior
        float modtransmit = susceptibility->getModTransmit() * interventions->GetInterventionReducedTransmit();
        infectiousness *= modtransmit;

        // Host weight is the product of MC weighting and relative biting
        float host_vector_weight = float(GetMonteCarloWeight() * GetRelativeBitingRate());
        float weighted_infectiousnesss = host_vector_weight * infectiousness;

        // Effects from vector intervention container
        IVectorInterventionsEffects* ivie = nullptr;
        if ( s_OK !=  interventions->QueryInterface(GET_IID(IVectorInterventionsEffects), (void**)&ivie) )
        {
            throw QueryInterfaceException( __FILE__, __LINE__, __FUNCTION__, "interventions", "IVectorInterventionsEffects", "IndividualHumanVector" );
        }

        // Here we deposit human-to-vector infectiousness based on proportional outcrossing of strain IDs
        gametocytes_strain_map_t::const_iterator gc1,gc2,end=m_female_gametocytes_by_strain.end();
        for (gc1=m_female_gametocytes_by_strain.begin(); gc1!=end; ++gc1)
        {
            for (gc2=gc1; gc2!=end; ++gc2)
            {
                // Fractional weight is product of component weights
                float strain_weight;
                if (gc1==gc2)
                {
                    strain_weight = pow( float(gc1->second) / float(m_female_gametocytes), 2 );
                    DepositFractionalContagionByStrain( weighted_infectiousnesss * strain_weight, ivie, gc1->first.GetCladeID(), gc1->first.GetGeneticID() );
                    continue;
                }

                // Two off-diagonal contributions given how pairwise iteration is done in this loop
                strain_weight = 2.0f * gc1->second * gc2->second / pow( float(m_female_gametocytes), 2 );
                LOG_DEBUG_F("Crossing two strains with weight %0.2f and %0.2f\n", gc1->second/float(m_female_gametocytes), gc2->second/(float)m_female_gametocytes);

                // Genetic ID from first component
                int geneticID = gc1->first.GetGeneticID();
                if ( geneticID != gc2->first.GetGeneticID() ) 
                {
                    // One outcrossing realization if genetic IDs are different
                    geneticID = GenomeMarkers::FromOutcrossing( GetRng(), geneticID, gc2->first.GetGeneticID() );
                    LOG_DEBUG_F("Crossing geneticID %d + %d --> %d\n", gc1->first.GetGeneticID(), gc2->first.GetGeneticID(), geneticID);
                }

                // Deposit fractional infectiousness to each of indoor and outdoor pools
                if ( gc1->first.GetCladeID() == gc2->first.GetCladeID() )
                {
                    DepositFractionalContagionByStrain( weighted_infectiousnesss * strain_weight, ivie, gc1->first.GetCladeID(), geneticID );
                }
                else
                {
                    // Deposit half the weight to each if cladeIDs are different.  
                    // Note that geneticID is not outcrossed independently for the different clade IDs.
                    DepositFractionalContagionByStrain( 0.5f * weighted_infectiousnesss * strain_weight, ivie, gc1->first.GetCladeID(), geneticID );
                    DepositFractionalContagionByStrain( 0.5f * weighted_infectiousnesss * strain_weight, ivie, gc2->first.GetCladeID(), geneticID );
                    LOG_DEBUG_F("Depositing contagion with cladeIDs %d and %d for geneticID=%d\n", gc1->first.GetCladeID(), gc2->first.GetCladeID(), geneticID);
                }
            }
        }
    }

    void IndividualHumanMalaria::DepositFractionalContagionByStrain(float weight, IVectorInterventionsEffects* ivie, float cladeID, float geneticID)
    {
        StrainIdentity id = StrainIdentity(cladeID, geneticID);
        parent->DepositFromIndividual( id, weight*ivie->GetblockIndoorVectorTransmit(),  NodeVector::human_indoor,  TransmissionRoute::TRANSMISSIONROUTE_HUMAN_TO_VECTOR_INDOOR );
        parent->DepositFromIndividual( id, weight*ivie->GetblockOutdoorVectorTransmit(), NodeVector::human_outdoor, TransmissionRoute::TRANSMISSIONROUTE_HUMAN_TO_VECTOR_OUTDOOR );
    }

    void IndividualHumanMalaria::ResetClinicalSymptoms()
    {
        ZERO_ARRAY(m_clinical_symptoms);
    }
 
    void IndividualHumanMalaria::ClearNewInfectionState()
    {
         IndividualHumanVector::ClearNewInfectionState();
         ResetClinicalSymptoms();
    }

    void IndividualHumanMalaria::AddClinicalSymptom(ClinicalSymptomsEnum::Enum symptom)
    {
        m_clinical_symptoms[symptom] = true;

        // Trigger observers of new clinical and severe malaria episodes
        if ( symptom == ClinicalSymptomsEnum::CLINICAL_DISEASE )
        {
            broadcaster->TriggerObservers( GetEventContext(), EventTrigger::NewClinicalCase );
        }
        else if ( symptom == ClinicalSymptomsEnum::SEVERE_DISEASE )
        {
            broadcaster->TriggerObservers( GetEventContext(), EventTrigger::NewSevereCase );
        }
    }

    bool IndividualHumanMalaria::HasClinicalSymptom(ClinicalSymptomsEnum::Enum symptom) const
    {
        return m_clinical_symptoms[symptom];
    }

    IMalariaSusceptibility* IndividualHumanMalaria::GetMalariaSusceptibilityContext() const
    {
        return malaria_susceptibility;
    }

    std::vector< std::pair<int,int> > IndividualHumanMalaria::GetInfectingStrainIds() const
    {
        std::vector< std::pair<int,int> > strainIds;
        StrainIdentity strain;
        for (auto inf : infections)
        {
            inf->GetInfectiousStrainID(&strain);
            strainIds.push_back( std::make_pair( strain.GetCladeID(), strain.GetGeneticID() ) );
        }
        return strainIds;
    }

    void IndividualHumanMalaria::CountPositiveSlideFields( RANDOMBASE * rng, int nfields, float uL_per_field, int& positive_asexual_fields, int& positive_gametocyte_fields) const
    {
        float asexual_density = GetMalariaSusceptibilityContext()->get_parasite_density();
        float gametocyte_density = (m_female_gametocytes + m_male_gametocytes) * m_inv_microliters_blood;

        float asexual_prob_per_field = EXPCDF(-asexual_density * uL_per_field);
        float gametocyte_prob_per_field = EXPCDF(-gametocyte_density * uL_per_field);

        // binomial random draw (or poisson/normal approximations thereof)
        positive_asexual_fields = rng->binomial_approx2(nfields, asexual_prob_per_field);
        positive_gametocyte_fields = rng->binomial_approx2(nfields, gametocyte_prob_per_field);
    }

    void IndividualHumanMalaria::PerformMalariaTest(int test_type)
    {
        if (test_type == MALARIA_TEST_BLOOD_SMEAR)
        {
            // first asexual parasites
            m_parasites_detected_by_blood_smear = malaria_susceptibility->CheckParasiteCountWithTest(test_type);

            // then gametocytes
            float gametocyte_density = (m_female_gametocytes + m_male_gametocytes) * m_inv_microliters_blood;
            m_gametocytes_detected = float(1.0 / params()->malaria_params->parasiteSmearSensitivity * GetRng()->Poisson(params()->malaria_params->parasiteSmearSensitivity * gametocyte_density));
        }
        else if (test_type == MALARIA_TEST_NEW_DIAGNOSTIC)
        {
            m_parasites_detected_by_new_diagnostic = (malaria_susceptibility->CheckForParasitesWithTest(test_type)) ? 1.0f : 0.0f;
        }
        else
            LOG_WARN("Tried to perform unknown malaria diagnostic!\n");
    }

    bool  IndividualHumanMalaria::CheckForParasitesWithTest(int test_type) const
    {
        if(test_type == MALARIA_TEST_BLOOD_SMEAR)
        {
            return (m_parasites_detected_by_blood_smear > 0.0f);
        }
        else if (test_type == MALARIA_TEST_NEW_DIAGNOSTIC)
        {
            return (m_parasites_detected_by_new_diagnostic > 0.0f);
        }
        else
        {
            LOG_WARN("Tried to check results of unknown malaria diagnostic!\n");
            return false;
        }
    }

    float IndividualHumanMalaria::CheckParasiteCountWithTest(int test_type) const 
    {
        if(test_type == MALARIA_TEST_BLOOD_SMEAR)
            return m_parasites_detected_by_blood_smear;
        else if (test_type == MALARIA_TEST_NEW_DIAGNOSTIC)
            return m_parasites_detected_by_new_diagnostic;
        else
        {
            LOG_WARN("Tried to check parasite count detected by unknown malaria diagnostic!\n");
            return 0.0f;
        }
    }

    bool  IndividualHumanMalaria::HasFever() const
    {
        return malaria_susceptibility->get_fever() > GET_CONFIGURABLE(SimulationConfig)->malaria_params->feverDetectionThreshold;
    }

    float IndividualHumanMalaria::CheckGametocyteCountWithTest(int test_type) const
    {
        if(test_type == MALARIA_TEST_BLOOD_SMEAR)
        {
            return m_gametocytes_detected;
        }
        else if (test_type == MALARIA_TEST_NEW_DIAGNOSTIC)
        {
            LOG_WARN("Gametocyte count detected by MALARIA_TEST_NEW_DIAGNOSTIC type is not currently defined.\n");
            return 0.0f;
        }
        else
        {
            LOG_WARN("Tried to check gametocyte count detected by unknown malaria diagnostic!\n");
            return 0.0f;
        }
    }

    float IndividualHumanMalaria::GetGametocyteDensity() const
    {
        return (m_female_gametocytes + m_male_gametocytes) * m_inv_microliters_blood;
    }

    void IndividualHumanMalaria::Drug_Report()
    {
        //malaria_susceptibility->Drug_Action_Report();
    }

    /*void IndividualHumanMalaria::malaria_infectivity_report()
    {
        EnvPtr->Report.Plotting << m_female_gametocytes * m_inv_microliters_blood << '\t' << infectiousness << std::endl;
    }*/

    const SimulationConfig*
    IndividualHumanMalaria::params()
    const
    {
        return GET_CONFIGURABLE(SimulationConfig);
    }

    void IndividualHumanMalaria::SetContextTo(INodeContext* context)
    {
        IndividualHumanVector::SetContextTo(context);

        // We could be called from CreateHuman in which case we haven't yet initialized malaria_susceptibility.
        if (malaria_susceptibility != nullptr)
        {
            m_CSP_antibody = malaria_susceptibility->RegisterAntibody(MalariaAntibodyType::CSP, 0);
        }
    }

    REGISTER_SERIALIZABLE(IndividualHumanMalaria);

    void serialize(IArchive& ar, IndividualHumanMalaria::gametocytes_strain_map_t& mapping)
    {
        size_t count = ar.IsWriter() ? mapping.size() : -1;
        ar.startArray(count);
        if (ar.IsWriter())
        {
            for (auto& entry : mapping)
            {
                StrainIdentity* strain = const_cast<StrainIdentity*>(&entry.first);
                ar.startObject();
                    ar.labelElement("key"); StrainIdentity::serialize(ar, strain);
                    ar.labelElement("value") & entry.second;
                ar.endObject();
            }
        }
        else
        {
            for (size_t i = 0; i < count; i++)
            {
                StrainIdentity* strain;
                int64_t value;
                ar.startObject();
                    ar.labelElement("key"); StrainIdentity::serialize(ar, strain);
                    ar.labelElement("value") & value;
                ar.endObject();
                mapping[*strain] = value;
            }
        }
        ar.endArray();
    }

    void IndividualHumanMalaria::serialize(IArchive& ar, IndividualHumanMalaria* obj)
    {
        IndividualHumanVector::serialize(ar, obj);
        IndividualHumanMalaria& individual = *obj;
        ar.labelElement("m_inv_microliters_blood") & individual.m_inv_microliters_blood;
        ar.labelElement("m_male_gametocytes") & individual.m_male_gametocytes;
        ar.labelElement("m_female_gametocytes") & individual.m_female_gametocytes;
        ar.labelElement("m_female_gametocytes_by_strain"); Kernel::serialize(ar, individual.m_female_gametocytes_by_strain);
        ar.labelElement("m_parasites_detected_by_blood_smear") & individual.m_parasites_detected_by_blood_smear;
        ar.labelElement("m_parasites_detected_by_new_diagnostic") & individual.m_parasites_detected_by_new_diagnostic;
        ar.labelElement("m_gametocytes_detected") & individual.m_gametocytes_detected;
        ar.labelElement("m_clinical_symptoms"); ar.serialize( individual.m_clinical_symptoms, ClinicalSymptomsEnum::CLINICAL_SYMPTOMS_COUNT);
        ar.labelElement("m_initial_infected_hepatocytes") & individual.m_initial_infected_hepatocytes;

        // ----------------------------------------------------------------------
        // --- This is a pointer to an object held in the Susceptibility object. 
        // --- It will be re-set after de-serialization. See SetContextTo()
        // ----------------------------------------------------------------------
        //ar.labelElement("m_CSP_antibody") & individual.m_CSP_antibody;
        // ----------------------------------------------------------------------
    }
}

