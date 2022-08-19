//
//  Block.hpp
//  single_hysteresis_driven_brownian_motor
//
//  Created by Raman SHESHKA on 04/02/2021.
//

#pragma once

#include "IModelMethods.hpp"
#include "Types.hpp"
#include "Real3.hpp"
#include <stdio.h>
#include <fstream>
#include <memory>
#include <vector>

class Block
{
public:
    Block(const Real inX, const Real inY, const Real inZ, const bool inS)
      : pos{inX, inY, inZ}, S{inS}
    {}

    Block() : pos{}, S{false} {}

    Real3 pos;
    bool S;
};
