/***************************************************************************************************

Copyright (c) 2018 Intellectual Ventures Property Holdings, LLC (IVPH) All rights reserved.

EMOD is licensed under the Creative Commons Attribution-Noncommercial-ShareAlike 4.0 License.
To view a copy of this license, visit https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode

***************************************************************************************************/

#pragma once

#include <map>

#include "Node.h"
#include "VectorHabitat.h"
#include "IVectorPopulation.h"
#include "LarvalHabitatMultiplier.h"
#include "VectorContexts.h"

class ReportVector;
class VectorSpeciesReport;

namespace Kernel
{
    struct IMigrationInfoVector;
    class SpatialReportVector;

    class NodeVector : public Node, public IVectorNodeContext, public INodeVector
    {
        GET_SCHEMA_STATIC_WRAPPER(NodeVector)
        IMPLEMENT_DEFAULT_REFERENCE_COUNTING()
        DECLARE_QUERY_INTERFACE()

        friend class ::ReportVector;
        friend class ::VectorSpeciesReport;
        friend class SpatialReportVector;

    public:
        static NodeVector *CreateNode(ISimulationContext *context, ExternalNodeId_t externalNodeId, suids::suid suid);
        virtual ~NodeVector();

        // INodeContext methods
        // IVectorNodeContext methods
        virtual VectorProbabilities* GetVectorLifecycleProbabilities() override;
        virtual IVectorHabitat*      GetVectorHabitatBySpeciesAndType( std::string& species, VectorHabitatType::Enum type, const Configuration* inputJson) override;
        virtual VectorHabitatList_t* GetVectorHabitatsBySpecies( std::string& species ) override;
        virtual float                GetLarvalHabitatMultiplier(VectorHabitatType::Enum type, const std::string& species ) const override;

        virtual IIndividualHuman* processImmigratingIndividual(IIndividualHuman*) override;
        virtual IIndividualHuman* addNewIndividual(float = 1.0f, float = 0.0f, int = 0, int = 0, float = 1.0f, float = 1.0f) override;

        virtual void PopulateFromDemographics() override;
        virtual void SetupIntranodeTransmission() override;
        virtual ITransmissionGroups* CreateTransmissionGroups() override;
        virtual void BuildTransmissionRoutes( float ) override;
        virtual void SetParameters( NodeDemographicsFactory *demographics_factory, ClimateFactory *climate_factory, bool white_list_enabled ) override;
        virtual void updateInfectivity(float dt = 0.0f) override;
        virtual void updatePopulationStatistics(float dt = 1.0f) override;
        void         updateVectorLifecycleProbabilities(float dt);

        void SetVectorPopulations(void);    //default--1 population as before
        virtual void AddVectors( const std::string& releasedSpecies, const VectorMatingStructure& _vector_genetics, uint32_t releasedNumber) override;

        virtual void SetupMigration( IMigrationInfoFactory * migration_factory, 
                                     const boost::bimap<ExternalNodeId_t, suids::suid>& rNodeIdSuidMap ) override;
        virtual void processImmigratingVector( IVectorCohort* immigrant ) override;
        void processEmigratingVectors( float dt );

        virtual const VectorPopulationReportingList_t& GetVectorPopulationReporting() const override;

    protected:

        std::map<std::string, VectorHabitatList_t> m_larval_habitats;
        std::list<IVectorPopulation*>  m_vectorpopulations;
        VectorPopulationReportingList_t m_VectorPopulationReportingList;

        VectorProbabilities* m_vector_lifecycle_probabilities;

        LarvalHabitatMultiplier larval_habitat_multiplier;

        IMigrationInfoVector* vector_migration_info;

        ITransmissionGroups* txOutdoor;

        NodeVector();
        NodeVector(ISimulationContext *context, ExternalNodeId_t externalNodeId, suids::suid node_suid);
        virtual void Initialize() override;

        virtual void setupEventContextHost() override;
        virtual void InitializeVectorPopulation( IVectorPopulation* vp );
        void VectorMigrationBasedOnFiles( float dt );
            
        virtual IIndividualHuman *createHuman( suids::suid id, float MCweight, float init_age, int gender) override;

        IVectorSimulationContext *context() const; // N.B. this is returning a non-const context because of the PostMigratingVector function

        virtual void UpdateTransmissionGroupPopulation(const tProperties& properties, float size_changes,float mc_weight) override;
        virtual void DepositFromIndividual( const IStrainIdentity& strain_IDs, float contagion_quantity, TransmissionGroupMembership_t individual, TransmissionRoute::Enum route) override;
        virtual void ExposeIndividual(IInfectable* candidate, TransmissionGroupMembership_t individual, float dt) override;

        virtual float GetTotalContagion( void ) override;

        DECLARE_SERIALIZABLE(NodeVector);

    private:
        virtual INodeContext *getContextPointer() override { return static_cast<INodeContext*>(this); }
        virtual void propagateContextToDependents() override;
    };
} // end namespace Kernel
