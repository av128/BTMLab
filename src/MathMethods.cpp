//
//  MathMethods.cpp
//  single_hysteresis_driven_brownian_motor
//
//  Created by Raman SHESHKA on 02/12/2020.
//

#include "CommonTools.hpp"
#include "ConfigurationInitializer.hpp"
#include "MathMethods.hpp"
#include "Types.hpp"
#include "math.h"
#include <stdio.h>
#include <cmath>
#include <random>
#include <vector>

Real squarePeriodicSignal(Real t)
{
    Config& instance = Config::instance();
    if ((t - static_cast<int>(floor(t / instance.characteristicTimeColorNoise)) *
                 instance.characteristicTimeColorNoise) <
        (0.5 * instance.characteristicTimeColorNoise))
    {
        return instance.amplitudeColorNoise;
    }
    else
    {
        return -instance.amplitudeColorNoise;
    }
}

Real cosinusSignal(Real t)
{
    Config& instance = Config::instance();
    return instance.amplitudeColorNoise * cos(2 * pi * t / instance.characteristicTimeColorNoise);
}

Real sinusPotential(Real x)
{
    Config& instance = Config::instance();
    return 0.5 * instance.phiMax * (sin(2.0 * pi / instance.spacePeriod * x) + 1.0);
}

Real derivativeSinusPotential(Real x)
{
    Config& instance = Config::instance();
    return instance.phiMax * pi / instance.spacePeriod * cos(2.0 * pi / instance.spacePeriod * x);
}

Real derivativeSawToothPeriodicPotential(Real x)
{
    Config& instance = Config::instance();
    int control;
    control = static_cast<int>(floor(x / instance.spacePeriod));
    if ((x - control * instance.spacePeriod) <
        (0.5 * instance.spacePeriod * (1 + instance.toothPosition)))
    {
        return 2 * instance.phiMax / (instance.spacePeriod * (1 + instance.toothPosition));
    }
    else
    {
        return -2 * instance.phiMax / (instance.spacePeriod * (1 - instance.toothPosition));
    }
}
