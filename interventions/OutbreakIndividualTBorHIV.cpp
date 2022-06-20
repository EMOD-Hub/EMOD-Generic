/***************************************************************************************************

Copyright (c) 2018 Intellectual Ventures Property Holdings, LLC (IVPH) All rights reserved.

EMOD is licensed under the Creative Commons Attribution-Noncommercial-ShareAlike 4.0 License.
To view a copy of this license, visit https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode

***************************************************************************************************/

#include "stdafx.h"

#include "OutbreakIndividual.h"
#include "IIndividualHuman.h"
#include "InterventionFactory.h"
#include "NodeEventContext.h"
#include "Exceptions.h"
#include "IdmString.h"
#include "OutbreakIndividualTBorHIV.h"
#include "IndividualCoInfection.h"

#include "TBContexts.h"
#include "IIndividualHumanHIV.h"

SETUP_LOGGING( "OutbreakIndividualTBorHIV" )

namespace Kernel
{
    BEGIN_QUERY_INTERFACE_BODY(OutbreakIndividualTBorHIV)
        HANDLE_INTERFACE(IConfigurable)
        HANDLE_INTERFACE(IDistributableIntervention)
        HANDLE_INTERFACE(IOutbreakIndividual)
        HANDLE_ISUPPORTS_VIA(IDistributableIntervention)
    END_QUERY_INTERFACE_BODY(OutbreakIndividualTBorHIV) 

    IMPLEMENT_FACTORY_REGISTERED(OutbreakIndividualTBorHIV) 

    OutbreakIndividualTBorHIV::OutbreakIndividualTBorHIV()
    {
        initSimTypes(1, "TBHIV_SIM");
    }

    bool OutbreakIndividualTBorHIV::Configure( const Configuration * inputJson )
    {
        bool ret = initConfig("Infection_Type", infection_type, inputJson, MetadataDescriptor::Enum("Infection_Type", "TB =1 or HIV = 0", MDD_ENUM_ARGS(TBHIVInfectionType)) );

        if (!JsonConfigurable::_dryrun && infection_type == TBHIVInfectionType::HIV && !IndividualHumanCoInfectionConfig::enable_coinfection )
        {
            throw IncoherentConfigurationException(__FILE__, __LINE__, __FUNCTION__, "Intervention in campaign", "OutbreakIndividualTBorHIV", "Enable_Coinfection = 0", "Infection_Type = HIV");
        }
            
        ret = OutbreakIndividual::Configure(inputJson);
        
        return ret;
    }

    QuickBuilder OutbreakIndividualTBorHIV::GetSchema()
    {
        return QuickBuilder(jsonSchemaBase);
    }

    bool OutbreakIndividualTBorHIV::Distribute(
        IIndividualHumanInterventionsContext *context,
        ICampaignCostObserver * pCCO
    )
    {
        bool success = true;
        StrainIdentity outbreak_strain(clade,genome);

        IndividualHumanCoInfection* Ind_coinf = dynamic_cast<IndividualHumanCoInfection*>(context->GetParent());  // Better be TBHIV at this point 
        IIndividualHuman* individual = dynamic_cast<IIndividualHuman*>(context->GetParent()); // QI in new code

        LOG_DEBUG("Infecting individual from Outbreak.\n");
        if (infection_type == TBHIVInfectionType::TB)
        {
            LOG_INFO("Infecting individual from Outbreak with TB.\n");
            
            if (Ind_coinf->HasTB())
            {
                LOG_DEBUG_F("Individual was selected for TB infection via OutbreakIndividualTBorHIV, but already infected with TB. Quitting here.\n");
                success = false; 
            }
            else
            {
                Ind_coinf->AcquireNewInfection(&outbreak_strain, TransmissionRoute::OUTBREAK, incubation_period_override);
            }
        }
        else if (infection_type == TBHIVInfectionType::HIV)
        {
            LOG_INFO("Infecting individual from Outbreak with HIV.\n");

            if (Ind_coinf->HasHIV())
            {
                LOG_DEBUG_F("Individual was selected for HIV infection via OutbreakIndividualTBorHIV, but already infected with HIV. Quitting here.\n");
                success = false;
            }
            else
            {
                Ind_coinf->AcquireNewInfectionHIV(&outbreak_strain, TransmissionRoute::OUTBREAK, incubation_period_override);
            }
        }
        else
        {
            LOG_INFO("Infection Type Neither HIV or TB, no infections from Outbreak");
            success = false;
        }

        return success;
    }
}

