/***************************************************************************************************

Copyright (c) 2018 Intellectual Ventures Property Holdings, LLC (IVPH) All rights reserved.

EMOD is licensed under the Creative Commons Attribution-Noncommercial-ShareAlike 4.0 License.
To view a copy of this license, visit https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode

***************************************************************************************************/

#include "stdafx.h"
#include "Configuration.h"
#include "ConfigurationImpl.h"
#include "Configure.h"
#include "Exceptions.h"
#include "IdmString.h"
#include "Log.h"
#include "Debug.h"
#include "Properties.h"
#include "NodeProperties.h"
#include "EventTrigger.h"
#include "EventTriggerNode.h"
#include "EventTriggerCoordinator.h"

#ifndef WIN32
#include <cxxabi.h>
#endif

SETUP_LOGGING( "JsonConfigurable" )

namespace Kernel
{
    // two functions. second is wrapper around this. Can consolidate into single.
    bool ignoreParameter( const json::QuickInterpreter * pJson, const char * condition_key, const char * condition_value )
    {
        if( condition_key != nullptr ) 
        {
            if( pJson &&
                pJson->Exist(condition_key) == true )
            {
                auto c_value = (*pJson)[condition_key];
                // condition_key is in config. Read value 

                if( condition_value == nullptr )
                {
                    // condition_value is null, so it's a bool and 1 (no other options)
                    auto c_value2 = (int) c_value.As<json::Number>();
                    if( c_value2 != 1 )
                    {
                        // Condition for using this param is false (mismatch), so returning
                        LOG_DEBUG_F( "bool condition_value found but is false/0. That makes this check fail.\n" );
                        return true;
                    }
                    else
                    {
                        LOG_DEBUG_F( "bool condition_value found and is true/1. That makes this check pass.\n" );
                        // Conditions match. Continue and return false at end.
                    }
                }
                else
                {
                    release_assert( condition_value );
                    // condition_value is not null, so it's a string (enum); let's read it.
                    auto c_value_from_config = GET_CONFIG_STRING(pJson, condition_key);
                    LOG_DEBUG_F( "string/enum condition_value (from config.json) = %s. Will check if matches schema condition_value (raw) = %s\n", c_value_from_config.c_str(), condition_value );
                    // see if schema condition value is multiples...
                    auto c_values = IdmString( condition_value ).split( ',' );
                    release_assert( c_values.size() > 0 );
                    LOG_DEBUG_F( "Found %d values in comma-separated list.\n", c_values.size() );
                    bool bFound = false;
                    for( std::string valid_condition_value : c_values )
                    {   
                        //remove spaces from valid conditions
                        valid_condition_value.erase(remove_if(valid_condition_value.begin(), valid_condition_value.end(), ::isspace), valid_condition_value.end());
                        LOG_DEBUG_F( "Comparing %s and %s.\n", valid_condition_value.c_str(), c_value_from_config.c_str() );
                        if( valid_condition_value == c_value_from_config )
                        // (enum) Condition for using this param is false, so returning.
                        {
                            bFound = true;
                        }
                    }
                    if( !bFound )
                    {
                        LOG_DEBUG_F( "string/enum condition_value (from config.json) not found in list (?) of valid values per schema. That makes this check fail.\n" );
                        return true;
                    }
                }
            } 
            else
            {
                // condition_key does not seem to exist in the json. That makes this fail.
                LOG_DEBUG_F( "condition_key %s does not seem to exist in the json. That makes this check fail.\n", condition_key );
                return true;
            }
        }
        return false;
    }

    bool ignoreParameter( const json::QuickInterpreter& schema, const json::QuickInterpreter * pJson )
    {
        if( schema.Exist( "depends-on" ) )
        {
            auto condition = getCondition( schema );
            std::string condition_key = std::get<0>( condition );
            std::string condition_value_str = std::get<1>( condition );
            
            const char * condition_value = condition_value_str.empty() ? nullptr : condition_value_str.c_str();
            

            if( ignoreParameter( pJson, condition_key.c_str(), condition_value ) )
            { 
                if( ignoreParameter( EnvPtr->Config, condition_key.c_str(), condition_value ) )
                {
                    return true;
                }
            }
        }
        else
        {
            LOG_DEBUG_F( "There is no dependency for this param.\n" );
        }
        return false;
    }

    std::pair<std::string, std::string> getCondition( const json::QuickInterpreter jsonObj )
    {
        std::string condition_value_str = "";
        std::string condition_key;

        if( jsonObj.Exist( "depends-on" ) )
        {
            auto condition = json_cast<const json::Object&>(jsonObj["depends-on"]);
            condition_key = condition.Begin()->name;
            const char * condition_value = nullptr;
            try {
                condition_value_str = (std::string) (json::QuickInterpreter( condition )[condition_key]).As<json::String>();
                condition_value = condition_value_str.c_str();
                LOG_DEBUG_F( "schema condition value appears to be string/enum: %s.\n", condition_value );
            }
            catch( ... )
            {
                //condition_value = std::to_string( (int) (json::QuickInterpreter( condition )[ condition_key ]).As<json::Number>() );
                LOG_DEBUG_F( "schema condition value appears to be bool, not string.\n" );
            }
        }
        return std::make_pair( condition_key, condition_value_str );
    }

    /// NodeSetConfig
    NodeSetConfig::NodeSetConfig()
    {}

    NodeSetConfig::NodeSetConfig(json::QuickInterpreter* qi)
    : _json(*qi)
    {
    }

    json::QuickBuilder
    NodeSetConfig::GetSchema()
    {
        json::QuickBuilder schema( jsonSchemaBase );
        auto tn = JsonConfigurable::_typename_label();
        auto ts = JsonConfigurable::_typeschema_label();
        schema[ tn ] = json::String( "idmType:NodeSet" );
        schema[ ts ]= json::Object();
        schema[ ts ][ "base" ] = json::String( "interventions.idmType.NodeSet" );
        //json::Writer::Write( schema, std::cout );
        return schema;
    }

    void
    NodeSetConfig::ConfigureFromJsonAndKey(
        const Configuration* inputJson,
        const std::string& key
    )
    {
        if( !inputJson->Exist( key ) )
        {
            throw MissingParameterFromConfigurationException( __FILE__, __LINE__, __FUNCTION__, inputJson->GetDataLocation().c_str(), key.c_str() );
        }

        _json = (*inputJson)[key];
        //std::cout << "NodeSetConfig::Configure called with json blob." << std::endl;
        //json::Writer::Write( _json, std::cout );
    }

    /// END NodeSetConfig

    /// EventConfig
    EventConfig::EventConfig()
    {}

    EventConfig::EventConfig(json::QuickInterpreter* qi)
        : _json(*qi)
    {
    }

    void
    EventConfig::ConfigureFromJsonAndKey(
        const Configuration* inputJson,
        const std::string& key
    )
    {
        if( !inputJson->Exist( key ) )
        {
            throw MissingParameterFromConfigurationException( __FILE__, __LINE__, __FUNCTION__, inputJson->GetDataLocation().c_str(), key.c_str() );
        }
        _json = (*inputJson)[key];
        //std::cout << "EventConfig::Configure called with json blob." << std::endl;
        //json::Writer::Write( _json, std::cout );
    }

    json::QuickBuilder
    EventConfig::GetSchema()
    {
        json::QuickBuilder schema( jsonSchemaBase );
        auto tn = JsonConfigurable::_typename_label();
        auto ts = JsonConfigurable::_typeschema_label();
        schema[ tn ] = json::String( "idmType:EventCoordinator" );
        schema[ ts ]= json::Object();
        schema[ ts ][ "base" ] = json::String( "interventions.idmType.EventCoordinator" );
        //json::Writer::Write( schema, std::cout );
        return schema;
    }

    /// InterventionConfig
    InterventionConfig::InterventionConfig()
    //: _qi( _json )
    {}

    InterventionConfig::InterventionConfig(json::QuickInterpreter* qi)
        : _json(*qi)
        //, _qi(*qi)
    {
        //json::Writer::Write( _json, std::cout );
        //std::cout << "Yay, we can write out our _json!" << std::endl;
    }

    void
    InterventionConfig::ConfigureFromJsonAndKey(
        const Configuration* inputJson,
        const std::string& key
    )
    {
        if( !inputJson->Exist( key ) )
        {
            throw MissingParameterFromConfigurationException( __FILE__, __LINE__, __FUNCTION__, inputJson->GetDataLocation().c_str(), key.c_str() );
        }
        // we need to set _json to inputJson
        _json = (*inputJson)[key];

        // This might be useful for debugging, but couldn't agree on how to whether to include or not. Uncomment if you need it.
        /*std::ostringstream msg;
        msg << "InterventionConfig::" << __FUNCTION__ << " called with json blob." << std::endl;
        json::Writer::Write( _json, msg );
        LOG_DEBUG_F( "%s", msg.str().c_str() );*/
    }

    json::QuickBuilder
    InterventionConfig::GetSchema()
    {
        json::QuickBuilder schema( jsonSchemaBase );
        auto tn = JsonConfigurable::_typename_label();
        auto ts = JsonConfigurable::_typeschema_label();
        schema[ tn ] = json::String( "idmType:Intervention" );
        schema[ ts ]= json::Object();
        schema[ ts ][ "base" ] = json::String( "interventions.idmAbstractType.Intervention" );
        //json::Writer::Write( schema, std::cout );
        return schema;
    }

    void InterventionConfig::serialize(IArchive& ar, InterventionConfig& config)
    {
#if defined(WIN32)
        if ( ar.IsWriter() )
        {
            std::ostringstream string_stream;
            json::Writer::Write( config._json, string_stream );
            ar & string_stream.str();
        }
        else
        {
            std::string json;
            ar & json;
            std::istringstream string_stream( json );
            json::Reader::Read( config._json, string_stream );
        }
#endif
    }

    IndividualInterventionConfig::IndividualInterventionConfig()
    {
    }

    json::QuickBuilder
    IndividualInterventionConfig::GetSchema()
    {
        json::QuickBuilder schema = InterventionConfig::GetSchema();
        auto tn = JsonConfigurable::_typename_label();
        auto ts = JsonConfigurable::_typeschema_label();
        schema[ tn ] = json::String( "idmType:IndividualIntervention" );
        schema[ ts ][ "base" ] = json::String( "interventions.idmAbstractType.IndividualIntervention" );
        return schema;
    }

    json::QuickBuilder
    NodeInterventionConfig::GetSchema()
    {
        json::QuickBuilder schema = InterventionConfig::GetSchema();
        auto tn = JsonConfigurable::_typename_label();
        auto ts = JsonConfigurable::_typeschema_label();
        schema[ tn ] = json::String( "idmType:NodeIntervention" );
        schema[ ts ][ "base" ] = json::String( "interventions.idmAbstractType.NodeIntervention" );
        return schema;
    }

    /// END OF InterventionConfig


    /// WaningConfig
    WaningConfig::WaningConfig()
    {}

    WaningConfig::WaningConfig(json::QuickInterpreter* qi)
    : _json(*qi)
    {
    }

    json::QuickBuilder
    WaningConfig::GetSchema()
    {
        json::QuickBuilder schema( jsonSchemaBase );
        auto tn = JsonConfigurable::_typename_label();
        auto ts = JsonConfigurable::_typeschema_label();
        schema[ tn ] = json::String( "idmType:WaningEffect" );
        schema[ ts ]= json::Object();
        schema[ ts ][ "base" ] = json::String( "interventions.idmType.WaningEffect" );
        //json::Writer::Write( schema, std::cout );
        return schema;
    }

    void
    WaningConfig::ConfigureFromJsonAndKey(
        const Configuration* inputJson,
        const std::string& key
    )
    {
        if( !inputJson->Exist( key ) )
        {
            throw MissingParameterFromConfigurationException( __FILE__, __LINE__, __FUNCTION__, inputJson->GetDataLocation().c_str(), key.c_str() );
        }
        _json = (*inputJson)[key];
        //std::cout << "WaningConfig::Configure called with json blob." << std::endl;
        //json::Writer::Write( _json, std::cout );
    }

    /// END WaningConfig

    namespace jsonConfigurable
    {
        ConstrainedString::ConstrainedString()
        :constraint_param(nullptr)
        {
        }

        ConstrainedString::ConstrainedString( const std::string &init_str )
        : constraint_param(nullptr)
        {
            *((std::string*)(this)) = init_str;
        }

        ConstrainedString::ConstrainedString( const char* init_str )
        : constraint_param(nullptr)
        {
            *((std::string*)(this)) = std::string( init_str );
        }

        const ConstrainedString&
        ConstrainedString::operator=( const std::string& new_value )
        {
            *((std::string*)(this)) = new_value;
            //release_assert( constraint_param );
            if( constraint_param && (constraint_param->count( new_value ) == 0) && (new_value != JsonConfigurable::default_string)  )
            {
                std::ostringstream msg;
                msg << "Constrained String" ;
                if( !parameter_name.empty() )
                {
                    msg << " (" << parameter_name << ")" ;
                }
                msg << " with specified value "
                    << new_value
                    << " invalid. Possible values are: " << std::endl ;
                for( auto value : (*constraint_param) )
                {
                    msg << value << std::endl;
                }
                throw GeneralConfigurationException( __FILE__, __LINE__, __FUNCTION__, msg.str().c_str() );
            }
            return *this ;
        }
    }

    const char * JsonConfigurable::default_description = "No Description Yet";
    const char * JsonConfigurable::default_string = "UNINITIALIZED STRING";
    bool JsonConfigurable::_dryrun = false;
    bool JsonConfigurable::_useDefaults = false;
    bool JsonConfigurable::_track_missing = true;
    bool JsonConfigurable::_possibleNonflatConfig = false;
    std::set< std::string > JsonConfigurable::empty_set;

    void updateSchemaWithCondition( json::Object& schema, const char* condition_key, const char* condition_value )
    {
        LOG_DEBUG_F( "Setting condition in schema for key %s (value=%s).\n", condition_key, ( condition_value ? condition_value : "1") );
        if( condition_key )
        {
            json::Object condition;
            if( condition_value )
            {
                condition[ condition_key ] = json::String( condition_value );
            }
            else
            { 
                condition[ condition_key ] = json::Number( 1 );
            }
            schema["depends-on"] = condition;
        }
    }

    JsonConfigurable::JsonConfigurable()
    : IConfigurable()
    , m_pData( nullptr )
    , jsonSchemaBase()
    {
        // -----------------------------------------------------------------------
        // --- We don't want to create the ConfigData in the constructor because
        // --- some subclasses are copied a lot and this causes this memory to be
        // --- created a lot when it is not needed.
        // -----------------------------------------------------------------------
    }

    JsonConfigurable::JsonConfigurable( const JsonConfigurable& rConfig )
    : IConfigurable()
    , m_pData( nullptr ) // !!! Don't copy stuff
    , jsonSchemaBase( rConfig.jsonSchemaBase )
    {
        if( rConfig.m_pData != nullptr )
        {
            release_assert( false );
        }
    }

    JsonConfigurable::~JsonConfigurable()
    {
        delete m_pData;
        m_pData = nullptr;
    }

    std::string JsonConfigurable::GetTypeName() const
    {
        std::string variable_type = typeid(*this).name();
#ifndef WIN32
        variable_type = abi::__cxa_demangle( variable_type.c_str(), 0, 0, nullptr );
        variable_type = variable_type.substr( 8 ); // remove "Kernel::"
#endif
        if( variable_type.find( "class Kernel::" ) == 0 )
        {
            variable_type = variable_type.substr( 14 );
        }
        else if( variable_type.find( "struct Kernel::" ) == 0 )
        {
            variable_type = variable_type.substr( 15 );
        }
        return variable_type;
    }

    JsonConfigurable::ConfigData* JsonConfigurable::GetConfigData()
    {
        // ---------------------------------------------------------------------------------------
        // --- We create the memory in this method for multiple reasons.
        // --- 1) We only create the memory when it is needed.  As stated above, this saves memory
        // --- objects are copied and don't need this memory at all.
        // --- 2) Since most objects call the set of initConfig() methods and then call Configure()
        // --- to initialize their variables, we want to delete this memory at the end of Configure().
        // --- However, there are some objects that call Configure() multiple times (call once to get
        // --- some of the parameters and call certain initConfigs() based on those parameters, call
        // --- a second time to initialize these new parameters).  This method allows us to create
        // --- the memory again if it is needed.
        // ---------------------------------------------------------------------------------------
        if( m_pData == nullptr )
        {
            m_pData = new ConfigData();
        }
        return m_pData;
    }

    json::Object& JsonConfigurable::GetSchemaBase()
    {
        return jsonSchemaBase;
    }

    void
    JsonConfigurable::initConfigTypeMap(
        const char* paramName,
        bool * pVariable,
        const char* description,
        bool defaultvalue,
        const char* condition_key, const char* condition_value
    )
    {
        LOG_DEBUG_F("initConfigTypeMap<bool>: %s\n", paramName);
        GetConfigData()->boolConfigTypeMap[ paramName ] = pVariable;
        json::Object newIntSchema;
        /* Use this when boolean configuration parameters are actually 'true'/'false'.
        newIntSchema["default"] = json::Boolean(defaultvalue); */
        newIntSchema["default"] = json::Number(defaultvalue ? 1 : 0);
        if ( _dryrun )
        {
            newIntSchema["description"] = json::String(description);
            newIntSchema["type"] = json::String( "bool" );
        }
        updateSchemaWithCondition( newIntSchema, condition_key, condition_value );
        jsonSchemaBase[paramName] = newIntSchema;
    }

    void
    JsonConfigurable::initConfigTypeMap(
        const char* paramName,
        int * pVariable,
        const char * description,
        int min, int max, int defaultvalue,
        const char* condition_key, const char* condition_value
    )
    {
        LOG_DEBUG_F( "initConfigTypeMap<int>: %s\n", paramName);
        GetConfigData()->intConfigTypeMap[ paramName ] = pVariable;
        json::Object newIntSchema;
        newIntSchema["min"] = json::Number(min);
        newIntSchema["max"] = json::Number(max);
        newIntSchema["default"] = json::Number(defaultvalue);
        if ( _dryrun )
        {
            newIntSchema["description"] = json::String(description);
            newIntSchema["type"] = json::String( "integer" );
        }
            
        updateSchemaWithCondition( newIntSchema, condition_key, condition_value );
        jsonSchemaBase[paramName] = newIntSchema;
    }

    void
        JsonConfigurable::initConfigTypeMap(
            const char* paramName,
            uint32_t * pVariable,
            const char * description,
            uint32_t min, uint32_t max, uint32_t defaultvalue,
            const char* condition_key, const char* condition_value
        )
    {
        LOG_DEBUG_F( "initConfigTypeMap<int>: %s\n", paramName );
        GetConfigData()->uint32ConfigTypeMap[ paramName ] = pVariable;
        json::Object newUint32Schema;
        newUint32Schema[ "min" ] = json::Number( min );
        newUint32Schema[ "max" ] = json::Number( max );
        newUint32Schema[ "default" ] = json::Number( defaultvalue );
        if( _dryrun )
        {
            newUint32Schema[ "description" ] = json::String( description );
            newUint32Schema[ "type" ] = json::String( "integer" );
        }

        updateSchemaWithCondition( newUint32Schema, condition_key, condition_value );
        jsonSchemaBase[ paramName ] = newUint32Schema;
    }

    void
    JsonConfigurable::initConfigTypeMap(
        const char* paramName,
        float * pVariable,
        const char * description,
        float min, float max, float defaultvalue,
        const char* condition_key, const char* condition_value
    )
    {
        LOG_DEBUG_F( "initConfigTypeMap<float>: %s\n", paramName);
        GetConfigData()->floatConfigTypeMap[ paramName ] = pVariable;
        json::Object newFloatSchema;
            newFloatSchema["min"] = json::Number(min);
        newFloatSchema["max"] = json::Number(max);
        newFloatSchema["default"] = json::Number(defaultvalue);
        if ( _dryrun )
        {
            newFloatSchema["description"] = json::String(description);
            newFloatSchema["type"] = json::String( "float" ); 
        }
        updateSchemaWithCondition( newFloatSchema, condition_key, condition_value );
        jsonSchemaBase[paramName] = newFloatSchema;
    }

    void
    JsonConfigurable::initConfigTypeMap(
        const char* paramName,
        double * pVariable,
        const char * description,
        double min, double max, double defaultvalue,
        const char* condition_key, const char* condition_value
    )
    {
        LOG_DEBUG_F( "initConfigTypeMap<double>: %s\n", paramName);
        GetConfigData()->doubleConfigTypeMap[ paramName ] = pVariable;
        json::Object newDoubleSchema;
        if ( _dryrun )
        {
            newDoubleSchema["description"] = json::String(description);
            newDoubleSchema["type"] = json::String("double");
        }
        newDoubleSchema["min"] = json::Number(min);
        newDoubleSchema["max"] = json::Number(max);
        newDoubleSchema["default"] = json::Number(defaultvalue);
        updateSchemaWithCondition( newDoubleSchema, condition_key, condition_value );
        jsonSchemaBase[paramName] = newDoubleSchema;
    }

    void
    JsonConfigurable::initConfigTypeMap(
        const char* paramName,
        std::string * pVariable,
        const char * description,
        const std::string& default_str,
        const char* condition_key, const char* condition_value
    )
    {
        LOG_DEBUG_F( "initConfigTypeMap<string>: %s\n", paramName);
        GetConfigData()->stringConfigTypeMap[ paramName ] = pVariable;
        json::Object newStringSchema;
        newStringSchema["default"] = json::String(default_str);
        if ( _dryrun )
        {
            newStringSchema["description"] = json::String(description);
            newStringSchema["type"] = json::String("string");
        }
        updateSchemaWithCondition( newStringSchema, condition_key, condition_value );
        jsonSchemaBase[paramName] = newStringSchema;
    }

    void
    JsonConfigurable::initConfigTypeMap(
        const char* paramName,
        jsonConfigurable::ConstrainedString * pVariable,
        const char * description,
        const std::string& default_str,
        const char* condition_key, const char* condition_value
    )
    {
        LOG_DEBUG_F( "initConfigTypeMap<ConstrainedString>: %s\n", paramName);
        GetConfigData()->conStringConfigTypeMap[ paramName ] = pVariable;
        json::Object newConStringSchema;
        newConStringSchema["default"] = json::String(default_str); // would be nice if this always in the constraint list!
        if ( _dryrun )
        {
            newConStringSchema["description"] = json::String(description);
            newConStringSchema["type"] = json::String("Constrained String");
            newConStringSchema["value_source"] = json::String( pVariable->constraints );
        }
        updateSchemaWithCondition( newConStringSchema, condition_key, condition_value );
        jsonSchemaBase[paramName] = newConStringSchema;
    }


    void
    JsonConfigurable::initConfigTypeMap(
        const char* paramName,
        jsonConfigurable::tStringSetBase * pVariable,
        const char* description,
        const char* condition_key,
        const char* condition_value
    )
    {
        LOG_DEBUG_F( "initConfigTypeMap<set<string>>: %s\n", paramName);
        GetConfigData()->stringSetConfigTypeMap[ paramName ] = pVariable;
        json::Object root;
        json::QuickBuilder newStringSetSchema( root );
        newStringSetSchema["default"] = json::Array();
        if ( _dryrun )
        {
            newStringSetSchema["description"] = json::String(description);
            newStringSetSchema["type"] = json::String( pVariable->getTypeName() );

            if( pVariable->getTypeName() == FIXED_STRING_SET_LABEL )
            {
                unsigned int counter = 0;
                newStringSetSchema["possible_values"] = json::Array();
                for( auto& value : ((jsonConfigurable::tFixedStringSet*)pVariable)->possible_values )
                {
                    newStringSetSchema["possible_values"][counter++] = json::String( value );
                }
            }
            else if( pVariable->getTypeName() == DYNAMIC_STRING_SET_LABEL )
            {
                newStringSetSchema["value_source"] = json::String( ((jsonConfigurable::tDynamicStringSet*)pVariable)->value_source );
            }
            else
            {
                // just a regular old string set, no problem.
            }
        }
        //updateSchemaWithCondition( newStringSetSchema, condition_key, condition_value );
        jsonSchemaBase[paramName] = newStringSetSchema.As<json::Object>();
    }

    void
    JsonConfigurable::initConfigTypeMap(
        const char* paramName,
        std::vector< std::string > * pVariable,
        const char* description,
        const char* constraint_schema,
        const std::set< std::string > &constraint_variable,
        const char* condition_key, const char* condition_value
    )
    {
        LOG_DEBUG_F( "initConfigTypeMap<vector<string>>: %s\n", paramName);
        GetConfigData()->vectorStringConfigTypeMap[ paramName ] = pVariable;
        GetConfigData()->vectorStringConstraintsTypeMap[ paramName ] = &constraint_variable;
        json::Object newVectorStringSchema;
        if ( _dryrun )
        {
            newVectorStringSchema["description"] = json::String(description);
            newVectorStringSchema["type"] = json::String("Vector String");

            if( constraint_schema )
            {
                newVectorStringSchema["value_source"] = json::String( constraint_schema );
            }
        }
        updateSchemaWithCondition( newVectorStringSchema, condition_key, condition_value );
        jsonSchemaBase[paramName] = newVectorStringSchema;
    }

    void
    JsonConfigurable::initConfigTypeMap(
        const char* paramName,
        std::vector< std::vector< std::string > > * pVariable,
        const char* description,
        const char* constraint_schema,
        const std::set< std::string > &constraint_variable,
        const char* condition_key, const char* condition_value
    )
    {
        LOG_DEBUG_F( "initConfigTypeMap<vector<vector<string>>>: %s\n", paramName);
        GetConfigData()->vector2dStringConfigTypeMap[ paramName ] = pVariable;
        GetConfigData()->vector2dStringConstraintsTypeMap[ paramName ] = &constraint_variable;
        json::Object newVectorStringSchema;
        newVectorStringSchema["description"] = json::String(description);
        newVectorStringSchema["type"] = json::String("Vector 2d String");
        if( constraint_schema )
        {
            newVectorStringSchema["value_source"] = json::String( constraint_schema );
        }
        updateSchemaWithCondition( newVectorStringSchema, condition_key, condition_value );
        jsonSchemaBase[paramName] = newVectorStringSchema;
    }

    void
        JsonConfigurable::initConfigTypeMap(
            const char* paramName,
            std::vector< float > * pVariable,
            const char* description,
            float min, float max, float defaultvalue, bool ascending,
            const char* condition_key, const char* condition_value
        )
    {
        LOG_DEBUG_F("initConfigTypeMap<vector<float>>: %s\n", paramName);
        GetConfigData()->vectorFloatConfigTypeMap[paramName] = pVariable;
        json::Object newVectorFloatSchema;
        if (_dryrun)
        {
            newVectorFloatSchema["description"] = json::String(description);
            newVectorFloatSchema["type"] = json::String("Vector Float");
        }
        newVectorFloatSchema["min"] = json::Number(min);
        newVectorFloatSchema["max"] = json::Number(max);
        newVectorFloatSchema["default"] = json::Number(defaultvalue);
        newVectorFloatSchema["ascending"] = json::Number(ascending ? 1 : 0);
        updateSchemaWithCondition(newVectorFloatSchema, condition_key, condition_value);
        jsonSchemaBase[paramName] = newVectorFloatSchema;
    }

    void
    JsonConfigurable::initConfigTypeMap(
        const char* paramName,
        std::vector< int > * pVariable,
        const char* description,
        int min, int max, int defaultvalue,
        const char* condition_key, const char* condition_value
    )
    {
        LOG_DEBUG_F( "initConfigTypeMap<vector<int>>: %s\n", paramName);
        GetConfigData()->vectorIntConfigTypeMap[ paramName ] = pVariable;
        json::Object newVectorIntSchema;
        if ( _dryrun )
        {
            newVectorIntSchema["description"] = json::String(description);
            newVectorIntSchema["type"] = json::String("Vector Int");
        }
        newVectorIntSchema["min"] = json::Number(min);
        newVectorIntSchema["max"] = json::Number(max);
        newVectorIntSchema["default"] = json::Number(defaultvalue);
        updateSchemaWithCondition( newVectorIntSchema, condition_key, condition_value );
        jsonSchemaBase[paramName] = newVectorIntSchema;
    }

    void
    JsonConfigurable::initConfigTypeMap(
        const char* paramName,
        std::vector< std::vector< float > > * pVariable,
        const char* description,
        float min, float max, float defaultvalue,
        const char* condition_key, const char* condition_value
    )
    {
        LOG_DEBUG_F( "initConfigTypeMap<vector,vector<float>>>: %s\n", paramName);
        GetConfigData()->vector2dFloatConfigTypeMap[ paramName ] = pVariable;
        json::Object newVector2dFloatSchema;
        if ( _dryrun )
        {
            newVector2dFloatSchema["description"] = json::String(description);
            newVector2dFloatSchema["type"] = json::String("Vector2d Float");
        }
        newVector2dFloatSchema["min"] = json::Number(min);
        newVector2dFloatSchema["max"] = json::Number(max);
        newVector2dFloatSchema["default"] = json::Number(defaultvalue);
        updateSchemaWithCondition( newVector2dFloatSchema, condition_key, condition_value );
        jsonSchemaBase[paramName] = newVector2dFloatSchema;
    }

    void
    JsonConfigurable::initConfigTypeMap(
        const char* paramName,
        std::vector< std::vector< int > > * pVariable,
        const char* description,
        int min, int max, int defaultvalue,
        const char* condition_key, const char* condition_value
    )
    {
        LOG_DEBUG_F( "initConfigTypeMap<vector,vector<int>>>: %s\n", paramName);
        GetConfigData()->vector2dIntConfigTypeMap[ paramName ] = pVariable;
        json::Object newVector2dIntSchema;
        if ( _dryrun )
        {
            newVector2dIntSchema["description"] = json::String(description);
            newVector2dIntSchema["type"] = json::String("Vector2d Int");
        }
        newVector2dIntSchema["min"] = json::Number(min);
        newVector2dIntSchema["max"] = json::Number(max);
        newVector2dIntSchema["default"] = json::Number(defaultvalue);
        updateSchemaWithCondition( newVector2dIntSchema, condition_key, condition_value );
        jsonSchemaBase[paramName] = newVector2dIntSchema;
    }

    // We have sets/vectors from json arrays, now add maps from json dictonaries
    // This will be for specific piece-wise constant maps of dates (fractional years)
    // to config values (floats first).
    void
    JsonConfigurable::initConfigTypeMap(
        const char* paramName,
        tFloatFloatMapConfigType * pVariable,
        const char* defaultDesc
    )
    {
        LOG_DEBUG_F( "initConfigTypeMap<pwcMap>: %s\n", paramName);
        GetConfigData()->ffMapConfigTypeMap[ paramName ] = pVariable;
        json::Object newNestedSchema;
        if ( _dryrun )
        {
            newNestedSchema["description"] = json::String(defaultDesc);
            newNestedSchema["type"] = json::String("nested json object (of key-value pairs)");
        }
        jsonSchemaBase[paramName] = newNestedSchema;
    }

    void
    JsonConfigurable::initConfigTypeMap(
        const char* paramName,
        tFloatFloatMapConfigType * pVariable,
        const char* description,
        const char* condition_key, const char* condition_value
    )
    {
        LOG_DEBUG_F( "initConfigTypeMap<pwcMap>: %s\n", paramName);
        GetConfigData()->ffMapConfigTypeMap[ paramName ] = pVariable;
        json::Object newNestedSchema;
        if ( _dryrun )
        {
            newNestedSchema["description"] = json::String(description);
            newNestedSchema["type"] = json::String("nested json object (of key-value pairs)");
        }
        updateSchemaWithCondition( newNestedSchema, condition_key, condition_value );
        jsonSchemaBase[paramName] = newNestedSchema;
    }

    void
    JsonConfigurable::initConfigTypeMap(
        const char* paramName,
        tStringFloatMapConfigType * pVariable,
        const char* defaultDesc
    )
    {
        LOG_DEBUG_F( "initConfigTypeMap<pwcMap>: %s\n", paramName);
        GetConfigData()->sfMapConfigTypeMap[ paramName ] = pVariable;
        json::Object newNestedSchema;
        if ( _dryrun )
        {
            newNestedSchema["description"] = json::String(defaultDesc);
            newNestedSchema["type"] = json::String("nested json object (of key-value pairs)");
        }
        jsonSchemaBase[paramName] = newNestedSchema;
    }

    void
    JsonConfigurable::initConfigTypeMap(
        const char* paramName,
        RangedFloat * pVariable,
        const char * description,
        float defaultvalue,
        const char* condition_key, const char* condition_value
    )
    {
        LOG_DEBUG_F( "initConfigTypeMap<RangedFloat>: %s\n", paramName);
        GetConfigData()->rangedFloatConfigTypeMap[ paramName ] = pVariable;
        json::Object newFloatSchema;
        newFloatSchema["min"] = json::Number( pVariable->getMin() );
        newFloatSchema["max"] = json::Number( pVariable->getMax() );
        newFloatSchema["default"] = json::Number(defaultvalue);
        if ( _dryrun )
        {
            newFloatSchema["description"] = json::String(description);
            newFloatSchema["type"] = json::String( "float" );
        }
        updateSchemaWithCondition( newFloatSchema, condition_key, condition_value );
        jsonSchemaBase[paramName] = newFloatSchema;
    }

    void
    JsonConfigurable::initConfigTypeMap(
        const char* paramName,
        NonNegativeFloat * pVariable,
        const char * description,
        float max,
        float defaultvalue,
        const char* condition_key, const char* condition_value
    )
    {
        RangedFloat * pTmp = (RangedFloat*) pVariable;
        return initConfigTypeMap( paramName, pTmp, description, defaultvalue, condition_key, condition_value );
    }


    void
    JsonConfigurable::initConfigTypeMap(
        const char* paramName,
        NaturalNumber * pVariable,
        const char * description,
        unsigned int max,
        NaturalNumber defaultvalue,
        const char* condition_key, const char* condition_value
    )
    {
        LOG_DEBUG_F( "initConfigTypeMap<NaturalNumber>: %s\n", paramName);
        GetConfigData()->naturalNumberConfigTypeMap[ paramName ] = pVariable;
        json::Object newNNSchema;
        newNNSchema["min"] = json::Number( 0 );
        newNNSchema["max"] = json::Number( max );
        newNNSchema["default"] = json::Number(defaultvalue);
        if ( _dryrun )
        {
            newNNSchema["description"] = json::String(description);
            newNNSchema["type"] = json::String( "NaturalNumber" );
        }
        updateSchemaWithCondition( newNNSchema, condition_key, condition_value );
        jsonSchemaBase[paramName] = newNNSchema;
    }

    void
    JsonConfigurable::initConfigTypeMap(
        const char* paramName,
        JsonConfigurable * pVariable,
        const char* defaultDesc,
        const char* condition_key, const char* condition_value
    )
    {
        LOG_DEBUG_F( "initConfigTypeMap<JsonConfigurable>: %s\n", paramName);

        // --------------------------------------------------------
        // --- Get the type of variable and declare it an "idmType"
        // --------------------------------------------------------
        std::string variable_type = pVariable->GetTypeName();
        variable_type = std::string("idmType:") + variable_type ;

        // ------------------------------------------------------
        // --- Put the schema for the special type into map first
        // --- so that its definition appears before it is used.
        // ------------------------------------------------------
        GetConfigData()->jcTypeMap[ paramName ] = pVariable;

        if ( _dryrun )
        {
            bool tmp = _dryrun ;
            _dryrun = true ;
            pVariable->Configure( nullptr );
            _dryrun = tmp ;
            jsonSchemaBase[ variable_type ] = pVariable->GetSchema();

            json::Object newNestedSchema;
            newNestedSchema["description"] = json::String(defaultDesc);
            newNestedSchema["type"] = json::String(variable_type);
            updateSchemaWithCondition( newNestedSchema, condition_key, condition_value );
            jsonSchemaBase[paramName] = newNestedSchema;
        }
    }

    void JsonConfigurable::initConfigComplexType(
        const char* paramName,
        IComplexJsonConfigurable * pVariable,
        const char* description,
        const char* condition_key, 
        const char* condition_value
    )
    {
        json::QuickBuilder custom_schema = pVariable->GetSchema();

        // going to get something back like : {
        //  "type_name" : "idmType:VectorAlleleEnumPair",
        //  "type_schema" : {
        //      "first" : ...,
        //      "second" : ...
        //      }
        //  }
        std::string custom_type_label = (std::string) custom_schema[ _typename_label() ].As<json::String>();
        json::String custom_type_label_as_json_string = json::String( custom_type_label );
        jsonSchemaBase[ custom_type_label ] = custom_schema[ _typeschema_label() ];
        json::Object newComplexTypeSchemaEntry;
        newComplexTypeSchemaEntry["description"] = json::String( description );
        newComplexTypeSchemaEntry["type"] = json::String( custom_type_label_as_json_string );
        if( condition_key )
        {
            updateSchemaWithCondition( newComplexTypeSchemaEntry, condition_key, condition_value );
        }
        jsonSchemaBase[ paramName ] = newComplexTypeSchemaEntry;
        GetConfigData()->complexTypeMap[ paramName ] = pVariable;
    }

    void
    JsonConfigurable::handleMissingParam( const std::string& key, const std::string& rDataLocation )
    {
        LOG_DEBUG_F( "%s: key = %s, _track_missing = %d.\n", __FUNCTION__, key.c_str(), _track_missing );
        if( _track_missing )
        {
            missing_parameters_set.insert(key);
        }
        else
        {
            std::stringstream ss;
            ss << key << " of " << GetTypeName();
            throw MissingParameterFromConfigurationException( __FILE__, __LINE__, __FUNCTION__, rDataLocation.c_str(), ss.str().c_str() );
        }
    }

    void
    JsonConfigurable::initConfigTypeMap(
        const char* paramName,
        IPKey * pVariable,
        const char * description
    )
    {
        LOG_DEBUG_F( "initConfigTypeMap<IPKey>: %s\n", paramName);
        json::Object newIPKeySchema;
        if( _dryrun )
        {
            newIPKeySchema["description"] = json::String(description);
            newIPKeySchema["type"] = json::String("Constrained String");
            newIPKeySchema[ "value_source" ] = json::String( IPKey::GetConstrainedStringConstraintKey() );
        }
        GetConfigData()->ipKeyTypeMap[ paramName ] = pVariable;
        jsonSchemaBase[paramName] = newIPKeySchema;

        if( pVariable->GetParameterName().empty() )
        {
            pVariable->SetParameterName( paramName );
        }
    }

    void
    JsonConfigurable::initConfigTypeMap(
        const char* paramName,
        IPKeyValue * pVariable,
        const char * description
    )
    {
        LOG_DEBUG_F( "initConfigTypeMap<IPKeyValue>: %s\n", paramName);
        json::Object newIPKeyValueSchema;
        if( _dryrun )
        {
            newIPKeyValueSchema["description"] = json::String(description);
            newIPKeyValueSchema[ "type" ] = json::String( "Constrained String" );
            newIPKeyValueSchema[ "value_source" ] = json::String( IPKey::GetConstrainedStringConstraintKeyValue() );
        }
        GetConfigData()->ipKeyValueTypeMap[ paramName ] = pVariable;
        jsonSchemaBase[paramName] = newIPKeyValueSchema;

        if( pVariable->GetParameterName().empty() )
        {
            pVariable->SetParameterName( paramName );
        }
    }

    void
    JsonConfigurable::initConfigTypeMap(
        const char* paramName,
        NPKey * pVariable,
        const char * description
    )
    {
        LOG_DEBUG_F( "initConfigTypeMap<NPKey>: %s\n", paramName);
        json::Object newNPKeySchema;
        if( _dryrun )
        {
            newNPKeySchema["description"] = json::String(description);
            newNPKeySchema[ "type" ] = json::String( "Constrained String" );
            newNPKeySchema[ "value_source" ] = json::String( NPKey::GetConstrainedStringConstraintKey() );
        }
        GetConfigData()->npKeyTypeMap[ paramName ] = pVariable;
        jsonSchemaBase[paramName] = newNPKeySchema;

        if( pVariable->GetParameterName().empty() )
        {
            pVariable->SetParameterName( paramName );
        }
    }

    void
    JsonConfigurable::initConfigTypeMap(
        const char* paramName,
        NPKeyValue * pVariable,
        const char * description
    )
    {
        LOG_DEBUG_F( "initConfigTypeMap<NPKeyValue>: %s\n", paramName);
        json::Object newNPKeyValueSchema;
        if( _dryrun )
        {
            newNPKeyValueSchema["description"] = json::String(description);
            newNPKeyValueSchema[ "type" ] = json::String( "Constrained String" );
            newNPKeyValueSchema[ "value_source" ] = json::String( NPKey::GetConstrainedStringConstraintKeyValue() );
        }
        GetConfigData()->npKeyValueTypeMap[ paramName ] = pVariable;
        jsonSchemaBase[paramName] = newNPKeyValueSchema;

        if( pVariable->GetParameterName().empty() )
        {
            pVariable->SetParameterName( paramName );
        }
    }

    void
        JsonConfigurable::initConfigTypeMap(
            const char* paramName,
            EventTrigger * pVariable,
            const char * description,
            const char* condition_key,
            const char* condition_value
        )
    {
        LOG_DEBUG_F( "initConfigTypeMap<EventTrigger>: %s\n", paramName );
        json::Object newTriggerSchema;
        json::QuickBuilder qb( newTriggerSchema );
        if( _dryrun )
        {
            qb[ "type" ] = json::String( "Constrained String" );
            qb[ "default" ] = json::String( "" );
            qb[ "description" ] = json::String( description );
            qb[ "value_source" ] = json::String( EventTriggerFactory::CONSTRAINT_SCHEMA_STRING );
            const std::vector<std::string>& built_in_names = EventTriggerFactory::GetInstance()->GetBuiltInNames();
            for( int i = 0; i < built_in_names.size(); ++i )
            {
                qb[ "Built-in" ][ i ] = json::String( built_in_names[ i ] );
            }
            updateSchemaWithCondition( newTriggerSchema, condition_key, condition_value );
        }
        GetConfigData()->eventTriggerTypeMap[ paramName ] = pVariable;
        jsonSchemaBase[ paramName ] = newTriggerSchema;
    }

    void
        JsonConfigurable::initConfigTypeMap(
            const char* paramName,
            std::vector<EventTrigger> * pVariable,
            const char * description,
            const char* condition_key,
            const char* condition_value
        )
    {
        LOG_DEBUG_F( "initConfigTypeMap<std::vector<EventTrigger>>: %s\n", paramName );
        json::Object newTriggerSchema;
        json::QuickBuilder qb( newTriggerSchema );
        if( _dryrun )
        {
            qb[ "type" ] = json::String( "Vector String" );
            qb[ "default" ] = json::String( "" );
            qb[ "description" ] = json::String( description );
            qb[ "value_source" ] = json::String( EventTriggerFactory::CONSTRAINT_SCHEMA_STRING );
            const std::vector<std::string>& built_in_names = EventTriggerFactory::GetInstance()->GetBuiltInNames();
            for( int i = 0 ; i < built_in_names.size() ; ++i )
            {
                qb[ "Built-in" ][ i ] = json::String( built_in_names[ i ] );
            }
            updateSchemaWithCondition( newTriggerSchema, condition_key, condition_value );
        }
        GetConfigData()->eventTriggerVectorTypeMap[ paramName ] = pVariable;
        jsonSchemaBase[ paramName ] = newTriggerSchema;
    }

    void
        JsonConfigurable::initConfigTypeMap(
            const char* paramName,
            EventTriggerNode * pVariable,
            const char * description,
            const char* condition_key,
            const char* condition_value
        )
    {
        LOG_DEBUG_F( "initConfigTypeMap<EventTriggerNode>: %s\n", paramName );
        json::Object newTriggerSchema;
        json::QuickBuilder qb( newTriggerSchema );
        if( _dryrun )
        {
            qb[ "type" ] = json::String( "Constrained String" );
            qb[ "default" ] = json::String( "" );
            qb[ "description" ] = json::String( description );
            qb[ "value_source" ] = json::String( EventTriggerNodeFactory::CONSTRAINT_SCHEMA_STRING );
            const std::vector<std::string>& built_in_names = EventTriggerNodeFactory::GetInstance()->GetBuiltInNames();
            for( int i = 0; i < built_in_names.size(); ++i )
            {
                qb[ "Built-in" ][ i ] = json::String( built_in_names[ i ] );
            }
            updateSchemaWithCondition( newTriggerSchema, condition_key, condition_value );
        }
        GetConfigData()->eventTriggerNodeTypeMap[ paramName ] = pVariable;
        jsonSchemaBase[ paramName ] = newTriggerSchema;
    }

    void
        JsonConfigurable::initConfigTypeMap(
            const char* paramName,
            std::vector<EventTriggerNode> * pVariable,
            const char * description,
            const char* condition_key,
            const char* condition_value
        )
    {
        LOG_DEBUG_F( "initConfigTypeMap<std::vector<EventTrigger>>: %s\n", paramName );
        json::Object newTriggerSchema;
        json::QuickBuilder qb( newTriggerSchema );
        if( _dryrun )
        {
            qb[ "type" ] = json::String( "Vector String" );
            qb[ "default" ] = json::String( "" );
            qb[ "description" ] = json::String( description );
            qb[ "value_source" ] = json::String( EventTriggerNodeFactory::CONSTRAINT_SCHEMA_STRING );
            const std::vector<std::string>& built_in_names = EventTriggerNodeFactory::GetInstance()->GetBuiltInNames();
            for( int i = 0; i < built_in_names.size(); ++i )
            {
                qb[ "Built-in" ][ i ] = json::String( built_in_names[ i ] );
            }
            updateSchemaWithCondition( newTriggerSchema, condition_key, condition_value );
        }
        GetConfigData()->eventTriggerNodeVectorTypeMap[ paramName ] = pVariable;
        jsonSchemaBase[ paramName ] = newTriggerSchema;
    }

    void
        JsonConfigurable::initConfigTypeMap(
            const char* paramName,
            EventTriggerCoordinator * pVariable,
            const char * description,
            const char* condition_key,
            const char* condition_value
        )
    {
        LOG_DEBUG_F( "initConfigTypeMap<EventTriggerCoordinator>: %s\n", paramName );

        json::Object newTriggerSchema;
        json::QuickBuilder qb( newTriggerSchema );
        if( _dryrun )
        {
            qb[ "type" ] = json::String( "Constrained String" );
            qb[ "default" ] = json::String( "" );
            qb[ "description" ] = json::String( description );
            qb[ "value_source" ] = json::String( EventTriggerCoordinatorFactory::CONSTRAINT_SCHEMA_STRING );
            const std::vector<std::string>& built_in_names = EventTriggerCoordinatorFactory::GetInstance()->GetBuiltInNames();
            for( int i = 0; i < built_in_names.size(); ++i )
            {
                qb[ "Built-in" ][ i ] = json::String( built_in_names[ i ] );
            }
            updateSchemaWithCondition( newTriggerSchema, condition_key, condition_value );
        }
        GetConfigData()->eventTriggerCoordinatorTypeMap[ paramName ] = pVariable;
        jsonSchemaBase[ paramName ] = newTriggerSchema;
    }

    void
        JsonConfigurable::initConfigTypeMap(
            const char* paramName,
            std::vector<EventTriggerCoordinator> * pVariable,
            const char * description,
            const char* condition_key,
            const char* condition_value
        )
    {
        LOG_DEBUG_F( "initConfigTypeMap<std::vector<EventTriggerCoordinator>>: %s\n", paramName );
        json::Object newTriggerSchema;
        json::QuickBuilder qb( newTriggerSchema );
        if( _dryrun )
        {
            qb[ "type" ] = json::String( "Vector String" );
            qb[ "default" ] = json::String( "" );
            qb[ "description" ] = json::String( description );
            qb[ "value_source" ] = json::String( EventTriggerCoordinatorFactory::CONSTRAINT_SCHEMA_STRING );
            const std::vector<std::string>& built_in_names = EventTriggerCoordinatorFactory::GetInstance()->GetBuiltInNames();
            for( int i = 0; i < built_in_names.size(); ++i )
            {
                qb[ "Built-in" ][ i ] = json::String( built_in_names[ i ] );
            }
            updateSchemaWithCondition( newTriggerSchema, condition_key, condition_value );
        }
        GetConfigData()->eventTriggerCoordinatorVectorTypeMap[ paramName ] = pVariable;
        jsonSchemaBase[ paramName ] = newTriggerSchema;
    }

    bool JsonConfigurable::Configure( const Configuration* inputJson )
    {
        if( _dryrun )
        {
            LOG_DEBUG("Returning from Configure because doing dryrun\n");
            return true;
        }

        LOG_DEBUG_F( "In %s, _useDefaults = %d\n", __FUNCTION__, _useDefaults );
        // Desired logic
        //
        //  | SPECIFIED | USE_DEFAULTS | BEHAVIOUR |
        //  |--------------------------------------|
        //  |   TRUE    |     TRUE     | USE_JSON  |
        //  |   TRUE    |     FALSE    | USE_JSON  |
        //  |   FALSE   |     TRUE     | USE_DEF   |
        //  |   FALSE   |     FALSE    |   ERROR   |
        //
        //  This reduces to:
        //
        //  |   TRUE    |       X      | USE_JSON  |
        //  |   FALSE   |     TRUE     | USE_DEF   |
        //  |   FALSE   |     FALSE    |   ERROR   |
        //
        // INIT STAGE
        // initVarFromConfig: iterate over all config keys...
        // until we figure that out, go the other way

        // ---------------------------------- BOOL ------------------------------------
        for (auto& entry : GetConfigData()->boolConfigTypeMap)
        {
            const std::string& key = entry.first;
            json::QuickInterpreter schema = jsonSchemaBase[key];

            if( ignoreParameter( schema, inputJson ) )
            {
                // param is missing and that's ok." << std::endl;
                continue;
            }

            // check if parameter was specified in input json (TODO: improve performance by getting the iterator here with Find() and reusing instead of GET_CONFIG_BOOLEAN below)
            if( inputJson->Exist(key) )
            {
                *(entry.second) = (bool)GET_CONFIG_BOOLEAN(inputJson,key.c_str());
            }
            else
            {
                if( _useDefaults )
                {
                    // using the default value
                    bool defaultValue = ((int)schema["default"].As<json::Number>() == 1);
                    *(entry.second) = defaultValue;
                    LOG_DEBUG_F( "Using the default value ( \"%s\" : %d ) for unspecified parameter.\n", key.c_str(), defaultValue );
                }
                else
                {
                    handleMissingParam( key, inputJson->GetDataLocation() );
                }
            }

            LOG_DEBUG_F("the key %s = bool %d\n", key.c_str(), *(entry.second));
        }

        // ---------------------------------- INT -------------------------------------
        for (auto& entry : GetConfigData()->intConfigTypeMap)
        {
            const std::string& key = entry.first;
            json::QuickInterpreter schema = jsonSchemaBase[key];
            int val = -1;

            if( ignoreParameter( schema, inputJson ) )
            {
                continue; // param is missing and that's ok." << std::endl;
            }

            // check if parameter was specified in input json (TODO: improve performance by getting the iterator here with Find() and reusing instead of GET_CONFIG_INTEGER below)
            if( inputJson->Exist(key) )
            {
                // get specified configuration parameter
                double jsonValueAsDouble = GET_CONFIG_DOUBLE( inputJson, key.c_str() );
                if( jsonValueAsDouble != (int) jsonValueAsDouble )
                {
                    std::ostringstream errMsg; // using a non-parameterized exception.
                    errMsg << "The value for parameter '"<< key << "' appears to be a decimal ("
                           << jsonValueAsDouble
                           << ") but needs to be an integer." << std::endl;
                    throw Kernel::GeneralConfigurationException( __FILE__, __LINE__, __FUNCTION__, errMsg.str().c_str() );
                }
                val = int(jsonValueAsDouble);
                // throw exception if value is outside of range
                EnforceParameterRange<int>( key, val, schema );
                *(entry.second) = val;
            }
            else
            {
                if( _useDefaults )
                {
                    // using the default value
                    val = (int)schema["default"].As<json::Number>();
                    LOG_INFO_F( "Using the default value ( \"%s\" : %d ) for unspecified parameter.\n", key.c_str(), val );
                    *(entry.second) = val;
                }
                else // not in config, not using defaults, no depends-on, just plain missing
                {
                    handleMissingParam( key, inputJson->GetDataLocation() );
                }
            }

            LOG_DEBUG_F("the key %s = int %d\n", key.c_str(), *(entry.second));
        }

        // ---------------------------------- Uint32_t -------------------------------------
        for( auto& entry : GetConfigData()->uint32ConfigTypeMap )
        {
            const std::string& key = entry.first;
            json::QuickInterpreter schema = jsonSchemaBase[ key ];
            uint32_t val = 0;

            if( ignoreParameter( schema, inputJson ) )
            {
                continue; // param is missing and that's ok." << std::endl;
            }

            // check if parameter was specified in input json (TODO: improve performance by getting the iterator here with Find() and reusing instead of GET_CONFIG_INTEGER below)
            if( inputJson->Exist( key ) )
            {
                // get specified configuration parameter
                double jsonValueAsDouble = GET_CONFIG_DOUBLE( inputJson, key.c_str() );
                if( jsonValueAsDouble != (uint32_t)jsonValueAsDouble )
                {
                    std::ostringstream errMsg; // using a non-parameterized exception.
                    errMsg << "The value for parameter '"<< key << "' appears to be a decimal ("
                           << jsonValueAsDouble
                           << ") but needs to be an integer." << std::endl;
                    throw Kernel::GeneralConfigurationException( __FILE__, __LINE__, __FUNCTION__, errMsg.str().c_str() );
                }
                val = uint32_t( jsonValueAsDouble );
                // throw exception if value is outside of range
                EnforceParameterRange<uint32_t>( key, val, schema );
                *(entry.second) = val;
            }
            else
            {
                if( _useDefaults )
                {
                    // using the default value
                    val = (uint32_t)schema[ "default" ].As<json::Number>();
                    LOG_INFO_F( "Using the default value ( \"%s\" : %d ) for unspecified parameter.\n", key.c_str(), val );
                    *(entry.second) = val;
                }
                else // not in config, not using defaults, no depends-on, just plain missing
                {
                    handleMissingParam( key, inputJson->GetDataLocation() );
                }
            }

            LOG_DEBUG_F( "the key %s = uint32_t %u\n", key.c_str(), *(entry.second) );
        }

        // ---------------------------------- FLOAT ------------------------------------
        for (auto& entry : GetConfigData()->floatConfigTypeMap)
        {
            const std::string& key = entry.first;
            json::QuickInterpreter schema = jsonSchemaBase[key];
            float val = -1.0f;

            if( ignoreParameter( schema, inputJson ) )
            {
                LOG_DEBUG_F( "(float) param %s failed condition check. Ignoring.\n", key.c_str() );
                continue; // param is missing and that's ok.
            }

            // Check if parameter was specified in input json (TODO: improve performance by getting the iterator here with Find() and reusing instead of GET_CONFIG_DOUBLE below)
            if( inputJson->Exist(key) )
            {
                val = (float) GET_CONFIG_DOUBLE( inputJson, key.c_str() );
                // throw exception if specified value is outside of range
                EnforceParameterRange<float>( key, val, schema);
                *(entry.second) = val;
            }
            else
            {
                if ( _useDefaults )
                {
                    // using the default value
                    val = (float)schema["default"].As<json::Number>();
                    EnforceParameterRange<float>( key, val, schema );
                    LOG_INFO_F( "Using the default value ( \"%s\" : %f ) for unspecified parameter.\n", key.c_str(), val );
                    *(entry.second) = val;
                }
                else // not in config, not using defaults, no depends-on, just plain missing
                {
                    handleMissingParam( key, inputJson->GetDataLocation() );
                }
            }

            LOG_DEBUG_F("the key %s = float %f\n", key.c_str(), *(entry.second));
        }

        // ---------------------------------- DOUBLE ------------------------------------
        for (auto& entry : GetConfigData()->doubleConfigTypeMap)
        {
            const std::string& key = entry.first;
            json::QuickInterpreter schema = jsonSchemaBase[key];
            double val = -1.0;

            if( ignoreParameter( schema, inputJson ) )
            {
                continue; // param is missing and that's ok.
            }

            // Check if parameter was specified in input json (TODO: improve performance by getting the iterator here with Find() and reusing instead of GET_CONFIG_DOUBLE below)
            if( inputJson->Exist(key) )
            {
                val = GET_CONFIG_DOUBLE( inputJson, key.c_str() );
                *(entry.second) = val;
                // throw exception if specified value is outside of range
                EnforceParameterRange<double>( key, val, schema);
            }
            else
            {
                if ( _useDefaults )
                {
                    // using the default value
                    val = schema["default"].As<json::Number>();
                    LOG_INFO_F( "Using the default value ( \"%s\" : %f ) for unspecified parameter.\n", key.c_str(), val );
                    *(entry.second) = val;
                }
                else 
                {
                    handleMissingParam( key, inputJson->GetDataLocation() );
                }
            }
            LOG_DEBUG_F("the key %s = double %f\n", key.c_str(), *(entry.second));
        }

        // ---------------------------------- RANGEDFLOAT ------------------------------------
        for (auto& entry : GetConfigData()->rangedFloatConfigTypeMap)
        {
            const std::string& key = entry.first;
            json::QuickInterpreter schema = jsonSchemaBase[key];
            float val = -1.0f;

            if( ignoreParameter( schema, inputJson ) )
            {
                continue; // param is missing and that's ok.
            }

            // Check if parameter was specified in input json
            LOG_DEBUG_F( "useDefaults = %d\n", _useDefaults );
            if( inputJson->Exist(key) )
            {
                // get specified configuration parameter
                val = GET_CONFIG_DOUBLE( inputJson, key.c_str() );
                *(entry.second) = val;
                // throw exception if specified value is outside of range even though this datatype enforces ranges inherently.
                EnforceParameterRange<float>( key, val, schema);
            }
            else
            {
                if( _useDefaults )
                {
                    // using the default value
                    val = (float)schema["default"].As<json::Number>();
                    EnforceParameterRange<float>( key, val, schema );
                    LOG_INFO_F( "Using the default value ( \"%s\" : %f ) for unspecified parameter.\n", key.c_str(), val );
                    *(entry.second) = val;
                }
                else 
                {
                    handleMissingParam( key, inputJson->GetDataLocation() );
                }
            }

            LOG_DEBUG_F("the key %s = float %f\n", key.c_str(), (float) *(entry.second));
        }

        // ---------------------------------- NATURALNUMBER ------------------------------------
        for (auto& entry : GetConfigData()->naturalNumberConfigTypeMap)
        {
            const std::string& key = entry.first;
            json::QuickInterpreter schema = jsonSchemaBase[key];
            int val = 0;

            if( ignoreParameter( schema, inputJson ) )
            {
                continue; // param is missing and that's ok.
            }

            // Check if parameter was specified in input json
            LOG_DEBUG_F( "useDefaults = %d\n", _useDefaults );
            if( inputJson->Exist(key) )
            {
                // get specified configuration parameter
                val = (int) GET_CONFIG_INTEGER( inputJson, key.c_str() );
                *(entry.second) = val;
                EnforceParameterRange<int>( key, val, schema);
            }
            else
            {
                if( _useDefaults )
                {
                    // using the default value
                    val = (int)schema["default"].As<json::Number>();
                    LOG_INFO_F( "Using the default value ( \"%s\" : %f ) for unspecified parameter.\n", key.c_str(), val );
                    *(entry.second) = val;
                }
                else 
                {
                    handleMissingParam( key, inputJson->GetDataLocation() );
                }
            }

            LOG_DEBUG_F("the key %s = int %f\n", key.c_str(), (int) *(entry.second));
        }

        // ---------------------------------- STRING ------------------------------------
        for (auto& entry : GetConfigData()->stringConfigTypeMap)
        {
            const std::string& key = entry.first;
            json::QuickInterpreter schema = jsonSchemaBase[key];

            if( ignoreParameter( schema, inputJson ) )
            {
                continue; // param is missing and that's ok.
            }

            std::string val = schema["default"].As<json::String>();
            if( inputJson->Exist(key) )
            {
                *(entry.second) = (std::string) GET_CONFIG_STRING( inputJson, (entry.first).c_str() );
            }
            else
            {
                if( _useDefaults )
                {
                    val = (std::string)schema["default"].As<json::String>();
                    LOG_INFO_F( "Using the default value ( \"%s\" : \"%s\" ) for unspecified parameter.\n", key.c_str(), val.c_str() );
                    *(entry.second) = val;
                }
                else
                {
                    handleMissingParam( key, inputJson->GetDataLocation() );
                }
            }
        }

        for (auto& entry : GetConfigData()->conStringConfigTypeMap)
        {
            const std::string& key = entry.first;
            entry.second->parameter_name = key ;
            json::QuickInterpreter schema = jsonSchemaBase[key];
            if( ignoreParameter( schema, inputJson ) )
            {
                continue; // param is missing and that's ok.
            }

            std::string val = schema["default"].As<json::String>();
            if( inputJson->Exist(key) )
            {
                *(entry.second) = (std::string) GET_CONFIG_STRING( inputJson, (entry.first).c_str() );
            }
            else
            {
                if( _useDefaults )
                {
                    val = (std::string)schema["default"].As<json::String>();
                    LOG_INFO_F( "Using the default value ( \"%s\" : \"%s\" ) for unspecified parameter.\n", key.c_str(), val.c_str() );
                    *(entry.second) = val;
                }
                else
                {
                    handleMissingParam( key, inputJson->GetDataLocation() );
                }
            }
        }

        // ---------------------------------- SET of STRINGs ------------------------------------
        for (auto& entry : GetConfigData()->stringSetConfigTypeMap)
        {
            const std::string& key = entry.first;
            json::QuickInterpreter schema = jsonSchemaBase[key];
            if( ignoreParameter( schema, inputJson ) )
            {
                continue; // param is missing and that's ok.
            }

            if ( inputJson->Exist(key) )
            {
                *(entry.second) = GET_CONFIG_STRING_SET( inputJson, (entry.first).c_str() );
            }
            else
            { 
                if( _useDefaults )
                {
                    //auto val = schema["default"].As<json::Array>();
                    //LOG_INFO_F( "Using the default value ( \"%s\" : <empty string set> ) for unspecified string set parameter.\n", key.c_str() );
                    //*(entry.second) = val;
                }
                else
                {
                    handleMissingParam( key, inputJson->GetDataLocation() );
                }
            }
        }

        // ---------------------------------- VECTOR of STRINGs ------------------------------------
        for (auto& entry : GetConfigData()->vectorStringConfigTypeMap)
        {
            const std::string& key = entry.first;
            json::QuickInterpreter schema = jsonSchemaBase[key];
            if( ignoreParameter( schema, inputJson ) )
            {
                continue; // param is missing and that's ok.
            }

            if ( inputJson->Exist(key) )
            {
                *(entry.second) = GET_CONFIG_VECTOR_STRING( inputJson, (entry.first).c_str() );
            }
            else
            {
                if( _useDefaults )
                {
                    // using the default value
                    LOG_INFO_F( "Using the default value ( \"%s\" : <empty string vector> ) for unspecified string vector parameter.\n", key.c_str() );
                }

                handleMissingParam( key, inputJson->GetDataLocation() );
            }

            auto allowed_values = GetConfigData()->vectorStringConstraintsTypeMap[ key ];
            for( auto &candidate : *(entry.second) )
            {
                if( allowed_values->size() > 0 && std::find( allowed_values->begin(), allowed_values->end(), candidate ) == allowed_values->end() )
                {
                    std::ostringstream msg;
                    msg << "Constrained strings (dynamic enum) with specified value "
                        << candidate
                        << " invalid. Possible values are: ";
                    for( auto value: *allowed_values )
                    {
                        msg << value << "...";
                    }
                    throw GeneralConfigurationException( __FILE__, __LINE__, __FUNCTION__, msg.str().c_str() );
                }
            }
        }

        // ---------------------------------- VECTOR VECTOR of STRINGs ------------------------------------
        for (auto& entry : GetConfigData()->vector2dStringConfigTypeMap)
        {
            const std::string& key = entry.first;
            json::QuickInterpreter schema = jsonSchemaBase[key];
            if( ignoreParameter( schema, inputJson ) )
            {
                continue; // param is missing and that's ok.
            }

            if ( inputJson->Exist(key) )
            {
                *(entry.second) = GET_CONFIG_VECTOR2D_STRING( inputJson, (entry.first).c_str() );
            }
            else
            {
                if( _useDefaults )
                {
                    // using the default value
                    LOG_INFO_F( "Using the default value ( \"%s\" : <empty string Vector2D> ) for unspecified string Vector2D parameter.\n", key.c_str() );
                }

                handleMissingParam( key, inputJson->GetDataLocation() );
            }
            auto allowed_values = GetConfigData()->vector2dStringConstraintsTypeMap[ key ];
            for( auto &candidate_vector : *(entry.second) )
            {
                for( auto &candidate : candidate_vector )
                {
                    if( allowed_values->size() > 0 && std::find( allowed_values->begin(), allowed_values->end(), candidate ) == allowed_values->end() )
                    {
                        std::ostringstream msg;
                        msg << "Constrained strings (dynamic enum) with specified value " 
                            << candidate 
                            << " invalid. Possible values are: ";
                        for( auto value: *allowed_values )
                        {
                            msg << value << "...";
                        }
                        throw GeneralConfigurationException( __FILE__, __LINE__, __FUNCTION__, msg.str().c_str() );
                    }
                }
            }
        }

        //----------------------------------- VECTOR of FLOATs ------------------------------
        for (auto& entry : GetConfigData()->vectorFloatConfigTypeMap)
        {
            const std::string& key = entry.first;
            json::QuickInterpreter schema = jsonSchemaBase[key];
            if( ignoreParameter( schema, inputJson ) )
            {
                continue; // param is missing and that's ok.
            }

            if( inputJson->Exist(key) )
            {
                std::vector<float> configValues = GET_CONFIG_VECTOR_FLOAT( inputJson, (entry.first).c_str() );
                *(entry.second) = configValues;

                EnforceVectorParameterRanges<float>(key, configValues, schema);
            }
            else if( !_useDefaults )
            {
                handleMissingParam( key, inputJson->GetDataLocation() );
            }
        }

        //----------------------------------- VECTOR of INTs ------------------------------
        for (auto& entry : GetConfigData()->vectorIntConfigTypeMap)
        {
            const std::string& key = entry.first;
            json::QuickInterpreter schema = jsonSchemaBase[key];
            if( ignoreParameter( schema, inputJson ) )
            {
                continue; // param is missing and that's ok.
            }

            if( inputJson->Exist(key) )
            {
                std::vector<int> configValues = GET_CONFIG_VECTOR_INT( inputJson, (entry.first).c_str() );
                *(entry.second) = configValues;

                EnforceVectorParameterRanges<int>(key, configValues, schema);
            }
            else if( !_useDefaults )
            {
                handleMissingParam( key, inputJson->GetDataLocation() );
            }
        }

        //----------------------------------- VECTOR VECTOR of FLOATs ------------------------------
        for (auto& entry : GetConfigData()->vector2dFloatConfigTypeMap)
        {
            const std::string& key = entry.first;
            json::QuickInterpreter schema = jsonSchemaBase[key];
            if( ignoreParameter( schema, inputJson ) )
            {
                continue; // param is missing and that's ok.
            }

            if( inputJson->Exist(key) )
            {
                std::vector<std::vector<float>> configValues = GET_CONFIG_VECTOR2D_FLOAT( inputJson, (entry.first).c_str() );
                *(entry.second) = configValues;

                for( auto values : configValues )
                {
                    EnforceVectorParameterRanges<float>(key, values, schema);
                }
            }
            else if( !_useDefaults )
            {
                handleMissingParam( key, inputJson->GetDataLocation() );
            }
        }

        //----------------------------------- VECTOR VECTOR of INTs ------------------------------
        for (auto& entry : GetConfigData()->vector2dIntConfigTypeMap)
        {
            const std::string& key = entry.first;
            json::QuickInterpreter schema = jsonSchemaBase[key];
            if( ignoreParameter( schema, inputJson ) )
            {
                continue; // param is missing and that's ok.
            }

            if( inputJson->Exist(key) )
            {
                std::vector<std::vector<int>> configValues = GET_CONFIG_VECTOR2D_INT( inputJson, (entry.first).c_str() );
                *(entry.second) = configValues;

                for( auto values : configValues )
                {
                    EnforceVectorParameterRanges<int>(key, values, schema);
                }
            }
            else if( !_useDefaults )
            {
                handleMissingParam( key, inputJson->GetDataLocation() );
            }
        }
/////////////////// END FIX BOUNDARY

        // ---------------------------------- COMPLEX MAP ------------------------------------
        for (auto& entry : GetConfigData()->complexTypeMap)
        {
            const auto & key = entry.first;
            json::QuickInterpreter schema = jsonSchemaBase[key];
            if ( ignoreParameter( schema, inputJson ) )
            {
               // param is missing and that's ok." << std::endl;
               continue;
            }

            IComplexJsonConfigurable * pJc = entry.second;
            if( inputJson->Exist( key ) )
            {
                pJc->ConfigureFromJsonAndKey( inputJson, key );
            }
            else if( !_useDefaults || !(pJc->HasValidDefault()) )
            {
                handleMissingParam( key, inputJson->GetDataLocation() );
            }
        }

        // ---------------------------------- FLOAT-FLOAT MAP ------------------------------------
        for (auto& entry : GetConfigData()->ffMapConfigTypeMap)
        {
            // NOTE that this could be used for general float to float, but right now hard-coding year-as-int to float
            const auto & key = entry.first;
            tFloatFloatMapConfigType * pFFMap = entry.second;
            const auto& tvcs_jo = json_cast<const json::Object&>( (*inputJson)[key] );
            for( auto data = tvcs_jo.Begin();
                      data != tvcs_jo.End();
                      ++data )
            {
                float year = atof( data->name.c_str() );
                auto tvcs = inputJson->As< json::Object >()[ key ];
                float constant = (float) ((json::QuickInterpreter( tvcs ))[ data->name ].As<json::Number>());
                LOG_DEBUG_F( "Inserting year %f and delay %f into map.\n", year, constant );
                pFFMap->insert( std::make_pair( year, constant ) );
            }
        }

        // ---------------------------------- STRING-FLOAT MAP ------------------------------------
        for (auto& entry : GetConfigData()->sfMapConfigTypeMap)
        {
            // NOTE that this could be used for general float to float, but right now hard-coding year-as-int to float
            const auto & key = entry.first;
            tStringFloatMapConfigType * pSFMap = entry.second;
            const auto& tvcs_jo = json_cast<const json::Object&>( (*inputJson)[key] );
            for( auto data = tvcs_jo.Begin();
                      data != tvcs_jo.End();
                      ++data )
            {
                auto tvcs = inputJson->As< json::Object >()[ key ];
                float value = (float) ((json::QuickInterpreter( tvcs ))[ data->name ].As<json::Number>());
                LOG_DEBUG_F( "Inserting string %s and value %f into map.\n", data->name.c_str(), value );
                pSFMap->insert( std::make_pair( data->name, value ) );
            }
        }

        // ---------------------------------- JsonConfigurable MAP ------------------------------------
        for (auto& entry : GetConfigData()->jcTypeMap)
        {
            const auto & key = entry.first;
            JsonConfigurable * pJc = entry.second;

            if( inputJson->Exist( key ) )
            {
                Configuration * p_config = Configuration::CopyFromElement( (*inputJson)[key], inputJson->GetDataLocation() );

                pJc->Configure( p_config );

                delete p_config ;
                p_config = nullptr;
            }
            else if( !_useDefaults )
            {
                handleMissingParam( key, inputJson->GetDataLocation() );
            }
        }


        // ---------------------------------- IPKey ------------------------------------
        for (auto& entry : GetConfigData()->ipKeyTypeMap)
        {
            const std::string& param_key = entry.first;
            json::QuickInterpreter schema = jsonSchemaBase[param_key];
            if ( inputJson->Exist(param_key) )
            {
                std::string tmp = (std::string) GET_CONFIG_STRING( inputJson, (entry.first).c_str() );
                if( !tmp.empty() )
                {
                    *(entry.second) = tmp;
                }
            }
            else if( !_useDefaults )
            {
                handleMissingParam( param_key, inputJson->GetDataLocation() );
            }
        }

        // ---------------------------------- IPKeyValue ------------------------------------
        for (auto& entry : GetConfigData()->ipKeyValueTypeMap)
        {
            const std::string& param_key = entry.first;
            json::QuickInterpreter schema = jsonSchemaBase[param_key];
            if ( inputJson->Exist(param_key) )
            {
                std::string tmp = (std::string) GET_CONFIG_STRING( inputJson, (entry.first).c_str() );
                if( !tmp.empty() )
                {
                    *(entry.second) = tmp;
                }
            }
            else if( !_useDefaults )
            {
                handleMissingParam( param_key, inputJson->GetDataLocation() );
            }
        }

        // ---------------------------------- NPKey ------------------------------------
        for( auto& entry : GetConfigData()->npKeyTypeMap )
        {
            const std::string& param_key = entry.first;
            json::QuickInterpreter schema = jsonSchemaBase[ param_key ];
            if( inputJson->Exist( param_key ) )
            {
                std::string tmp = (std::string) GET_CONFIG_STRING( inputJson, (entry.first).c_str() );
                if( !tmp.empty() )
                {
                    *(entry.second) = tmp;
                }
            }
            else if( !_useDefaults )
            {
                handleMissingParam( param_key, inputJson->GetDataLocation() );
            }
        }

        // ---------------------------------- NPKeyValue ------------------------------------
        for( auto& entry : GetConfigData()->npKeyValueTypeMap )
        {
            const std::string& param_key = entry.first;
            json::QuickInterpreter schema = jsonSchemaBase[ param_key ];
            if( inputJson->Exist( param_key ) )
            {
                std::string tmp = (std::string) GET_CONFIG_STRING( inputJson, (entry.first).c_str() );
                if( !tmp.empty() )
                {
                    *(entry.second) = tmp;
                }
            }
            else if( !_useDefaults )
            {
                handleMissingParam( param_key, inputJson->GetDataLocation() );
            }
        }

        // ---------------------------------- EventTrigger  ------------------------------------
        for( auto& entry : GetConfigData()->eventTriggerTypeMap )
        {
            const std::string& param_key = entry.first;
            json::QuickInterpreter schema = jsonSchemaBase[ param_key ];
            if( !inputJson->Exist( param_key ) && _useDefaults )
            {
                if( _useDefaults )
                {
                    LOG_INFO_F( "Using the default value ( \"%s\" : \"%s\" ) for unspecified parameter.\n", param_key.c_str(), "");
                }
                else
                {
                    handleMissingParam( param_key, inputJson->GetDataLocation() );
                }
            }
            else
            {
                std::string candidate = (std::string) GET_CONFIG_STRING( inputJson, (entry.first).c_str() );
                *(entry.second) = EventTriggerFactory::GetInstance()->CreateTrigger( param_key, candidate );
            }
        }

        // ---------------------------------- EventTriggerVector  ------------------------------------
        for( auto& entry : GetConfigData()->eventTriggerVectorTypeMap )
        {
            const std::string& param_key = entry.first;
            json::QuickInterpreter schema = jsonSchemaBase[ param_key ];
            if( !inputJson->Exist( param_key ) && _useDefaults )
            {
                LOG_INFO_F( "Using the default value ( \"%s\" : \"\" ) for unspecified parameter.\n", param_key.c_str() );
                handleMissingParam( param_key, inputJson->GetDataLocation() );
            }
            else
            {
                std::vector<std::string> candidate_list = GET_CONFIG_VECTOR_STRING( inputJson, (entry.first).c_str() );
                *(entry.second) = EventTriggerFactory::GetInstance()->CreateTriggerList( param_key, candidate_list );
            }
        }

        // ---------------------------------- EventTriggerNode  ------------------------------------
        for( auto& entry : GetConfigData()->eventTriggerNodeTypeMap )
        {
            const std::string& param_key = entry.first;
            json::QuickInterpreter schema = jsonSchemaBase[ param_key ];
            if( !inputJson->Exist( param_key ) && _useDefaults )
            {
                if( _useDefaults )
                {
                    LOG_INFO_F( "Using the default value ( \"%s\" : \"%s\" ) for unspecified parameter.\n", param_key.c_str(), "" );                    
                }
                else
                {
                    handleMissingParam( param_key, inputJson->GetDataLocation() );
                }
            }
            else
            {
                std::string candidate = (std::string) GET_CONFIG_STRING( inputJson, (entry.first).c_str() );
                *(entry.second) = EventTriggerNodeFactory::GetInstance()->CreateTrigger( param_key, candidate );
            }
        }

        // ---------------------------------- EventTriggerNodeVector  ------------------------------------
        for( auto& entry : GetConfigData()->eventTriggerNodeVectorTypeMap )
        {
            const std::string& param_key = entry.first;
            json::QuickInterpreter schema = jsonSchemaBase[ param_key ];
            if( !inputJson->Exist( param_key ) && _useDefaults )
            {
                LOG_INFO_F( "Using the default value ( \"%s\" : \"\" ) for unspecified parameter.\n", param_key.c_str() );
                handleMissingParam( param_key, inputJson->GetDataLocation() );
            }
            else
            {
                std::vector<std::string> candidate_list = GET_CONFIG_VECTOR_STRING( inputJson, (entry.first).c_str() );
                *(entry.second) = EventTriggerNodeFactory::GetInstance()->CreateTriggerList( param_key, candidate_list );
            }
        }

        // ---------------------------------- EventTriggerCoordinator  ------------------------------------
        for( auto& entry : GetConfigData()->eventTriggerCoordinatorTypeMap )
        {
            const std::string& param_key = entry.first;
            json::QuickInterpreter schema = jsonSchemaBase[ param_key ];
            if( !inputJson->Exist( param_key ) && _useDefaults )
            {
                if( _useDefaults )
                {
                    LOG_INFO_F( "Using the default value ( \"%s\" : \"%s\" ) for unspecified parameter.\n", param_key.c_str(), "" );
                }
                else
                {
                    handleMissingParam( param_key, inputJson->GetDataLocation() );
                }
            }
            else
            {
                std::string candidate = (std::string) GET_CONFIG_STRING( inputJson, (entry.first).c_str() );
                *(entry.second) = EventTriggerCoordinatorFactory::GetInstance()->CreateTrigger( param_key, candidate );
            }
        }

        // ---------------------------------- EventTriggerCoordinatorVector  ------------------------------------
        for( auto& entry : GetConfigData()->eventTriggerCoordinatorVectorTypeMap )
        {
            const std::string& param_key = entry.first;
            json::QuickInterpreter schema = jsonSchemaBase[ param_key ];
            if( !inputJson->Exist( param_key ) && _useDefaults )
            {
                LOG_INFO_F( "Using the default value ( \"%s\" : \"\" ) for unspecified parameter.\n", param_key.c_str() );
                handleMissingParam( param_key, inputJson->GetDataLocation() );
            }
            else
            {
                std::vector<std::string> candidate_list = GET_CONFIG_VECTOR_STRING( inputJson, (entry.first).c_str() );
                *(entry.second) = EventTriggerCoordinatorFactory::GetInstance()->CreateTriggerList( param_key, candidate_list );
            }
        }

        delete m_pData;
        m_pData = nullptr;

        if( jsonSchemaBase.Exist( "Sim_Types" ) )
        {
            json::Element sim_types = jsonSchemaBase[ "Sim_Types" ];
            jsonSchemaBase.Clear();
            jsonSchemaBase[ "Sim_Types" ] = sim_types;
        }
        else
        {
            jsonSchemaBase.Clear();
        }

        return true;
    }

    QuickBuilder JsonConfigurable::GetSchema()
    {
        return QuickBuilder( jsonSchemaBase );
    }

    std::set< std::string > JsonConfigurable::missing_parameters_set;

    JsonConfigurable::name2CreatorMapType&
    JsonConfigurable::get_registration_map()
    {
        static JsonConfigurable::name2CreatorMapType name2CreatorMap;
        return name2CreatorMap;
    }

    JsonConfigurable::Registrator::Registrator( const char* classname, get_schema_funcptr_t gs_callback )
    {
        const std::string stored_class_name = std::string( classname );
        get_registration_map()[ stored_class_name ] = gs_callback;
    }
}
