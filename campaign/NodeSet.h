/***************************************************************************************************

Copyright (c) 2018 Intellectual Ventures Property Holdings, LLC (IVPH) All rights reserved.

EMOD is licensed under the Creative Commons Attribution-Noncommercial-ShareAlike 4.0 License.
To view a copy of this license, visit https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode

***************************************************************************************************/

#pragma once

#include <string>

#include "IdmApi.h"
#include "Configuration.h"
#include "ExternalNodeId.h"
#include "FactorySupport.h"
#include "Configure.h"
#include "ISerializable.h"
#include "ObjectFactory.h"

namespace Kernel
{
    struct INodeEventContext;

    struct IDMAPI INodeSet : public ISerializable
    {
        virtual bool Contains(INodeEventContext *ndc) = 0; // must provide access to demographics id, lat, long, etc
        virtual std::vector<ExternalNodeId_t> IsSubset(const std::vector<ExternalNodeId_t>& demographic_node_ids) = 0;
    };

    class NodeSetFactory : public ObjectFactory<INodeSet,NodeSetFactory>
    {
    };

    // class defines a simple set of nodes...either by id, 
    class IDMAPI NodeSetAll : public INodeSet, public JsonConfigurable
    {
        DECLARE_FACTORY_REGISTERED_EXPORT(NodeSetFactory, NodeSetAll, INodeSet)

    public:
        DECLARE_CONFIGURED(NodeSetAll)
        IMPLEMENT_DEFAULT_REFERENCE_COUNTING()  

        virtual bool Contains(INodeEventContext *ndc);
        virtual std::vector<ExternalNodeId_t> IsSubset(const std::vector<ExternalNodeId_t>& demographic_node_ids);

        virtual IConfigurable*  GetConfigurable()  override  { return JsonConfigurable::GetConfigurable(); }

    protected:
#pragma warning( push )
#pragma warning( disable: 4251 ) // See IdmApi.h for details
        DECLARE_SERIALIZABLE(NodeSetAll);
#pragma warning( pop )
    };

    class IDMAPI NodeListConfig : public JsonConfigurable, public IComplexJsonConfigurable
    {
        IMPLEMENT_DEFAULT_REFERENCE_COUNTING()

        public:
            NodeListConfig() {}
            virtual void ConfigureFromJsonAndKey( const Configuration* inputJson, const std::string& key ) override;
            virtual json::QuickBuilder GetSchema() override;
            virtual bool  HasValidDefault() const override { return false; }
#pragma warning( push )
#pragma warning( disable: 4251 ) // See IdmApi.h for details
            std::list< ExternalNodeId_t > nodelist;
#pragma warning( pop )
    };

    class IDMAPI NodeSetNodeList : public INodeSet, public JsonConfigurable
    {
        DECLARE_FACTORY_REGISTERED_EXPORT(NodeSetFactory, NodeSetNodeList, INodeSet)

    public:
        DECLARE_CONFIGURED(NodeSetNodeList)
        IMPLEMENT_DEFAULT_REFERENCE_COUNTING()  

        virtual bool Contains(INodeEventContext *ndc);
        virtual std::vector<ExternalNodeId_t> IsSubset(const std::vector<ExternalNodeId_t>& demographic_node_ids);

        virtual IConfigurable*  GetConfigurable()  override  { return JsonConfigurable::GetConfigurable(); }

    protected:
        NodeListConfig nodelist_config;
    };
};
