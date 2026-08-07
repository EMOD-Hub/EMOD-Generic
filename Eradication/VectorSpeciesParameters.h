
#pragma once

#include <vector>

#include "Common.h"
#include "VectorContexts.h"
#include "VectorEnums.h"
#include "Configure.h"

namespace Kernel
{
    class LarvalHabitatParams : public JsonConfigurable, public IComplexJsonConfigurable
    {
    public:
        LarvalHabitatParams() {}

        IMPLEMENT_DEFAULT_REFERENCE_COUNTING()


        virtual void ConfigureFromJsonAndKey( const Configuration* inputJson, const std::string& key ) override;
        virtual json::QuickBuilder GetSchema() override;
        std::map< VectorHabitatType::Enum, const Configuration* > habitat_map;
    };

    class VectorSpeciesParameters : public JsonConfigurable
    {
        IMPLEMENT_DEFAULT_REFERENCE_COUNTING()

    public:
        static VectorSpeciesParameters* CreateVectorSpeciesParameters( const Configuration* inputJson, 
                                                                       const std::string& vector_species_name );
        virtual ~VectorSpeciesParameters();
        bool Configure( const ::Configuration *json );

        LarvalHabitatParams habitat_params;
        float aquaticarrhenius1;
        float aquaticarrhenius2;
        float infectedarrhenius1;
        float infectedarrhenius2;
        float cyclearrhenius1;
        float cyclearrhenius2;
        float cyclearrheniusreductionfactor;
        float immatureduration;
        float daysbetweenfeeds;
        float anthropophily;
        float eggbatchsize;
        float infectedeggbatchmod;
        float eggsurvivalrate;
        float infectiousmortalitymod;
        float aquaticmortalityrate;
        float adultlifeexpectancy;
        float transmissionmod;
        float acquiremod;
        float infectioushfmortmod;
        float indoor_feeding;
        float nighttime_feeding;

        // derived values (e.g. 1/adultlifeexpectanc = adultmortality)
        float adultmortality;
        float immaturerate;

        static void serialize(IArchive&, VectorSpeciesParameters*&);

    protected:
        VectorSpeciesParameters();
        void Initialize(const std::string& vector_species_name);

    private:
        std::string _species;

    };
}
