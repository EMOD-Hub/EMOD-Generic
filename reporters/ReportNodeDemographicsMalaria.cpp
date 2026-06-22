
#pragma once

#include "stdafx.h"

#include "ReportNodeDemographicsMalaria.h"
#include "FactorySupport.h"

#include "NodeEventContext.h"
#include "IIndividualHuman.h"
#include "IIndividualHumanContext.h"
#include "ReportUtilities.h"
#include "Properties.h"
#include "NodeProperties.h"
#include "SimulationConfig.h"
#include "MalariaParameters.h"
#include "IGenomeMarkers.h"
#include "StrainIdentity.h"
#include "MalariaContexts.h"

SETUP_LOGGING( "ReportNodeDemographicsMalaria" )

namespace Kernel
{
    IMPLEMENT_FACTORY_REGISTERED(ReportNodeDemographicsMalaria)

    ReportNodeDemographicsMalaria::ReportNodeDemographicsMalaria()
        : ReportNodeDemographics( "ReportNodeDemographicsMalaria.csv" )
        , m_GenomeMarkerColumns()
    {
    }
    ReportNodeDemographicsMalaria::~ReportNodeDemographicsMalaria()
    {
    }

    void ReportNodeDemographicsMalaria::Initialize( unsigned int nrmSize )
    {
        SimulationConfig* p_sim_config = GET_CONFIGURABLE( SimulationConfig );
        std::vector<std::pair<std::string, uint64_t>> marker_combos = p_sim_config->malaria_params->pGenomeMarkers->CreatePossibleCombinations();

        for( auto& combo : marker_combos )
        {
            m_GenomeMarkerColumns.push_back( ReportUtilitiesMalaria::GenomeMarkerColumn( combo.first, combo.second ) );
        }

        ReportNodeDemographics::Initialize( nrmSize );

        for( int g = 0; g < m_Data.size(); ++g )
        {
            for( int a = 0; a < m_Data[ g ].size(); ++a )
            {
                for( int i = 0; i < m_Data[ g ][ a ].size(); ++i )
                {
                    NodeDataMalaria* p_ndm = (NodeDataMalaria*)m_Data[ g ][ a ][ i ];
                    p_ndm->genome_marker_columns = m_GenomeMarkerColumns;
                }
            }
        }
    }

    std::string ReportNodeDemographicsMalaria::GetHeader() const
    {
        std::stringstream header ;

        header << ReportNodeDemographics::GetHeader();

        header << "," << "AvgParasiteDensity";
        header << "," << "AvgGametocyteDensity";
        header << "," << "NumInfections";

        for( auto& r_column : m_GenomeMarkerColumns )
        {
            header << "," << r_column.GetColumnName();
        }

        return header.str();
    }

    void ReportNodeDemographicsMalaria::LogIndividualData( IIndividualHuman* individual, NodeData* pNodeData ) 
    {
        if( individual->IsInfected() )
        {
            IMalariaHumanContext* p_human_malaria = individual->GetIndividualContext()->GetIndividualMalaria();
            release_assert(p_human_malaria);

            std::vector<std::pair<uint32_t,uint64_t>> all_strains = p_human_malaria->GetInfectingStrainIds();

            NodeDataMalaria* p_ndm = (NodeDataMalaria*)pNodeData;
            p_ndm->avg_parasite_density   += p_human_malaria->GetMalariaSusceptibilityContext()->get_parasite_density();
            p_ndm->avg_gametocyte_density += p_human_malaria->GetGametocyteDensity();
            p_ndm->num_infections     += all_strains.size();

            for( auto& r_column : p_ndm->genome_marker_columns )
            {
                for( auto& strain : all_strains )
                {
                    if( r_column.GetBitMask() == strain.second )
                    {
                        r_column.AddCount( 1 );
                    }
                }
            }
        }
    }

    NodeData* ReportNodeDemographicsMalaria::CreateNodeData()
    {
        return new NodeDataMalaria();
    }

    void ReportNodeDemographicsMalaria::WriteNodeData( const NodeData* pData )
    {
        ReportNodeDemographics::WriteNodeData( pData );
        NodeDataMalaria* p_ndm = (NodeDataMalaria*)pData;
        GetOutputStream() << "," << p_ndm->avg_parasite_density / float( p_ndm->num_people );
        GetOutputStream() << "," << p_ndm->avg_gametocyte_density / float( p_ndm->num_people );
        GetOutputStream() << "," << p_ndm->num_infections;
        for( auto& r_column : p_ndm->genome_marker_columns )
        {
            GetOutputStream() << "," << r_column.GetCount();
        }
    }

}
