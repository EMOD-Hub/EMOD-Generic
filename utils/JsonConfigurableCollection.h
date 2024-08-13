/***************************************************************************************************

Copyright (c) 2018 Intellectual Ventures Property Holdings, LLC (IVPH) All rights reserved.

EMOD is licensed under the Creative Commons Attribution-Noncommercial-ShareAlike 4.0 License.
To view a copy of this license, visit https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode

***************************************************************************************************/

#pragma once

#include "Configure.h"

namespace Kernel
{
    // This template allows one to have a collection/vector of JsonConfigurable objects.
    // The object created from this will need to use the initConfigComplexType() since this
    // is a complex object.  The JSON will be an array of the JsonConfigurable object used
    // in the template, i.e. JsonConfigurableClass.
    template<class JsonConfigurableClass>
    class JsonConfigurableCollection : public JsonConfigurable, public IComplexJsonConfigurable
    {
    public:
        JsonConfigurableCollection( const std::string& rIdmTypeName )
            : JsonConfigurable()
            , m_IdmTypeName( rIdmTypeName )
            , m_Collection()
        { }

        JsonConfigurableCollection( const JsonConfigurableCollection& rMaster )
            : JsonConfigurable( rMaster )
            , m_IdmTypeName( rMaster.m_IdmTypeName )
            , m_Collection() // DO NOT COPY
        {
            // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
            // !!! Do not copy the elements of m_Collection.  They are pointers to objects owned by rMaster.
            // !!! Implementers of the template must implement their own copy constructor so that they
            // !!! can copy the elements.
            // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
        }

        virtual ~JsonConfigurableCollection()
        {
            for( JsonConfigurableClass* p_jcc : m_Collection )
            {
                delete p_jcc;
            }
        }

        //JsonConfigurable methods
        virtual int32_t AddRef() { return 1; }
        virtual int32_t Release() { return 1; }

        // IComplexJsonConfigurable methods
        virtual bool  HasValidDefault() const override
        {
            return false;
        }

        virtual json::QuickBuilder GetSchema() override
        {
            JsonConfigurableClass* p_jcc = CreateObject();
            if( JsonConfigurable::_dryrun )
            {
                p_jcc->Configure( nullptr );
            }

            std::string object_schema_name = p_jcc->GetTypeName();
            std::string idm_type_schema = "idmType:" + object_schema_name;

            json::QuickBuilder schema( GetSchemaBase() );
            auto tn = JsonConfigurable::_typename_label();
            auto ts = JsonConfigurable::_typeschema_label();
            schema[ tn ] = json::String( idm_type_schema );
            schema[ "item_type" ] = json::String( object_schema_name );

            // Add the schema for the objects to be retrieved by JsonConfigurable::Configure()
            // Also add the "class" parameter so the python classes will be generated
            schema[ ts ] = p_jcc->GetSchema();
            schema[ ts ][ "class" ] = json::String( object_schema_name );

            delete p_jcc;

            return schema;
        }

        virtual void ConfigureFromJsonAndKey( const Configuration* inputJson, const std::string& key ) override
        {
            // Temporary object created so we can 'operate' on json with the desired tools
            auto p_config = Configuration::CopyFromElement( (*inputJson)[ key ], inputJson->GetDataLocation() );

            const auto& json_array = json_cast<const json::Array&>((*p_config));
            for( auto data = json_array.Begin(); data != json_array.End(); ++data )
            {
                Configuration* p_object_config = Configuration::CopyFromElement( *data, inputJson->GetDataLocation() );

                JsonConfigurableClass* p_jcc = CreateObject();
                p_jcc->Configure( p_object_config );

                Add( p_jcc );

                delete p_object_config;
            }
            delete p_config;
        }

        // Other methods
        virtual void CheckConfiguration()
        {
        }

        virtual void Add( JsonConfigurableClass* pJcc )
        {
            m_Collection.push_back( pJcc );
        }

        int Size() const
        {
            return m_Collection.size();
        }

        JsonConfigurableClass* operator[]( int index )
        {
            return m_Collection[ index ];
        }

        static void serialize( IArchive& ar, JsonConfigurableCollection<JsonConfigurableClass>& rJcc )
        {
            size_t count = ar.IsWriter() ? rJcc.m_Collection.size() : -1;

            ar.startArray( count );
            if( ar.IsWriter() )
            {
                for( JsonConfigurableClass* p_obj : rJcc.m_Collection )
                {
                    ar.startObject();
                        ar & *p_obj;
                    ar.endObject();
                }
            }
            else
            {
                for( size_t i = 0; i < count; ++i )
                {
                    JsonConfigurableClass* p_obj = rJcc.CreateObject();
                    ar.startObject();
                        ar & *p_obj;
                    ar.endObject();
                    rJcc.m_Collection.push_back( p_obj );
                }
            }
            ar.endArray();
        }

    protected:
        virtual JsonConfigurableClass* CreateObject() = 0;

        std::string m_IdmTypeName;
        std::vector<JsonConfigurableClass*> m_Collection;
    };
}