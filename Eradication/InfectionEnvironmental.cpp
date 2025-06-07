
#include "stdafx.h"

#if defined(ENABLE_ENVIRONMENTAL)

#include "Sugar.h"
#include "InfectionEnvironmental.h"
#include "IIndividualHumanContext.h"

SETUP_LOGGING( "InfectionEnvironmental" )

namespace Kernel
{
    InfectionEnvironmental::InfectionEnvironmental()
    { }

    InfectionEnvironmental::~InfectionEnvironmental(void)
    { }

    InfectionEnvironmental::InfectionEnvironmental(IIndividualHumanContext *context) : Kernel::Infection(context)
    { }

    InfectionEnvironmental *InfectionEnvironmental::CreateInfection(IIndividualHumanContext *context, suids::suid _suid)
    {
        InfectionEnvironmental *newinfection = _new_ InfectionEnvironmental(context);
        newinfection->Initialize(_suid);

        return newinfection;
    }

    REGISTER_SERIALIZABLE(InfectionEnvironmental);

    void InfectionEnvironmental::serialize(IArchive& ar, InfectionEnvironmental* obj)
    {
        Infection::serialize(ar, obj);
    }
}

#endif // ENABLE_ENVIRONMENTAL
