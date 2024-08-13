/***************************************************************************************************

Copyright (c) 2018 Intellectual Ventures Property Holdings, LLC (IVPH) All rights reserved.

EMOD is licensed under the Creative Commons Attribution-Noncommercial-ShareAlike 4.0 License.
To view a copy of this license, visit https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode

***************************************************************************************************/

#pragma once

#include "IdmApi.h"
#include "Debug.h"
#include <string>
#include <list>
#include <vector>
#include <stdint.h>

#pragma warning (disable: 4267)
#pragma warning (disable: 4244)
#pragma warning (disable: 4250)

namespace Kernel
{
    struct IConfigurable;

    struct IDMAPI ISupports
    {
        // TODO: not implementing these because we have a simple hierarchical ownership structure...for the time being, but should be done soon <ERAD-285>
        virtual int32_t AddRef() = 0; // these return signed values because subtle concurrency issues could result in negative refcounts being returned in theory
        virtual int32_t Release() = 0;

        virtual IConfigurable* GetConfigurable() { return nullptr; }

        virtual ~ISupports() {}
    };
    
    class ISupportException : public std::exception
    {
    public:
        ISupportException()
        {
        }
        virtual std::string what()
        {
            return std::string( "Fatal exception: Release() called on object with negative reference count." );
        }
    };

// TODO: need some method to ensure that refcounts will be serialized...maybe a macro that must exist in the serialize method or else a compile error is thrown
// small helper class to keep ref counts auto-initialized
class IDMAPI RefCount
{
public:
    RefCount() : m_refcount(0) { }
    RefCount(int32_t _rc) : m_refcount(_rc) { }
    operator int32_t&() { return m_refcount; }
    template<class Archive> void serialize(Archive &ar, const unsigned int v) { ar & m_refcount; }
protected:
    int32_t m_refcount;
};

#ifdef WIN32
#include "windows.h"    // bring in InterlockedIncrement and InterlockedDecrement
#define IMPLEMENT_DEFAULT_REFERENCE_COUNTING() \
    protected: \
        RefCount m_refcount; \
    private:\
        /* force classes wishing to be serialized to always serialize their refcount. slightly ugly but vital user safeguard. */\
        /* user must rename their serialize method serialize_impl */\
        template<class Archive>\
        void serialize(Archive &ar, const unsigned int v) { ar & m_refcount; }\
    public:\
        virtual int32_t AddRef() { return InterlockedIncrement((LONG volatile *)&m_refcount); }\
        virtual int32_t Release()\
        {\
            int32_t count = InterlockedDecrement((LONG volatile *)&m_refcount);\
            if(count == 0){ delete this; }\
            if (count < 0) { throw ISupportException(); }\
            return count;\
        }
#define IMPLEMENT_ONLY_REFERENCE_COUNTING() \
    protected: \
        RefCount m_refcount; \
    public:\
        virtual int32_t AddRef() { return InterlockedIncrement((LONG volatile *)&m_refcount); }\
        virtual int32_t Release()\
        {\
            int32_t count = InterlockedDecrement((LONG volatile *)&m_refcount);\
            if(count == 0){ delete this; }\
            if (count < 0) { throw ISupportException(); }\
            return count;\
        }
#else
#define IMPLEMENT_DEFAULT_REFERENCE_COUNTING() \
    protected: \
        RefCount m_refcount; \
    private:\
        /* force classes wishing to be serialized to always serialize their refcount. slightly ugly but vital user safeguard. */\
        /* user must rename their serialize method serialize_impl */\
        template<class Archive>\
        void serialize(Archive &ar, const unsigned int v) { ar & m_refcount; }\
    public:\
        virtual int32_t AddRef() { return ++m_refcount; }\
        virtual int32_t Release()\
        {\
            int32_t count = --m_refcount;\
            if(count == 0){ delete this; }\
            if (count < 0) { throw ISupportException(); }\
            return count;\
        }
#define IMPLEMENT_ONLY_REFERENCE_COUNTING() \
    protected: \
        RefCount m_refcount; \
    public:\
        virtual int32_t AddRef() { return ((int32_t volatile*)&++m_refcount); }\
        virtual int32_t Release()\
        {\
            int32_t count = ((int32_t volatile*)&--m_refcount);\
            if(count == 0){ delete this; }\
            if (count < 0) { throw ISupportException(); }\
            return count;\
        }
#endif

// for classes that need to implement these methods but don't actually have reference counted behavior
#define IMPLEMENT_NO_REFERENCE_COUNTING() \
    public:\
        virtual int32_t AddRef() { return 1; }\
        virtual int32_t Release() { return 1; }
}
