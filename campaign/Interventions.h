/***************************************************************************************************

Copyright (c) 2018 Intellectual Ventures Property Holdings, LLC (IVPH) All rights reserved.

EMOD is licensed under the Creative Commons Attribution-Noncommercial-ShareAlike 4.0 License.
To view a copy of this license, visit https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode

***************************************************************************************************/

#pragma once

#include <string>
#include <list>

#include "IdmApi.h"
#include "ISupports.h"
#include "Configure.h"                 // for JsonConfigurable
#include "InterventionEnums.h"
#include "Properties.h"
#include "NodeProperties.h"
#include "BroadcasterObserver.h"

#include "ISerializable.h"
#include "IArchive.h"

namespace Kernel
{
    struct IIndividualHumanContext;
    struct IIndividualHumanEventContext;
    struct IIndividualHumanInterventionsContext;
    struct IInterventionConsumer;
    struct ISTIInterventionsContainer;
    struct IHIVInterventionsContainer;
    struct IPolioDrugEffects;
    struct ITBInterventionsContainer;
    struct ITyphoidVaccineEffectsApply;
    struct IVectorInterventionsEffects;
    struct IMalariaDrugEffects;
    struct IMalariaDrugEffectsApply;
    struct IDrug;
    struct IHealthSeekingBehavior;
    struct IPolioVaccine;
    struct INodeEventContext;
    struct IEventCoordinator2;

    struct IDMAPI ICampaignCostObserver : ISupports
    {
        virtual void notifyCampaignExpenseIncurred( float expenseIncurred, const IIndividualHumanEventContext* pIndiv ) = 0;
    };

#pragma warning(push)
#pragma warning(disable: 4251) // See IdmApi.h for details
    struct IDMAPI IDistributableIntervention : ISerializable
    {
        // Distribute transfers ownership of this object to the context if it succeeds, the context becomes responsible for freeing it
        // returns false if cannot distribute to the individual represented by this context, for whatever reason
        virtual const std::string& GetName() const = 0;
        virtual bool Distribute(IIndividualHumanInterventionsContext *context, ICampaignCostObserver * const pICCO ) = 0;
        virtual void SetContextTo(IIndividualHumanContext *context) = 0;
        virtual void Update(float dt) = 0;
        virtual bool Expired() = 0;
        virtual void SetExpired( bool isExpired ) = 0;
        virtual void OnExpiration() = 0;
        virtual IDistributableIntervention* Clone()  = 0;
        virtual bool NeedsInfectiousLoopUpdate() const = 0;
        virtual bool NeedsPreInfectivityUpdate() const = 0;

        virtual IDrug*                     GetDrug()             = 0;
        virtual IMalariaDrugEffects*       GetMalariaDrug()      = 0;
        virtual IHealthSeekingBehavior*    GetHSB()              = 0;
        virtual IPolioVaccine*             GetPolioVax()         = 0;

        virtual ~IDistributableIntervention() { }
    };
#pragma warning(pop)

    // Interface for an individual's intervention container
    struct IIndividualHumanInterventionsContext : ISerializable // ISupports
    {
        // specific helpers for interoperating with the interventions container might go here
        virtual void SetContextTo(IIndividualHumanContext *context) = 0;
        virtual IIndividualHumanContext* GetParent() = 0;
        virtual std::list<IDistributableIntervention*> GetInterventionsByType(const std::string &type_name) = 0;
        virtual std::list<IDistributableIntervention*> GetInterventionsByName(const std::string &intervention_name) = 0;
        virtual void PurgeExisting( const std::string &iv_name ) = 0;
        virtual void PurgeExistingByName( const std::string &iv_name ) = 0;
        virtual bool ContainsExisting( const std::string &iv_name ) = 0;
        virtual bool ContainsExistingByName( const std::string &name ) = 0;

        virtual std::list<IDrug*> GetDrugInterventions() = 0;

        virtual void ChangeProperty( const char *property, const char* new_value ) = 0;

        virtual IInterventionConsumer*        GetInterventionConsumer()   = 0;
        virtual ISTIInterventionsContainer*   GetContainerSTI()           = 0;
        virtual IHIVInterventionsContainer*   GetContainerHIV()           = 0;
        virtual IPolioDrugEffects*            GetContainerPolio()         = 0;
        virtual ITBInterventionsContainer*    GetContainerTB()            = 0;
        virtual ITyphoidVaccineEffectsApply*  GetContainerTyphoid()       = 0;
        virtual IVectorInterventionsEffects*  GetContainerVector()        = 0;
        virtual IMalariaDrugEffectsApply*     GetMalariaDrugApply()       = 0;
        virtual IMalariaDrugEffects*          GetMalariaDrugStats()       = 0;

        virtual ~IIndividualHumanInterventionsContext() {}
    };

    struct IDMAPI INodeDistributableIntervention : ISupports
    {
        // Distribute transfers ownership of this object to the context if it succeeds, the context becomes responsible for freeing it
        // returns false if cannot distribute to the individual represented by this context, for whatever reason
        virtual const std::string& GetName() const = 0;
        virtual bool Distribute(INodeEventContext *context, IEventCoordinator2* pEC = nullptr ) = 0;
        virtual void SetContextTo(INodeEventContext *context) = 0;
        virtual void Update(float dt) = 0;
        virtual bool Expired() = 0;
        virtual void SetExpired( bool isExpired ) = 0;
        virtual void OnExpiration() = 0;
        virtual INodeDistributableIntervention * Clone()  = 0;

        virtual ~INodeDistributableIntervention() { }
    };

    struct IInterventionConsumer : ISupports
    {
        virtual bool GiveIntervention( IDistributableIntervention * pIV ) = 0;
    };

    struct IDMAPI INodeInterventionConsumer : ISupports
    {
        virtual bool GiveIntervention( INodeDistributableIntervention * pIV ) = 0;
    };

    struct IDMAPI IBaseIntervention : ISupports
    {
        virtual float GetCostPerUnit() const = 0;
    };

    // TODO - BaseInterventions looks concrete, but can't be instantiated. :(
    struct IDMAPI BaseIntervention : IDistributableIntervention, IBaseIntervention, JsonConfigurable
    {
        IMPLEMENT_DEFAULT_REFERENCE_COUNTING()

        virtual const std::string& GetName() const override { return name; };
        virtual void SetContextTo(IIndividualHumanContext *context) override;
        virtual float GetCostPerUnit() const override { return cost_per_unit; }
        virtual bool Expired() override ;
        virtual void SetExpired( bool isExpired ) override;
        virtual void OnExpiration() override;
        virtual bool NeedsInfectiousLoopUpdate() const { return false; }
        virtual bool NeedsPreInfectivityUpdate() const { return false; }

        virtual IDrug*                     GetDrug()          override     { return nullptr; }
        virtual IMalariaDrugEffects*       GetMalariaDrug()   override     { return nullptr; }
        virtual IHealthSeekingBehavior*    GetHSB()           override     { return nullptr; }
        virtual IPolioVaccine*             GetPolioVax()      override     { return nullptr; }

        virtual IConfigurable* GetConfigurable() override { return JsonConfigurable::GetConfigurable(); }

    protected:
        BaseIntervention();
        BaseIntervention( const BaseIntervention& );
        virtual ~BaseIntervention();
        virtual bool Configure(const Configuration* inputJson) override;
        virtual bool Distribute(IIndividualHumanInterventionsContext *context, ICampaignCostObserver * const pICCO ) override;

        virtual bool AbortDueToDisqualifyingInterventionStatus( IIndividualHumanContext* pHuman );
        virtual bool UpdateIndividualsInterventionStatus();

        static void serialize( IArchive& ar, BaseIntervention* obj );

        IIndividualHumanContext *parent;
        std::string name;
        float cost_per_unit;
        bool expired;
        bool dont_allow_duplicates;
        bool enable_iv_replacement;
        bool first_time;
        IPKeyValueContainer disqualifying_properties;
        IPKeyValue status_property;
        EventTrigger::Enum event_trigger_distributed;
        EventTrigger::Enum event_trigger_expired;
    };

    struct BaseNodeIntervention : INodeDistributableIntervention, IBaseIntervention, JsonConfigurable
    {
        IMPLEMENT_DEFAULT_REFERENCE_COUNTING()

    public:
        virtual bool Configure( const Configuration* inputJson ) override;
        virtual const std::string& GetName() const override { return name; };
        virtual float GetCostPerUnit() const override { return cost_per_unit; }
        virtual bool Expired() override;
        virtual void SetExpired( bool isExpired ) override;
        virtual void OnExpiration() override;
        virtual void SetContextTo( INodeEventContext *context ) override;

        virtual IConfigurable* GetConfigurable() override { return JsonConfigurable::GetConfigurable(); }

    protected:
        BaseNodeIntervention();
        BaseNodeIntervention( const BaseNodeIntervention& );
        virtual ~BaseNodeIntervention();

        virtual bool Distribute(INodeEventContext *context, IEventCoordinator2* pEC = nullptr ) override;

        virtual bool AbortDueToDisqualifyingInterventionStatus( INodeEventContext* context );
        virtual bool UpdateNodesInterventionStatus();

        static void serialize( IArchive& ar, BaseNodeIntervention* obj );

        INodeEventContext *parent;
        std::string name;
        float cost_per_unit;
        bool expired;
        bool dont_allow_duplicates;
        bool enable_iv_replacement;
        bool first_time;
        NPKeyValueContainer disqualifying_properties;
        NPKeyValue status_property;
    };
}
