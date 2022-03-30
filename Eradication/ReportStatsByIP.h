/***************************************************************************************************

Copyright (c) 2018 Intellectual Ventures Property Holdings, LLC (IVPH) All rights reserved.

EMOD is licensed under the Creative Commons Attribution-Noncommercial-ShareAlike 4.0 License.
To view a copy of this license, visit https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode

***************************************************************************************************/

#pragma once

#include "Configure.h"
#include "Properties.h"

namespace Kernel
{
    struct INodeEventContext;
    struct IIndividualHumanEventContext;

    typedef std::function<bool( IIndividualHumanEventContext* )> individual_qualified_function_t;

    struct StatsPerIP
    {
        uint32_t num_individuals;
        uint32_t num_infected;

        StatsPerIP()
            : num_individuals( 0 )
            , num_infected( 0 )
        {
        }

        void Clear()
        {
            num_individuals = 0;
            num_infected = 0;
        }
    };

    class ReportStatsByIP
    {
    public:
        ReportStatsByIP();
        ~ReportStatsByIP();

        void SetIPKeyNames( const std::string& rParameterName,
                            const jsonConfigurable::tDynamicStringSet& rIPKeyNames );
        std::string GetHeader() const;
        void ResetData();

        void CollectDataFromNode( INodeEventContext* pNEC, individual_qualified_function_t isQualifiedfunc );

        std::string GetReportData();
        const StatsPerIP& GetStatsTotal() const;
        const StatsPerIP& GetStats( IPKeyValue kv ) const;

    private:
        std::vector<IPKey> m_IpKeys;
        StatsPerIP m_StatsTotal;
        std::map<std::string, StatsPerIP> m_StatsByIP;
    };
}
