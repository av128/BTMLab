//
//  main.m
//  N_driven_brownian_motor_3deg
//
//  Created by Raman SHESHKA on 02/12/2020.
//
#include "BlockGroup.hpp"
#include "CommonTools.hpp"
#include "ConfigurationInitializer.hpp"
#include "OutputBuffer.hpp"
#include <chrono>
#include <cmath>
#include <ctime>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <memory>
#include <numeric>
#include <vector>


int main(int argc, const char* argv[])
{
    Config& instance = Config::instance();
    const std::vector<double>& cargoVector =
        linspace(instance.cargoMin, instance.cargoMax, instance.numberPoints);
    auto startTimeSimulation = std::chrono::steady_clock::now();
    std::time_t rightNowTime =
        std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());

    std::filesystem::path dataDir =
        std::filesystem::current_path() / "data";
    if (!(std::filesystem::exists(dataDir)))
    {
        std::cout << dataDir.generic_string() << " doesn't Exists" << std::endl;
        if (std::filesystem::create_directory(dataDir))
        {
            std::cout << "Create folder for " << dataDir.generic_string() << std::endl;
        }
    }
    std::filesystem::path cwd1 = dataDir / instance.averageTrajectoriesFile;
    std::filesystem::path cwd2 = dataDir / instance.singleStochasticPath;
    std::filesystem::path cwd3 = dataDir / instance.phaseBlockState;
    // std::ofstream outputGeneral(instance.file1);
    // std::ofstream outputSingleRealization(instance.file2);
    std::cout << "file1 : " << cwd1.generic_string() << std::endl;
    std::cout << "file2 : " << cwd2.generic_string() << std::endl;
    std::cout << "file3 : " << cwd3.generic_string() << std::endl;
    std::ofstream outputPhase(cwd3.generic_string(), std::ofstream::app);
    outputPhase << "block;"
                << "TimePhase;"
                << "SpacePhase" << std::endl;
    for (int l = 0; l < instance.phaseTime.size(); l++)
    {
        outputPhase << l << ";" << instance.phaseTime[l] << ";" << instance.phaseSpace[l]
                    << std::endl;
    }
    outputPhase.close();
    for (int k = 0; k < cargoVector.size(); k++)
    {
        std::vector<std::shared_ptr<BlockGroup>> blockGroupNrealizations;
        std::cout << "(cargo) f_ext = " << cargoVector[k] << std::endl;
        blockGroupNrealizations.reserve(instance.numberRealizations);
        for (int j = 0; j < instance.numberRealizations; j++)
        {
            blockGroupNrealizations.emplace_back(new BlockGroup(instance.blockNumber, instance.modelType));
        }

        // double time = 0.;
        int numberTimeSteps = static_cast<int>(instance.numberPeriodsTotalDuration *
                                               instance.characteristicTimeColorNoise / instance.dt);
        int numberRelaxationTimeSteps = static_cast<int>(
            instance.numberPeriodsToRelax * instance.characteristicTimeColorNoise / instance.dt);

        // std::ofstream outputSingleRealization(cwd2.generic_string(), std::ofstream::app);
        // outputSingleRealization << "cargo = " << cargoVector[k] << std::endl;
        // outputSingleRealization <<"step;"<<"block;" << "X;"<< "Y;"<< "Z;"<< "S" << std::endl;
        // initialization of first state
        for (int j = 0; j < instance.numberRealizations; j++)
        {
            blockGroupNrealizations[j]->initializeBlockGroup();
        }
        for (int i = 0; i < numberTimeSteps; i++)
        {
            // time += instance.dt;
            std::vector<Block> averageBlockGroup;
            averageBlockGroup.resize(instance.blockNumber);

            for (int j = 0; j < instance.numberRealizations; j++)
            {
                blockGroupNrealizations[j]->forwardBlockGroupStep(cargoVector[k]);
                if (i % instance.dtSave == 0)
                {
                    blockGroupNrealizations[j]->accumulateBlockGroup(averageBlockGroup);
                    if (j == instance.singleTrajectoryNumberSave)
                    {
                        blockGroupNrealizations[j]->outputBlockGroup(k);
                    }
                }
            }
            if (i % instance.dtSave == 0 && i > numberRelaxationTimeSteps)
            {
                for (int jj = 0; jj < instance.blockNumber; jj++)
                {
                    averageBlockGroup[jj].pos /= instance.numberRealizations;
                    OutputBuffer::instance().insertAverage(k, jj, averageBlockGroup[jj].pos.x,
                        averageBlockGroup[jj].pos.y, averageBlockGroup[jj].pos.z);
                }
            }
        }
    }
    OutputBuffer::instance().writeToFile(cwd2.generic_string());
    OutputBuffer::instance().writeAverageToFile(cwd1.generic_string());
    auto endTimeSimulation = std::chrono::steady_clock::now();
    std::chrono::duration<double> elapsedTimeSimulation = endTimeSimulation - startTimeSimulation;
    std::ofstream LOGS(instance.logsfile, std::ofstream::app);
    LOGS << "amplitudeColorNoise = " << instance.amplitudeColorNoise << ";" << std::endl;
    LOGS << "characteristicTimeColorNoise = " << instance.characteristicTimeColorNoise << ";"
         << std::endl;
    LOGS << "xD = " << instance.xD << ";" << std::endl;
    LOGS << "yD = " << instance.yD << ";" << std::endl;
    LOGS << "zD = " << instance.zD << ";" << std::endl;
    LOGS << "dt = " << instance.dt << ";" << std::endl;
    LOGS << "numberPeriods = " << instance.numberPeriodsTotalDuration << ";" << std::endl;
    LOGS << "START : " << rightNowTime << ";" << std::endl;
    LOGS << "duration = " << elapsedTimeSimulation.count() << "s;" << std::endl;
    LOGS.close();
    return 0;
}
//  (\_(\
//  (=' :')
//  (,(")(")
