//
//  ConfigurationInitializer.cpp
//  single_hysteresis_driven_brownian_motor
//
//  Created by Raman SHESHKA on 03/02/2021.
//

#include "ConfigParams.hpp"
#include "ConfigurationInitializer.hpp"
#include "MathMethods.hpp"
#include <iostream>
#include <random>
#include "CommonTools.hpp"

Config::Config()
{
    ConfigParams config("Config.json");
    std::cout << "Starting the parsing\n";

    config.print();

    // In the subObject sanity check is skipped, could be added later if necessary
    // Need to make sure all the sub-members are present, otherwise json will create a null
    // member for the name
    if (config.isModuleMember("model_characteristics"))
    {
        auto module = config.getModuleParams("model_characteristics");
        deviceDescription = module.getParam("deviceDescription").val;
        deviceConfiguration = module.getParam("deviceConfiguration").val;
        modelMemory = module.getParam("modelMemory").valAsBool();
        modelType = module.getParam("modelType").val;
    }

    if (config.isModuleMember("potentials_description"))
    {
        auto module = config.getModuleParams("potentials_description");

        spacePeriodicPotential = module.getParam("spacePeriodicPotential").val;
        doubleWellPotential = module.getParam("doubleWellPotential").val;

        if (spacePeriodicPotential == "SawToothPotential" || spacePeriodicPotential == "")
        {
            spacePeriodicPotentialFunction = derivativeSawToothPeriodicPotential;
        }
        else if (spacePeriodicPotential == "SinusPotential")
        {
            spacePeriodicPotentialFunction = derivativeSinusPotential;
        }
    }

    if (config.isModuleMember("interaction"))
    {
        auto module = config.getModuleParams("interaction");

        alpha = module.getParam("alpha").valAsDouble();
        beta = module.getParam("beta").valAsDouble();
        zeta = module.getParam("zeta").valAsDouble();
        interactionStiffness = module.getParam("interactionStiffness").valAsDouble();
    }

    if (config.isModuleMember("piece_wise_space_periodic_function"))
    {
        auto module = config.getModuleParams("piece_wise_space_periodic_function");

        toothPosition = module.getParam("toothPosition").valAsDouble();
        phiMax = module.getParam("phiMax").valAsDouble();
        spacePeriod = module.getParam("spacePeriod").valAsDouble();
    }

    if (config.isModuleMember("piece_wise_double_well_function"))
    {
        auto module = config.getModuleParams("piece_wise_double_well_function");

        leftWellPosition = module.getParam("leftWellPosition").valAsDouble();
        saddlePointPosition = module.getParam("saddlePointPosition").valAsDouble();
        rightWellPosition = module.getParam("rightWellPosition").valAsDouble();
        leftWellStiffness = module.getParam("leftWellStiffness").valAsDouble();
        rightWellStiffness = module.getParam("rightWellStiffness").valAsDouble();
    }
    if (config.isModuleMember("quartic_function"))
    {
        auto module = config.getModuleParams("quartic_function");

        h = module.getParam("h").valAsDouble();
        c = module.getParam("c").valAsDouble();
    }
    if (config.isModuleMember("external_load"))
    {
        auto module = config.getModuleParams("external_load");

        cargoMin = module.getParam("cargoMin").valAsDouble();
        cargoMax = module.getParam("cargoMax").valAsDouble();
        numberPoints = module.getParam("numberPoints").valAsInt();
    }
    if (config.isModuleMember("white_noise_properties"))
    {
        auto module = config.getModuleParams("white_noise_properties");

        xD = module.getParam("xD").valAsDouble();
        yD = module.getParam("yD").valAsDouble();
        zD = module.getParam("zD").valAsDouble();
    }

    if (config.isModuleMember("color_noise_properties"))
    {
        auto module = config.getModuleParams("color_noise_properties");

        colorNoiseFunctionName = module.getParam("colorNoiseFunctionName").val;
        characteristicTimeColorNoise =
            module.getParam("characteristicTimeColorNoise").valAsDouble();
        amplitudeColorNoise = module.getParam("amplitudeColorNoise").valAsDouble();

        if (colorNoiseFunctionName == "SquareWave")
        {
            colorNoiseFunction = squarePeriodicSignal;
        }
        else if (colorNoiseFunctionName == "CosinusWave")
        {
            colorNoiseFunction = cosinusSignal;
        }
    }

    if (config.isModuleMember("state"))
    {
        blockNumber = config.getModuleParams("state").getParam("blockNumber").valAsInt();
        phaseSpaceUse = config.getModuleParams("state").getParam("phaseSpaceUse").valAsBool();
        phaseTimeDistribution = config.getModuleParams("state").getParam("phaseTimeDistribution").val;
    }
    if (config.isModuleMember("numerics"))
    {
        auto module = config.getModuleParams("numerics");

        dt = module.getParam("dt").valAsDouble();
        dtSave = module.getParam("dtSave").valAsInt();
        numberRealizations = module.getParam("numberRealizations").valAsInt();
        numberPeriodsTotalDuration = module.getParam("numberPeriodsTotalDuration").valAsInt();
        numberPeriodsToRelax = module.getParam("numberPeriodsToRelax").valAsInt();
        singleTrajectoryNumberSave = module.getParam("singleTrajectoryNumberSave").valAsInt();
        initialState = module.getParam("initialState").val;
    }

    if (config.isModuleMember("output"))
    {
        auto module = config.getModuleParams("output");

        averageTrajectoriesFile = module.getParam("averageTrajectoriesFile").val;
        singleStochasticPath = module.getParam("singleStochasticPath").val;
        phaseBlockState = module.getParam("phaseBlockState").val;
        logsfile = module.getParam("logsfile").val;
    }

    if (modelType == "y_ps")
        initStatePhaseY();
    else if (modelType == "xy")
        initStatePhaseXY();
    else
        std::runtime_error("Unkown model <" + modelType + ">");
}

// void Config::initState(std::vector<Block> & blockForward, std::vector<Block> & blockBackward)
// const{
//  if(initialState == "IdentityArrayZeros"){
//    for(int i = 0; i < blockNumber; i++){
//        blockForward.emplace_back(0.,0., 0.,true);
//        blockBackward.emplace_back(0.,0.,0.,true);
//    }
//  }
//  else if(initialState == "UniformDistribution"){
//      blockBackward.resize(blockNumber);
//      for(int i = 0; i < blockNumber; i++){
//          std::random_device rd{};
//          std::mt19937 gen{rd()};
//          std::uniform_real_distribution<> uDis_0_1(0., 1.);
//          blockForward.emplace_back(i + uDis_0_1(gen), i + uDis_0_1(gen) - leftWellPosition,
//          0.,true); blockBackward[i] = blockForward[i];
//      }
//    }
//}

void Config::initStatePhaseY()
{
    phaseTime.resize(blockNumber);
    phaseSpace.resize(blockNumber);
    if (phaseTimeDistribution == "RegularDistribution")
    {
        phaseTime = linspace(0, 0.5 * characteristicTimeColorNoise, blockNumber);
        phaseSpace = linspace(0, spacePeriod, blockNumber);
    }
    else
    {
        for (int i = 0; i < blockNumber; i++)
        {
            std::random_device rd{};
            std::mt19937 gen{rd()};
            std::uniform_real_distribution<> uDis_0_tau(0., characteristicTimeColorNoise);
            std::uniform_real_distribution<> uDis_0_L(0., spacePeriod);
            phaseTime[i] = uDis_0_tau(gen);
            phaseSpace[i] = uDis_0_L(gen);
        }
    }
}

void Config::initStatePhaseXY()
{
    phaseTime.resize(blockNumber);
    phaseSpace.resize(blockNumber);
    if (phaseTimeDistribution == "RegularDistribution")
    {
        phaseTime = linspace(0, characteristicTimeColorNoise, blockNumber);
        if (phaseSpaceUse == false)
        {
            for (int i = 0; i < blockNumber; i++)
            {
                phaseSpace[i] = 0;
            }
        }
        else
        {
            phaseSpace = linspace(0, spacePeriod, blockNumber);
        }
    }
    else
    {
        for (int i = 0; i < blockNumber; i++)
        {
            std::random_device rd{};
            std::mt19937 gen{rd()};
            std::uniform_real_distribution<> uDis_0_tau(0., characteristicTimeColorNoise);
            phaseTime[i] = uDis_0_tau(gen);
            if (phaseSpaceUse == false)
            {
                phaseSpace[i] = 0;
            }
            else
            {
                std::uniform_real_distribution<> uDis_0_L(0., spacePeriod);
                phaseSpace[i] = uDis_0_L(gen);
            }
        }
    }
}