/***************************************************************************************************

Copyright (c) 2018 Intellectual Ventures Property Holdings, LLC (IVPH) All rights reserved.

EMOD is licensed under the Creative Commons Attribution-Noncommercial-ShareAlike 4.0 License.
To view a copy of this license, visit https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode

***************************************************************************************************/

#include "stdafx.h"
#include "Outbreak.h"

#include "Exceptions.h"
#include "SimulationConfig.h"
#include "InterventionFactory.h"
#include "NodeEventContext.h"  // for IOutbreakConsumer methods
#include "StrainIdentity.h"
#include "ISimulationContext.h"
#include "RANDOM.h"

SETUP_LOGGING( "Outbreak" )

// Important: Use the instance method to obtain the intervention factory obj instead of static method to cross the DLL boundary
// NO USAGE like this:  GET_CONFIGURABLE(SimulationConfig)->example_variable in DLL

#define MAX_INDIVIDUAL_AGE_IN_YRS (120)

namespace Kernel
{
    BEGIN_QUERY_INTERFACE_BODY(Outbreak)
        HANDLE_INTERFACE(IConfigurable)
        //HANDLE_INTERFACE(IDistributableIntervention)
        HANDLE_INTERFACE(IBaseIntervention)
        HANDLE_INTERFACE(INodeDistributableIntervention)
        HANDLE_INTERFACE(IOutbreak)
        HANDLE_ISUPPORTS_VIA(INodeDistributableIntervention)
    END_QUERY_INTERFACE_BODY(Outbreak)


    IMPLEMENT_FACTORY_REGISTERED(Outbreak)

    Outbreak::Outbreak() : import_age(DAYSPERYEAR)
    {
        initSimTypes( 10, "GENERIC_SIM" , "VECTOR_SIM" , "MALARIA_SIM", "AIRBORNE_SIM", "POLIO_SIM", "TBHIV_SIM", "STI_SIM", "HIV_SIM", "PY_SIM", "TYPHOID_SIM" );
        initConfigTypeMap( "Clade", &clade, Clade_DESC_TEXT, 0, 9, 0 );
        initConfigTypeMap( "Genome",  &genome,  Genome_DESC_TEXT, -1, 16777216, 0 );
        initConfigTypeMap( "Incubation_Period_Override", &incubation_period_override, Incubation_Period_Override_DESC_TEXT,-1, INT_MAX, -1);
    }

    bool Outbreak::Configure(const Configuration * inputJson)
    {
        initConfigTypeMap( "Number_Cases_Per_Node",  &num_cases_per_node,  Num_Import_Cases_Per_Node_DESC_TEXT, 0, INT_MAX, 1 );
        initConfigTypeMap( "Import_Age", &import_age, Import_Age_DESC_TEXT, 0, MAX_INDIVIDUAL_AGE_IN_YRS*DAYSPERYEAR, DAYSPERYEAR );
        initConfigTypeMap( "Probability_Of_Infection", &prob_infection, Probability_Of_Infection_DESC_TEXT, 1.0 );
        
        JsonConfigurable::Configure( inputJson );
        return true;
    }

    bool Outbreak::Distribute(INodeEventContext *context, IEventCoordinator2* pEC)
    {
        bool wasDistributed = false;

        IOutbreakConsumer *ioc;
        if (s_OK == context->QueryInterface(GET_IID(IOutbreakConsumer), (void**)&ioc))
        {
            ioc->AddImportCases(GetNewStrainIdentity( context ), import_age, num_cases_per_node, prob_infection);
            wasDistributed = true;
        }
        else
        {
            throw QueryInterfaceException( __FILE__, __LINE__, __FUNCTION__, "context", "IOutbreakConsumer", "INodeEventContext" );
        }

        return wasDistributed;
    }

    void Outbreak::Update( float dt )
    {
        LOG_WARN("updating outbreak (?!?)\n");
        // Distribute() doesn't call GiveIntervention() for this intervention, so it isn't added to the NodeEventContext's list of NDI
    }

    Kernel::StrainIdentity* Outbreak::GetNewStrainIdentity(INodeEventContext *context)
    {
        StrainIdentity *outbreak_strainID = nullptr;

        // Important: Use the instance method to obtain the intervention factory obj instead of static method to cross the DLL boundary
        // NO usage of GET_CONFIGURABLE(SimulationConfig)->example_variable in DLL
        IGlobalContext *pGC = nullptr;
        const SimulationConfig* simConfigObj = nullptr;
        if (s_OK == context->QueryInterface(GET_IID(IGlobalContext), (void**)&pGC))
        {
            simConfigObj = pGC->GetSimulationConfigObj();
        }
        if (!simConfigObj)
        {
            throw GeneralConfigurationException( __FILE__, __LINE__, __FUNCTION__, "The pointer to IInterventionFactory object is not valid (could be DLL specific)" );
        }

        if( clade < 0 )
        {
            throw ConfigurationRangeException( __FILE__, __LINE__, __FUNCTION__, "clade", clade, 0 );
        }

        outbreak_strainID = _new_ StrainIdentity(clade, genome);

        return outbreak_strainID;
    }
}

#if 0
namespace Kernel {
    template<class Archive>
    void serialize(Archive &ar, Outbreak &ob, const unsigned int v)
    {
        ar & ob.clade;
        ar & ob.genome;
        ar & ob.import_age;
    }
}
#endif
