/***************************************************************************************************

Copyright (c) 2018 Intellectual Ventures Property Holdings, LLC (IVPH) All rights reserved.

EMOD is licensed under the Creative Commons Attribution-Noncommercial-ShareAlike 4.0 License.
To view a copy of this license, visit https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode

***************************************************************************************************/

#pragma once

#include <string>
#include <list>
#include <vector>

#include "BoostLibWrapper.h"

#include "Interventions.h"
#include "Configuration.h"
#include "InterventionFactory.h"
#include "InterventionEnums.h"
#include "FactorySupport.h"
#include "Configure.h"
#include "NodeProperties.h"

namespace Kernel
{
    struct IDistribution;

    class NodePropertyValueChanger : public BaseNodeIntervention
    {
    public:

        DECLARE_FACTORY_REGISTERED(InterventionFactory, NodePropertyValueChanger, INodeDistributableIntervention)

    public:
        NodePropertyValueChanger();
        NodePropertyValueChanger( const NodePropertyValueChanger& rThat );
        virtual ~NodePropertyValueChanger();

        virtual bool Configure( const Configuration * config ) override;

        // INodeDistributableIntervention
        virtual QueryResult QueryInterface(iid_t iid, void **ppvObject) override;
        virtual bool Distribute( INodeEventContext *pNodeEventContext, IEventCoordinator2 *pEC ) override;
        virtual void Update(float dt) override;

        virtual int AddRef() override;
        virtual int Release() override;

    protected:
        NPKeyValue m_TargetKeyValue;
        float probability;
        float revert;
        float max_duration;
        float action_timer;
        float reversion_timer;

    private:
        IDistribution* distribution;
    };
}
