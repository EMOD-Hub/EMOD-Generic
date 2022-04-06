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

    class ClimateKoppen : public Climate
    {

    public:
        enum KoppenType { Af, Am, As, Aw,
                          Bsh, Bsk, Bwh, Bwk,
                          Cfa, Cfb, Cfc, Csa, Csb, Csc, Cwa, Cwb, Cwc,
                          Dfa, Dfb, Dfc, Dfd, Dsa, Dsb, Dsc, Dwa, Dwb, Dwc, Dwd,
                          EF, ET };

    protected:
        KoppenType koppen_type;
        float t_average;
        float t_range;
        bool in_southern_hemisphere;

        static bool rainfall_flip[];
        static int rainfall_ranges[][4];
        static float rainfall_base[][4];

        static bool humidity_flip[];
        static int humidity_ranges[][4];
        static float humidity_base[][4];
        static float humidity_variance[][4];
        int humidity_index;

        static const float temperature_variance;
        static const float lapse_rate;

    public:
        static ClimateKoppen * CreateClimate( ClimateUpdateResolution::Enum update_resolution,
                                              INodeContext * _parent,
                                              int climate_type,
                                              float altitude,
                                              float latitude,
                                              float start_time,
                                              RANDOMBASE* pRNG );

        virtual void UpdateWeather( float, float, RANDOMBASE* pRNG ) override;

    protected:
        ClimateKoppen() { }
        ClimateKoppen(ClimateUpdateResolution::Enum update_resolution, INodeContext * _parent, int climate_type, float altitude, float latitude);

        virtual bool IsPlausible();

        virtual void AddStochasticity( RANDOMBASE* pRNG );

    private:
        void RoomUpDown(const float Ta, const float Tr, float &RoomUp, float &RoomDown);
    };
}
