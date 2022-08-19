//
// Created by Raman SHESHKA on 26/12/2021.
//

#pragma once

#include "IModelMethods.hpp"

class XYModelMethods: public IModelMethods
{
public:
    virtual Real quarticPotential(Real x, Real y) override;
    virtual Real derivativeQuarticPotential(Real x, Real y) override;
    virtual Real coupling(Real x, Real y, Real z) override;
    virtual Real derivativeDoubleWellPieceWisePotential(Real x, Real y) override;

    virtual Real xDerivativeGSoftDevice(
        Real x, Real y, Real z, Real x_, Real y_, Real z_, bool S, Real t, Real phaseSpace, Real phaseTime) override
    {
        return -instance.spacePeriodicPotentialFunction(x + phaseSpace) + derivativeDoubleWellPieceWisePotential(x,y) - instance.colorNoiseFunction(t + phaseTime);
    }

    virtual Real yDerivativeGSoftDevice(
        Real x, Real y, Real z, Real x_, Real y_, Real z_, bool S, Real t, Real phaseTime) override
    {
        return -derivativeDoubleWellPieceWisePotential(x, y) - coupling(x, y, z) + instance.colorNoiseFunction(t + phaseTime);
    }

    explicit XYModelMethods();
    ~XYModelMethods();
};
