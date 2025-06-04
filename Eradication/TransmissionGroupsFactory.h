
#pragma once

#include "ITransmissionGroups.h"

namespace Kernel
{
    class RANDOMBASE;

    class TransmissionGroupsFactory
    {
    public:
        static ITransmissionGroups* CreateNodeGroups( TransmissionGroupType::Enum groupsType, RANDOMBASE* prng );
    };
}