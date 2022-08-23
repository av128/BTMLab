//
//  ConfigurationInitializer.hpp
//  single_hysteresis_driven_brownian_motor
//
//  Created by Raman SHESHKA on 03/02/2021.
//

#pragma once

#include <stdio.h>
#include <functional>
#include <sstream>
#include <string>
#include <vector>
//#include "Block.hpp"

typedef double Real;
typedef int Integer;

class Config
{
public:
    // void initState(std::vector<Block> & blockForward, std::vector<Block> & blockBackward) const;
    static Config& instance()
    {
        static Config instance;
        return instance;
    }
    std::vector<Real> phaseTime;
    std::vector<Real> phaseSpace;
    std::function<Real(Real)> spacePeriodicPotentialFunction;
    std::function<Real(Real)> colorNoiseFunction;
    std::function<Real(Real, Real, Real, Real, bool, Real, Real)> xDerivativeGeneral;
    std::function<Real(Real, Real, Real, Real, Real, Real, bool, Real)> yDerivativeGeneral;
    std::function<Real(Real, Real)> derivativeDoubleWellPotential;
    Real alpha;
    Real beta;
    Real zeta;
    Real phiMax;
    Real spacePeriod;
    Real toothPosition;
    Real leftWellPosition;
    Real saddlePointPosition;
    Real rightWellPosition;
    Real leftWellStiffness;
    Real rightWellStiffness;
    Real h;
    Real c;
    Real interactionStiffness;
    Real cargoMin;
    Real cargoMax;
    Real xD;
    Real yD;
    Real zD;
    Real dt;
    Real characteristicTimeColorNoise;
    Real amplitudeColorNoise;
    Integer numberPeriodsTotalDuration;
    Integer numberRealizations;
    Integer dtSave;
    Integer blockNumber;
    Integer numberPeriodsToRelax;
    Integer singleTrajectoryNumberSave;
    Integer numberPoints;
    std::string deviceConfiguration;
    std::string deviceDescription;
    std::string EnergyFunctionProperty;
    std::string spacePeriodicPotential;
    std::string doubleWellPotential;
    std::string colorNoiseFunctionName;
    std::string modelType;
    std::string initialState;
    std::string averageTrajectoriesFile;
    std::string singleStochasticPath;
    std::string phaseBlockState;
    std::string logsfile;
    std::string phaseTimeDistribution;
    bool modelMemory;
    bool phaseSpaceUse;

protected:
    Config();
    void initStatePhaseY();
    void initStatePhaseXY();
};
