/***************************************************************************************************

Copyright (c) 2018 Intellectual Ventures Property Holdings, LLC (IVPH) All rights reserved.

EMOD is licensed under the Creative Commons Attribution-Noncommercial-ShareAlike 4.0 License.
To view a copy of this license, visit https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode

***************************************************************************************************/

#include "stdafx.h"
#ifdef ENABLE_PYTHON

#include "Sugar.h"
#include "Debug.h"
#include "Environment.h"
#include "PyInterventionsContainer.h"

SETUP_LOGGING( "PyInterventionsContainer" )

namespace Kernel
{
    PyInterventionsContainer::PyInterventionsContainer()
    {
    }

    PyInterventionsContainer::~PyInterventionsContainer()
    {
    }
}

#endif // ENABLE_PYTHON
