
#pragma once

#include <map>

#include "Configure.h"
#include "VectorEnums.h"
#include "IdmApi.h"
#include "JsonConfigurableCollection.h"
#include "ExternalNodeId.h"

namespace Kernel
{
    class LarvalHabitatMultiplierSpec : public JsonConfigurable
    {
        GET_SCHEMA_STATIC_WRAPPER(LarvalHabitatMultiplierSpec)

    public:
        LarvalHabitatMultiplierSpec();
        virtual bool Configure(const Configuration* config) override;
        IMPLEMENT_DEFAULT_REFERENCE_COUNTING()

        float GetFactor() const;
        VectorHabitatType::Enum GetHabitat() const;
        std::string GetSpecies() const;

    private:
        float m_factor;
        VectorHabitatType::Enum m_habitat_name;
        std::string m_species;
        bool m_configured;
    };

    class LHMSpecList : public JsonConfigurableCollection<LarvalHabitatMultiplierSpec>
    {
    public:
        LHMSpecList();
        virtual ~LHMSpecList();

    protected:
        virtual LarvalHabitatMultiplierSpec* CreateObject() override;
    };

    class LarvalHabitatMultiplier : public JsonConfigurable
    {
    public:
        IMPLEMENT_DEFAULT_REFERENCE_COUNTING()

    public:
        LarvalHabitatMultiplier( bool usedByIntervention = false, float minValue = 0.0f, float maxValue = FLT_MAX, float defaultValue = 1.0f );
        ~LarvalHabitatMultiplier();

        void Initialize();

        // ------------------------------------
        // --- JsonConfigurable
        // ------------------------------------
        virtual bool Configure(const Configuration * config) override;

        bool WasInitialized() const;
        float GetMultiplier( VectorHabitatType::Enum, const std::string& species ) const;
        void SetMultiplier( VectorHabitatType::Enum, float multiplier );
        void SetAsReduction( const LarvalHabitatMultiplier& rRegularLHM );
        void SetExternalNodeId(ExternalNodeId_t externalNodeId);
        float GetDefaultValue() const;

    private:
        void ProcessMultipliers(LHMSpecList &spec_list);
        bool EntryAffectsHabitatAndSpecies(LarvalHabitatMultiplierSpec * entry, 
                                           VectorHabitatType::Enum habitat_type,
                                           const std::string & species_name);
        void UnsetAllFactors();

        bool m_UsedByIntervention;
        float m_MinValue;
        float m_MaxValue;
        float m_DefaultValue;
        bool m_Initialized;
        std::map<VectorHabitatType::Enum,std::map<std::string,float>> m_Multiplier;
        ExternalNodeId_t m_externalNodeId;
    };
}
