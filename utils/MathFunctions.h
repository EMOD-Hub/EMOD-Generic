
#pragma once

#include <math.h>
#include "Exceptions.h"
#include "IdmApi.h"
#include "Types.h"

namespace Kernel {
    // calculate the great-circle distance between two points along the surface a spherical earth in kilometers
    double IDMAPI CalculateDistanceKm( double lon_1_deg, double lat_1_deg, double lon_2_deg, double lat_2_deg );

    float IDMAPI NTimeStepProbability( NonNegativeFloat PerTimeStepProbability, float dt);
}
