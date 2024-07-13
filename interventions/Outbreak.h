/***************************************************************************************************

Copyright (c) 2018 Intellectual Ventures Property Holdings, LLC (IVPH) All rights reserved.

EMOD is licensed under the Creative Commons Attribution-Noncommercial-ShareAlike 4.0 License.
To view a copy of this license, visit https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode

***************************************************************************************************/

#pragma once

#include <string>
#include <list>
#include <vector>

#include "Interventions.h"
#include "Configuration.h"
#include "InterventionFactory.h"
#include "InterventionEnums.h"
#include "EventCoordinator.h"
#include "Configure.h"

namespace Kernel
{
    class StrainIdentity;


    struct IOutbreak : public ISupports
    {
        virtual uint32_t GetClade()     const = 0;
        virtual uint64_t GetGenome()    const = 0;
        virtual float    GetImportAge() const = 0;

        virtual ~IOutbreak() { }; // needed for cleanup via interface pointer
    };

    class Outbreak : public IOutbreak, public BaseNodeIntervention
    {
        IMPLEMENT_DEFAULT_REFERENCE_COUNTING()
        DECLARE_CONFIGURED(Outbreak)
        DECLARE_FACTORY_REGISTERED(NodeIVFactory, Outbreak, INodeDistributableIntervention)

    public:
        Outbreak();
        virtual ~Outbreak() { }

        // INodeDistributableIntervention
        virtual QueryResult QueryInterface(iid_t iid, void **ppvObject);
        virtual bool Distribute(INodeEventContext *context, IEventCoordinator2* pEC);
        virtual void SetContextTo(INodeEventContext *context) { /* not needed for this intervention */ }
        virtual void Update(float dt);

        // IOutbreak
        virtual uint32_t  GetClade()     const   { return clade; }
        virtual uint64_t  GetGenome()    const   { return genome; }
        virtual float     GetImportAge() const   { return import_age; }

    protected:
        uint32_t    clade;
        uint64_t    genome;
        int         num_cases_per_node;
        float       import_age;
        float       inf_prob;
        float       female_prob;
        float       mc_weight;
    };
}
