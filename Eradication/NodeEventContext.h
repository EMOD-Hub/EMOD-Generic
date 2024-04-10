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
#include "Types.h"
#include "SimulationEnums.h"
#include "Configuration.h"
#include "suids.hpp"
#include "IndividualEventContext.h"
#include "Interventions.h" // for IIndividualHumanEventObserver
#include "ExternalNodeId.h"

namespace Kernel
{
    class  Simulation;
    struct IBTNTI;      // BasicTestNodeTargetedIntervention

    struct INodeContext;
    struct IIndividualEventBroadcaster;
    struct NodeDemographics;
    class  Node;
    class  StrainIdentity;

    class  IndividualHuman;
    class  RANDOMBASE;
    struct IdmDateTime;

    class  INodeMalariaInterventionEffects;
    struct INodeTyphoidInterventionEffects;
    struct INodeVectorInterventionEffects;
    class  INodeVectorInterventionEffectsApply;

    struct ITravelLinkedDistributionSource : ISupports
    {
        virtual void ProcessDeparting(IIndividualHumanEventContext *dc) = 0;
        virtual void ProcessArriving(IIndividualHumanEventContext *dc) = 0;
    };

    struct IVisitIndividual
    {
        virtual bool visitIndividualCallback(IIndividualHumanEventContext* ihec, float& incrementalCostOut, ICampaignCostObserver* pICCO ) = 0;
        virtual int  GetMaxEvents() const = 0;
    };

    struct IDMAPI INodeEventContext : public ISupports 
    {
        // If you prefer lambda functions/functors, you can use this.
        typedef std::function<void (IIndividualHumanEventContext*)> individual_visit_function_t;
        virtual void VisitIndividuals(individual_visit_function_t func) = 0;
        virtual int VisitIndividuals(IVisitIndividual* pIndividualVisitImpl) = 0;

        virtual const IdmDateTime& GetTime() const = 0;

        // to update any node-owned interventions
        virtual void UpdateInterventions(float = 0.0f) = 0;
        
        // methods to install hooks for arrival/departure/birth/etc
        enum TravelEventType { Arrival = 0, Departure = 1 };
        virtual void RegisterTravelDistributionSource(ITravelLinkedDistributionSource *tles, TravelEventType type) = 0;
        virtual void UnregisterTravelDistributionSource(ITravelLinkedDistributionSource *tles, TravelEventType type) = 0;

        virtual const suids::suid & GetId() const = 0;
        virtual void SetContextTo(INodeContext* context) = 0;
        virtual void SetupTxRoutes() = 0;

        virtual std::list<INodeDistributableIntervention*> GetInterventionsByType(const std::string& type_name) = 0;
        virtual std::list<INodeDistributableIntervention*> GetInterventionsByName(const std::string& intervention_name) = 0;
        virtual void PurgeExisting( const std::string& iv_name ) = 0;
        virtual void PurgeExistingByName( const std::string& iv_name ) = 0;
        virtual bool ContainsExisting( const std::string& iv_name ) = 0;
        virtual bool ContainsExistingByName( const std::string& iv_name ) = 0;

        virtual void AddImportCases( const StrainIdentity* outbreak_strainID, float import_age, int num_cases_per_node, float inf_prob, float female_prob, float mc_weight) = 0;

        virtual bool IsInPolygon(float* vertex_coords, int numcoords) = 0;
        virtual bool IsInPolygon( const json::Array &poly ) = 0;
        virtual bool IsInExternalIdSet( const std::list<ExternalNodeId_t>& nodelist ) = 0;
        virtual RANDOMBASE* GetRng() = 0;

        virtual int GetIndividualHumanCount() const = 0;
        virtual ExternalNodeId_t GetExternalId() const = 0;

        virtual INodeContext*                GetNodeContext()                = 0;
        virtual INodeInterventionConsumer*   GetNodeInterventionConsumer()   = 0;
        virtual ICampaignCostObserver*       GetCampaignCostObserver()       = 0;
        virtual IIndividualEventBroadcaster* GetIndividualEventBroadcaster() = 0;

        virtual INodeMalariaInterventionEffects*     GetNodeMalariaInterventionEffects()     = 0;
        virtual INodeTyphoidInterventionEffects*     GetNodeTyphoidInterventionEffects()     = 0;
        virtual INodeVectorInterventionEffects*      GetNodeVectorInterventionEffects()      = 0;
        virtual INodeVectorInterventionEffectsApply* GetNodeVectorInterventionEffectsApply() = 0;

        virtual void  IncrementCampaignCost(float cost)                          = 0;

        virtual void  UpdateBirthRateMultiplier(float mult_val)                                      = 0;
        virtual void  UpdateConnectionModifiers(float inbound, float outbound)                       = 0;
        virtual void  UpdateInfectivityMultiplier(float mult_val, TransmissionRoute::Enum txRoute)   = 0;

        virtual float GetBirthRateMultiplier()                                      const = 0;
        virtual float GetInboundConnectionModifier()                                const = 0;
        virtual float GetOutboundConnectionModifier()                               const = 0;
        virtual float GetInfectivityMultiplier(TransmissionRoute::Enum txRoute)     const = 0;
    };

    struct IBTNTIConsumer : public ISupports
    {
        virtual void GiveBTNTI(IBTNTI* BTNTI) = 0;
        virtual const IBTNTI* GetBTNTI() = 0; // look at the BTNTI they have...why not, some campaigns may want to introspect on this sort of thing. returns NULL if there is no BTNTI currently
    };
}
