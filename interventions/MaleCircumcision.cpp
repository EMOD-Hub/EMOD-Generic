/***************************************************************************************************

Copyright (c) 2018 Intellectual Ventures Property Holdings, LLC (IVPH) All rights reserved.

EMOD is licensed under the Creative Commons Attribution-Noncommercial-ShareAlike 4.0 License.
To view a copy of this license, visit https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode

***************************************************************************************************/

#include "stdafx.h"
#include "MaleCircumcision.h"
#include "InterventionEnums.h"
#include "IndividualEventContext.h"
#include "IIndividualHumanContext.h"
#include "NodeEventContext.h"
#include "ISTIInterventionsContainer.h"
#include "IIndividualHumanSTI.h"
#include "SimulationEnums.h"

SETUP_LOGGING( "MaleCircumcision" )

namespace Kernel
{
    IMPLEMENT_FACTORY_REGISTERED(MaleCircumcision)
    
    MaleCircumcision::MaleCircumcision()
        : BaseIntervention()
        , m_ReducedAcquire(1.0)
        , m_ApplyIfHigherReducedAcquire(false)
        , m_DistrbutedEventTrigger()
        , ivc(nullptr)
        , has_been_applied(false)
    {
        initSimTypes( 2, "STI_SIM", "HIV_SIM" );
    }

    bool MaleCircumcision::Configure( const Configuration * inputJson )
    {
        initConfigTypeMap( "Circumcision_Reduced_Acquire", &m_ReducedAcquire, Male_Circumcision_Reduced_Acquire_DESC_TEXT, 0.0f, 1.0f, 0.60f );
        initConfigTypeMap( "Apply_If_Higher_Reduced_Acquire", &m_ApplyIfHigherReducedAcquire, Male_Circumcision_Apply_If_Higher_Reduced_Acquire_DESC_TEXT, false ); 
        initConfig( "Distributed_Event_Trigger", m_DistrbutedEventTrigger, inputJson, MetadataDescriptor::Enum("Distributed_Event_Trigger", Male_Circumcision_Distributed_Event_Trigger_DESC_TEXT, MDD_ENUM_ARGS( EventTrigger ) ) );

        return BaseIntervention::Configure( inputJson );
    }

    bool MaleCircumcision::Distribute( IIndividualHumanInterventionsContext* context, ICampaignCostObserver* const pCCO )
    {
        ivc = context->GetContainerSTI();
        release_assert(ivc);

        // -----------------------------------------------------------------
        // --- Make sure the the person is male and not already circumcised
        // -----------------------------------------------------------------
        IIndividualHumanSTI* p_sti = context->GetParent()->GetIndividualSTI();
        release_assert(p_sti);

        if( context->GetParent()->GetEventContext()->GetGender() == Gender::FEMALE )
        {
            LOG_WARN("Trying to circumcise females.\n");
            return false;
        }

        // --------------------------------------------------------------------------
        // --- If the man already has received the intervention, don't take any more.
        // --- First come, first accepted.
        // --- This avoid problem where he gets more than one intervention in the
        // --- same timestep.
        // !!! Added the !IsCircumcised() due to requirement to not expire!!!
        // --------------------------------------------------------------------------
        if( (context->GetInterventionsByType(typeid(MaleCircumcision).name()).size() > 0) && !p_sti->IsCircumcised() )
        {
            return false;
        }

        if( p_sti->IsCircumcised() )
        {
            // -----------------------------------------------------------------------------------------------
            // --- If the individual is already circumcised,
            // --- we want to model the ability to get a better circumcision (i.e. m_ApplyIfHigherReducedAcquire),
            // --- and this intervention is a better one (higher reduced require), then
            // --- distribute the intervention.
            // --- The code is the negative because we want to return false.
            // -----------------------------------------------------------------------------------------------
            if( !m_ApplyIfHigherReducedAcquire || (m_ReducedAcquire <= ivc->GetCircumcisedReducedAcquire()) )
            {
                return false;
            }
        }

        // -------------------------------
        // --- Distribute the intervention
        // -------------------------------
        bool ret = BaseIntervention::Distribute( context, pCCO );

        // ----------------------------------------------------------------------------------
        // --- If the user defines a trigger, broadcast that the circumcision was distributed
        // ----------------------------------------------------------------------------------
        if( ret && m_DistrbutedEventTrigger != EventTrigger::NoTrigger )
        {
            IIndividualEventBroadcaster* broadcaster = context->GetParent()->GetEventContext()->GetNodeEventContext()->GetIndividualEventBroadcaster();
            broadcaster->TriggerObservers( context->GetParent()->GetEventContext(), m_DistrbutedEventTrigger );
        }
        return ret;
    }

    void MaleCircumcision::Update( float dt )
    {
        if( !BaseIntervention::UpdateIndividualsInterventionStatus() ) return;

        if( !has_been_applied )
        {
            ivc->ApplyCircumcision( m_ReducedAcquire );
            has_been_applied = true;
        }
        // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
        // !!! This should expire but ReferenceTrackingEventCoordinator needs
        // !!! the interventions to stay around.
        // !!! If changing to allow the intervention to expire, fix Distribute().
        // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
        //expired = true;
    }

    void MaleCircumcision::SetContextTo( IIndividualHumanContext *context )
    {
        BaseIntervention::SetContextTo( context );

        ivc = context->GetInterventionsContext()->GetContainerSTI();
        release_assert(ivc);
    }

    bool MaleCircumcision::ApplyIfHigherReducedAcquire() const
    {
        return m_ApplyIfHigherReducedAcquire;
    }

    float MaleCircumcision::GetReducedAcquire() const
    {
        return m_ReducedAcquire;
    }

    REGISTER_SERIALIZABLE(MaleCircumcision);

    void MaleCircumcision::serialize(IArchive& ar, MaleCircumcision* obj)
    {
        BaseIntervention::serialize( ar, obj );
        MaleCircumcision& mc = *obj;
        ar.labelElement("m_ReducedAcquire"             ) & mc.m_ReducedAcquire;
        ar.labelElement("m_ApplyIfHigherReducedAcquire") & mc.m_ApplyIfHigherReducedAcquire;
        ar.labelElement("m_DistrbutedEventTrigger"     ) & (uint32_t&)mc.m_DistrbutedEventTrigger;
    }
}
