
#include "stdafx.h"
#include "TBDrugTypeParameters.h"
#include "TBHIVDrugTypeParameters.h"
#include "Exceptions.h"
#include "Log.h"

SETUP_LOGGING("TBHIVDTP")


namespace Kernel
{
    TBHIVDrugTypeParameters::TBHIVDrugTypeParameters()
        : TBDrugTypeParameters()
        , TB_drug_inactivation_rate_mdr(0.0f)
        , TB_drug_inactivation_rate_hiv(0.0f)
        , TB_drug_cure_rate_mdr(0.0f)
        , TB_drug_cure_rate_hiv(0.0f)
        , TB_drug_resistance_rate_hiv(0.0f)
        , TB_drug_relapse_rate_mdr(0.0f)
        , TB_drug_relapse_rate_hiv(0.0f)
        , TB_drug_mortality_rate_mdr(0.0f)
        , TB_drug_mortality_rate_hiv(0.0f)
        , TB_reduced_acquire(0.0f)
        , TB_reduced_transmit(0.0f)
    {
        initConfigTypeMap( "TB_Drug_Inactivation_Rate_HIV", &TB_drug_inactivation_rate_hiv, TB_Drug_Inactivation_Rate_HIV_DESC_TEXT,  0.0f, 1.0f, 1.0f, "Enable_Coinfection" );
        initConfigTypeMap( "TB_Drug_Inactivation_Rate_MDR", &TB_drug_inactivation_rate_mdr, TB_Drug_Inactivation_Rate_MDR_DESC_TEXT,  0.0f, 1.0f, 1.0f, "Enable_Coinfection" );

        initConfigTypeMap( "TB_Drug_Cure_Rate_HIV",         &TB_drug_cure_rate_hiv,         TB_Drug_Cure_Rate_HIV_DESC_TEXT,          0.0f, 1.0f, 1.0f, "Enable_Coinfection" );
        initConfigTypeMap( "TB_Drug_Cure_Rate_MDR",         &TB_drug_cure_rate_mdr,         TB_Drug_Cure_Rate_MDR_DESC_TEXT,          0.0f, 1.0f, 1.0f, "Enable_Coinfection" );

        initConfigTypeMap( "TB_Drug_Resistance_Rate_HIV",   &TB_drug_resistance_rate_hiv,   TB_Drug_Resistance_Rate_HIV_DESC_TEXT,    0.0f, 1.0f, 0.0f, "Enable_Coinfection" );

        initConfigTypeMap( "TB_Drug_Relapse_Rate_MDR",      &TB_drug_relapse_rate_mdr,      TB_Drug_Relapse_Rate_MDR_DESC_TEXT,       0.0f, 1.0f, 0.0f, "Enable_Coinfection" );
        initConfigTypeMap( "TB_Drug_Relapse_Rate_HIV",      &TB_drug_relapse_rate_hiv,      TB_Drug_Relapse_Rate_HIV_DESC_TEXT,       0.0f, 1.0f, 0.0f, "Enable_Coinfection" );

        initConfigTypeMap( "TB_Drug_Mortality_Rate_MDR",    &TB_drug_mortality_rate_mdr,    TB_Drug_Mortality_Rate_MDR_DESC_TEXT,     0.0f, 1.0f, 0.0f, "Enable_Coinfection" );
        initConfigTypeMap( "TB_Drug_Mortality_Rate_HIV",    &TB_drug_mortality_rate_hiv,    TB_Drug_Mortality_Rate_HIV_DESC_TEXT,     0.0f, 1.0f, 0.0f, "Enable_Coinfection" );

        initConfigTypeMap( "TB_Reduced_Transmit",           &TB_reduced_transmit,           TB_Reduced_Transmit_TBHIV_DESC_TEXT,      0.0f, 1.0f, 0.0f, "Enable_Coinfection" );
        initConfigTypeMap( "TB_Reduced_Acquire",            &TB_reduced_acquire,            TB_Reduced_Acquire_TBHIV_DESC_TEXT,       0.0f, 1.0f, 0.0f, "Enable_Coinfection" );
    }

    TBHIVDrugTypeParameters::~TBHIVDrugTypeParameters()
    { }


    // ***** TBHIVDrugCollection is a container for 0 or more TBHIVDrugTypeParameters *****
    TBHIVDrugCollection::TBHIVDrugCollection()
        : JsonConfigurableCollection("TBHIV_Drug_Params")
    {
    }

    TBHIVDrugCollection::~TBHIVDrugCollection()
    {
    }

    TBHIVDrugTypeParameters* TBHIVDrugCollection::CreateObject()
    {
        return new TBHIVDrugTypeParameters();
    }

    const TBHIVDrugTypeParameters& TBHIVDrugCollection::GetDrug( const std::string& rName ) const
    {
        TBHIVDrugTypeParameters* p_found = nullptr;
        for( auto p_drug : m_Collection )
        {
            if( p_drug->GetName() == rName )
            {
                p_found = p_drug;
            }
        }
        if( p_found == nullptr )
        {
            std::stringstream ss;
            ss << "'" << rName << "' is an unknown drug.\n";
            ss << "Valid drug names are:\n";
            for( auto p_drug : m_Collection )
            {
                ss << p_drug->GetName() << "\n";
            }
            throw GeneralConfigurationException( __FILE__, __LINE__, __FUNCTION__, ss.str().c_str() );
        }
        return *p_found;
    }
}
