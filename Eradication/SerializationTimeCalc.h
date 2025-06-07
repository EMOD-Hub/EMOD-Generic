
#pragma once
#include "Configure.h"
#include "SimulationEnums.h"
#include <deque>
#include <vector>

using namespace Kernel;

class SerializationTimeCalc : public JsonConfigurable
{
IMPLEMENT_DEFAULT_REFERENCE_COUNTING()
GET_SCHEMA_STATIC_WRAPPER(SerializationTimeCalc)
public:
    std::deque<int32_t> GetSerializedTimeSteps(int32_t steps, float start_time, float step_size);
    virtual bool Configure(const Configuration *config);
    
private:
    std::deque<int32_t> _ProcessConfig(std::vector<int32_t> &specified_timesteps, float start_time, float step_size, int32_t steps);

    SerializationType::Enum m_serialization_type;
    std::vector<int32_t> m_serialization_time_steps;
    std::vector<float> m_serialization_times;
};
