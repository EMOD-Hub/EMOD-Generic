/***************************************************************************************************

Copyright (c) 2018 Intellectual Ventures Property Holdings, LLC (IVPH) All rights reserved.

EMOD is licensed under the Creative Commons Attribution-Noncommercial-ShareAlike 4.0 License.
To view a copy of this license, visit https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode

***************************************************************************************************/

#pragma once

#include <string>
#include <vector>

#include "Diagnostics.h"
#include "EventTrigger.h"
#include "JsonConfigurableCollection.h"

namespace Kernel
{
    class RangeThreshold : public JsonConfigurable
    {
        IMPLEMENT_NO_REFERENCE_COUNTING()
        virtual QueryResult QueryInterface(iid_t iid, void **ppvObject) { return e_NOINTERFACE; }

    public:
        RangeThreshold( const char* pLowDesc, const char* pHighDesc, const char* pEventDesc );
        RangeThreshold( const RangeThreshold& rMaster );
        virtual ~RangeThreshold();

        virtual bool Configure( const Configuration* inputJson ) override;

        static void serialize( IArchive& ar, RangeThreshold& rt );

        float m_Low;
        float m_High;
        EventTrigger::Enum m_Event;
    };

    class RangeThresholdList : public JsonConfigurableCollection<RangeThreshold>
    {
    public:
        RangeThresholdList( const char* pLowDesc, const char* pHighDesc, const char* pEventDesc );
        RangeThresholdList( const RangeThresholdList& rMaster );
        virtual ~RangeThresholdList();

    protected:
        virtual RangeThreshold* CreateObject() override;

        const char* m_pLowDesc;
        const char* m_pHighDesc;
        const char* m_pEventDesc;
    };

    class AgeDiagnostic : public SimpleDiagnostic 
    {
        IMPLEMENT_DEFAULT_REFERENCE_COUNTING()
        DECLARE_QUERY_INTERFACE()
        DECLARE_FACTORY_REGISTERED(InterventionFactory, AgeDiagnostic, IDistributableIntervention)

    public: 
        AgeDiagnostic();
        AgeDiagnostic( const AgeDiagnostic& );
        virtual ~AgeDiagnostic();

        virtual bool Configure( const Configuration* pConfig ) override;

    protected:
        AgeDiagnostic( const char* pLowDesc, const char* pHighDesc, const char* pEventDesc );
        virtual void ConfigureRangeThresholds( const Configuration* inputJson );
        virtual bool positiveTestResult() override;
        virtual float GetValue() const;

        RangeThresholdList range_thresholds;

        DECLARE_SERIALIZABLE(AgeDiagnostic);
    };
}
