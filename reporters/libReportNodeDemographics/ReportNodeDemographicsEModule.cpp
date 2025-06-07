
#include "stdafx.h"

#include "ReportNodeDemographics.h"
#include "DllInterfaceHelper.h"
#include "FactorySupport.h"

//******************************************************************************

//******************************************************************************

SETUP_LOGGING( "ReportNodeDemographics" )

static const char* _sim_types[] = { "*", nullptr };

Kernel::DllInterfaceHelper DLL_HELPER( _module, _sim_types );

//******************************************************************************
// DLL Methods
//******************************************************************************

#ifdef __cplusplus
extern "C" {
#endif

DTK_DLLEXPORT char*
__cdecl GetEModuleVersion(char* sVer, const Environment* pEnv)
{
    return DLL_HELPER.GetEModuleVersion( sVer, pEnv );
}

DTK_DLLEXPORT void
__cdecl GetSupportedSimTypes(char* simTypes[])
{
    DLL_HELPER.GetSupportedSimTypes( simTypes );
}

DTK_DLLEXPORT const char*
__cdecl GetType()
{
    return DLL_HELPER.GetType();
}

DTK_DLLEXPORT Kernel::IReport*
__cdecl GetReportInstantiator()
{
    return new Kernel::ReportNodeDemographics();
}

#ifdef __cplusplus
}
#endif

//******************************************************************************