#pragma once

#include <random>
#include "Types.hpp"

constexpr Real pi_x2 = pi * 2.0;

class FND
{
public:
    FND(const Real mu, const Real sigma) : m_mu(mu), m_sigma(sigma), m_hasVal(false) {}

    Real operator()()
    {
        if (m_hasVal)
        {
            m_hasVal = false;
            return m_n1;
        }

        const auto u1 = Real(m_rng()) / Real(m_rng.max());
        const auto u2 = Real(m_rng()) / Real(m_rng.max());
        const auto mag = m_sigma * sqrt(-2.0 * log(u1));

        m_hasVal = true;
        m_n1 = mag * sin(pi_x2 * u2) + m_mu;
        return mag * cos(pi_x2 * u2) + m_mu;
    }

private:
    std::minstd_rand m_rng;
    const Real m_mu;
    const Real m_sigma;
    Real m_n1;
    bool m_hasVal;
};