//
//  SDEsolver.h
//  single_hysteresis_driven_brownian_motor
//
//  Created by Raman SHESHKA on 01/02/2021.
//

#pragma once

#include "Block.hpp"
#include <vector>

class SDEsolver
{
public:
    explicit SDEsolver();  // methode constructor
    Block eulerStepParallel(const Block& currentBlock, const Block& currentBackwardBlock,
        const double zzz, const double zNoiseTerm, Integer& time, const Integer index) const;
};  // unique place where i have to put ;