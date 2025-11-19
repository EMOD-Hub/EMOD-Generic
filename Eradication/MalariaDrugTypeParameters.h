
#pragma once

#include "Configure.h"
#include "JsonConfigurableCollection.h"

namespace Kernel 
{
    struct IStrainIdentity;
    struct IGenomeMarkers;

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

    class GenomeMarkerModifiers : public JsonConfigurable
    {
    public:
        IMPLEMENT_NO_REFERENCE_COUNTING()

        GenomeMarkerModifiers( const std::string& rMarkerName = "", uint64_t genomeBitMask = 0 );
        virtual ~GenomeMarkerModifiers();

        // JsonConfigurable methods
        virtual bool Configure( const Configuration * inputJson ) override;

        // other methods
        inline const std::string& GetMarkerName() const { return m_MarkerName; }
        inline float              GetC50()        const { return m_C50; }
        inline float              GetMaxKilling() const { return m_MaxKilling; }

        inline bool HasGenomeBit( uint64_t genomeBits ) const
        { 
            return ((genomeBits & m_GenomeBitMask) != 0);
        }

    private:
        std::string m_MarkerName;
        float m_C50;
        float m_MaxKilling;
        uint64_t m_GenomeBitMask;
    };

    class DrugResistantModifiers : public JsonConfigurable, public IComplexJsonConfigurable
    {
    public:
        IMPLEMENT_NO_REFERENCE_COUNTING()

        DrugResistantModifiers( const IGenomeMarkers& rGenomeMarkers );
        virtual ~DrugResistantModifiers();

        // IComplexJsonConfigurable methods
        virtual bool  HasValidDefault() const override { return false; }
        virtual json::QuickBuilder GetSchema() override;
        virtual void ConfigureFromJsonAndKey( const Configuration* inputJson, const std::string& key ) override;

        // Other methods
        int Size() const;
        const GenomeMarkerModifiers& operator[]( int index ) const;

        float GetC50( const IStrainIdentity& rStrain ) const;
        float GetMaxKilling( const IStrainIdentity& rStrain ) const;

    private:
        std::vector<GenomeMarkerModifiers> m_ModifierCollection;
    };

    class MalariaDrugTypeParameters : public JsonConfigurable
    {
        IMPLEMENT_DEFAULT_REFERENCE_COUNTING()

    public:
        static MalariaDrugTypeParameters* CreateMalariaDrugTypeParameters( const Configuration* inputJson, 
                                                                           const std::string& drugType,
                                                                           const IGenomeMarkers& rGenomeMarkers );

        virtual ~MalariaDrugTypeParameters();
        virtual bool Configure( const ::Configuration *json );

        typedef map< std::string, MalariaDrugTypeParameters* > tMDTPMap;

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
        const DrugResistantModifiers& GetResistantModifiers() const;

    protected:
        MalariaDrugTypeParameters( const std::string& drugType, const IGenomeMarkers& rGenomeMarkers );
        void Initialize(const std::string& drugType);

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

        DrugResistantModifiers m_Modifiers;

    private:
        std::string _drugType;
    };
}
