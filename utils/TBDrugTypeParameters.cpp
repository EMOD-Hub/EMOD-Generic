
#include "stdafx.h"
#include "TBDrugTypeParameters.h"

#include "Exceptions.h"
#include "Log.h"

SETUP_LOGGING("TBDTP")

namespace Kernel
{
    TBDrugTypeParameters::TBDrugTypeParameters()
        : TB_drug_name()
        , TB_drug_inactivation_rate(0.0f)
        , TB_drug_cure_rate(0.0f)
        , TB_drug_resistance_rate(0.0f)
        , TB_drug_relapse_rate(0.0f)
        , TB_drug_mortality_rate(0.0f)
    {
        initConfigTypeMap("TB_Drug_Name",                         &TB_drug_name,                         Drug_Name_DESC_TEXT,                            "");

        initConfigTypeMap("TB_Drug_Inactivation_Rate",            &TB_drug_inactivation_rate,            TB_Drug_Inactivation_Rate_DESC_TEXT,            0.0f,      1.0f, 1.0f);
        initConfigTypeMap("TB_Drug_Cure_Rate",                    &TB_drug_cure_rate,                    TB_Drug_Cure_Rate_DESC_TEXT,                    0.0f,      1.0f, 1.0f);
        initConfigTypeMap("TB_Drug_Resistance_Rate",              &TB_drug_resistance_rate,              TB_Drug_Resistance_Rate_DESC_TEXT,              0.0f,      1.0f, 0.0f); // 0.0 means no resistance
        initConfigTypeMap("TB_Drug_Relapse_Rate",                 &TB_drug_relapse_rate,                 TB_Drug_Relapse_Rate_DESC_TEXT,                 0.0f,      1.0f, 0.0f); // 0.0 means no relapse
        initConfigTypeMap("TB_Drug_Mortality_Rate",               &TB_drug_mortality_rate,               TB_Drug_Mortality_Rate_DESC_TEXT,               0.0f,      1.0f, 0.0f); // 0.0 means no mortality
        initConfigTypeMap("TB_Drug_Primary_Decay_Time_Constant",  &TB_drug_primary_decay_time_constant,  TB_Drug_Primary_Decay_Time_Constant_DESC_TEXT,  0.0f, 100000.0f, 0.0f);
    }

    TBDrugTypeParameters::~TBDrugTypeParameters()
    { }

    bool TBDrugTypeParameters::Configure(const ::Configuration *config)
    {
        return JsonConfigurable::Configure( config );
    }

    const std::string& TBDrugTypeParameters::GetName() const
    {
        return TB_drug_name;
    }
}

