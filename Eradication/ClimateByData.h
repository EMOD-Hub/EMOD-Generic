/***************************************************************************************************

Copyright (c) 2018 Intellectual Ventures Property Holdings, LLC (IVPH) All rights reserved.

EMOD is licensed under the Creative Commons Attribution-Noncommercial-ShareAlike 4.0 License.
To view a copy of this license, visit https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode

***************************************************************************************************/

#pragma once

#include <fstream>
#include <iostream>
#include <math.h>
#include <vector>

#include "Climate.h"

namespace Kernel
{
    class RANDOMBASE;

    class ClimateByData : public Climate
    {
    protected:
        int num_datapoints;
        int num_years;

        // vectors containing the entire timeline of data
        std::vector<float> airtemperature_data;
        std::vector<float> landtemperature_data;
        std::vector<float> rainfall_data;
        std::vector<float> humidity_data;

    public:
        static ClimateByData * CreateClimate( ClimateUpdateResolution::Enum update_resolution,
                                              INodeContext * _parent,
                                              int datapoints,
                                              std::ifstream& airtemperature_file,
                                              std::ifstream& landtemperature_file,
                                              std::ifstream& rainfall_file,
                                              std::ifstream& humidity_file,
                                              float start_time,
                                              RANDOMBASE* pRNG );

        virtual void UpdateWeather( float, float, RANDOMBASE* pRNG ) override;
        virtual ~ClimateByData();

    private:
        ClimateByData();
        ClimateByData(ClimateUpdateResolution::Enum update_resolution, INodeContext * _parent);

        virtual bool IsPlausible();

        void ReadDataFromFiles(int datapoints, std::ifstream& airtemperature_file, std::ifstream& landtemperature_file, std::ifstream& rainfall_file, std::ifstream& humidity_file);
    };
}
