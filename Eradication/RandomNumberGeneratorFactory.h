
#pragma once

#include "Configure.h"
#include "SimulationEnums.h"
#include "ExternalNodeId.h"

namespace Kernel
{
    class RANDOMBASE;

    class RandomNumberGeneratorFactory : public JsonConfigurable, public ISerializable
    {
    public:
        GET_SCHEMA_STATIC_WRAPPER( RandomNumberGeneratorFactory )
        IMPLEMENT_NO_REFERENCE_COUNTING()

        RandomNumberGeneratorFactory();
        ~RandomNumberGeneratorFactory();

        virtual bool Configure( const Configuration* inputJson ) override;

        RandomNumberGeneratorPolicy::Enum GetPolicy() const;

        void SetNodeIds( const std::vector<ExternalNodeId_t>& rNodeIds );

        RANDOMBASE* CreateRng( ExternalNodeId_t externalNodeId= UINT_MAX );

        void CreateFromSerializeData( bool fromSerializedData );

        DECLARE_SERIALIZABLE( RandomNumberGeneratorFactory );

    private:

        uint32_t CreateSeed( ExternalNodeId_t externalNodeId );
        RANDOMBASE* CreateRng( uint32_t seed, uint32_t cachedCount );
        uint32_t GetCachedCount();

        RandomNumberGeneratorType::Enum m_RngType;
        RandomNumberGeneratorPolicy::Enum m_Policy;
        bool m_CreateFromSerializedData;
        uint16_t m_RunNumber;
        std::vector<ExternalNodeId_t> m_NodeIds;
        uint32_t m_NodeIdsIndex;
        RANDOMBASE* m_pSeedRng;
    };
}