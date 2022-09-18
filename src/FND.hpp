#pragma once

#include "Types.hpp"
#include <random>
#include <iostream>
#include "omp.h"


constexpr Real pi_x2 = pi * 2.0;
constexpr Integer FND_SIZE = 1048576 * 64;
constexpr Integer FND_HALF_SIZE = FND_SIZE / 2;

class FND
{
public:
    FND(const Real mu, const Real sigma) : m_mu(mu), m_sigma(sigma), m_index{0}
    {
        m_nVals.resize(FND_SIZE);
        // static int call = 0;
        // std::cerr << "FND Instance: " << call++ << "\n";
    }

    void generate()
    {
        // static int call = 0;
        // std::cerr << "FND Generate Instance: " << call++ << "\n";

#pragma omp parallel for
        for (Integer i = 0; i < FND_HALF_SIZE; i++)
        {
            const auto u1 = Real(m_rng()) / Real(m_rng.max());
            const auto u2 = Real(m_rng()) / Real(m_rng.max());
            const auto mag = m_sigma * sqrt(-2.0 * log(u1));

            m_nVals[2 * i] = mag * cos(pi_x2 * u2) + m_mu;
            m_nVals[2 * i + 1] = mag * sin(pi_x2 * u2) + m_mu;
        }
    }

    Real operator()()
    {
        if (m_index >= FND_SIZE)
        {
            generate();
            m_index = 0;
        }

        auto val = m_nVals[m_index];
        m_index++;
        return val;
    }

private:
    std::minstd_rand m_rng;
    const Real m_mu;
    const Real m_sigma;
    std::vector<Real> m_nVals;
    Integer m_index;
};