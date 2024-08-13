/***************************************************************************************************

Copyright (c) 2018 Intellectual Ventures Property Holdings, LLC (IVPH) All rights reserved.

EMOD is licensed under the Creative Commons Attribution-Noncommercial-ShareAlike 4.0 License.
To view a copy of this license, visit https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode

***************************************************************************************************/

#include "stdafx.h"
#include "MasterInterventionsContainer.h"
#include "TBInterventionsContainer.h"
#include "HIVInterventionsContainer.h"
#include "TBInterventionsContainer.h"
#include "Log.h"
#include "Sugar.h"
#include <assert.h>

SETUP_LOGGING( "MasterInterventionsContainer" )

namespace Kernel
{
    MasterInterventionsContainer::MasterInterventionsContainer()
        : InterventionsContainer()
        , InterventionsContainerList()
    {
    }

    MasterInterventionsContainer::~MasterInterventionsContainer()
    {        
        for( auto container : InterventionsContainerList )
        {
            delete container;
        }
        InterventionsContainerList.clear();
    }

    void MasterInterventionsContainer::InitInterventionContainers()
    {
        // object is constructed everytime it is desirialized, add intervebtions only once 
        assert( InterventionsContainerList.size() <= 2 );
        if( InterventionsContainerList.size() == 0 )
        {
            InterventionsContainerList.push_back( _new_ TBInterventionsContainer() );
            InterventionsContainerList.push_back( _new_ HIVInterventionsContainer() );
        }
    }

    void MasterInterventionsContainer::SetContextTo( IIndividualHumanContext* context)
    {
        InitInterventionContainers();

       for (auto container : InterventionsContainerList)
        {
            container->SetContextTo(context);
        }

        //set our own context
        parent = context;
    }

    IIndividualHumanContext* MasterInterventionsContainer::GetParent()
    {
        return parent;
    }

    std::list<IDistributableIntervention*> MasterInterventionsContainer::GetInterventionsByType(const std::string& type_name)
    {
        for (auto container : InterventionsContainerList)
        {
            //first check if it is TB or HIV container, only use the TB container for base class functions
            ITBInterventionsContainer* pTBIC = container->GetContainerTB();
            if (pTBIC)
            {
                return container->GetInterventionsByType(type_name);
            }
        }

        std::list<IDistributableIntervention*> emptyList;
        return emptyList;
    }
    
    void MasterInterventionsContainer::PurgeExisting( const std::string& iv_name )
    {
        throw IllegalOperationException( __FILE__, __LINE__, __FUNCTION__, "MasterInterventionsContainer::PurgeExisting should never be called (for now)." );
    }

    void MasterInterventionsContainer::InfectiousLoopUpdate(float dt)
    {
        for (auto container : InterventionsContainerList)
        {
            container->InfectiousLoopUpdate(dt);
        }
    }

    void MasterInterventionsContainer::Update( float dt )
    {
        for( auto container : InterventionsContainerList )
        {
            container->Update( dt );
        }
    }

    IHIVInterventionsContainer* MasterInterventionsContainer::GetContainerHIV()
    {
        IHIVInterventionsContainer* p_ret = nullptr;

        for (auto container : InterventionsContainerList)
        {
            p_ret = container->GetContainerHIV();
            if(p_ret)
            {
                break;
            }
        }

        return p_ret;
    }

    ITBInterventionsContainer* MasterInterventionsContainer::GetContainerTB()
    {
        ITBInterventionsContainer* p_ret = nullptr;

        for (auto container : InterventionsContainerList)
        {
            p_ret = container->GetContainerTB();
            if(p_ret)
            {
                break;
            }
        }

        return p_ret;
    }

    void MasterInterventionsContainer::UpdateIVAcquireRate( float acq, IVRoute::Enum vax_route)
    {
        for (auto container : InterventionsContainerList)
        {
            container->UpdateIVAcquireRate(acq, vax_route);
        }    
    }

    void MasterInterventionsContainer::UpdateIVTransmitRate( float xmit, IVRoute::Enum vax_route)
    {
        for (auto container : InterventionsContainerList)
        {
            container->UpdateIVTransmitRate(xmit, vax_route);
        }    
    }

    void MasterInterventionsContainer::UpdateIVMortalityRate( float mort, IVRoute::Enum vax_route)
    {
        for (auto container : InterventionsContainerList)
        {
            container->UpdateIVMortalityRate(mort, vax_route);
        }    
    }

    float MasterInterventionsContainer::GetInterventionReducedAcquire(TransmissionRoute::Enum tx_route) const
    {
        for (auto container : InterventionsContainerList)
        {
            //first check if it is TB or HIV container, only use the TB container for base class functions
            ITBInterventionsContainer* pTBIC = container->GetContainerTB();
            if (pTBIC)
            {
                return container->GetInterventionReducedAcquire(tx_route);
            }
        }

        return 0.0f;
    }

    float MasterInterventionsContainer::GetInterventionReducedTransmit(TransmissionRoute::Enum tx_route) const
    {
        for (auto container : InterventionsContainerList)
        {
            //first check if it is TB or HIV container, only use the TB container for base class functions
            ITBInterventionsContainer* pTBIC = container->GetContainerTB();
            if (pTBIC)
            {
                return container->GetInterventionReducedTransmit(tx_route);
            }
        }

        return 1.0f;
    }

    float MasterInterventionsContainer::GetInterventionReducedMortality(TransmissionRoute::Enum tx_route) const
    {
        for (auto container : InterventionsContainerList)
        {
            //first check if it is TB or HIV container, only use the TB container for base class functions
            ITBInterventionsContainer* pTBIC = container->GetContainerTB();
            if (pTBIC)
            {
                return container->GetInterventionReducedMortality(tx_route);
            }
        }

        return 0.0f;
    }

    bool MasterInterventionsContainer::GiveIntervention( IDistributableIntervention* pIV )
    {
        for (auto container : InterventionsContainerList)
        {
            //first check if it is TB or HIV container, only use the TB container for base class functions
            ITBInterventionsContainer* pTBIC = container->GetContainerTB();
            if (pTBIC)
            {
                return container->GiveIntervention(pIV);
            }
        }

        return false;
    }

    REGISTER_SERIALIZABLE(MasterInterventionsContainer);

    void MasterInterventionsContainer::serialize(IArchive& ar, MasterInterventionsContainer* obj)
    {
        MasterInterventionsContainer& container = *obj;
        ar.labelElement("InterventionsContainerList") & container.InterventionsContainerList;
    }
}
