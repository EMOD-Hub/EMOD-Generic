
#pragma once
#include "Configure.h"
#include "SimulationEnums.h"

namespace Kernel 
{
    class TBDrugTypeParameters : public JsonConfigurable
    {
        friend class AntiTBPropDepDrug;

        IMPLEMENT_DEFAULT_REFERENCE_COUNTING()

    public:
        TBDrugTypeParameters();
        virtual ~TBDrugTypeParameters();

        bool Configure( const ::Configuration *json );

        const std::string& GetName() const;

    protected:
        std::string TB_drug_name;

        float TB_drug_inactivation_rate;
        float TB_drug_cure_rate;
        float TB_drug_resistance_rate;
        float TB_drug_relapse_rate;
        float TB_drug_mortality_rate;
        float TB_drug_primary_decay_time_constant;

    private:
        TBDrugType::Enum _drugType;
    };
}
