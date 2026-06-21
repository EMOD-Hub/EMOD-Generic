
#include "stdafx.h"

#include "ReportVectorStatsMalaria.h"
#include "NodeEventContext.h"
#include "Individual.h"
#include "VectorContexts.h"
#include "VectorPopulation.h"
#include "ReportUtilities.h"
#include "SimulationConfig.h"
#include "MalariaParameters.h"
#include "IGenomeMarkers.h"
#include "StrainIdentity.h"


SETUP_LOGGING( "ReportVectorStatsMalaria" )

namespace Kernel
{
    IMPLEMENT_FACTORY_REGISTERED(ReportVectorStatsMalaria)

    ReportVectorStatsMalaria::ReportVectorStatsMalaria()
        : ReportVectorStats( "ReportVectorStatsMalaria.csv" )
        , genome_marker_columns()
    {
    }

    ReportVectorStatsMalaria::~ReportVectorStatsMalaria()
    {
    }

    bool ReportVectorStatsMalaria::Configure( const Configuration * inputJson )
    {
        bool ret = ReportVectorStats::Configure( inputJson );

        if( ret )
        {
            SimulationConfig* p_sim_config = GET_CONFIGURABLE( SimulationConfig );
            std::vector<std::pair<std::string,uint64_t>> marker_combos = p_sim_config->malaria_params->pGenomeMarkers->CreatePossibleCombinations();

            for( auto& combo : marker_combos )
            {
                genome_marker_columns.push_back( ReportUtilitiesMalaria::GenomeMarkerColumn( combo.first, combo.second ) );
            }
        }
        return ret;
    }

    std::string ReportVectorStatsMalaria::GetHeader() const
    {
        std::stringstream header ;

        header << ReportVectorStats::GetHeader();

        for( auto& r_column : genome_marker_columns )
        {
            header << ", " << r_column.GetColumnName();
        }

        return header.str();
    }

    void ReportVectorStatsMalaria::ResetOtherCounters()
    {
        for( auto& r_column : genome_marker_columns )
        {
            r_column.ResetCount();
        }
    }

    void ReportVectorStatsMalaria::CollectOtherData( IVectorPopulation* pIVPR )
    {
        StrainIdentity strain;
        for( auto& r_column : genome_marker_columns )
        {
            strain.SetGeneticID( r_column.GetBitMask() );
            uint32_t gm_infected = pIVPR->getInfectedCount( &strain );
            uint32_t gm_infectious = pIVPR->getInfectiousCount( &strain );
            r_column.AddCount( gm_infected + gm_infectious );
        }
    }

    void ReportVectorStatsMalaria::WriteOtherData()
    {
        for( auto& r_column : genome_marker_columns )
        {
            GetOutputStream() << "," << r_column.GetCount();
        }
    }
}
