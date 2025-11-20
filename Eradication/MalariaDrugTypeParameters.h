
#pragma once

#include "Configure.h"
#include "JsonConfigurableCollection.h"

namespace Kernel 
{
    struct IGenomeMarkers;
    struct IStrainIdentity;



    class DoseFractionByAge : public JsonConfigurable
    {
        IMPLEMENT_DEFAULT_REFERENCE_COUNTING()
    public:
        DoseFractionByAge();
        virtual ~DoseFractionByAge();

        // JsonConfigurable methods
        virtual bool Configure( const Configuration * inputJson ) override;

        float GetAgeDays() const;
        float GetDoseFraction() const;

    protected:
        float m_AgeDays;
        float m_DoseFraction;
    };

    class DoseMap : public JsonConfigurableCollection<DoseFractionByAge>
    {
        IMPLEMENT_DEFAULT_REFERENCE_COUNTING()

    public:
        DoseMap();
        virtual ~DoseMap();

        virtual void CheckConfiguration() override;

        float GetFractionalDose( float ageInDays ) const;

    protected:
        virtual DoseFractionByAge* CreateObject() override;
    };

    class DrugModifier : public JsonConfigurable
    {
    public:
        DrugModifier();
        virtual ~DrugModifier();

        IMPLEMENT_NO_REFERENCE_COUNTING()

        // JsonConfigurable methods
        virtual bool Configure( const Configuration* inputJson ) override;

        // other methods
        const std::string& GetDrugResistantString() const;
        float GetC50()        const;
        float GetMaxKilling() const;

    private:
        std::string m_DrugString;
        float m_C50;
        float m_MaxKilling;
    };

    class DrugResistanceModifierCollection : public JsonConfigurableCollection<DrugModifier>
    {
    public:
        IMPLEMENT_NO_REFERENCE_COUNTING()

        DrugResistanceModifierCollection();
        virtual ~DrugResistanceModifierCollection();

        virtual void CheckConfiguration() override;

        float GetC50( const IStrainIdentity& rStrain ) const;
        float GetMaxKilling( const IStrainIdentity& rStrain ) const;

    protected:
        virtual DrugModifier* CreateObject() override;
    };

    class MalariaDrugTypeParameters : public JsonConfigurable
    {
        IMPLEMENT_DEFAULT_REFERENCE_COUNTING()

    public:
        static MalariaDrugTypeParameters* CreateMalariaDrugTypeParameters(const Configuration* inputJson,
                                                                          const std::string& drugType,
                                                                          const IGenomeMarkers& rGenomeMarkers);

        virtual ~MalariaDrugTypeParameters();
        virtual bool Configure( const ::Configuration *json );

        typedef map< std::string, MalariaDrugTypeParameters* > tMDTPMap;

        const std::string& GetName()    const;
        float GetMaxDrugIRBCKill()      const;
        float GetKillRateHepatocyte()   const;
        float GetKillRateGametocyte02() const;
        float GetKillRateGametocyte34() const;
        float GetKillRateGametocyteM()  const;
        float GetPkpdC50()              const;
        float GetCMax()                 const;
        float GetVd()                   const;
        float GetDecayT1()              const;
        float GetDecayT2()              const;
        int   GetFullTreatmentDoses()   const;
        float GetDoseInterval()         const;
        float GetBodyWeightExponent()   const;

        const DoseMap& GetDoseMap() const;
        const DrugResistanceModifierCollection& GetResistantModifiers() const;

    protected:
        friend class MalariaDrugTypeCollection;
        MalariaDrugTypeParameters();
        void Initialize(const std::string& drugType);

        std::string drug_name;
        float max_drug_IRBC_kill;
        float drug_hepatocyte_killrate;
        float drug_gametocyte02_killrate;
        float drug_gametocyte34_killrate;
        float drug_gametocyteM_killrate;
        float drug_pkpd_c50;
        float drug_Cmax;
        float drug_Vd;
        float drug_decay_T1;
        float drug_decay_T2;
        int   drug_fulltreatment_doses;
        float drug_dose_interval;
        float bodyweight_exponent;
        DoseMap dose_map;

        DrugResistanceModifierCollection m_Modifiers;
    };

    class MalariaDrugTypeCollection : public JsonConfigurableCollection<MalariaDrugTypeParameters>
    {
    public:
        static MalariaDrugTypeCollection* GetInstanceNonConst();
        static const MalariaDrugTypeCollection* GetInstance();
        static void DeleteInstance();

        virtual ~MalariaDrugTypeCollection();

        virtual void CheckConfiguration() override;
        const jsonConfigurable::tDynamicStringSet& GetDrugNames() const;
        const MalariaDrugTypeParameters& GetDrug( const std::string& rName ) const;

    protected:
        static MalariaDrugTypeCollection* m_pInstance;

        MalariaDrugTypeCollection();

        virtual MalariaDrugTypeParameters* CreateObject() override;

        jsonConfigurable::tDynamicStringSet m_DrugNames;
    };
}
