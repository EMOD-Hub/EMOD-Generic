
#pragma once

#include <functional>
#include "FactorySupport.h"
#include "IReport.h"
#include "ObjectFactory.h"

namespace Kernel
{

    class ReportFactory : public ObjectFactory<IReport,ReportFactory>
    {
    };
}
