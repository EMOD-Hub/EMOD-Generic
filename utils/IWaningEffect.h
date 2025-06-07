
#pragma once

#include <string>
#include <map>

#include "ISerializable.h"
#include "Configure.h"

namespace Kernel
{
    struct IIndividualHumanContext;
    struct INodeEventContext;

    struct IWaningEffect : ISerializable
    {
        virtual IWaningEffect* Clone()                               = 0;

        virtual bool  Configure(const Configuration*)                = 0;
        virtual void  Update(float)                                  = 0;
        virtual float Current()                               const  = 0;
        virtual bool  Expired()                               const  = 0;
        virtual void  SetContextTo(IIndividualHumanContext*)         = 0;
        virtual void  SetContextTo(INodeEventContext*)               = 0;
        virtual float GetInitial()                            const  = 0;
        virtual void  SetInitial(float)                              = 0;

        virtual JsonConfigurable* GetConfigurable()                  = 0;
    };

    class WaningEffectFactory
    {
    public:
        static IWaningEffect* CreateInstance();
    };
}
