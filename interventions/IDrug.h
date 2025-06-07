
#pragma once

#include "ISupports.h"

namespace Kernel
{
    struct IDrug : ISupports
    {
        virtual const std::string& GetDrugName() const = 0;
        virtual float GetDrugCurrentEfficacy() const = 0;
        virtual IDrug* GetDrug() = 0;
    };
}
