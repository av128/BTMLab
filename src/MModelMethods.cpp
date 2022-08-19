//
// Created by Raman SHESHKA on 26/12/2021.
//

#include "MModelMethods.hpp"
#include "math.h"

MModelMethods::MModelMethods() {}

MModelMethods::~MModelMethods() {}

Real MModelMethods::quarticPotential(Real x, Real y)
{
    return -0.25 * pow(instance.h, 4) * pow((y), 2) + 0.5 * pow(instance.c, 2) * pow((y), 4);
}
Real MModelMethods::derivativeQuarticPotential(Real x, Real y)
{
    return -0.5 * pow(instance.h, 4) * y + 2.0 * pow(instance.c, 2) * pow(y, 3);
}
Real MModelMethods::coupling(Real x, Real y, Real z)
{
    return instance.interactionStiffness * (x + y - z);
}
Real MModelMethods::derivativeDoubleWellPieceWisePotential(Real x, Real y)
{
    return (y < instance.saddlePointPosition ?
                instance.leftWellStiffness * (y - instance.leftWellPosition) :
                instance.rightWellStiffness * (y - instance.rightWellPosition));
}
