/***************************************************************************************************

Copyright (c) 2018 Intellectual Ventures Property Holdings, LLC (IVPH) All rights reserved.

EMOD is licensed under the Creative Commons Attribution-Noncommercial-ShareAlike 4.0 License.
To view a copy of this license, visit https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode

***************************************************************************************************/

#include "stdafx.h"
#include "Configuration.h"
#include "Configure.h"
#include "Exceptions.h"
#include "IdmString.h"
#include "Log.h"
#include "Debug.h"
#include "Properties.h"
#include "NodeProperties.h"
#include "EventTrigger.h"

#ifndef WIN32
#include <cxxabi.h>
#endif

SETUP_LOGGING( "JsonConfigurable" )

namespace Kernel
{
    // Two ignoreParameter functions; second one is a wrapper around the first.
    // Second function calls the first function twice, first time using a variable pJson configuration, and then a second
    // time using the simulation configuration stored in the EnvPtr. This pattern allows some cross-file dependencies.
    // Example: a campaign parameter can be contingent on a config parameter. Note that the config in the EnvPtr may
    // be null; if the config in the EnvPtr is null (e.g., in a DLL), then an ignorable parameter may still be required.
    bool ignoreParameter( const json::QuickInterpreter * pJson, const char * condition_key, const char * condition_value )
    {
        if( condition_key != nullptr ) 
        {
            if( pJson && pJson->Exist(condition_key) == true )
            {
                // condition_key is in config. Read value 
                auto c_value = (*pJson)[condition_key];

                if( condition_value == nullptr )
                {
                    // condition_value is null, so condition_key is a bool and condition_value is true
                    if( static_cast<int>(c_value.As<json::Number>()) == 0 )
                    {
                        // Condition for using this param is false (mismatch), so returning
                        LOG_DEBUG_F( "bool condition_value found but is false/0. That makes this check fail.\n" );
                        return true;
                    }
                    else
                    {
                        // Conditions match. Continue and return false at end.
                        LOG_DEBUG_F( "bool condition_value found and is true/1. That makes this check pass.\n" );
                    }
                }
                else if( (std::string(condition_key)).rfind("Enable_", 0) == 0 )
                {
                    // condition_key starts with "Enable_", so condition_key is a bool
                    if( static_cast<int>(c_value.As<json::Number>()) != std::stoi(std::string(condition_value),nullptr) )
                    {
                        // Condition for using this param is false (mismatch), so returning
                        LOG_DEBUG_F( "bool condition_value found but does not match specified value. That makes this check fail.\n" );
                        return true;
                    }
                    else
                    {
                        // Conditions match. Continue and return false at end.
                        LOG_DEBUG_F( "bool condition_value found and matches specified value. That makes this check pass.\n" );
                    }
                }
                else
                { 
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
            auto condition_list = json_cast<const json::Object&>(schema["depends-on"]);
            for(auto itr1 = condition_list.Begin(); itr1 != condition_list.End(); itr1++)
            {
                std::string  condition_key       = itr1->name;
                std::string  condition_value_str = "";
                std::string  condition_value_int = "";
                const char * condition_value     = nullptr;

                try
                {
                    condition_value_str = (json::QuickInterpreter(itr1->element)).As<json::String>();
                    condition_value     = condition_value_str.c_str();
                    LOG_DEBUG_F( "schema condition value appears to be string/enum: %s.\n", condition_value );
                }
                catch(...)
                {
                    LOG_DEBUG_F( "schema condition value appears to be bool, not string.\n" );
                    // A bool conditions starting with Enable_ could be null or integer; 
                    if( (std::string(condition_key)).rfind("Enable_", 0) == 0 )
                    {
                        try
                        {
                            condition_value_int = std::to_string(static_cast<int>((json::QuickInterpreter(itr1->element)).As<json::Number>()));
                            condition_value     = condition_value_int.c_str();
                        }
                        catch(...)
                        {
                            // Pass
                        }
                    }
                }

                if( ignoreParameter( pJson, condition_key.c_str(), condition_value ) )
                { 
                    if( ignoreParameter( EnvPtr->Config, condition_key.c_str(), condition_value ) )
                    {
                        return true;
                    }
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

    json::QuickBuilder NodeSetConfig::GetSchema()
    {
        json::QuickBuilder schema( jsonSchemaBase );
        auto tn = JsonConfigurable::_typename_label();
        auto ts = JsonConfigurable::_typeschema_label();
        schema[ tn ] = json::String( "idmType:NodeSet" );
        schema[ ts ]= json::Object();
        schema[ ts ][ "base" ] = json::String( "interventions.idmType.NodeSet" );
        return schema;
    }

    void NodeSetConfig::ConfigureFromJsonAndKey( const Configuration* inputJson, const std::string& key )
    {
        if( !inputJson->Exist( key ) )
        {
            throw MissingParameterFromConfigurationException( __FILE__, __LINE__, __FUNCTION__, inputJson->GetDataLocation().c_str(), key.c_str() );
        }

        _json = (*inputJson)[key];
    }

    /// END NodeSetConfig

    /// EventConfig
    EventConfig::EventConfig()
    {}

    EventConfig::EventConfig(json::QuickInterpreter* qi)
        : _json(*qi)
    {
    }

    void EventConfig::ConfigureFromJsonAndKey( const Configuration* inputJson, const std::string& key )
    {
        if( !inputJson->Exist( key ) )
        {
            throw MissingParameterFromConfigurationException( __FILE__, __LINE__, __FUNCTION__, inputJson->GetDataLocation().c_str(), key.c_str() );
        }
        _json = (*inputJson)[key];
    }

    json::QuickBuilder EventConfig::GetSchema()
    {
        json::QuickBuilder schema( jsonSchemaBase );
        auto tn = JsonConfigurable::_typename_label();
        auto ts = JsonConfigurable::_typeschema_label();
        schema[ tn ] = json::String( "idmType:EventCoordinator" );
        schema[ ts ]= json::Object();
        schema[ ts ][ "base" ] = json::String( "interventions.idmType.EventCoordinator" );
        return schema;
    }

    /// InterventionConfig
    InterventionConfig::InterventionConfig()
        : _json()
    { }

    InterventionConfig::InterventionConfig(json::QuickInterpreter* qi)
        : _json(*qi)
    { }

    void InterventionConfig::ConfigureFromJsonAndKey(const Configuration* inputJson, const std::string& key)
    {
        if(!inputJson->Exist(key))
        {
            throw MissingParameterFromConfigurationException(__FILE__, __LINE__, __FUNCTION__, inputJson->GetDataLocation().c_str(), key.c_str());
        }

        _json = (*inputJson)[key];
    }

    json::QuickBuilder InterventionConfig::GetSchema()
    {
        json::QuickBuilder schema(jsonSchemaBase);
        auto tn = JsonConfigurable::_typename_label();
        auto ts = JsonConfigurable::_typeschema_label();
        schema[tn] = json::String("idmType:Intervention");
        schema[ts] = json::Object();
        schema[ts]["base"] = json::String( "interventions.idmAbstractType.Intervention" );

        return schema;
    }

    void InterventionConfig::serialize(IArchive& ar, InterventionConfig& config)
    {
        if (ar.IsWriter())
        {
            std::ostringstream string_stream;
            json::Writer::Write(config._json, string_stream);
            std::string tmp = string_stream.str();
            ar & tmp;
        }
        else
        {
            std::string json;
            ar & json;
            std::istringstream string_stream(json);
            json::Reader::Read(config._json, string_stream);
        }
    }

    IndividualInterventionConfig::IndividualInterventionConfig()
        : InterventionConfig()
    { }

    IndividualInterventionConfig::IndividualInterventionConfig(json::QuickInterpreter* qi)
        : InterventionConfig(qi)
    { }

    json::QuickBuilder IndividualInterventionConfig::GetSchema()
    {
        json::QuickBuilder schema = InterventionConfig::GetSchema();
        auto tn = JsonConfigurable::_typename_label();
        auto ts = JsonConfigurable::_typeschema_label();
        schema[tn] = json::String("idmType:IndividualIntervention");
        schema[ts]["base"] = json::String("interventions.idmAbstractType.IndividualIntervention");

        return schema;
    }

    IndividualInterventionConfigList::IndividualInterventionConfigList()
        : IndividualInterventionConfig()
    {
        _json = json::Array();
    }

    IndividualInterventionConfigList::IndividualInterventionConfigList(json::QuickInterpreter* qi)
        : IndividualInterventionConfig(qi)
    { }

    json::QuickBuilder IndividualInterventionConfigList::GetSchema()
    {
        json::QuickBuilder schema = IndividualInterventionConfig::GetSchema();
        auto tn = JsonConfigurable::_typename_label();
        auto ts = JsonConfigurable::_typeschema_label();
        schema[tn] = json::String("idmType:IndividualInterventionList");
        IndividualInterventionConfig i_i_config;
        schema[ts] = json::Array();
        schema[ts][0] = i_i_config.GetSchema();
        schema["default"] = json::Array();

        return schema;
    }

    NodeInterventionConfig::NodeInterventionConfig()
        : InterventionConfig()
    { }

    NodeInterventionConfig::NodeInterventionConfig(json::QuickInterpreter* qi)
        : InterventionConfig(qi)
    { }

    json::QuickBuilder NodeInterventionConfig::GetSchema()
    {
        json::QuickBuilder schema = InterventionConfig::GetSchema();
        auto tn = JsonConfigurable::_typename_label();
        auto ts = JsonConfigurable::_typeschema_label();
        schema[tn] = json::String("idmType:NodeIntervention");
        schema[ts]["base"] = json::String("interventions.idmAbstractType.NodeIntervention");

        return schema;
    }

    NodeInterventionConfigList::NodeInterventionConfigList()
        : NodeInterventionConfig()
    {
        _json = json::Array();
    }

    NodeInterventionConfigList::NodeInterventionConfigList(json::QuickInterpreter* qi)
        : NodeInterventionConfig(qi)
    { }

    json::QuickBuilder NodeInterventionConfigList::GetSchema()
    {
        json::QuickBuilder schema = InterventionConfig::GetSchema();
        auto tn = JsonConfigurable::_typename_label();
        auto ts = JsonConfigurable::_typeschema_label();
        schema[tn] = json::String("idmType:NodeInterventionList");
        NodeInterventionConfig n_i_config;
        schema[ts] = json::Array();
        schema[ts][0] = n_i_config.GetSchema();
        schema["default"] = json::Array();

        return schema;
    }
    /// END OF InterventionConfig


    namespace jsonConfigurable
    {
        ConstrainedString::ConstrainedString()
            : constraint_param(nullptr)
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

        const ConstrainedString& ConstrainedString::operator=( const std::string& new_value )
        {
            *((std::string*)(this)) = new_value;
            if( constraint_param &&
                  ( ((constraint_param->size() > 0) && (constraint_param->count( new_value ) == 0) && (new_value != JsonConfigurable::default_string)) ||
                    ((constraint_param->size() == 0) && !new_value.empty() && (new_value != JsonConfigurable::default_string))
                  )
              )
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
    std::set< std::string > JsonConfigurable::empty_set;

    void updateSchemaWithCondition( json::Object& schema, const char* condition_key, const char* condition_value )
    {
        LOG_DEBUG_F( "Setting condition in schema for key %s (value=%s).\n", condition_key, ( condition_value ? condition_value : "1") );

        if( condition_key )
        {
            if(!schema.Exist("depends-on"))
            {
                schema["depends-on"] = json::Object();
            }

            if( condition_value == nullptr )
            {
                // condition_value is null, so condition_key is a bool and condition_value is implicitly true
                json_cast<json::Object&>(schema["depends-on"])[ condition_key ] = json::Number( 1 );

            }
            else if( (std::string(condition_key)).rfind("Enable_", 0) == 0 )
            {
                // condition_key starts with "Enable_", so condition_key is a bool
                json_cast<json::Object&>(schema["depends-on"])[ condition_key ] = json::Number( std::stoi(std::string(condition_value),nullptr) );
            }
            else
            {
                // condition_value is not null, so it's a string (enum)
                json_cast<json::Object&>(schema["depends-on"])[ condition_key ] = json::String( condition_value );
            }
        }
    }

    JsonConfigurable::JsonConfigurable()
        : m_pData( nullptr )
        , jsonSchemaBase()
    {
        // -----------------------------------------------------------------------
        // --- We don't want to create the ConfigData in the constructor because
        // --- some subclasses are copied a lot and this causes this memory to be
        // --- created a lot when it is not needed.
        // -----------------------------------------------------------------------
    }

    JsonConfigurable::JsonConfigurable( const JsonConfigurable& rConfig )
        : m_pData( nullptr ) // !!! Don't copy stuff
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

    IConfigurable* JsonConfigurable::GetConfigurable()
    {
        return static_cast<IConfigurable*>(this);
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

    bool JsonConfigurable::MatchesDependency(const json::QuickInterpreter*              pJson,
                                             const char*                                condition_key,
                                             const char*                                condition_value,
                                             const std::map<std::string, std::string>*  depends_list)
    {
        json::Object newParamSchema;

        updateSchemaWithCondition(newParamSchema, condition_key, condition_value);
        if(depends_list)
        {
            for(auto const pair: *depends_list)
            {
                updateSchemaWithCondition(newParamSchema, (pair.first).c_str(), (pair.second).c_str());
            }
        }

        if(ignoreParameter(newParamSchema, pJson))
        {
            return false;
        }

        return true;
    }

    void
    JsonConfigurable::initConfigTypeMap(
        const char* paramName,
        bool * pVariable,
        const char* description,
        bool defaultvalue,
        const char* condition_key, const char* condition_value,
        const std::map<std::string, std::string>* depends_list
    )
    {
        LOG_DEBUG_F("initConfigTypeMap<bool>: %s\n", paramName);
        GetConfigData()->boolConfigTypeMap[ paramName ] = pVariable;
        json::Object newParamSchema;
        newParamSchema["default"] = json::Number(defaultvalue ? 1 : 0);
        if ( _dryrun )
        {
            newParamSchema["description"] = json::String(description);
            newParamSchema["type"] = json::String( "bool" );
        }

        updateSchemaWithCondition(newParamSchema, condition_key, condition_value);
        if(depends_list)
        {
            for(auto const pair: *depends_list)
            {
                updateSchemaWithCondition(newParamSchema, (pair.first).c_str(), (pair.second).c_str());
            }
        }

        jsonSchemaBase[paramName] = newParamSchema;
    }

    void
    JsonConfigurable::initConfigTypeMap(
        const char* paramName,
        int * pVariable,
        const char * description,
        int min, int max, int defaultvalue,
        const char* condition_key, const char* condition_value,
        const std::map<std::string, std::string>* depends_list
    )
    {
        LOG_DEBUG_F( "initConfigTypeMap<int>: %s\n", paramName);
        GetConfigData()->intConfigTypeMap[ paramName ] = pVariable;
        json::Object newParamSchema;
        newParamSchema["min"] = json::Number(min);
        newParamSchema["max"] = json::Number(max);
        newParamSchema["default"] = json::Number(defaultvalue);
        if ( _dryrun )
        {
            newParamSchema["description"] = json::String(description);
            newParamSchema["type"] = json::String( "integer" );
        }

        updateSchemaWithCondition(newParamSchema, condition_key, condition_value);
        if(depends_list)
        {
            for(auto const pair: *depends_list)
            {
                updateSchemaWithCondition(newParamSchema, (pair.first).c_str(), (pair.second).c_str());
            }
        }

        jsonSchemaBase[paramName] = newParamSchema;
    }

    void
    JsonConfigurable::initConfigTypeMap(
        const char* paramName,
        uint32_t * pVariable,
        const char * description,
        uint32_t min, uint32_t max, uint32_t defaultvalue,
        const char* condition_key, const char* condition_value,
        const std::map<std::string, std::string>* depends_list
    )
    {
        LOG_DEBUG_F( "initConfigTypeMap<int>: %s\n", paramName );
        GetConfigData()->uint32ConfigTypeMap[ paramName ] = pVariable;
        json::Object newParamSchema;
        newParamSchema[ "min" ] = json::Number( min );
        newParamSchema[ "max" ] = json::Number( max );
        newParamSchema[ "default" ] = json::Number( defaultvalue );
        if( _dryrun )
        {
            newParamSchema[ "description" ] = json::String( description );
            newParamSchema[ "type" ] = json::String( "integer" );
        }

        updateSchemaWithCondition(newParamSchema, condition_key, condition_value);
        if(depends_list)
        {
            for(auto const pair: *depends_list)
            {
                updateSchemaWithCondition(newParamSchema, (pair.first).c_str(), (pair.second).c_str());
            }
        }

        jsonSchemaBase[ paramName ] = newParamSchema;
    }

    void
    JsonConfigurable::initConfigTypeMap(
        const char* paramName,
        uint64_t * pVariable,
        const char * description,
        uint64_t min, uint64_t max, uint64_t defaultvalue,
        const char* condition_key, const char* condition_value,
        const std::map<std::string, std::string>* depends_list
    )
    {
        LOG_DEBUG_F( "initConfigTypeMap<int>: %s\n", paramName );
        GetConfigData()->uint64ConfigTypeMap[ paramName ] = pVariable;
        json::Object newParamSchema;
        newParamSchema[ "min" ] = json::Number( min );
        newParamSchema[ "max" ] = json::Number( max );
        newParamSchema[ "default" ] = json::Number( defaultvalue );
        if( _dryrun )
        {
            newParamSchema[ "description" ] = json::String( description );
            newParamSchema[ "type" ] = json::String( "integer" );
        }

        updateSchemaWithCondition(newParamSchema, condition_key, condition_value);
        if(depends_list)
        {
            for(auto const pair: *depends_list)
            {
                updateSchemaWithCondition(newParamSchema, (pair.first).c_str(), (pair.second).c_str());
            }
        }

        jsonSchemaBase[ paramName ] = newParamSchema;
    }

    void
    JsonConfigurable::initConfigTypeMap(
        const char* paramName,
        float * pVariable,
        const char * description,
        float min, float max, float defaultvalue,
        const char* condition_key, const char* condition_value,
        const std::map<std::string, std::string>* depends_list
    )
    {
        LOG_DEBUG_F( "initConfigTypeMap<float>: %s\n", paramName);
        GetConfigData()->floatConfigTypeMap[ paramName ] = pVariable;
        json::Object newParamSchema;
        newParamSchema["min"] = json::Number(min);
        newParamSchema["max"] = json::Number(max);
        newParamSchema["default"] = json::Number(defaultvalue);
        if ( _dryrun )
        {
            newParamSchema["description"] = json::String(description);
            newParamSchema["type"] = json::String( "float" ); 
        }

        updateSchemaWithCondition(newParamSchema, condition_key, condition_value);
        if(depends_list)
        {
            for(auto const pair: *depends_list)
            {
                updateSchemaWithCondition(newParamSchema, (pair.first).c_str(), (pair.second).c_str());
            }
        }

        jsonSchemaBase[paramName] = newParamSchema;
    }

    void
    JsonConfigurable::initConfigTypeMap(
        const char* paramName,
        double * pVariable,
        const char * description,
        double min, double max, double defaultvalue,
        const char* condition_key, const char* condition_value,
        const std::map<std::string, std::string>* depends_list
    )
    {
        LOG_DEBUG_F( "initConfigTypeMap<double>: %s\n", paramName);
        GetConfigData()->doubleConfigTypeMap[ paramName ] = pVariable;
        json::Object newParamSchema;
        newParamSchema["min"] = json::Number(min);
        newParamSchema["max"] = json::Number(max);
        newParamSchema["default"] = json::Number(defaultvalue);
        if ( _dryrun )
        {
            newParamSchema["description"] = json::String(description);
            newParamSchema["type"] = json::String("double");
        }

        updateSchemaWithCondition(newParamSchema, condition_key, condition_value);
        if(depends_list)
        {
            for(auto const pair: *depends_list)
            {
                updateSchemaWithCondition(newParamSchema, (pair.first).c_str(), (pair.second).c_str());
            }
        }

        jsonSchemaBase[paramName] = newParamSchema;
    }

    void
    JsonConfigurable::initConfigTypeMap(
        const char* paramName,
        std::string * pVariable,
        const char * description,
        const std::string& default_str,
        const char* condition_key, const char* condition_value,
        const std::map<std::string, std::string>* depends_list
    )
    {
        LOG_DEBUG_F( "initConfigTypeMap<string>: %s\n", paramName);
        GetConfigData()->stringConfigTypeMap[ paramName ] = pVariable;
        json::Object newParamSchema;
        newParamSchema["default"] = json::String(default_str);
        if ( _dryrun )
        {
            newParamSchema["description"] = json::String(description);
            newParamSchema["type"] = json::String("string");
        }

        updateSchemaWithCondition(newParamSchema, condition_key, condition_value);
        if(depends_list)
        {
            for(auto const pair: *depends_list)
            {
                updateSchemaWithCondition(newParamSchema, (pair.first).c_str(), (pair.second).c_str());
            }
        }

        jsonSchemaBase[paramName] = newParamSchema;
    }

    void
    JsonConfigurable::initConfigTypeMap(
        const char* paramName,
        jsonConfigurable::ConstrainedString * pVariable,
        const char * description,
        const std::string& default_str,
        const char* condition_key, const char* condition_value,
        const std::map<std::string, std::string>* depends_list
    )
    {
        LOG_DEBUG_F( "initConfigTypeMap<ConstrainedString>: %s\n", paramName);
        GetConfigData()->conStringConfigTypeMap[ paramName ] = pVariable;
        json::Object newParamSchema;
        newParamSchema["default"] = json::String(default_str); // would be nice if this always in the constraint list!
        if ( _dryrun )
        {
            newParamSchema["description"] = json::String(description);
            newParamSchema["type"] = json::String("Constrained String");
            newParamSchema["value_source"] = json::String( pVariable->constraints );
        }

        updateSchemaWithCondition(newParamSchema, condition_key, condition_value );
        if(depends_list)
        {
            for(auto const pair: *depends_list)
            {
                updateSchemaWithCondition(newParamSchema, (pair.first).c_str(), (pair.second).c_str());
            }
        }

        jsonSchemaBase[paramName] = newParamSchema;
    }


    void
    JsonConfigurable::initConfigTypeMap(
        const char* paramName,
        jsonConfigurable::tStringSetBase * pVariable,
        const char* description,
        const char* condition_key, const char* condition_value,
        const std::map<std::string, std::string>* depends_list
    )
    {
        LOG_DEBUG_F( "initConfigTypeMap<set<string>>: %s\n", paramName);
        GetConfigData()->stringSetConfigTypeMap[ paramName ] = pVariable;
        json::Object newParamSchema;
        if ( _dryrun )
        {
            newParamSchema["description"] = json::String(description);
            newParamSchema["type"] = json::String( pVariable->getTypeName() );
            newParamSchema["default"] = json::Array();
        
            if( pVariable->getTypeName() == FIXED_STRING_SET_LABEL )
            {
                json::Array pval_array;
                for( auto& value : ((jsonConfigurable::tFixedStringSet*)pVariable)->possible_values )
                {
                    pval_array.Insert(json::String(value), pval_array.End());
                }
                newParamSchema["possible_values"] = pval_array;
            }
            else if( pVariable->getTypeName() == DYNAMIC_STRING_SET_LABEL )
            {
                newParamSchema["value_source"] = json::String( ((jsonConfigurable::tDynamicStringSet*)pVariable)->value_source );
            }
            else
            {
                // just a regular old string set, no problem.
            }
        }

        updateSchemaWithCondition(newParamSchema, condition_key, condition_value );
        if(depends_list)
        {
            for(auto const pair: *depends_list)
            {
                updateSchemaWithCondition(newParamSchema, (pair.first).c_str(), (pair.second).c_str());
            }
        }

        jsonSchemaBase[paramName] = newParamSchema;
    }

    void
    JsonConfigurable::initConfigTypeMap(
        const char* paramName,
        std::vector< std::string > * pVariable,
        const char* description,
        const char* constraint_schema,
        const std::set< std::string > &constraint_variable,
        const char* condition_key, const char* condition_value,
        const std::map<std::string, std::string>* depends_list
    )
    {
        LOG_DEBUG_F( "initConfigTypeMap<vector<string>>: %s\n", paramName);
        GetConfigData()->vectorStringConfigTypeMap[ paramName ] = pVariable;
        GetConfigData()->vectorStringConstraintsTypeMap[ paramName ] = &constraint_variable;
        json::Object newParamSchema;
        if ( _dryrun )
        {
            newParamSchema["description"] = json::String(description);
            newParamSchema["type"] = json::String("Vector String");
            newParamSchema[ "default" ] = json::Array();

            if( constraint_schema )
            {
                newParamSchema["value_source"] = json::String( constraint_schema );
            }
        }

        updateSchemaWithCondition( newParamSchema, condition_key, condition_value );
        if(depends_list)
        {
            for(auto const pair: *depends_list)
            {
                updateSchemaWithCondition(newParamSchema, (pair.first).c_str(), (pair.second).c_str());
            }
        }

        jsonSchemaBase[paramName] = newParamSchema;
    }

    void
    JsonConfigurable::initConfigTypeMap(
        const char* paramName,
        std::vector< std::vector< std::string > > * pVariable,
        const char* description,
        const char* constraint_schema,
        const std::set< std::string > &constraint_variable,
        const char* condition_key, const char* condition_value,
        const std::map<std::string, std::string>* depends_list
    )
    {
        LOG_DEBUG_F( "initConfigTypeMap<vector<vector<string>>>: %s\n", paramName);
        GetConfigData()->vector2dStringConfigTypeMap[ paramName ] = pVariable;
        GetConfigData()->vector2dStringConstraintsTypeMap[ paramName ] = &constraint_variable;
        json::Object newParamSchema;
        newParamSchema["description"] = json::String(description);
        newParamSchema["type"] = json::String("Vector 2d String");
        newParamSchema[ "default" ] = json::Array();
        if( constraint_schema )
        {
            newParamSchema["value_source"] = json::String( constraint_schema );
        }

        updateSchemaWithCondition( newParamSchema, condition_key, condition_value );
        if(depends_list)
        {
            for(auto const pair: *depends_list)
            {
                updateSchemaWithCondition(newParamSchema, (pair.first).c_str(), (pair.second).c_str());
            }
        }

        jsonSchemaBase[paramName] = newParamSchema;
    }

    void
    JsonConfigurable::initConfigTypeMap(
        const char* paramName,
        std::vector< std::vector< std::vector<float> > > * pVariable,
        const char* description,
        float min, float max,
        const char* condition_key, const char* condition_value,
        const std::map<std::string, std::string>* depends_list
    )
    {
        LOG_DEBUG_F( "initConfigTypeMap<vector<vector<vector<float>>>: %s\n", paramName );
        GetConfigData()->vector3dFloatConfigTypeMap[paramName] = pVariable;
        json::Object newParamSchema;
        newParamSchema["min"] = json::Number(min);
        newParamSchema["max"] = json::Number(max);
        if ( _dryrun )
        {
            newParamSchema["description"] = json::String(description);
            newParamSchema["type"] = json::String("Vector3d Float");
            newParamSchema[ "default" ] = json::Array();
        }

        updateSchemaWithCondition( newParamSchema, condition_key, condition_value );
        if(depends_list)
        {
            for(auto const pair: *depends_list)
            {
                updateSchemaWithCondition(newParamSchema, (pair.first).c_str(), (pair.second).c_str());
            }
        }

        jsonSchemaBase[paramName] = newParamSchema;
    }

    void
    JsonConfigurable::initConfigTypeMap(
        const char* paramName,
        std::vector< float > * pVariable,
        const char* description,
        float min, float max, bool ascending,
        const char* condition_key, const char* condition_value,
        const std::map<std::string, std::string>* depends_list
    )
    {
        LOG_DEBUG_F("initConfigTypeMap<vector<float>>: %s\n", paramName);
        GetConfigData()->vectorFloatConfigTypeMap[paramName] = pVariable;
        json::Object newParamSchema;
        newParamSchema["min"] = json::Number(min);
        newParamSchema["max"] = json::Number(max);
        newParamSchema["ascending"] = json::Number(ascending ? 1 : 0);
        if (_dryrun)
        {
            newParamSchema["description"] = json::String(description);
            newParamSchema["type"] = json::String("Vector Float");
            newParamSchema["default"] = json::Array();
        }

        updateSchemaWithCondition(newParamSchema, condition_key, condition_value);
        if(depends_list)
        {
            for(auto const pair: *depends_list)
            {
                updateSchemaWithCondition(newParamSchema, (pair.first).c_str(), (pair.second).c_str());
            }
        }

        jsonSchemaBase[paramName] = newParamSchema;
    }

    void
    JsonConfigurable::initConfigTypeMap(
        const char* paramName,
        std::vector< bool > * pVariable,
        const char* description,
        const char* condition_key, const char* condition_value,
        const std::map<std::string, std::string>* depends_list
    )
    {
        LOG_DEBUG_F( "initConfigTypeMap<vector<bool>>: %s\n", paramName);
        GetConfigData()->vectorBoolConfigTypeMap[ paramName ] = pVariable;
        json::Object newParamSchema;
        if ( _dryrun )
        {
            newParamSchema["description"] = json::String(description);
            newParamSchema["type"] = json::String("Vector Bool");
            newParamSchema["default"] = json::Array();
        }

        updateSchemaWithCondition(newParamSchema, condition_key, condition_value);
        if(depends_list)
        {
            for(auto const pair: *depends_list)
            {
                updateSchemaWithCondition(newParamSchema, (pair.first).c_str(), (pair.second).c_str());
            }
        }

        jsonSchemaBase[paramName] = newParamSchema;
    }

    void
    JsonConfigurable::initConfigTypeMap(
        const char* paramName,
        std::vector< int > * pVariable,
        const char* description,
        int min, int max, bool ascending,
        const char* condition_key, const char* condition_value,
        const std::map<std::string, std::string>* depends_list
    )
    {
        LOG_DEBUG_F( "initConfigTypeMap<vector<int>>: %s\n", paramName);
        GetConfigData()->vectorIntConfigTypeMap[ paramName ] = pVariable;
        json::Object newParamSchema;
        newParamSchema["min"] = json::Number(min);
        newParamSchema["max"] = json::Number(max);
        newParamSchema["ascending"] = json::Number(ascending ? 1 : 0);
        if ( _dryrun )
        {
            newParamSchema["description"] = json::String(description);
            newParamSchema["type"] = json::String("Vector Int");
            newParamSchema["default"] = json::Array();
        }

        updateSchemaWithCondition(newParamSchema, condition_key, condition_value);
        if(depends_list)
        {
            for(auto const pair: *depends_list)
            {
                updateSchemaWithCondition(newParamSchema, (pair.first).c_str(), (pair.second).c_str());
            }
        }

        jsonSchemaBase[paramName] = newParamSchema;
    }

    void
    JsonConfigurable::initConfigTypeMap(
        const char* paramName,
        std::vector< uint32_t > * pVariable,
        const char* description,
        uint32_t min, uint32_t max, bool ascending,
        const char* condition_key, const char* condition_value,
        const std::map<std::string, std::string>* depends_list
    )
    {
        LOG_DEBUG_F( "initConfigTypeMap<vector<uint32_t>>: %s\n", paramName );
        GetConfigData()->vectorUint32ConfigTypeMap[paramName] = pVariable;
        json::Object newParamSchema;
        newParamSchema["min"] = json::Number( min );
        newParamSchema["max"] = json::Number( max );
        newParamSchema["ascending"] = json::Number( ascending ? 1 : 0 );
        if( _dryrun )
        {
            newParamSchema["description"] = json::String( description );
            newParamSchema["type"] = json::String( "Vector Uint32" );
            newParamSchema["default"] = json::Array();
        }

        updateSchemaWithCondition( newParamSchema, condition_key, condition_value );
        if(depends_list)
        {
            for(auto const pair: *depends_list)
            {
                updateSchemaWithCondition(newParamSchema, (pair.first).c_str(), (pair.second).c_str());
            }
        }

        jsonSchemaBase[paramName] = newParamSchema;
    }

    void
    JsonConfigurable::initConfigTypeMap(
        const char* paramName,
        std::vector< std::vector< float > > * pVariable,
        const char* description,
        float min, float max,
        const char* condition_key, const char* condition_value,
        const std::map<std::string, std::string>* depends_list
    )
    {
        LOG_DEBUG_F( "initConfigTypeMap<vector,vector<float>>>: %s\n", paramName);
        GetConfigData()->vector2dFloatConfigTypeMap[ paramName ] = pVariable;
        json::Object newParamSchema;
        newParamSchema["min"] = json::Number(min);
        newParamSchema["max"] = json::Number(max);
        if ( _dryrun )
        {
            newParamSchema["description"] = json::String(description);
            newParamSchema["type"] = json::String("Vector2d Float");
            newParamSchema[ "default" ] = json::Array();
        }

        updateSchemaWithCondition(newParamSchema, condition_key, condition_value);
        if(depends_list)
        {
            for(auto const pair: *depends_list)
            {
                updateSchemaWithCondition(newParamSchema, (pair.first).c_str(), (pair.second).c_str());
            }
        }

        jsonSchemaBase[paramName] = newParamSchema;
    }

    void
    JsonConfigurable::initConfigTypeMap(
        const char* paramName,
        std::vector< std::vector< int > > * pVariable,
        const char* description,
        int min, int max,
        const char* condition_key, const char* condition_value,
        const std::map<std::string, std::string>* depends_list
    )
    {
        LOG_DEBUG_F( "initConfigTypeMap<vector,vector<int>>>: %s\n", paramName);
        GetConfigData()->vector2dIntConfigTypeMap[ paramName ] = pVariable;
        json::Object newParamSchema;
        newParamSchema["min"] = json::Number(min);
        newParamSchema["max"] = json::Number(max);
        if ( _dryrun )
        {
            newParamSchema["description"] = json::String(description);
            newParamSchema["type"] = json::String("Vector2d Int");
            newParamSchema["default"] = json::Array();
        }

        updateSchemaWithCondition(newParamSchema, condition_key, condition_value);
        if(depends_list)
        {
            for(auto const pair: *depends_list)
            {
                updateSchemaWithCondition(newParamSchema, (pair.first).c_str(), (pair.second).c_str());
            }
        }

        jsonSchemaBase[paramName] = newParamSchema;
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
        json::Object newParamSchema;
        if ( _dryrun )
        {
            newParamSchema["description"] = json::String(defaultDesc);
            newParamSchema["type"] = json::String("nested json object (of key-value pairs)");
        }

        jsonSchemaBase[paramName] = newParamSchema;
    }

    void
    JsonConfigurable::initConfigTypeMap(
        const char* paramName,
        tFloatFloatMapConfigType * pVariable,
        const char* description,
        const char* condition_key, const char* condition_value,
        const std::map<std::string, std::string>* depends_list
    )
    {
        LOG_DEBUG_F( "initConfigTypeMap<pwcMap>: %s\n", paramName);
        GetConfigData()->ffMapConfigTypeMap[ paramName ] = pVariable;
        json::Object newParamSchema;
        if ( _dryrun )
        {
            newParamSchema["description"] = json::String(description);
            newParamSchema["type"] = json::String("nested json object (of key-value pairs)");
        }

        updateSchemaWithCondition(newParamSchema, condition_key, condition_value);
        if(depends_list)
        {
            for(auto const pair: *depends_list)
            {
                updateSchemaWithCondition(newParamSchema, (pair.first).c_str(), (pair.second).c_str());
            }
        }

        jsonSchemaBase[paramName] = newParamSchema;
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
        json::Object newParamSchema;
        if ( _dryrun )
        {
            newParamSchema["description"] = json::String(defaultDesc);
            newParamSchema["type"] = json::String("nested json object (of key-value pairs)");
        }

        jsonSchemaBase[paramName] = newParamSchema;
    }

    void
    JsonConfigurable::initConfigTypeMap(
        const char* paramName,
        RangedFloat * pVariable,
        const char * description,
        float defaultvalue,
        const char* condition_key, const char* condition_value,
        const std::map<std::string, std::string>* depends_list
    )
    {
        LOG_DEBUG_F( "initConfigTypeMap<RangedFloat>: %s\n", paramName);
        GetConfigData()->rangedFloatConfigTypeMap[ paramName ] = pVariable;
        json::Object newParamSchema;
        newParamSchema["min"] = json::Number( pVariable->getMin() );
        newParamSchema["max"] = json::Number( pVariable->getMax() );
        newParamSchema["default"] = json::Number(defaultvalue);
        if ( _dryrun )
        {
            newParamSchema["description"] = json::String(description);
            newParamSchema["type"] = json::String( "float" );
        }

        updateSchemaWithCondition(newParamSchema, condition_key, condition_value);
        if(depends_list)
        {
            for(auto const pair: *depends_list)
            {
                updateSchemaWithCondition(newParamSchema, (pair.first).c_str(), (pair.second).c_str());
            }
        }

        jsonSchemaBase[paramName] = newParamSchema;
    }

    void
    JsonConfigurable::initConfigTypeMap(
        const char* paramName,
        NonNegativeFloat * pVariable,
        const char * description,
        float max,
        float defaultvalue,
        const char* condition_key, const char* condition_value,
        const std::map<std::string, std::string>* depends_list
    )
    {
        LOG_DEBUG_F( "initConfigTypeMap<NonNegativeFloat>: %s\n", paramName);
        GetConfigData()->nonNegativeFloatConfigTypeMap[ paramName ] = pVariable;
        json::Object newParamSchema;
        newParamSchema["min"] = json::Number( pVariable->getMin() );
        newParamSchema["max"] = json::Number( pVariable->getMax() );
        newParamSchema["default"] = json::Number(defaultvalue);
        if ( _dryrun )
        {
            newParamSchema["description"] = json::String(description);
            newParamSchema["type"] = json::String( "float" );
        }

        updateSchemaWithCondition(newParamSchema, condition_key, condition_value);
        if(depends_list)
        {
            for(auto const pair: *depends_list)
            {
                updateSchemaWithCondition(newParamSchema, (pair.first).c_str(), (pair.second).c_str());
            }
        }

        jsonSchemaBase[paramName] = newParamSchema;
    }


    void
    JsonConfigurable::initConfigTypeMap(
        const char* paramName,
        NaturalNumber * pVariable,
        const char * description,
        unsigned int max,
        NaturalNumber defaultvalue,
        const char* condition_key, const char* condition_value,
        const std::map<std::string, std::string>* depends_list
    )
    {
        LOG_DEBUG_F( "initConfigTypeMap<NaturalNumber>: %s\n", paramName);
        GetConfigData()->naturalNumberConfigTypeMap[ paramName ] = pVariable;
        json::Object newParamSchema;
        newParamSchema["min"] = json::Number( 0 );
        newParamSchema["max"] = json::Number( max );
        newParamSchema["default"] = json::Number(defaultvalue);
        if ( _dryrun )
        {
            newParamSchema["description"] = json::String(description);
            newParamSchema["type"] = json::String( "NaturalNumber" );
        }

        updateSchemaWithCondition(newParamSchema, condition_key, condition_value);
        if(depends_list)
        {
            for(auto const pair: *depends_list)
            {
                updateSchemaWithCondition(newParamSchema, (pair.first).c_str(), (pair.second).c_str());
            }
        }

        jsonSchemaBase[paramName] = newParamSchema;
    }

    void
    JsonConfigurable::initConfigTypeMap(
        const char* paramName,
        JsonConfigurable * pVariable,
        const char* defaultDesc,
        const char* condition_key, const char* condition_value,
        const std::map<std::string, std::string>* depends_list
    )
    {
        LOG_DEBUG_F( "initConfigTypeMap<JsonConfigurable>: %s\n", paramName);
        GetConfigData()->jcTypeMap[ paramName ] = pVariable;
        json::Object newParamSchema;

        if ( _dryrun )
        {
            std::string variable_type = pVariable->GetTypeName();
            variable_type = std::string("idmType:") + variable_type ;

            bool tmp = _dryrun ;
            _dryrun = true ;
            pVariable->Configure( nullptr );
            _dryrun = tmp ;
            jsonSchemaBase[ variable_type ] = pVariable->GetSchema();

            newParamSchema["description"] = json::String(defaultDesc);
            newParamSchema["type"] = json::String(variable_type);
        }

        updateSchemaWithCondition( newParamSchema, condition_key, condition_value );
        if(depends_list)
        {
            for(auto const pair: *depends_list)
            {
                updateSchemaWithCondition(newParamSchema, (pair.first).c_str(), (pair.second).c_str());
            }
        }

        jsonSchemaBase[paramName] = newParamSchema;
    }

    void JsonConfigurable::initConfigComplexType(
        const char* paramName,
        IComplexJsonConfigurable * pVariable,
        const char* description,
        const char* condition_key, const char* condition_value,
        const std::map<std::string, std::string>* depends_list
    )
    {
        json::QuickBuilder custom_schema = pVariable->GetSchema();
        GetConfigData()->complexTypeMap[ paramName ] = pVariable;

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
        json::Object newParamSchema;
        newParamSchema["description"] = json::String( description );
        newParamSchema["type"] = json::String( custom_type_label_as_json_string );

        if(pVariable->HasValidDefault())
        {
            newParamSchema["default"] = custom_schema["default"];
        }

        updateSchemaWithCondition( newParamSchema, condition_key, condition_value );
        if(depends_list)
        {
            for(auto const pair: *depends_list)
            {
                updateSchemaWithCondition(newParamSchema, (pair.first).c_str(), (pair.second).c_str());
            }
        }

        jsonSchemaBase[ paramName ] = newParamSchema;
    }

    void JsonConfigurable::initConfigComplexCollectionType(
        const char* paramName,
        IComplexJsonConfigurable * pVariable,
        const char* description,
        const char* condition_key, 
        const char* condition_value,
        const std::map<std::string, std::string>* depends_list
    )
    {
        json::QuickBuilder custom_schema = pVariable->GetSchema();

        // going to get something back like : {
        //  "type" : "Vector <element class name>",
        //  "item_type" : "<element class name>,
        //  "default" : []
        //  }
        std::string custom_type_label = (std::string) custom_schema[ _typename_label() ].As<json::String>();
        jsonSchemaBase[ custom_type_label ] = custom_schema[ _typeschema_label() ];

        std::string item_type = std::string("idmType:") + std::string(custom_schema[ "item_type" ].As<json::String>());

        json::Object newParamSchema;
        newParamSchema["description"] = json::String( description );
        newParamSchema["type"] = json::String( std::string("Vector ") + item_type );
        newParamSchema["item_type"] = json::String( item_type );
        newParamSchema["default"] = json::Array();

        updateSchemaWithCondition( newParamSchema, condition_key, condition_value );
        if(depends_list)
        {
            for(auto const pair: *depends_list)
            {
                updateSchemaWithCondition(newParamSchema, (pair.first).c_str(), (pair.second).c_str());
            }
        }

        jsonSchemaBase[ paramName ] = newParamSchema;
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
        GetConfigData()->ipKeyTypeMap[ paramName ] = pVariable;
        json::Object newParamSchema;
        newParamSchema["default"] = json::String("");
        if( _dryrun )
        {
            newParamSchema["description"] = json::String(description);
            newParamSchema["type"] = json::String("Constrained String");
            newParamSchema[ "value_source" ] = json::String( IPKey::GetConstrainedStringConstraintKey() );
        }

        jsonSchemaBase[paramName] = newParamSchema;

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
        GetConfigData()->ipKeyValueTypeMap[ paramName ] = pVariable;
        json::Object newParamSchema;
        newParamSchema["default"] = json::String("");
        if( _dryrun )
        {
            newParamSchema["description"] = json::String(description);
            newParamSchema[ "type" ] = json::String( "Constrained String" );
            newParamSchema[ "value_source" ] = json::String( IPKey::GetConstrainedStringConstraintKeyValue() );
        }

        jsonSchemaBase[paramName] = newParamSchema;

        if( pVariable->GetParameterName().empty() )
        {
            pVariable->SetParameterName( paramName );
        }
    }

    void
    JsonConfigurable::initConfigTypeMap(
        const char* paramName,
        std::vector<IPKeyValue>* pVariable,
        const char* description,
        const char* condition_key,
        const char* condition_value,
        const std::map<std::string, std::string>* depends_list
    )
    {
        LOG_DEBUG_F("initConfigTypeMap<vector<IPKeyValue>>: %s\n", paramName);
        GetConfigData()->iPKeyValueVectorMapType[paramName] = pVariable;
        json::Object newParamSchema;
        if (_dryrun)
        {
            newParamSchema["description"] = json::String(description);
            newParamSchema["type"] = json::String("Vector Constrained String");
            newParamSchema[ "default" ] = json::Array();
            newParamSchema["value_source"] = json::String(IPKey::GetConstrainedStringConstraintKeyValue());

            updateSchemaWithCondition( newParamSchema, condition_key, condition_value );
            if(depends_list)
            {
                for(auto const pair: *depends_list)
                {
                    updateSchemaWithCondition(newParamSchema, (pair.first).c_str(), (pair.second).c_str());
                }
            }
        }

        jsonSchemaBase[paramName] = newParamSchema;
    }

    void
    JsonConfigurable::initConfigTypeMap(
        const char* paramName,
        NPKey * pVariable,
        const char * description
    )
    {
        LOG_DEBUG_F( "initConfigTypeMap<NPKey>: %s\n", paramName);
        GetConfigData()->npKeyTypeMap[ paramName ] = pVariable;
        json::Object newParamSchema;
        newParamSchema["default"] = json::String("");
        if( _dryrun )
        {
            newParamSchema["description"] = json::String(description);
            newParamSchema[ "type" ] = json::String( "Constrained String" );
            newParamSchema[ "value_source" ] = json::String( NPKey::GetConstrainedStringConstraintKey() );
        }

        jsonSchemaBase[paramName] = newParamSchema;

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
        GetConfigData()->npKeyValueTypeMap[ paramName ] = pVariable;
        json::Object newParamSchema;
        newParamSchema["default"] = json::String("");
        if( _dryrun )
        {
            newParamSchema["description"] = json::String(description);
            newParamSchema[ "type" ] = json::String( "Constrained String" );
            newParamSchema[ "value_source" ] = json::String( NPKey::GetConstrainedStringConstraintKeyValue() );
        }

        jsonSchemaBase[paramName] = newParamSchema;

        if( pVariable->GetParameterName().empty() )
        {
            pVariable->SetParameterName( paramName );
        }
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
                    LOG_DEBUG_F( "Using the default value ( \"%s\" : %d ) for unspecified parameter.\n", key.c_str(), val );
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
                    LOG_DEBUG_F( "Using the default value ( \"%s\" : %d ) for unspecified parameter.\n", key.c_str(), val );
                    *(entry.second) = val;
                }
                else // not in config, not using defaults, no depends-on, just plain missing
                {
                    handleMissingParam( key, inputJson->GetDataLocation() );
                }
            }

            LOG_DEBUG_F( "the key %s = uint32_t %u\n", key.c_str(), *(entry.second) );
        }

        // ---------------------------------- uint64_t -------------------------------------
        for( auto& entry : GetConfigData()->uint64ConfigTypeMap )
        {
            const std::string& key = entry.first;
            json::QuickInterpreter schema = jsonSchemaBase[ key ];
            uint64_t val = 0;

            if( ignoreParameter( schema, inputJson ) )
            {
                continue; // param is missing and that's ok." << std::endl;
            }

            // check if parameter was specified in input json (TODO: improve performance by getting the iterator here with Find() and reusing instead of GET_CONFIG_INTEGER below)
            if( inputJson->Exist( key ) )
            {
                // get specified configuration parameter
                double jsonValueAsDouble = GET_CONFIG_DOUBLE( inputJson, key.c_str() );
                if( jsonValueAsDouble != (uint64_t)jsonValueAsDouble )
                {
                    std::ostringstream errMsg; // using a non-parameterized exception.
                    errMsg << "The value for parameter '"<< key << "' appears to be a decimal ("
                           << jsonValueAsDouble
                           << ") but needs to be an integer." << std::endl;
                    throw Kernel::GeneralConfigurationException( __FILE__, __LINE__, __FUNCTION__, errMsg.str().c_str() );
                }
                val = uint64_t( jsonValueAsDouble );
                // throw exception if value is outside of range
                EnforceParameterRange<uint64_t>( key, val, schema );
                *(entry.second) = val;
            }
            else
            {
                if( _useDefaults )
                {
                    // using the default value
                    val = (uint64_t)schema[ "default" ].As<json::Number>();
                    LOG_DEBUG_F( "Using the default value ( \"%s\" : %d ) for unspecified parameter.\n", key.c_str(), val );
                    *(entry.second) = val;
                }
                else // not in config, not using defaults, no depends-on, just plain missing
                {
                    handleMissingParam( key, inputJson->GetDataLocation() );
                }
            }

            LOG_DEBUG_F( "the key %s = uint64_t %u\n", key.c_str(), *(entry.second) );
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
                    LOG_DEBUG_F( "Using the default value ( \"%s\" : %f ) for unspecified parameter.\n", key.c_str(), val );
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
                    LOG_DEBUG_F( "Using the default value ( \"%s\" : %f ) for unspecified parameter.\n", key.c_str(), val );
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
                    LOG_DEBUG_F( "Using the default value ( \"%s\" : %f ) for unspecified parameter.\n", key.c_str(), val );
                    *(entry.second) = val;
                }
                else 
                {
                    handleMissingParam( key, inputJson->GetDataLocation() );
                }
            }

            LOG_DEBUG_F("the key %s = float %f\n", key.c_str(), (float) *(entry.second));
        }

        // ---------------------------------- NONNEGATIVEFLOAT ------------------------------------
        for (auto& entry : GetConfigData()->nonNegativeFloatConfigTypeMap)
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
                    LOG_DEBUG_F( "Using the default value ( \"%s\" : %f ) for unspecified parameter.\n", key.c_str(), val );
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
                    LOG_DEBUG_F( "Using the default value ( \"%s\" : %f ) for unspecified parameter.\n", key.c_str(), val );
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
                    LOG_DEBUG_F( "Using the default value ( \"%s\" : \"%s\" ) for unspecified parameter.\n", key.c_str(), val.c_str() );
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
                    LOG_DEBUG_F( "Using the default value ( \"%s\" : \"%s\" ) for unspecified parameter.\n", key.c_str(), val.c_str() );
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
                    //LOG_DEBUG_F( "Using the default value ( \"%s\" : <empty string set> ) for unspecified string set parameter.\n", key.c_str() );
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
                    LOG_DEBUG_F( "Using the default value ( \"%s\" : <empty string vector> ) for unspecified string vector parameter.\n", key.c_str() );
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
                    LOG_DEBUG_F( "Using the default value ( \"%s\" : <empty string Vector2D> ) for unspecified string Vector2D parameter.\n", key.c_str() );
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

        //----------------------------------- VECTOR of BOOLs ------------------------------
        for (auto& entry : GetConfigData()->vectorBoolConfigTypeMap)
        {
            const std::string& key = entry.first;
            json::QuickInterpreter schema = jsonSchemaBase[key];
            if( ignoreParameter( schema, inputJson ) )
            {
                continue; // param is missing and that's ok.
            }

            if( inputJson->Exist(key) )
            {
                std::vector<bool> configValues = GET_CONFIG_VECTOR_BOOL( inputJson, (entry.first).c_str() );
                *(entry.second) = configValues;
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
            const std::string& key = entry.first;
            JsonConfigurable* pJc = entry.second;
            json::QuickInterpreter schema = jsonSchemaBase[key];

            if ( ignoreParameter( schema, inputJson ) )
            {
               // param is missing and that's okay
               continue;
            }

            if( inputJson->Exist(key) )
            {
                Configuration* p_config = Configuration::CopyFromElement( (*inputJson)[key], inputJson->GetDataLocation() );
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
