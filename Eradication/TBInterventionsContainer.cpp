/***************************************************************************************************

Copyright (c) 2018 Intellectual Ventures Property Holdings, LLC (IVPH) All rights reserved.

EMOD is licensed under the Creative Commons Attribution-Noncommercial-ShareAlike 4.0 License.
To view a copy of this license, visit https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode

***************************************************************************************************/

#include "stdafx.h"

#include "TBInterventionsContainer.h"
#include "TBContexts.h"

#include "Drugs.h" // for IDrug interface
#include "Log.h"
#include "Sugar.h"
#include "IHealthSeekingBehavior.h" //for IHealthSeekingBehavior interface
#include "NodeEventContext.h"    // for INodeEventContext (ICampaignCostObserver)
#include "EventTrigger.h"
#include "IIndividualHumanContext.h"

SETUP_LOGGING( "TBInterventionsContainer" )

namespace Kernel
{
    BEGIN_QUERY_INTERFACE_DERIVED(TBInterventionsContainer, InterventionsContainer)
        HANDLE_INTERFACE(ITBInterventionsContainer)
    END_QUERY_INTERFACE_DERIVED(TBInterventionsContainer, InterventionsContainer)

    TBInterventionsContainer::TBInterventionsContainer() :
        InterventionsContainer()
        , m_is_tb_tx_naive_TBIVC(true)
        , m_failed_tx_TBIVC(false)
        , m_ever_relapsed_TBIVC(false)
    {
    }

    TBInterventionsContainer::~TBInterventionsContainer()
    {
    }

    ITBInterventionsContainer* TBInterventionsContainer::GetContainerTB()
    {
        return static_cast<ITBInterventionsContainer*>(this);
    }

    void TBInterventionsContainer::InfectiousLoopUpdate( float dt )
    {
        TB_drug_effects.clear();

        InterventionsContainer::InfectiousLoopUpdate( dt );
    }

    void TBInterventionsContainer::UpdateHealthSeekingBehaviors(float new_probability_of_seeking)
    {
        int hsb_count = 0;
        for (auto intervention : interventions)
        {
            IHealthSeekingBehavior* p_hsb = intervention->GetHSB();
            if ( p_hsb )
            {
                p_hsb->UpdateProbabilityofSeeking( new_probability_of_seeking );
                hsb_count++;
            }
        }
        LOG_DEBUG_F("Number of IHealthSeekingBehavior in intervention list %d\n", hsb_count); 
    }

    int TBInterventionsContainer::GetNumTBDrugsActive()
    {
        std::list<IDrug*> list_of_tb_drugs = GetDrugInterventions();
        LOG_DEBUG_F("Number of IDrug in intervention list %d\n",  list_of_tb_drugs.size());
        
        int num_tb_drugs_active = 0;
        for (IDrug* ITB_Drug : list_of_tb_drugs)
        {
            release_assert( ITB_Drug );
            if (ITB_Drug->GetDrugCurrentEfficacy() > 0)
            {
                num_tb_drugs_active +=1;
            }
        }
        LOG_DEBUG_F("Number of IDrug in intervention list that are active %d\n", num_tb_drugs_active);

        return num_tb_drugs_active;
    }
   
    void TBInterventionsContainer::ApplyDrugVaccineReducedAcquireEffect(float prob)
    {
        InterventionsContainer::UpdateIVAcquireRate(prob, IVRoute::ALL);
    }

    void TBInterventionsContainer::ApplyDrugVaccineReducedTransmitEffect(float prob)
    {
        InterventionsContainer::UpdateIVTransmitRate(prob, IVRoute::ALL);
    }

    void TBInterventionsContainer::ApplyTBDrugEffects(TBDrugEffects_t effects, TBDrugType::Enum drug_type)
    {
        TB_drug_effects[drug_type].clearance_rate    += effects.clearance_rate;
        TB_drug_effects[drug_type].inactivation_rate += effects.inactivation_rate;
        TB_drug_effects[drug_type].resistance_rate += effects.resistance_rate;
        TB_drug_effects[drug_type].relapse_rate += effects.relapse_rate;
        TB_drug_effects[drug_type].mortality_rate += effects.mortality_rate;

    }

    void TBInterventionsContainer::UpdateTreatmentStatus( const EventTrigger::Enum& new_treatment_status )
    {
        //this function is called when the drug is started and stopped/expires
        
        //first get the pointer to the person, parent is the generic individual
        IIndividualHumanTB* tb_patient = parent->GetIndividualTB();
        LOG_DEBUG_F( "Individual %d disease state is active %d, latent %d, pending relapse %d \n", parent->GetSuid().data,tb_patient->HasActiveInfection(), tb_patient->HasLatentInfection(), tb_patient->HasPendingRelapseInfection() );
        
        IIndividualEventBroadcaster* broadcaster = parent->GetEventContext()->GetNodeEventContext()->GetIndividualEventBroadcaster();

        if( new_treatment_status == EventTrigger::TBStartDrugRegimen )
        {
            LOG_DEBUG_F( "Individual %d starting the drug, broadcasting that this person started \n", parent->GetSuid().data );
            broadcaster->TriggerObservers( parent->GetEventContext(), EventTrigger::TBStartDrugRegimen );

            LOG_DEBUG( "Started drug regimen, update tx_naive flag to false in TB IVC \n" );
            m_is_tb_tx_naive_TBIVC = false;
        }
        else if( new_treatment_status == EventTrigger::TBStopDrugRegimen )
        {
            //figure out if they failed or relapsed now that they have stopped their drug regimen
            //future should check if this works with multiple drugs on board at the same time

            //if the person still has active disease at the time this drug expires, they have failed
            if( tb_patient->HasActiveInfection() )
            {
                LOG_DEBUG_F( "Individual %d finished the drug but still has active disease, broadcasting that this person failed \n", parent->GetSuid().data );
                broadcaster->TriggerObservers( parent->GetEventContext(), EventTrigger::TBFailedDrugRegimen );

                //Update the person's failed flag to false in the TBInterventionsContainer
                LOG_DEBUG( "Finished drug regimen but failed, update failed flag to true in TB IVC \n" );
                m_failed_tx_TBIVC = true;
            }
            else if( tb_patient->HasPendingRelapseInfection() )
            {
                LOG_DEBUG_F( "Individual %d finished the drug but is pending relapse broadcasting that this person is pending relapse\n", parent->GetSuid().data );
                broadcaster->TriggerObservers( parent->GetEventContext(), EventTrigger::TBRelapseAfterDrugRegimen );

                //Update the person's failed flag to false in the TBInterventionsContainer
                LOG_DEBUG( "Finished drug regimen but now pending relapse, update ever relapsed flag to true in TB IVC \n" );
                m_ever_relapsed_TBIVC = true;
            }
            else
            {
                LOG_DEBUG( "the person finished the drug but did not fail or relapse" );
            }
        }
        else
        {
            LOG_DEBUG("the person finished the drug but did not fail or relapse");
        }
    }

    bool TBInterventionsContainer::GetTxNaiveStatus() const
    {
        return m_is_tb_tx_naive_TBIVC;
    }

    bool TBInterventionsContainer::GetTxFailedStatus() const
    {
        return m_failed_tx_TBIVC;
    }

    bool TBInterventionsContainer::GetTxEverRelapsedStatus() const
    {
        return m_ever_relapsed_TBIVC;
    }

    TBDrugEffectsMap_t TBInterventionsContainer::GetDrugEffectsMap() 
    { 
        return TB_drug_effects; 
    }

    REGISTER_SERIALIZABLE(TBInterventionsContainer);

    void TBDrugEffects_t::serialize(IArchive& ar, TBDrugEffects_t& effects)
    {
        ar.startObject();
            ar.labelElement("clearance_rate") & effects.clearance_rate;
            ar.labelElement("inactivation_rate") & effects.inactivation_rate;
            ar.labelElement("resistance_rate") & effects.resistance_rate;
            ar.labelElement("relapse_rate") & effects.relapse_rate;     // Boost serialization didn't include this member.
            ar.labelElement("mortality_rate") & effects.mortality_rate; // Boost serialization didn't include this member.
        ar.endObject();
    }

    // clorton TODO - could serialize(map<A, B>) be templatized in IArchive.h?
    void serialize(IArchive& ar, TBDrugEffectsMap_t& map)
    {
        size_t count = ar.IsWriter() ? map.size() : -1;
        ar.startArray(count);
        if (ar.IsWriter())
        {
            for (auto& entry : map)
            {
                ar.startObject();
                    ar.labelElement("key") & (uint32_t&)entry.first;
                    ar.labelElement("value") & entry.second;
                ar.endObject();
            }
        }
        else
        {
            for (size_t i = 0; i < count; i++)
            {
                TBDrugType::Enum type;
                TBDrugEffects_t effects;
                ar.startObject();
                    ar.labelElement("key") & (uint32_t&)type;
                    ar.labelElement("value") & effects;
                ar.endObject();
                map[type] = effects;
            }
        }
        ar.endArray();
    }

    void TBInterventionsContainer::serialize(IArchive& ar, TBInterventionsContainer* obj)
    {
        InterventionsContainer::serialize(ar, obj);
        TBInterventionsContainer& interventions = *obj;
        ar.labelElement("TB_drug_effects"); Kernel::serialize(ar, interventions.TB_drug_effects);
        ar.labelElement("m_is_tb_tx_naive_TBIVC") & interventions.m_is_tb_tx_naive_TBIVC;
        ar.labelElement("m_failed_tx_TBIVC") & interventions.m_failed_tx_TBIVC;
        ar.labelElement("m_ever_relapsed_TBIVC") & interventions.m_ever_relapsed_TBIVC;
    }
}

