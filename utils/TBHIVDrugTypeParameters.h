
#pragma once

#include "Configure.h"
#include "JsonConfigurableCollection.h"
#include "TBDrugTypeParameters.h"

namespace Kernel 

{
    class TBHIVDrugTypeParameters : public TBDrugTypeParameters
    {
        friend class AntiTBPropDepDrug;
        friend class TBHIVConfigurableTBdrug;

        IMPLEMENT_DEFAULT_REFERENCE_COUNTING()

    public:
        TBHIVDrugTypeParameters();
        virtual ~TBHIVDrugTypeParameters();

    protected:
        float TB_drug_inactivation_rate_mdr;
        float TB_drug_inactivation_rate_hiv;

        float TB_drug_cure_rate_mdr;
        float TB_drug_cure_rate_hiv;

        float TB_drug_resistance_rate_hiv;

        float TB_drug_relapse_rate_mdr;
        float TB_drug_relapse_rate_hiv;

        float TB_drug_mortality_rate_hiv;
        float TB_drug_mortality_rate_mdr;

        float TB_reduced_transmit;
        float TB_reduced_acquire;
    };

    class TBHIVDrugCollection : public JsonConfigurableCollection<TBHIVDrugTypeParameters>
    {
    public:
        IMPLEMENT_NO_REFERENCE_COUNTING()

        TBHIVDrugCollection();
        virtual ~TBHIVDrugCollection();

        const TBHIVDrugTypeParameters& GetDrug( const std::string& rName ) const;

    protected:
        virtual TBHIVDrugTypeParameters* CreateObject() override;
    };
}
