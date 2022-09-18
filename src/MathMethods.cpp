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
    static Real invCharacteristicTimeColorNoise = 1.0 / instance.characteristicTimeColorNoise;

    if ((t - floor(t * invCharacteristicTimeColorNoise) * instance.characteristicTimeColorNoise) <
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
    static Real invSpacePeriod = 1.0 / instance.spacePeriod;
    static Real res =
        2.0 * instance.phiMax / (instance.spacePeriod * (1.0 + instance.toothPosition));

    if ((x - floor(x * invSpacePeriod) * instance.spacePeriod) <
        (0.5 * instance.spacePeriod * (1.0 + instance.toothPosition)))
    {
        return res;
    }
    else
    {
        return -res;
    }
}
