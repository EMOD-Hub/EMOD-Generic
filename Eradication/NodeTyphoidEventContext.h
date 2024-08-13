/***************************************************************************************************

Copyright (c) 2018 Intellectual Ventures Property Holdings, LLC (IVPH) All rights reserved.

EMOD is licensed under the Creative Commons Attribution-Noncommercial-ShareAlike 4.0 License.
To view a copy of this license, visit https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode

***************************************************************************************************/

#pragma once

#include "ISupports.h"
#include "NodeEventContext.h"
#include "NodeEventContextHost.h"
#include "TyphoidInterventionsContainer.h"
#include "Types.h"

namespace Kernel
{
    class Simulation;

    struct INodeTyphoidInterventionEffects : public ISupports
    {
        virtual float GetEnviroDepositAttenuation( const IPKeyValueContainer * props = nullptr )   const = 0;
        virtual float GetEnviroExposuresAttenuation( const IPKeyValueContainer * props = nullptr ) const = 0;
        virtual float GetEnviroDoseAttenuation( const IPKeyValueContainer * props = nullptr )      const = 0;

        virtual void ApplyReducedSheddingEffect( float rate, const std::string& properties_raw )         = 0;
        virtual void ApplyReducedDoseEffect( float rate, const std::string& properties_raw )             = 0;
        virtual void ApplyReducedNumberExposuresEffect( float rate, const std::string& properties_raw )  = 0;
    };

    // The purpose of this class is to the typhoid-specific interventions container for node-level interventions.
    // Like all interventions containers, it mediates between individuals and interventions via interventions-effects.
    // The first node-level typyoid-specific intervention is the TyphoidWASH, but the container shouldn't have any
    // intervention-specific hard-coded.  
    class NodeTyphoidEventContextHost :
        public NodeEventContextHost,
        public INodeTyphoidInterventionEffects
    {
        IMPLEMENT_NO_REFERENCE_COUNTING()

    public:
        NodeTyphoidEventContextHost(Node* _node);
        virtual ~NodeTyphoidEventContextHost();
 
        virtual INodeTyphoidInterventionEffects* GetNodeTyphoidInterventionEffects() override;

        // INodeTyphoidInterventionEffects
        virtual float GetEnviroDepositAttenuation( const IPKeyValueContainer * props = nullptr ) const override;
        virtual float GetEnviroExposuresAttenuation( const IPKeyValueContainer * props = nullptr ) const override;
        virtual float GetEnviroDoseAttenuation( const IPKeyValueContainer * props = nullptr ) const override;

        virtual void ApplyReducedSheddingEffect( float rate, const std::string& properties_raw ) override;
        virtual void ApplyReducedDoseEffect( float rate, const std::string& properties_raw ) override;
        virtual void ApplyReducedNumberExposuresEffect( float rate, const std::string& properties_raw ) override;

    protected: 

    private:
        NodeTyphoidEventContextHost() : NodeEventContextHost(nullptr) { }
        // We have maps because the effects operate on individual properties normally, and these are strings. 
        std::map< std::string, ProbabilityNumber > current_shedding_attenuation_environment;
        std::map< std::string, ProbabilityNumber > current_dose_attenuation_environment;
        std::map< std::string, ProbabilityNumber > current_exposures_attenuation_environment;
    };
}
