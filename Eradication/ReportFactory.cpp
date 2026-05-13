
#include "stdafx.h"
#include "ReportFactory.h"
#include "ObjectFactoryTemplates.h"

SETUP_LOGGING( "ReportFactory" )

namespace Kernel
{
    ReportFactory* ReportFactory::_instance = nullptr;

    template ReportFactory* ObjectFactory<IReport, ReportFactory>::getInstance();
}
