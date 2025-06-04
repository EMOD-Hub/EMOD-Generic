
#pragma once

#ifdef ENABLE_PYTHON
#include <string>
#include <list>
#include <vector>

#include "Interventions.h"
#include "InterventionsContainer.h"

namespace Kernel
{
    class IPyVaccine;

    class PyInterventionsContainer : public InterventionsContainer
    {
        IMPLEMENT_DEFAULT_REFERENCE_COUNTING()

    public:
        PyInterventionsContainer();
        virtual ~PyInterventionsContainer();
    };
}
#endif
