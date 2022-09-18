//
// Created by Raman SHESHKA on 26/12/2021.
//

#pragma once

#include "ConfigurationInitializer.hpp"
#include "MathMethods.hpp"

class IModelMethods
{
public:
    virtual Real quarticPotential(Real x, Real y) = 0; // pure virtual methods
    virtual Real derivativeQuarticPotential(Real x, Real y) = 0;
    virtual Real coupling(Real x, Real y, Real z) = 0;
    virtual Real derivativeDoubleWellPieceWisePotential(Real x, Real y) = 0;

    virtual Real xDerivativeGSoftDevice(
        Real x, Real y, Real z, Real x_, Real y_, Real z_, bool S, Real t, Real phaseSpace, Real phaseTime)
    {
        std::runtime_error("Function Not Implemented");
        return Real{};
    }

    virtual Real yDerivativeGSoftDevice(
        Real x, Real y, Real z, Real x_, Real y_, Real z_, bool S, Real t, Real phaseTime)
    {
        std::runtime_error("Function Not Implemented");
        return Real{};
    }

    explicit IModelMethods() : instance(Config::instance()) {}

protected:
    const Config &instance;
};