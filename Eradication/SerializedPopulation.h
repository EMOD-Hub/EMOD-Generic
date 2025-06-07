
#pragma once

#include <cstdint>

namespace Kernel{
    struct ISimulation;
    class Simulation;
}

namespace SerializedState {
    Kernel::ISimulation* LoadSerializedSimulation(const char* filename);
    void SaveSerializedSimulation(Kernel::Simulation* sim, uint32_t time_step, bool compress);

    // Declare these so the friend declarations in Simulation.h will work:
    struct Header;
    Kernel::ISimulation* ReadDtkVersion2(FILE* f, const char* filename, Header& header);
    Kernel::ISimulation* ReadDtkVersion34(FILE* f, const char* filename, Header& header);
}
