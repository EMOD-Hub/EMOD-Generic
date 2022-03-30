/***************************************************************************************************

Copyright (c) 2018 Intellectual Ventures Property Holdings, LLC (IVPH) All rights reserved.

EMOD is licensed under the Creative Commons Attribution-Noncommercial-ShareAlike 4.0 License.
To view a copy of this license, visit https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode

***************************************************************************************************/

#pragma once

#include "DllDefs.h"
#include "ProgVersion.h"
#include "Environment.h"
#include "RANDOM.h"
#include "IReport.h"
#include "NoCrtWarnings.h"

#define DLL_LOG(lvl, x, ...)   do { if((EnvPtr !=nullptr) && (EnvPtr->Log !=nullptr) && EnvPtr->Log->CheckLogLevel(Logger::lvl, "DllInterfaceHelper"))  EnvPtr->Log->Log(Logger::lvl, "DllInterfaceHelper", x, ##__VA_ARGS__); } while(0)


namespace Kernel
{
    class DllInterfaceHelper
    {
    public:
        DllInterfaceHelper( const char* rTypeName, 
                            const char** simTypes,
                            report_instantiator_function_t rif = nullptr )
            : m_RNG( nullptr )
            , m_TypeName( rTypeName )
            , m_SupportedSimTypes( simTypes )
            , m_ReportInstantiatorFunc( rif )
        {
        };

        char* GetEModuleVersion( char* sVer, const Environment* pEnv )
        {            
            Environment::setInstance(const_cast<Environment*>(pEnv));
            CreateRandomNumberGenerator( pEnv );
            ProgDllVersion pv;
            DLL_LOG( INFO, "%s: Version=%s  Branch=%s  SccsDate=%s  BuilderName=%s  BuildDate=%s\n",
                     m_TypeName, 
                     pv.getVersion(),
                     pv.getSccsBranch(),
                     pv.getSccsDate(),
                     pv.getBuilderName(),
                     pv.getBuildDate() );
            fflush(stdout);

            if (sVer)
            {
                int length = strlen(pv.getVersion()) + 1 ;
                strcpy_s(sVer, length, pv.getVersion());
            }
            return sVer;
        };

        void GetSupportedSimTypes( char* simTypes[] )
        {
            int i=0;
            while (m_SupportedSimTypes[i] != NULL && i < SIMTYPES_MAXNUM )
            {
                // allocation will be freed by the caller
                int length = strlen(m_SupportedSimTypes[i]) + 1 ;
                simTypes[i] = new char[length];
                strcpy_s(simTypes[i], length, m_SupportedSimTypes[i]);
                i++;
            }
            simTypes[i] = NULL;
        };

        const char* GetType()
        {
            DLL_LOG( INFO, "GetType called for %s\n", m_TypeName );
            return m_TypeName;
        };

        void GetReportInstantiator( Kernel::report_instantiator_function_t* pif )
        {
            DLL_LOG( INFO, "GetReportInstantiator called for %s\n", m_TypeName );
            *pif = m_ReportInstantiatorFunc ;
        };

        RANDOMBASE* GetRandomNumberGenerator() { return m_RNG; };

    private:
        void CreateRandomNumberGenerator( const Environment* pEnv )
        {
            if( pEnv->Config != nullptr )
            {
                uint16_t run_number = GET_CONFIG_INTEGER( pEnv->Config, "Run_Number" );
                uint16_t randomseed[2];
                randomseed[0] = (uint16_t) run_number;
                randomseed[1] = (uint16_t) pEnv->MPI.Rank;
                m_RNG = new PSEUDO_DES(*((uint32_t*) randomseed));
            }
        }

        RANDOMBASE * m_RNG;
        const char* m_TypeName ;
        const char** m_SupportedSimTypes ;
        report_instantiator_function_t m_ReportInstantiatorFunc ;
    };

}