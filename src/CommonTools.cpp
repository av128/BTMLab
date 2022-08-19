//
// Created by Raman SHESHKA on 26/12/2021.
//
#include <cmath>
#include <vector>
// linspace function generates N points between min and max and return as vector.
std::vector<double> linspace(double min, double max, int n)
{
    std::vector<double> result;
    // vector iterator
    int iterator = 0;
    for (int i = 0; i <= n - 2; i++)
    {
        double temp = min + i * (max - min) / (floor((double)n) - 1);
        result.insert(result.begin() + iterator, temp);
        iterator += 1;
    }
    // iterator += 1;
    // TODO: avoid insertion in vector
    result.insert(result.begin() + iterator, max);
    return result;
}

double hBranch(double arg, double arg1, double arg2)
{
    return 0.5 * (1 - tanh((arg - arg1) / arg2));
}

double derivativeHbranch(double arg, double arg1, double arg2)
{
    return -0.5 / arg2 *
           (1 - tanh((arg - arg1) / arg2) * tanh((arg - arg1) / arg2));  // sech^2(x) = 1 -
                                                                         // tanh^2(x)
}