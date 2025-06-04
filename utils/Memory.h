
#pragma once

#include "Configure.h"

namespace Kernel
{
    class MemoryGauge : public JsonConfigurable
    {
        GET_SCHEMA_STATIC_WRAPPER( MemoryGauge )
    public:

        static void CheckMemoryFailure( bool onlyCheckForFailure );

        MemoryGauge();
        virtual ~MemoryGauge();

        virtual bool Configure( const Configuration* inputJson ) override;
        virtual int32_t AddRef() { return -1; }
        virtual int32_t Release() { return -1; }

    private:
        static void*    m_ProcessHandle;
        static uint64_t m_WorkingSetWarningMB;
        static uint64_t m_WorkingSetHaltMB;
        static uint64_t m_LastPeakSizeMB;
    };
}