
/***************************************************************************************************

EMOD is licensed under the Creative Commons Attribution-Noncommercial-ShareAlike 4.0 License.
To view a copy of this license, visit https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode

***************************************************************************************************/

#pragma once

#include "IdmApi.h"
#include "CajunIncludes.h"
#include "Configuration.h"      // for MetadataDescriptor namespace
#include "ConfigurationImpl.h"  // for MetadataDescriptor namespace
#include "Exceptions.h"
#include "Debug.h"
#include "Types.h"

// 'resource' files with description texts for schema
#include "config_params.rc"
#include "polio_params.rc"
#include "iv_params.rc"

#include <stdarg.h>
#include <stdexcept>
#include <sstream>
#include <fstream>
#include <map>
#include <set>
#include <vector>
#include <iterator>

#ifndef WIN32
#include <limits>
#include <values.h>
#endif

#include "Environment.h"
#include "Log.h"
#include "EventTrigger.h" // just a simple enum

namespace Kernel
{
    class IPKey;
    class IPKeyValue;
    class NPKey;
    class NPKeyValue;

    struct IDMAPI IComplexJsonConfigurable
    {
        virtual void ConfigureFromJsonAndKey( const Configuration* inputJson, const std::string& key ) = 0;
        virtual json::QuickBuilder GetSchema() = 0;
        virtual bool  HasValidDefault() const = 0;
    };

#define FIXED_STRING_SET_LABEL "Fixed String Set"
#define DYNAMIC_STRING_SET_LABEL "Dynamic String Set"

    namespace jsonConfigurable
    {
        typedef std::set< std::string > tStringSet;
        class tStringSetBase : public tStringSet
        {
            public:
                virtual std::string getTypeName() const = 0;
        };

        class tFixedStringSet : public tStringSetBase
        {
            public:
                tFixedStringSet() {}
                virtual std::string getTypeName() const
                {
                    return std::string( FIXED_STRING_SET_LABEL );
                }
                tStringSet possible_values;
        };

        class tDynamicStringSet : public tStringSetBase
        {
            public:
                tDynamicStringSet() {}
                virtual std::string getTypeName() const
                {
                    return std::string( DYNAMIC_STRING_SET_LABEL );
                }
                std::string value_source;

                static void serialize( IArchive& ar, tDynamicStringSet& dyn )
                {
                    ar.startObject();
                    ar.labelElement("values");
                    size_t count = ar.IsWriter() ? dyn.size() : -1;

                    ar.startArray(count);
                    if( ar.IsWriter() )
                    {
                        for (auto& entry : dyn)
                        {
                            std::string str = entry;
                            ar & str;
                        }
                    }
                    else
                    {
                        for (size_t i = 0; i < count; ++i)
                        {
                            std::string entry;
                            ar & entry;
                            dyn.insert( entry );
                        }
                    }
                    ar.endArray();
                    ar.labelElement("value_source") & dyn.value_source;
                    ar.endObject();
                }
        };

        class ConstrainedString : public std::string
        {
            public:
                ConstrainedString();

                ConstrainedString( const std::string &init_str );
                ConstrainedString( const char *init_str );

                virtual const ConstrainedString& operator=( const std::string& new_value );

                std::string constraints;
                tStringSet * constraint_param;
                std::string parameter_name;
        };

        typedef std::map< std::string, ConstrainedString * > tConStringConfigTypeMapType;
    }

    bool ignoreParameter( const json::QuickInterpreter& schema, const json::QuickInterpreter * pJson );
    bool ignoreParameter( const json::QuickInterpreter * pJson, const char * condition_key, const char * condition_value = nullptr );

    std::pair<std::string, std::string> getCondition( const json::QuickInterpreter jsonObj );

    void updateSchemaWithCondition( json::Object& schema, const char* condition_key, const char* condition_value );


    class IDMAPI JsonConfigurable : public IConfigurable
    {
        friend class InterventionFactory;
        friend class DemographicRestrictions;
        friend class DistributionConstantConfigurable;
        friend class DurationDistribution;
        friend class DistributionExponentialConfigurable;
        friend class DistributionGammaConfigurable;
        friend class DistributionGaussianConfigurable;
        friend class DistributionLogNormalConfigurable;
        friend class DistributionPoissonConfigurable;
        friend class DistributionWeibullConfigurable;
        friend class DistributionDualConstantConfigurable;
        friend class DistributionDualExponentialConfigurable;
        friend class DistributionUniformConfigurable;
        friend class DistributionPiecewiseConstantConfigurable;
        friend class DistributionPiecewiseLinearConfigurable;

    public:
        typedef std::map< float, float > tFloatFloatMapConfigType;
        typedef std::map< std::string, float > tStringFloatMapConfigType;
        static const char * default_string;

        virtual IConfigurable* GetConfigurable() override;

    private:
        typedef std::map< std::string, bool * > tBoolConfigTypeMapType;
        typedef std::map< std::string, int * > tIntConfigTypeMapType;
        typedef std::map< std::string, uint32_t * > tUint32ConfigTypeMapType;
        typedef std::map< std::string, uint64_t * > tUint64ConfigTypeMapType;
        typedef std::map< std::string, float * > tFloatConfigTypeMapType;
        typedef std::map< std::string, double * > tDoubleConfigTypeMapType;
        typedef std::map< std::string, std::string * > tStringConfigTypeMapType;
        typedef std::map< std::string, void * > tEnumConfigTypeMapType;
        typedef std::map< std::string, std::set< std::string > * > tStringSetConfigTypeMapType;
        typedef std::map< std::string, std::vector< std::string > * > tVectorStringConfigTypeMapType;
        typedef std::map< std::string, std::vector< std::vector< std::string > > * > tVector2dStringConfigTypeMapType;
        typedef std::map< std::string, const std::set< std::string > * > tVectorStringConstraintsTypeMapType;
        typedef std::map< std::string, std::vector< float > * > tVectorFloatConfigTypeMapType;
        typedef std::map< std::string, std::vector< bool > * > tVectorBoolConfigTypeMapType;
        typedef std::map< std::string, std::vector< int > * > tVectorIntConfigTypeMapType;
        typedef std::map< std::string, std::vector< uint32_t > * >tVectorUint32ConfigTypeMapType;
        typedef std::map< std::string, std::vector< std::vector< float > > * > tVector2dFloatConfigTypeMapType;
        typedef std::map< std::string, std::vector< std::vector< std::vector< float > > >* > tVector3dFloatConfigTypeMapType;
        typedef std::map< std::string, std::vector< std::vector< int > > * > tVector2dIntConfigTypeMapType;
        typedef std::map< std::string, tFloatFloatMapConfigType * > tFloatFloatMapConfigTypeMapType;
        typedef std::map< std::string, tStringFloatMapConfigType * > tStringFloatMapConfigTypeMapType;
        typedef std::map< std::string, RangedFloat * > tRangedFloatConfigTypeMapType;
        typedef std::map< std::string, NonNegativeFloat * > tNonNegativeFloatConfigTypeMapType;
        typedef std::map< std::string, NaturalNumber * > tNNConfigTypeMapType;
        typedef std::map< std::string, JsonConfigurable * > tJsonConfigurableMapType;
        typedef std::map< std::string, IComplexJsonConfigurable * > tComplexJsonConfigurableMapType;
        typedef std::map< std::string, IPKey * > tIPKeyMapType;
        typedef std::map< std::string, IPKeyValue * > tIPKeyValueMapType;
        typedef std::map< std::string, std::vector<IPKeyValue>* > tIPKeyValueVectorMapType;
        typedef std::map< std::string, NPKey * > tNPKeyMapType;
        typedef std::map< std::string, NPKeyValue * > tNPKeyValueMapType;

    public:
        virtual json::QuickBuilder GetSchema();

        virtual std::string GetTypeName() const;

        static bool _dryrun;
        static bool _useDefaults;
        static bool _track_missing;
        typedef json::QuickBuilder (*get_schema_funcptr_t)(void);
        typedef std::map< std::string, get_schema_funcptr_t > name2CreatorMapType;
        static name2CreatorMapType &get_registration_map();

        struct IDMAPI Registrator
        {
            Registrator( const char* class_name, get_schema_funcptr_t gs_callback );
        };

#pragma warning( push )
#pragma warning( disable: 4251 ) // See IdmApi.h for details
        static jsonConfigurable::tStringSet missing_parameters_set;

        bool MatchesDependency(const json::QuickInterpreter*              pJson,
                               const char*                                condition_key   = nullptr,
                               const char*                                condition_value = nullptr,
                               const std::map<std::string, std::string>*  depends_list    = nullptr);

        // TEST ONLY - componentTests needs to clear this so that other tests don't fail
        static void ClearMissingParameters() { missing_parameters_set.clear() ; }

    protected:
        struct ConfigData
        {
            tBoolConfigTypeMapType boolConfigTypeMap;
            tIntConfigTypeMapType intConfigTypeMap;
            tUint32ConfigTypeMapType uint32ConfigTypeMap;
            tUint64ConfigTypeMapType uint64ConfigTypeMap;
            tFloatConfigTypeMapType floatConfigTypeMap;
            tDoubleConfigTypeMapType doubleConfigTypeMap;
            tEnumConfigTypeMapType enumConfigTypeMap;
            tStringConfigTypeMapType stringConfigTypeMap;
            tStringSetConfigTypeMapType stringSetConfigTypeMap;
            jsonConfigurable::tConStringConfigTypeMapType conStringConfigTypeMap;
            tVectorStringConfigTypeMapType vectorStringConfigTypeMap;
            tVector2dStringConfigTypeMapType vector2dStringConfigTypeMap;
            tVectorStringConstraintsTypeMapType vectorStringConstraintsTypeMap;
            tVectorStringConstraintsTypeMapType vector2dStringConstraintsTypeMap;
            tVectorFloatConfigTypeMapType vectorFloatConfigTypeMap;
            tVectorBoolConfigTypeMapType vectorBoolConfigTypeMap;
            tVectorIntConfigTypeMapType vectorIntConfigTypeMap;
            tVectorUint32ConfigTypeMapType vectorUint32ConfigTypeMap;
            tVector2dFloatConfigTypeMapType vector2dFloatConfigTypeMap;
            tVector3dFloatConfigTypeMapType vector3dFloatConfigTypeMap;
            tVector2dIntConfigTypeMapType vector2dIntConfigTypeMap;
            tFloatFloatMapConfigTypeMapType ffMapConfigTypeMap;
            tStringFloatMapConfigTypeMapType sfMapConfigTypeMap;
            tRangedFloatConfigTypeMapType rangedFloatConfigTypeMap;
            tNonNegativeFloatConfigTypeMapType nonNegativeFloatConfigTypeMap;
            tNNConfigTypeMapType naturalNumberConfigTypeMap;
            tJsonConfigurableMapType jcTypeMap;
            tComplexJsonConfigurableMapType complexTypeMap;
            tIPKeyMapType ipKeyTypeMap ;
            tIPKeyValueMapType ipKeyValueTypeMap;
            tIPKeyValueVectorMapType iPKeyValueVectorMapType;
            tNPKeyMapType npKeyTypeMap ;
            tNPKeyValueMapType npKeyValueTypeMap; 
        };
    private:
        // make this private so subclasses have to call GetConfigData()
        ConfigData* m_pData;
        
    protected:
        json::Object jsonSchemaBase;

        ConfigData* GetConfigData();
        json::Object& GetSchemaBase();

        static std::set< std::string > empty_set;
#pragma warning( pop )

        JsonConfigurable();
        JsonConfigurable( const JsonConfigurable& rConfig );
        virtual ~JsonConfigurable();

        static const char * default_description;

        void initSimTypes( unsigned int count, ... )
        {
            json::Array arraySimTypes;
            va_list ap;
            va_start( ap, count );
            for( unsigned int idx = 0; idx<count; idx++ )
            {
                arraySimTypes.Insert( json::String( va_arg( ap, const char* ) ) );
            }
            va_end( ap );
            jsonSchemaBase[ "Sim_Types" ] = arraySimTypes;
        }

        void initConfigTypeMap(
            const char* paramName,
            bool * pVariable,
            const char* description = default_description,
            bool defaultvalue = false,
            const char* condition_key = nullptr, const char* condition_value = nullptr,
            const std::map<std::string, std::string>* depends_list = nullptr
        );

        void initConfigTypeMap(
            const char* paramName,
            int * pVariable,
            const char* description = default_description,
            int min = INT_MIN, int max = INT_MAX, int defaultvalue = 0,
            const char* condition_key = nullptr, const char* condition_value = nullptr,
            const std::map<std::string, std::string>* depends_list = nullptr
        );

        void initConfigTypeMap(
            const char* paramName,
            uint32_t * pVariable,
            const char* description = default_description,
            uint32_t min = 0, uint32_t max = UINT_MAX, uint32_t defaultvalue = 0,
            const char* condition_key = nullptr, const char* condition_value = nullptr,
            const std::map<std::string, std::string>* depends_list = nullptr
        );

        void initConfigTypeMap(
            const char* paramName,
            uint64_t * pVariable,
            const char* description = default_description,
            uint64_t min = 0, uint64_t max = UINT_MAX, uint64_t defaultvalue = 0,
            const char* condition_key = nullptr, const char* condition_value = nullptr,
            const std::map<std::string, std::string>* depends_list = nullptr
        );

        void initConfigTypeMap(
            const char* paramName,
            float * pVariable,
            const char* description = default_description,
            float min = -FLT_MAX, float max = FLT_MAX, float defaultvalue = 1.0,
            const char* condition_key = nullptr, const char* condition_value = nullptr,
            const std::map<std::string, std::string>* depends_list = nullptr
        );

        void initConfigTypeMap(
            const char* paramName,
            double * pVariable,
            const char* description = default_description,
            double min = -DBL_MAX, double max = DBL_MAX, double defaultvalue = 1.0,
            const char* condition_key = nullptr, const char* condition_value = nullptr,
            const std::map<std::string, std::string>* depends_list = nullptr
        );

        void initConfigTypeMap(
            const char* paramName,
            std::string * pVariable,
            const char* description = default_description,
            const std::string& default_str = default_string,
            const char* condition_key = nullptr, const char* condition_value = nullptr,
            const std::map<std::string, std::string>* depends_list = nullptr
        );

        void initConfigTypeMap(
            const char* paramName,
            jsonConfigurable::ConstrainedString * pVariable,
            const char* description = default_description,
            const std::string& default_str = default_string,
            const char* condition_key = nullptr, const char* condition_value = nullptr,
            const std::map<std::string, std::string>* depends_list = nullptr
        );

        void initConfigTypeMap(
            const char* paramName,
            jsonConfigurable::tStringSetBase * pVariable,
            const char* description = default_description,
            const char* condition_key = nullptr, const char* condition_value = nullptr,
            const std::map<std::string, std::string>* depends_list = nullptr
        );

        void initConfigTypeMap(
            const char* paramName,
            std::vector< std::string > * pVariable,
            const char* description = default_description,
            const char* constraint_schema = nullptr,
            const std::set< std::string > &constraint_variable = empty_set,
            const char* condition_key = nullptr, const char* condition_value = nullptr,
            const std::map<std::string, std::string>* depends_list = nullptr
        );

        void initConfigTypeMap(
            const char* paramName,
            std::vector< std::vector< std::string > > * pVariable,
            const char* description = default_description,
            const char* constraint_schema = nullptr,
            const std::set< std::string > &constraint_variable = empty_set,
            const char* condition_key = nullptr, const char* condition_value = nullptr,
            const std::map<std::string, std::string>* depends_list = nullptr
        );

        void initConfigTypeMap(
            const char* paramName,
            std::vector< float > * pVariable,
            const char* description = default_description,
            float min = -FLT_MAX, float max = FLT_MAX, bool ascending = false,
            const char* condition_key = nullptr, const char* condition_value = nullptr,
            const std::map<std::string, std::string>* depends_list = nullptr
        );

        void initConfigTypeMap(
            const char* paramName,
            std::vector< bool > * pVariable,
            const char* description = default_description,
            const char* condition_key = nullptr, const char* condition_value = nullptr,
            const std::map<std::string, std::string>* depends_list = nullptr
        );

        void initConfigTypeMap(
            const char* paramName,
            std::vector< int > * pVariable,
            const char* description = default_description,
            int min = -INT_MAX, int max = INT_MAX, bool ascending = false,
            const char* condition_key = nullptr, const char* condition_value = nullptr,
            const std::map<std::string, std::string>* depends_list = nullptr
        );

        void initConfigTypeMap(
            const char* paramName,
            std::vector< uint32_t > * pVariable,
            const char* description = default_description,
            uint32_t min = 0, uint32_t max = UINT32_MAX, bool ascending = false,
            const char* condition_key = nullptr, const char* condition_value = nullptr,
            const std::map<std::string, std::string>* depends_list = nullptr
        );

        void initConfigTypeMap(
            const char* paramName,
            std::vector< std::vector< float > > * pVariable,
            const char* description = default_description,
            float min = -FLT_MAX, float max = FLT_MAX,
            const char* condition_key = nullptr, const char* condition_value = nullptr,
            const std::map<std::string, std::string>* depends_list = nullptr
        );

        void initConfigTypeMap( const char* paramName,
            std::vector< std::vector< std::vector<float> > > * pVariable,
            const char* description = default_description,
            float min = -FLT_MAX, float max = FLT_MAX,
            const char* condition_key = nullptr, const char* condition_value = nullptr,
            const std::map<std::string, std::string>* depends_list = nullptr
        );

        void initConfigTypeMap(
            const char* paramName,
            std::vector< std::vector< int > > * pVariable,
            const char* description = default_description,
            int min = -INT_MAX, int max = INT_MAX,
            const char* condition_key = nullptr, const char* condition_value = nullptr,
            const std::map<std::string, std::string>* depends_list = nullptr
        );

        void initConfigTypeMap(
            const char* paramName,
            tFloatFloatMapConfigType * pVariable,
            const char* defaultDesc
        );

        void initConfigTypeMap(
            const char* paramName,
            tFloatFloatMapConfigType * pVariable,
            const char* description,
            const char* condition_key = nullptr, const char* condition_value = nullptr,
            const std::map<std::string, std::string>* depends_list = nullptr
        );

        void initConfigTypeMap(
            const char* paramName,
            tStringFloatMapConfigType * pVariable,
            const char* defaultDesc
        );

        void initConfigTypeMap(
            const char* paramName,
            RangedFloat * pVariable,
            const char* description = default_description,
            float defaultvalue = 1.0,
            const char* condition_key = nullptr, const char* condition_value = nullptr,
            const std::map<std::string, std::string>* depends_list = nullptr
        );

        void initConfigTypeMap(
            const char* paramName,
            NonNegativeFloat * pVariable,
            const char* description = default_description,
            float max = 1.0,
            float defaultvalue = 1.0,
            const char* condition_key = nullptr, const char* condition_value = nullptr,
            const std::map<std::string, std::string>* depends_list = nullptr
        );

        void initConfigTypeMap(
            const char* paramName,
            NaturalNumber * pVariable,
            const char* description = default_description,
            unsigned int max = INT_MAX,
            NaturalNumber defaultvalue = 1,
            const char* condition_key = nullptr, const char* condition_value = nullptr,
            const std::map<std::string, std::string>* depends_list = nullptr
        );

        void initConfigTypeMap(
            const char* paramName,
            JsonConfigurable * pVariable,
            const char* defaultDesc,
            const char* condition_key=nullptr, const char* condition_value=nullptr,
            const std::map<std::string, std::string>* depends_list = nullptr
        );

        void initConfigTypeMap(
            const char* paramName,
            IPKey * pVariable,
            const char* defaultDesc
        );

        void initConfigTypeMap(
            const char* paramName,
            IPKeyValue * pVariable,
            const char* defaultDesc
        );

        void initConfigTypeMap(
            const char* paramName,
            std::vector<IPKeyValue>* pVariable,
            const char* description = default_description,
            const char* condition_key = nullptr,
            const char* condition_value = nullptr,
            const std::map<std::string, std::string>* depends_list = nullptr
        );

        void initConfigTypeMap(
            const char* paramName,
            NPKey * pVariable,
            const char* defaultDesc
        );

        void initConfigTypeMap(
            const char* paramName,
            NPKeyValue * pVariable,
            const char* defaultDesc
        );

        template< typename T >
        void EnforceParameterRange( const std::string& key, T value, json::QuickInterpreter& jsonObj )
        {
            T min = (T)jsonObj["min"].As<json::Number>();
            T max = (T)jsonObj["max"].As<json::Number>();

            // min and max values are included in the range of valid numbers 
            if ( value > max )
            {
                throw Kernel::ConfigurationRangeException( __FILE__, __LINE__, __FUNCTION__, key.c_str(), value, max );
            }

            if ( value < min )
            {
                throw Kernel::ConfigurationRangeException( __FILE__, __LINE__, __FUNCTION__, key.c_str(), value, min );
            }
        }


        template< typename T >
        void EnforceParameterAscending(const std::string& key, const std::vector<T> & values)
        {
            //Try to find a value to the left of an element with a value that is greater or equal 
            if( values.size() > 1 )
            {
                for (auto it = values.cbegin(); it != values.cend() - 1; ++it)
                {
                    if (*it >= *(it + 1))
                    {
                        std::stringstream error_string;
                        error_string << "The values in " << key << " must be unique and in ascending order.";
                        throw InvalidInputDataException(__FILE__, __LINE__, __FUNCTION__, error_string.str().c_str());
                    }
                }
            }
        }

        template< typename T >
        void EnforceVectorParameterRanges( const std::string& key, std::vector<T> values, json::QuickInterpreter& jsonObj )
        {
            for (T& value : values)
            {
                EnforceParameterRange<T>(key, value, jsonObj);
            }

            if (jsonObj.Exist("ascending") && jsonObj["ascending"].As<json::Number>())
            {
                EnforceParameterAscending<T>(key, values);
            }
        }

        template< typename myclass >
        bool initConfig(
            const char* key,
            myclass &thevar,
            const Configuration * pJson,
            const MetadataDescriptor::Enum &enum_md,
            const char* condition_key = nullptr, const char * condition_value = nullptr,
            const std::map<std::string, std::string> * depends_list = nullptr
        )
        {
            MetadataDescriptor::Enum * pEnumMd = const_cast<MetadataDescriptor::Enum *>(&enum_md);
            json::Object newParamSchema = json_cast<const json::Object&>(pEnumMd->GetSchemaElement());

            updateSchemaWithCondition(newParamSchema, condition_key, condition_value);
            if(depends_list)
            {
                for(auto const pair: *depends_list)
                {
                    updateSchemaWithCondition(newParamSchema, (pair.first).c_str(), (pair.second).c_str());
                }
            }

            jsonSchemaBase[key] = newParamSchema;

            if( ignoreParameter( newParamSchema, pJson ) )
            {
                return true; // param is missing and that's ok.
            }

            if (pJson && pJson->Exist(key) == false)
            {
                if( _useDefaults )
                {
                    if( (EnvPtr != nullptr) && EnvPtr->Log->CheckLogLevel(Logger::INFO, "JsonConfigurable"))
                    {
                        EnvPtr->Log->Log(Logger::INFO, "JsonConfigurable", "Using the default value ( \"%s\" : \"%s\" ) for unspecified parameter.\n", key, enum_md.enum_value_specs[0].first.c_str() );
                    }
                    thevar = (myclass) enum_md.enum_value_specs[0].second;
                }
                else
                {
                    handleMissingParam( key, pJson->GetDataLocation() );
                }

                return false;
            }

            std::string enum_value_string(GET_CONFIG_STRING(pJson, key));
            std::transform(enum_value_string.begin(), enum_value_string.end(), enum_value_string.begin(), ::toupper);

            bool found = false;

            for (auto& vs : enum_md.enum_value_specs)
            {
                string copy(vs.first);
                std::transform(copy.begin(), copy.end(), copy.begin(), ::toupper);
                if (copy == enum_value_string)
                {
                    thevar = (myclass) vs.second;
                    found = true;
                    break;
                }
            }

            if( !found )
            {
                if( _dryrun )
                {
                    return false;
                }
                else
                {
                    std::ostringstream errorMsgFullList;
                    errorMsgFullList << "Failed to find enum match for value "
                                     << GET_CONFIG_STRING(pJson, key)
                                     << " and key "
                                     << key
                                     << ". Possible values are: ";
                    std::vector< std::string > enum_key_list;

                    for (auto& vs : enum_md.enum_value_specs)
                    {
                        enum_key_list.push_back( vs.first );
                    }
                    std::copy(enum_key_list.begin(), enum_key_list.end() - 1, std::ostream_iterator<std::string>(errorMsgFullList, ", "));
                    errorMsgFullList << enum_key_list.back();
                    throw Kernel::GeneralConfigurationException( __FILE__, __LINE__, __FUNCTION__, errorMsgFullList.str().c_str() );
                }
            }

            return found;
        }

        template< typename myclass >
        bool initVectorConfig(
            const char* key,
            std::vector<myclass> &thevector,
            const json::QuickInterpreter * pJson,
            const MetadataDescriptor::Enum &enum_md,
            const char* condition_key = nullptr, const char * condition_value = nullptr,
            const std::map<std::string, std::string>* depends_list = nullptr
        )
        {
            MetadataDescriptor::Enum * pEnumMd = const_cast<MetadataDescriptor::Enum *>(&enum_md);
            json::Object newParamSchema = json_cast<const json::Object&>(pEnumMd->GetSchemaElement());

            updateSchemaWithCondition(newParamSchema, condition_key, condition_value);
            if(depends_list)
            {
                for(auto const pair: *depends_list)
                {
                    updateSchemaWithCondition(newParamSchema, (pair.first).c_str(), (pair.second).c_str());
                }
            }

            jsonSchemaBase[key] = newParamSchema;

            if( ignoreParameter( newParamSchema, pJson ) )
            {
                return true; // param is missing and that's ok.
            }

            if (pJson && pJson->Exist(key) == false && _useDefaults )
            {
                if( (EnvPtr != nullptr) && EnvPtr->Log->CheckLogLevel(Logger::INFO, "JsonConfigurable"))
                {
                    EnvPtr->Log->Log(Logger::INFO, "JsonConfigurable", "Using the default value ( \"%s\" : [ \"%s\" ] ) for unspecified parameter.\n", key, enum_md.enum_value_specs[0].first.c_str() );
                }
                thevector.push_back( (myclass) enum_md.enum_value_specs[0].second );

                if( _track_missing )
                {
                    missing_parameters_set.insert(key);
                }

                return false;
            }

            std::vector<std::string> enum_value_strings = GET_CONFIG_VECTOR_STRING(pJson, key);

            // parsing: specified case
            for (auto& enum_value_string : enum_value_strings)
            {
                string lower(enum_value_string);
                std::transform(lower.begin(), lower.end(), lower.begin(), ::toupper);
                bool found = false;

                for (auto& vs : enum_md.enum_value_specs)
                {
                    string copy(vs.first);
                    std::transform(copy.begin(), copy.end(), copy.begin(), ::toupper);
                    if (copy == lower)
                    {
                        thevector.push_back( (myclass) vs.second );
                        found = true;
                        break;
                    }
                }

                // error handling
                if( found == false )
                {
                    if( _dryrun )
                    {
                        return false;
                    }
                    else
                    {
                        std::ostringstream errorMsgFullList;
                        errorMsgFullList << "Failed to find enum match for value "
                                         << lower
                                         << " and key "
                                         << key
                                         << ". Possible values are: ";
                        std::vector< std::string > enum_key_list;

                        for (auto& vs : enum_md.enum_value_specs)
                        {
                            enum_key_list.push_back( vs.first );
                        }
                        std::copy(enum_key_list.begin(), enum_key_list.end() - 1, std::ostream_iterator<std::string>(errorMsgFullList, ", "));
                        errorMsgFullList << enum_key_list.back();
                        throw Kernel::GeneralConfigurationException( __FILE__, __LINE__, __FUNCTION__, errorMsgFullList.str().c_str() );
                    }
                }
            }

            return true;
        }

        // OK, so here's the story with initConfigComplexType. The JsonConfigurable base
        // class is for any class that has member variables that need to be initialized
        // from parameters in config.json or campaign.json. Up to now that has been done
        // by the class doing 1 of the following 3 things:
        // 1) initConfig. This is for enums only and does an immediate initialization of
        // the variable from the json (via the Configuration class, which is a
        // json::QuickInterpreter.)
        // 2) initConfigTypeMap. This stores a map of param names to variable pointers,
        // and is used for all types for which an overload of that function has been
        // created in JsonConfigurable. That includes plain-old-datatypes, plus a few
        // others.
        // 3) Custom JSON Parsing. More complex, customized structures -- especially
        // those initialized by nested json objects (or arrays) had been read in through
        // some custom parsing, and the schema had to be provided explicitly.
        //
        // initConfigComplexType replaces #3, and could ultimately replace some of #2.
        //
        void initConfigComplexType(
            const char* paramName,
            IComplexJsonConfigurable * pVariable,
            const char* description = default_description,
            const char* condition_key = nullptr, const char* condition_value = nullptr,
            const std::map<std::string, std::string>* depends_list = nullptr
        );

        // This is really for objects that are based on the JsonConfigurableColleciton
        // template.  This code is needed because the schema generation is different.
        void initConfigComplexCollectionType(
            const char* paramName,
            IComplexJsonConfigurable * pVariable,
            const char* description = default_description,
            const char* condition_key = nullptr, const char* condition_value = nullptr,
            const std::map<std::string, std::string>* depends_list = nullptr
        );

        virtual bool Configure( const Configuration* inputJson );

        static const char * _typename_label() { return "type_name"; }
        static const char * _typeschema_label()  { return "type_schema"; }
        void handleMissingParam( const std::string& key, const std::string& rDataLocation );
    };

    // No, we don't need everything from JsonConfigurable. No, this is not the final solution.
    // Yes, there is an opportunity to doing something cleverer with base classes.
    class InterventionConfig : public JsonConfigurable, public IComplexJsonConfigurable
    {
        IMPLEMENT_DEFAULT_REFERENCE_COUNTING()

        public:
            InterventionConfig();
            InterventionConfig(json::QuickInterpreter* qi);

            virtual json::QuickBuilder GetSchema() override;
            virtual void ConfigureFromJsonAndKey( const Configuration* inputJson, const std::string& key ) override;
            virtual bool  HasValidDefault() const override { return false; }
            json::Element _json;
            static void serialize(IArchive&, InterventionConfig&);
    };

    class IndividualInterventionConfig : public InterventionConfig
    {
        public:
            IndividualInterventionConfig();
            IndividualInterventionConfig(json::QuickInterpreter* qi);

            virtual json::QuickBuilder GetSchema() override;
    };

    class IndividualInterventionConfigList : public IndividualInterventionConfig
    {
        public:
            IndividualInterventionConfigList();
            IndividualInterventionConfigList(json::QuickInterpreter* qi);

            virtual json::QuickBuilder GetSchema() override;

            virtual bool  HasValidDefault() const override {return true;}
    };

    class NodeInterventionConfig : public InterventionConfig
    {
        public:
            NodeInterventionConfig();
            NodeInterventionConfig(json::QuickInterpreter* qi);

            virtual json::QuickBuilder GetSchema() override;
    };

    class NodeInterventionConfigList : public NodeInterventionConfig
    {
        public:
            NodeInterventionConfigList();
            NodeInterventionConfigList(json::QuickInterpreter* qi);

            virtual json::QuickBuilder GetSchema() override;

            virtual bool  HasValidDefault() const override {return true;}
    };

    class NodeSetConfig : public JsonConfigurable, public IComplexJsonConfigurable
    {
        IMPLEMENT_DEFAULT_REFERENCE_COUNTING()
        public:
            NodeSetConfig();
            NodeSetConfig(json::QuickInterpreter* qi);
            virtual json::QuickBuilder GetSchema() override;
            virtual void ConfigureFromJsonAndKey( const Configuration* inputJson, const std::string& key ) override;
            virtual bool  HasValidDefault() const override { return false; }
            json::Element _json;
    };

    class EventConfig : public JsonConfigurable, public IComplexJsonConfigurable
    {
        IMPLEMENT_DEFAULT_REFERENCE_COUNTING()
        public:
            EventConfig();
            EventConfig(json::QuickInterpreter* qi);
            json::QuickBuilder GetSchema() override;
            virtual void ConfigureFromJsonAndKey( const Configuration* inputJson, const std::string& key ) override;
            virtual bool  HasValidDefault() const override { return false; }
            json::Element _json;
    };

    #define GET_SCHEMA_STATIC_WRAPPER(x)\
    static json::QuickBuilder GetSchemaImpl() \
    { \
        JsonConfigurable::_dryrun = true; \
        x * newInst = new x(); \
        newInst->Configure(nullptr); \
        return newInst->GetSchema(); \
    } \
    private: \
    static JsonConfigurable::Registrator registrator;

    #define GET_SCHEMA_STATIC_WRAPPER_IMPL(x,y) \
    JsonConfigurable::Registrator y::registrator( #x, &y::GetSchemaImpl );
}
