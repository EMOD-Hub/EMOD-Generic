
#pragma once

namespace Kernel
{
    struct IIndividualHumanSTI;
    struct IIndividualHumanEventContext;
}

namespace ReportUtilitiesSTI
{
    Kernel::IIndividualHumanSTI* GetTransmittingPartner( Kernel::IIndividualHumanEventContext* recipientContext );
}
