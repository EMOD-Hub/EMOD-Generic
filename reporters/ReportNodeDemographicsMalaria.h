
#pragma once

#include "ReportNodeDemographics.h"
#include "ReportUtilitiesMalaria.h"

namespace Kernel
{
    class NodeDataMalaria : public NodeData
    {
    public:
        NodeDataMalaria()
            : NodeData()
            , avg_parasite_density(0.0)
            , avg_gametocyte_density(0.0)
            , num_infections(0)
            , genome_marker_columns()
        {
        };

        virtual void Reset()
        {
            NodeData::Reset();
            num_infections = 0;
            avg_parasite_density = 0.0;
            avg_gametocyte_density = 0.0;
            for( auto& r_column : genome_marker_columns )
            {
                r_column.ResetCount();
            }
        }

        float avg_parasite_density;
        float avg_gametocyte_density;
        uint32_t num_infections;
        std::vector<ReportUtilitiesMalaria::GenomeMarkerColumn> genome_marker_columns;
    };

    class ReportNodeDemographicsMalaria: public ReportNodeDemographics
    {
        DECLARE_FACTORY_REGISTERED(ReportFactory, ReportNodeDemographicsMalaria, IReport)

    public:
        ReportNodeDemographicsMalaria();
        virtual ~ReportNodeDemographicsMalaria();

        // ReportNodeDemographics
        virtual void Initialize( unsigned int nrmSize ) override;
        virtual std::string GetHeader() const override;

    protected:
        virtual NodeData* CreateNodeData() override;
        virtual void WriteNodeData( const NodeData* pData ) override;
        virtual void LogIndividualData( IIndividualHuman* individual, NodeData* pNodeData ) override;

    private:
        std::vector<ReportUtilitiesMalaria::GenomeMarkerColumn> m_GenomeMarkerColumns;
    };
}
