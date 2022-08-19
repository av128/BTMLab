//
// Created by Raman SHESHKA on 26/12/2021.
//

#include "XYModelMethods.hpp"
#include "math.h"

XYModelMethods::XYModelMethods() {}

XYModelMethods::~XYModelMethods() {}

Real XYModelMethods::quarticPotential(Real x, Real y)
{
    return -0.25 * pow(instance.h, 4) * pow((y -x), 2) + 0.5 * pow(instance.c, 2) * pow((y-x), 4);
}
Real XYModelMethods::derivativeQuarticPotential(Real x, Real y)
{
    return -0.5 * pow(instance.h, 4) * (y-x) + 2.0 * pow(instance.c, 2) * pow((y-x), 3);
}
Real XYModelMethods::coupling(Real x, Real y, Real z)
{
    return instance.interactionStiffness * (y - z);
}
Real XYModelMethods::derivativeDoubleWellPieceWisePotential(Real x, Real y)
{
    return ((y - x) < instance.saddlePointPosition ?
                      instance.leftWellStiffness * ((y - x) - instance.leftWellPosition) :
                      instance.rightWellStiffness * ((y - x) - instance.rightWellPosition));
}
