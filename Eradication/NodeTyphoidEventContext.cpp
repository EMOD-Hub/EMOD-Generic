/***************************************************************************************************

Copyright (c) 2018 Intellectual Ventures Property Holdings, LLC (IVPH) All rights reserved.

EMOD is licensed under the Creative Commons Attribution-Noncommercial-ShareAlike 4.0 License.
To view a copy of this license, visit https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode

***************************************************************************************************/

#pragma once

#include "stdafx.h"
#include "NodeTyphoidEventContext.h"

#include "NodeTyphoid.h"
#include "SimulationEventContext.h"
#include "Debug.h"
#include "Log.h"
#include "IdmString.h"

SETUP_LOGGING( "NodeTyphoidEventContext" )

namespace Kernel
{
    NodeTyphoidEventContextHost::NodeTyphoidEventContextHost(Node* _node) 
        : NodeEventContextHost(_node) 
    {
        current_shedding_attenuation_environment.insert( std::make_pair( "default", 1.0f) );
        current_dose_attenuation_environment.insert( std::make_pair( "default", 1.0f) );
        current_exposures_attenuation_environment.insert( std::make_pair( "default", 1.0f) );
    }

    NodeTyphoidEventContextHost::~NodeTyphoidEventContextHost()
    {
    }

    QueryResult NodeTyphoidEventContextHost::QueryInterface( iid_t iid, void** ppinstance )
    {
        release_assert(ppinstance);
        return e_NOINTERFACE;
    }

    INodeTyphoidInterventionEffects* NodeTyphoidEventContextHost::GetNodeTyphoidInterventionEffects()
    {
        return static_cast<INodeTyphoidInterventionEffects*>(this);
    }

    void NodeTyphoidEventContextHost::ApplyReducedSheddingEffect( float rate, const std::string& props )
    {
        LOG_VALID_F( "%s: Set current_shedding_attenuation_environment to %f for node (property=%s).\n", __FUNCTION__, rate, props.c_str() );
        current_shedding_attenuation_environment[ props ] = rate;
    }

    void NodeTyphoidEventContextHost::ApplyReducedDoseEffect( float rate, const std::string &props )
    {
        LOG_VALID_F( "%s: Set current_dose_attenuation_environment to %f for node (property=%s).\n", __FUNCTION__, rate, props.c_str() );
        current_dose_attenuation_environment[ props ] = rate;
    }

    void NodeTyphoidEventContextHost::ApplyReducedNumberExposuresEffect( float rate, const std::string &props )
    {
        LOG_VALID_F( "%s: Set current_exposures_attenuation_environment to %f for node (property=%s).\n", __FUNCTION__, rate , props.c_str() );
        current_exposures_attenuation_environment[ props ] = rate;
    }

    float NodeTyphoidEventContextHost::GetEnviroDepositAttenuation( const IPKeyValueContainer * pProps ) const
    {
        // We're just assuming SINGLE property for now!!!
        // // Go through the list of all key-value pair strings in our map and see if any of them match the 
        // props of the caller.  
        // If props is empty AND local attenuator map has "default" value, use that.
        for( const auto &kv: current_shedding_attenuation_environment )
        {
            auto ipkeyvaluerawstr = kv.first; 
            // The following code block is very repetitive; thinking about using a private function that takes the map var
            // as a parameter; but while that' might be clever, it might just be less easy to read.
            if( /*pProps->Size() == 0 &&*/ ipkeyvaluerawstr == "default" )
            {
                LOG_VALID_F( "%s: Returning current_shedding_attenuation_environment (node) of %f for 'default' properties.\n", __FUNCTION__, float(kv.second) );
                return kv.second;
            }

            if( pProps->Contains( ipkeyvaluerawstr  ) )
            {
                LOG_VALID_F( "%s: Returning current_shedding_attenuation_environment (node) of %f for properties %s.\n", __FUNCTION__, float(kv.second), ipkeyvaluerawstr.c_str() );
                return kv.second;
            }
        }
        return 1.0f; // valid if user has a property but intervention only distributed to another prop
    }

    float NodeTyphoidEventContextHost::GetEnviroExposuresAttenuation( const IPKeyValueContainer * pProps ) const
    {
        for( const auto &kv: current_exposures_attenuation_environment )
        {
            auto ipkeyvaluerawstr = kv.first;
            if( /*pProps->Size() == 0 &&*/ ipkeyvaluerawstr == "default" )
            {
                LOG_VALID_F( "%s: Returning current_shedding_attenuation_environment (node) of %f for 'default' properties.\n", __FUNCTION__, float(kv.second) );
                return kv.second;
            }

            if( pProps->Contains( ipkeyvaluerawstr  ) )
            {
                LOG_VALID_F( "%s: Returning current_exposures_attenuation_environment (node) of %f for properties %s.\n", __FUNCTION__, float(kv.second), ipkeyvaluerawstr.c_str() );
                return kv.second;
            }
        }
        return 1.0f; // valid if user has a property but intervention only distributed to another prop
    }

    float NodeTyphoidEventContextHost::GetEnviroDoseAttenuation( const IPKeyValueContainer * pProps ) const
    {
        for( const auto &kv: current_dose_attenuation_environment )
        {
            auto ipkeyvaluerawstr = kv.first;
            if( /*pProps->Size() == 0 &&*/ ipkeyvaluerawstr == "default" )
            {
                LOG_VALID_F( "%s: Returning current_shedding_attenuation_environment (node) of %f for 'default' properties.\n", __FUNCTION__, float(kv.second) );
                return kv.second;
            }

            if( pProps->Contains( ipkeyvaluerawstr  ) )
            {
                LOG_VALID_F( "%s: Returning current_dose_attenuation_environment (node) of %f for properties %s.\n", __FUNCTION__, float(kv.second), ipkeyvaluerawstr.c_str() );
                return kv.second;
            }
        }
        return 1.0f; // valid if user has a property but intervention only distributed to another prop
    }
}
