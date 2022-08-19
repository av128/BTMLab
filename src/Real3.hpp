#pragma once

#include "Types.hpp"

struct Real3
{
    Real x, y, z;

    void operator+=(const Real3& v)
    {
        x += v.x;
        y += v.y;
        z += v.z;
    }

    void operator/=(const Real& v)
    {
        x /= v;
        y /= v;
        z /= v;
    }
};