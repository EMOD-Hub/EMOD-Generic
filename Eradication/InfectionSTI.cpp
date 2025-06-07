
#include "stdafx.h"

#include "Debug.h"
#include "InfectionSTI.h"

SETUP_LOGGING( "InfectionSTI" )

namespace Kernel
{
    bool InfectionSTIConfig::Configure( const Configuration * config )
    {
        LOG_DEBUG("Configure\n");

        bool bRet = JsonConfigurable::Configure( config );
        return bRet;
    }

    InfectionSTI *InfectionSTI::CreateInfection(IIndividualHumanContext *context, suids::suid _suid)
    {
        InfectionSTI *newinfection = _new_ InfectionSTI(context);
        newinfection->Initialize(_suid);

        return newinfection;
    }

    InfectionSTI::~InfectionSTI(void) { }
    InfectionSTI::InfectionSTI() { }
    InfectionSTI::InfectionSTI(IIndividualHumanContext *context) : Infection(context) { }

    void InfectionSTI::Update(float dt, ISusceptibilityContext* immunity)
    {
        Infection::Update( dt, immunity );
    }

    REGISTER_SERIALIZABLE(InfectionSTI);

    void InfectionSTI::serialize(IArchive& ar, InfectionSTI* obj)
    {
        Infection::serialize( ar, obj );
        InfectionSTI& infection = *obj;
    }
}
