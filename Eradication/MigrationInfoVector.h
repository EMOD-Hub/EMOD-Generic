/***************************************************************************************************

Copyright (c) 2018 Intellectual Ventures Property Holdings, LLC (IVPH) All rights reserved.

EMOD is licensed under the Creative Commons Attribution-Noncommercial-ShareAlike 4.0 License.
To view a copy of this license, visit https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode

***************************************************************************************************/

#pragma once

#include "stdafx.h"
#include "IMigrationInfoVector.h"
#include "Migration.h"
#include "EnumSupport.h"

namespace Kernel
{
    struct IVectorSimulationContext;

    // ----------------------------------
    // --- MigrationInfoNullVector
    // ----------------------------------
    class IDMAPI MigrationInfoNullVector : public MigrationInfoNull, public IMigrationInfoVector
    {
    public:
        IMPLEMENT_DEFAULT_REFERENCE_COUNTING()
        DECLARE_QUERY_INTERFACE()

    public:
        //IMigrationInfoVector
        virtual void UpdateRates( const suids::suid& rThisNodeId,
                                  const std::string& rSpeciesID,
                                  IVectorSimulationContext* pivsc ) {};

    protected:
        friend class MigrationInfoFactoryVector;
        friend class MigrationInfoFactoryVectorDefault;

        MigrationInfoNullVector();
        virtual ~MigrationInfoNullVector();
    };

    // ----------------------------------
    // --- MigrationInfoVector
    // ----------------------------------

    class IDMAPI MigrationInfoVector : public MigrationInfoFixedRate, public IMigrationInfoVector
    {
    public:
        IMPLEMENT_DEFAULT_REFERENCE_COUNTING()  
        DECLARE_QUERY_INTERFACE()
    public:
        virtual ~MigrationInfoVector();

        virtual void PickMigrationStep( RANDOMBASE* pRNG,
                                        IIndividualHumanContext * traveler, 
                                        suids::suid &destination, 
                                        MigrationType::Enum &migration_type,
                                        float &timeUntilTrip ) override;

        // IMigrationInfoVector
        virtual void UpdateRates( const suids::suid& rThisNodeId, 
                                  const std::string& rSpeciesID, 
                                  IVectorSimulationContext* pivsc ) override;

    protected:
        friend class MigrationInfoFactoryVector;
        friend class MigrationInfoFactoryVectorDefault;

        MigrationInfoVector( INodeContext* _parent,
                             ModiferEquationType::Enum equation,
                             float habitatModifier,
                             float foodModifier,
                             float stayPutModifier );

        virtual void Initialize( const std::vector<std::vector<MigrationRateData>>& rRateData ) override;
        virtual void SaveRawRates( std::vector<float>& r_rate_cdf ) override;
        float CalculateModifiedRate( const suids::suid& rNodeId, 
                                     float rawRate, 
                                     float populationRatio, 
                                     float habitatRatio );

        typedef std::function<int( const suids::suid& rNodeId, 
                                   const std::string& rSpeciesID, 
                                   IVectorSimulationContext* pivsc )> tGetValueFunc;

        std::vector<float> GetRatios( const std::vector<suids::suid>& rReachableNodes, 
                                      const std::string& rSpeciesID, 
                                      IVectorSimulationContext* pivsc, 
                                      tGetValueFunc getValueFunc );

    private:
#pragma warning( push )
#pragma warning( disable: 4251 ) // See IdmApi.h for details
        std::vector<float> m_RawMigrationRate;
        suids::suid m_ThisNodeId;
        ModiferEquationType::Enum m_ModifierEquation;
        float m_ModifierHabitat;
        float m_ModifierFood;
        float m_ModifierStayPut;
#pragma warning( pop )
    };


    // ----------------------------------
    // --- MigrationInfoFactoryVector
    // ----------------------------------

    class IDMAPI MigrationInfoFactoryVector : public MigrationInfoFactoryFile, public IMigrationInfoFactoryVector
    {
    public:
        MigrationInfoFactoryVector();
        virtual ~MigrationInfoFactoryVector();

        // MigrationInfoFactoryFile
        virtual void Initialize( const std::string& idreference ) override;

        // IMigrationInfoFactoryVector
        virtual IMigrationInfoVector* CreateMigrationInfoVector( 
            INodeContext *parent_node, 
            const boost::bimap<ExternalNodeId_t, suids::suid>& rNodeIdSuidMap ) override;

    private:
#pragma warning( push )
#pragma warning( disable: 4251 ) // See IdmApi.h for details
        std::vector<MigrationInfoFile*> m_InfoFileListVector;
#pragma warning( pop )
    };

    // ----------------------------------
    // --- MigrationInfoFactoryVectorDefault
    // ----------------------------------

    class IDMAPI MigrationInfoFactoryVectorDefault : public MigrationInfoFactoryDefault, public IMigrationInfoFactoryVector
    {
    public:
        MigrationInfoFactoryVectorDefault( int defaultTorusSize );
        MigrationInfoFactoryVectorDefault();
        virtual ~MigrationInfoFactoryVectorDefault();

        // IMigrationInfoFactoryVector
        virtual IMigrationInfoVector* CreateMigrationInfoVector( 
            INodeContext *parent_node, 
            const boost::bimap<ExternalNodeId_t, suids::suid>& rNodeIdSuidMap ) override;

    private:
    };
}
