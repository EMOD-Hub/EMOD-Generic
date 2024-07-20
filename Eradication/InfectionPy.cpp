/***************************************************************************************************

Copyright (c) 2018 Intellectual Ventures Property Holdings, LLC (IVPH) All rights reserved.

EMOD is licensed under the Creative Commons Attribution-Noncommercial-ShareAlike 4.0 License.
To view a copy of this license, visit https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode

***************************************************************************************************/

#include "stdafx.h"

#ifdef ENABLE_PYTHON

#include "InfectionPy.h"
#include "SusceptibilityPy.h"
#include "InterventionsContainer.h"
#include "Environment.h"
#include "Debug.h"

#include "Common.h"
#include "MathFunctions.h"
#include "RANDOM.h"
#include "Exceptions.h"
#include "StrainIdentity.h"

using namespace std;

SETUP_LOGGING( "InfectionPy" )

namespace Kernel
{
    GET_SCHEMA_STATIC_WRAPPER_IMPL(Py.Infection,InfectionPyConfig)
    BEGIN_QUERY_INTERFACE_BODY(InfectionPyConfig)
    END_QUERY_INTERFACE_BODY(InfectionPyConfig)

    bool InfectionPyConfig::Configure( const Configuration * config )
    {
        LOG_DEBUG( "Configure\n" );

        bool bRet = JsonConfigurable::Configure( config );
        return bRet;
    }

    BEGIN_QUERY_INTERFACE_BODY(InfectionPy)
        HANDLE_INTERFACE(IInfectionPy)
    END_QUERY_INTERFACE_BODY(InfectionPy)

    InfectionPy::InfectionPy()
    {
    }

    InfectionPy::InfectionPy(IIndividualHumanContext* context)
        : Infection(context)
    {
    }

    IInfectionPy* InfectionPy::GetInfectionPy()
    {
        return static_cast<IInfectionPy*>(this);
    }

    void InfectionPy::Initialize(suids::suid _suid)
    {
        Infection::Initialize(_suid);
    }

    InfectionPy *InfectionPy::CreateInfection(IIndividualHumanContext *context, suids::suid _suid)
    {
        InfectionPy *newinfection = _new_ InfectionPy(context);
        newinfection->Initialize(_suid);

        return newinfection;
    }

    InfectionPy::~InfectionPy()
    {
    }

    void InfectionPy::InitInfectionImmunology(Susceptibility* _immunity)
    {
        StateChange = InfectionStateChange::New;
        return Infection::InitInfectionImmunology( _immunity );
    }

    void InfectionPy::Update(float dt, ISusceptibilityContext* _immunity)
    {
        return;
    }

    void InfectionPy::Clear()
    {
        StateChange = InfectionStateChange::Cleared;
    }
}

#endif // ENABLE_PYTHON
