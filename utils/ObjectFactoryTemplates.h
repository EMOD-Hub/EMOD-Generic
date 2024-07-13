/***************************************************************************************************

Copyright (c) 2018 Intellectual Ventures Property Holdings, LLC (IVPH) All rights reserved.

EMOD is licensed under the Creative Commons Attribution-Noncommercial-ShareAlike 4.0 License.
To view a copy of this license, visit https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode

***************************************************************************************************/

#pragma once

#include "Configure.h"
#include "ObjectFactory.h"
#ifndef WIN32
#include <cxxabi.h>
#endif

namespace Kernel
{
    template<class IObject, class Factory>
    Factory * ObjectFactory<IObject,Factory>::getInstance()
    {
        return (_instance != nullptr) ? _instance : _instance = new Factory();
    }

    template<class IObject, class Factory>
    ObjectFactory<IObject,Factory>::ObjectFactory()
        : m_RegisteredClasses()
        , m_FactorySchema()
    {
    }

    template<class IObject, class Factory>
    ObjectFactory<IObject,Factory>::~ObjectFactory()
    {
    }

    template<class IObject, class Factory>
    void ObjectFactory<IObject,Factory>::Register( const char *classname, instantiator_function_t _if )
    {
        std::string classnameString( classname );
        m_RegisteredClasses[ classnameString ] = _if;
    }

    template<class IObject, class Factory>
    json::QuickBuilder ObjectFactory<IObject,Factory>::GetSchema()
    {
        JsonConfigurable::_dryrun = true;
        for( auto& entry : m_RegisteredClasses )
        {
            // --------------------------------------------------------------
            // --- Create a fake JSON object with the "class" element defined
            // --- so the CreateInstance() method will construct the object
            // --------------------------------------------------------------
            const std::string& class_name = entry.first;
            json::Object fakeJson;
            fakeJson["class"] = json::String(class_name);
            instantiator_function_t creator = entry.second;

            // ----------------------
            // --- Create the object
            // ----------------------
            ISupports* pForSchema = creator();
            release_assert( pForSchema );

            // --------------------------------
            // --- Get the schema for the class
            // --------------------------------
            IConfigurable* p_config = pForSchema->GetConfigurable();
            release_assert( p_config );

            Configuration* fakeConfig = Configuration::CopyFromElement( fakeJson );
            p_config->Configure( fakeConfig );
            json::QuickBuilder schema = p_config->GetSchema();

            // -------------------------------------------------------
            // --- Add the schema to the schema for the whole factory
            // -------------------------------------------------------
            ModifySchema( schema, pForSchema );
            schema[std::string("class")] = json::String(class_name);
            m_FactorySchema[ class_name ] = schema;

            delete fakeConfig;
            fakeConfig = nullptr;
        }

        json::QuickBuilder retSchema = json::QuickBuilder(m_FactorySchema);
        return retSchema;
    }

    template<class IObject, class Factory>
    IObject* ObjectFactory<IObject, Factory>::CreateInstance( const Configuration *pConfig,
                                                              const char* parameterName,
                                                              bool nullOrEmptyOrNoClassNotError )
    {
        // -------------------------------
        // --- Create object and configure
        // -------------------------------
        bool is_valid_element = CheckElement( pConfig, parameterName, nullOrEmptyOrNoClassNotError );

        IObject* obj = nullptr;
        if( is_valid_element )
        {
            obj = CreateInstanceFromSpecs<IObject>( pConfig, m_RegisteredClasses );
            if( obj == nullptr )
            {
                std::stringstream ss;
                ss << "Error loading '" << GET_CONFIG_STRING( pConfig, "class" ) << "' via "
                    << "'" << GetFactoryName() << "' for '" << parameterName << "' in <" << pConfig->GetDataLocation() << ">.";
                throw FactoryCreateFromJsonException( __FILE__, __LINE__, __FUNCTION__, ss.str().c_str() );
            }
        }
        return obj;
    }

    template<class IObject, class Factory>
    IObject* ObjectFactory<IObject, Factory>::CreateInstance( const json::Element& rJsonElement,
                                                              const std::string& rDataLocation,
                                                              const char* parameterName,
                                                              bool nullOrEmptyOrNoClassNotError )
    {
        Configuration* p_config = Configuration::CopyFromElement( rJsonElement, rDataLocation );
        IObject* p_obj = CreateInstance( p_config, parameterName, nullOrEmptyOrNoClassNotError );
        delete p_config;
        return p_obj;
    }

    template<class IObject, class Factory>
    bool ObjectFactory<IObject, Factory>::CheckElement( const Configuration* pConfig,
                                                        const char* parameterName,
                                                        bool nullOrEmptyOrNoClassNotError )
    {
        if( pConfig->operator const json::Element &().Type() == json::NULL_ELEMENT )
        {
            if( nullOrEmptyOrNoClassNotError )
            {
                return false;
            }
            else
            {
                std::stringstream ss;
                ss << "'" << GetFactoryName() << "' found the element to be NULL for '" << parameterName << "' in <" << pConfig->GetDataLocation() << ">.";
                throw FactoryCreateFromJsonException( __FILE__, __LINE__, __FUNCTION__, ss.str().c_str() );
            }
        }

        if( pConfig->operator const json::Element &().Type() != json::OBJECT_ELEMENT )
        {
            std::stringstream ss;
            ss << "'" << GetFactoryName() << "' found the element specified by '" << parameterName << "'\n"
               << "to NOT be a JSON object in <" << pConfig->GetDataLocation() << ">.";
            throw FactoryCreateFromJsonException( __FILE__, __LINE__, __FUNCTION__, ss.str().c_str() );
        }
        json::Object json_obj = pConfig->As<json::Object>();

        if( json_obj.Size() == 0 )
        {
            if( nullOrEmptyOrNoClassNotError )
            {
                return false;
            }
            else
            {
                std::stringstream ss;
                ss << "'" << GetFactoryName() << "' found zero elements in JSON for '" << parameterName << "' in <" << pConfig->GetDataLocation() << ">.";
                throw FactoryCreateFromJsonException( __FILE__, __LINE__, __FUNCTION__, ss.str().c_str() );
            }
        }
        
        if( !pConfig->Exist( "class" ) )
        {
            if( nullOrEmptyOrNoClassNotError )
            {
                return false;
            }
            else
            {
                std::stringstream json_text;
                json::Writer::Write( *pConfig, json_text );

                std::stringstream ss;
                ss << "'"
                    << GetFactoryName()
                    << "' could not instantiate object from json.\n"
                    << "The 'class' parameter was not specified in parameter '" << parameterName << "'\n"
                    << "that has the following JSON:\n"
                    << json_text.str();
                throw FactoryCreateFromJsonException( __FILE__, __LINE__, __FUNCTION__, ss.str().c_str() );
            }
        }

        std::string class_name = GET_CONFIG_STRING( pConfig, "class" );
        if( m_RegisteredClasses.find( class_name ) == m_RegisteredClasses.end() )
        {
            if( nullOrEmptyOrNoClassNotError )
            {
                return false;
            }
            else
            {
                std::stringstream json_text;
                json::Writer::Write( *pConfig, json_text );

                std::stringstream ss;
                ss << "'" << GetFactoryName() << "' could not find class '" << class_name << "'.\n"
                   << "It was specified in parameter '" << parameterName << "' in <" << pConfig->GetDataLocation() << ">.\n"
                   << "This parameter had the following JSON:\n"
                   << json_text.str() << "\n"
                   << "Valid classes for this parameter are:\n";
                for( auto entry : m_RegisteredClasses )
                {
                    ss << entry.first << "\n";
                }
                throw FactoryCreateFromJsonException( __FILE__, __LINE__, __FUNCTION__, ss.str().c_str() );
            }
        }
        return true;
    }

    template<class IObject, class Factory>
    std::string ObjectFactory<IObject, Factory>::GetFactoryName()
    {
        std::string factory_name = typeid(*this).name();
#ifdef WIN32
        factory_name = factory_name.substr( 14 ); // remove "class Kernel::"
#else
        factory_name = abi::__cxa_demangle( factory_name.c_str(), 0, 0, nullptr );
        factory_name = factory_name.substr( 8 ); // remove "Kernel::"
#endif
        return factory_name;
    }

    template<class IObject, class Factory>
    void ObjectFactory<IObject, Factory>::CheckSimType( IObject* pObject )
    {
        if( JsonConfigurable::_dryrun )
        {
            return;
        }
        release_assert(pObject);

        std::string sim_type_str = GET_CONFIG_STRING( EnvPtr->Config, "Simulation_Type" );

        IConfigurable* p_config = pObject->GetConfigurable();
        release_assert(p_config);

        json::Object json_object = p_config->GetSchema().As<json::Object>();

        json::QuickInterpreter sim_type_qi( json_object[ "Sim_Types" ] );
        json::QuickInterpreter sim_type_array( sim_type_qi.As<json::Array>() );

        int num_types = sim_type_qi.As<json::Array>().Size();
        assert( num_types > 0 );

        if( std::string( sim_type_array[ 0 ].As<json::String>() ) == std::string( "*" ) )
        {
            // wild card means it is valid for any simulation type
            return;
        }

        std::string supported;
        for( int i = 0; i < num_types; i++ )
        {
            std::string supported_sim_type = std::string( sim_type_array[ i ].As<json::String>() );
            if( sim_type_str == supported_sim_type )
            {
                return;
            }
            supported += "'" + supported_sim_type + "', ";
        }
        supported = supported.substr( 0, supported.length() - 2 );

        std::stringstream ss;
        ss << "The '" << typeid(*pObject).name() << "' object is not valid with the current 'Simulation_Type' (='" << sim_type_str << "').  ";
        ss << "This object is only supported for the following simulation types: " << supported;
        throw GeneralConfigurationException( __FILE__, __LINE__, __FUNCTION__, ss.str().c_str() );
    }

}
