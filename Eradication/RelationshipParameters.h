
#pragma once

#include "IRelationshipParameters.h"
#include "Configure.h"

namespace Kernel 
{
    class IDMAPI RelationshipParameters : public IRelationshipParameters, public JsonConfigurable
    {
    public:
        RelationshipParameters( RelationshipType::Enum type );
        ~RelationshipParameters();

        IMPLEMENT_DEFAULT_REFERENCE_COUNTING()
        virtual bool Configure( const Configuration* config );

        RelationshipType::Enum GetType() const { return m_Type; }

        virtual float GetCoitalActRate()                const { return m_CoitalActRate;                }
        virtual float GetDurationWeibullHeterogeneity() const { return m_DurationWeibullHeterogeneity; }
        virtual float GetDurationWeibullScale()         const { return m_DurationWeibullScale;         }
        virtual const Sigmoid& GetCondomUsage()         const { return m_CondomUsage;                  }

        virtual const std::vector<RelationshipMigrationAction::Enum>& GetMigrationActions() const { return m_MigrationActions; }
        virtual const std::vector<float>& GetMigrationActionsCDF() const { return m_MigrationActionsCDF; }

    private:
#pragma warning( push )
#pragma warning( disable: 4251 ) // See IdmApi.h for details
        RelationshipType::Enum m_Type;
        float m_CoitalActRate;
        float m_DurationWeibullHeterogeneity;
        float m_DurationWeibullScale;
        Sigmoid m_CondomUsage;
        std::vector<RelationshipMigrationAction::Enum> m_MigrationActions;
        std::vector<float> m_MigrationActionsCDF;
#pragma warning( pop )
    };
}
