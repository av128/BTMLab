//
//  MathMethods.h
//  single_hysteresis_driven_brownian_motor
//
//  Created by Raman SHESHKA on 02/12/2020.
//

#pragma once

#include <fstream>
#include <sstream>
#include <string>

typedef double Real;


Real squarePeriodicSignal(Real t);
Real cosinusSignal(Real t);
Real sinusPotential(Real x);
Real derivativeSinusPotential(Real x);
Real derivativeSawToothPeriodicPotential(Real x);