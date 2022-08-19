//
// Created by Raman SHESHKA on 27/02/2022.
//
#include "Block.hpp"
#include "ConfigurationInitializer.hpp"
#include <array>
#include <map>
#include <mutex>
#include <vector>

class OutputBuffer
{
public:
    static OutputBuffer& instance()
    {
        static OutputBuffer instance;
        return instance;
    }
    void insertBlock(const int cargoId, const int blockId, const Block block)
    {
        std::lock_guard<std::mutex> guard(mutex);
        positions[cargoId][blockId].push_back(block);
    }
    void insertAverage(const int cargoId, const int blockId, const float meanX, const float meanY,
        const float meanZ)
    {
        std::lock_guard<std::mutex> guard(mutexAverage);
        std::array<float, 3> averageVector = {meanX, meanY, meanZ};
        averagePositions[cargoId][blockId].push_back(averageVector);
    }
    void writeToFile(const std::string& filePath) const
    {
        std::ofstream outputStream(filePath);
        const int sizeOutputBuffer = positions[0].at(0).size();
        outputStream << "cargo;"
                     << "block;"
                     << "step;"
                     << "X;"
                     << "Y;"
                     << "Z;"
                     << "S" << std::endl;
        for (int i = 0; i < positions.size(); i++)
        {
            const std::map<int, std::vector<Block>>& mapContent = positions[i];
            for (int j = 0; j < mapContent.size(); j++)
            {
                const std::vector<Block>& blockContent = mapContent.at(j);
                for (int k = 0; k < sizeOutputBuffer; k++)
                {
                    outputStream << i << ";" << j << ";" << k << ";" << blockContent[k].pos.x << ";"
                                 << blockContent[k].pos.y << ";" << blockContent[k].pos.z << ";"
                                 << blockContent[k].S << std::endl;
                }
            }
        }
        outputStream.close();
    }
    void writeAverageToFile(const std::string& filePath) const
    {
        std::ofstream outputStream(filePath);
        const int sizeOutputBuffer = averagePositions[0].at(0).size();
        outputStream << "cargo;"
                     << "block;"
                     << "step;"
                     << "meanX;"
                     << "meanY;"
                     << "meanZ" << std::endl;
        for (int i = 0; i < averagePositions.size(); i++)
        {
            const std::map<int, std::vector<std::array<float, 3>>>& mapContent =
                averagePositions[i];
            for (int j = 0; j < mapContent.size(); j++)
            {
                const std::vector<std::array<float, 3>>& blockContent = mapContent.at(j);
                for (int k = 0; k < sizeOutputBuffer; k++)
                {
                    outputStream << i << ";" << j << ";" << k << ";" << blockContent[k][0] << ";"
                                 << blockContent[k][1] << ";" << blockContent[k][2] << std::endl;
                }
            }
        }
        outputStream.close();
    }

private:
    explicit OutputBuffer()
    {
        positions.resize(Config::instance().numberPoints);
        averagePositions.resize(Config::instance().numberPoints);
    }
    std::vector<std::map<int, std::vector<Block>>> positions;
    std::vector<std::map<int, std::vector<std::array<float, 3>>>> averagePositions;
    mutable std::mutex mutex;
    mutable std::mutex mutexAverage;
};
