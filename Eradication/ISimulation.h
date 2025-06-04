
#pragma once

#include <string>
#include <functional>
#include "ISerializable.h"
#include "Configuration.h"
#include "IdmDateTime.h"
#include "IdmApi.h"

namespace Kernel
{
    struct INodeContext;
    struct SimParams;
    class RANDOMBASE;

    struct IDMAPI ISimulation : ISerializable
    {
        virtual void Initialize(const ::Configuration *config) = 0;
        virtual bool Populate() = 0;
        virtual void Update() = 0;

        virtual const SimParams* GetParams() const = 0;

        virtual int  GetSimulationTimestep() const = 0;
        virtual const IdmDateTime& GetSimulationTime() const = 0;
        virtual bool TimeToStop() = 0;

        virtual RANDOMBASE* GetRng() = 0; // should really only be accessed by Node

        typedef std::function<void(INodeContext*)> callback_t;
        virtual void RegisterNewNodeObserver(void* id, Kernel::ISimulation::callback_t observer) = 0;
        virtual void UnregisterNewNodeObserver(void* id) = 0;

        virtual void WriteReportsData() = 0;
        virtual ~ISimulation() {}
    };
}
