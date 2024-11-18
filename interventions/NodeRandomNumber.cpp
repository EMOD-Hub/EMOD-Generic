/***************************************************************************************************

Copyright (c) 2018 Intellectual Ventures Property Holdings, LLC (IVPH) All rights reserved.

EMOD is licensed under the Creative Commons Attribution-Noncommercial-ShareAlike 4.0 License.
To view a copy of this license, visit https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode

***************************************************************************************************/

#include "stdafx.h"
#include "NodeRandomNumber.h"
#include "NodeEventContext.h"
#include "INodeContext.h"
#include "RANDOM.h"

SETUP_LOGGING("NodeRandomNumber")

namespace Kernel
{
    IMPLEMENT_FACTORY_REGISTERED( NodeRandomNumber )

    NodeRandomNumber::NodeRandomNumber()
        : BaseNodeIntervention()
        , total_num(0)
    { }

    NodeRandomNumber::NodeRandomNumber(const NodeRandomNumber& original)
        : BaseNodeIntervention(original)
        , total_num(original.total_num)
    { }

    NodeRandomNumber::~NodeRandomNumber()
    { }

    bool NodeRandomNumber::Configure(const Configuration* inputJson)
    {
        initConfigTypeMap( "Total_Numbers", &total_num, NRN_Total_Numbers_DESC_TEXT, 0, INT_MAX, 0 );

        return BaseNodeIntervention::Configure( inputJson );
    }

    bool NodeRandomNumber::Distribute(INodeEventContext* context, IEventCoordinator2* pEC)
    {
        for(int k1 = 0; k1 < total_num; k1++)
        {
            context->GetRng()->e();
        }

        return true;
    }

    void NodeRandomNumber::Update(float dt)
    {
        // Distribute() doesn't call GiveIntervention() for this intervention, so it isn't added to the NodeEventContext's list of NDI
        release_assert(false);
    }

    void NodeRandomNumber::serialize(IArchive& ar, NodeRandomNumber* obj)
    {
        BaseNodeIntervention::serialize(ar, obj);

        NodeRandomNumber& rand_num_obj = *obj;

        ar.labelElement("total_num")   &   rand_num_obj.total_num;
    }
}
