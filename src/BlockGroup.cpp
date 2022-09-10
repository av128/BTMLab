//
//  Block.cpp
//  single_hysteresis_driven_brownian_motor
//
//  Created by Raman SHESHKA on 04/02/2021.
//
#include "BlockGroup.hpp"
#include "ConfigurationInitializer.hpp"
#include "MModelMethods.hpp"
#include "XYModelMethods.hpp"
#include "MathMethods.hpp"
#include "OutputBuffer.hpp"
#include "FND.hpp"

// constructor BlockGroup
BlockGroup::BlockGroup(const int blockNumber, const std::string model)
    : m_blockForward(), m_blockBackward(), m_blockNumber(blockNumber), m_timeStep(0), m_model(nullptr)
{
    m_blockForward.reserve(m_blockNumber);  // empty vector
    m_blockBackward.reserve(m_blockNumber); // empty vector
    // TODO add condition check if config is RL
    if (model == "y_ps")
        m_model = std::make_shared<MModelMethods>();
    else if (model == "xy")
        m_model = std::make_shared<XYModelMethods>();
    else
        std::runtime_error("Unknown Model <" + model + ">");
}

BlockGroup::~BlockGroup() {}

void BlockGroup::initializeBlockGroup()
{
    const Config &instance = Config::instance();
    if (instance.initialState == "IdentityArrayZeros")
    {
        for (int i = 0; i < m_blockNumber; i++)
        {
            m_blockForward.emplace_back(0., 0., 0., true);
            m_blockBackward.emplace_back(0., 0., 0., true);
        }
    }
    else if (instance.initialState == "UniformDistribution")
    {
        m_blockBackward.resize(m_blockNumber);
        for (int i = 0; i < m_blockNumber; i++)
        {
            // TODO: better write a small wrapper of rng
            std::random_device rd{};
            std::mt19937 gen{rd()};
            std::uniform_real_distribution<> uDis_0_1(0., 1.);
            m_blockForward.emplace_back(instance.spacePeriod * uDis_0_1(gen),
                                        uDis_0_1(gen) - instance.leftWellPosition, 0., true);
            m_blockBackward[i] = m_blockForward[i];
        }
    }
}
void BlockGroup::forwardBlockGroupStep(const double cargoStep)
{
    auto &instance = Config::instance();
    // SDEsolver sdeSolver;//instance of class object

    Real couplingFunctionalSum = 0.;
    // values near the mean are the most likely
    // standard deviation affects the dispersion of generated values from the mean
    static FND fnd(0, 1); // fast normal distribution
    Real zNoiseTerm = fnd();
    for (int i = 0; i < m_blockNumber; i++)
    {
        couplingFunctionalSum += m_model->coupling(
            m_blockForward[i].pos.x, m_blockForward[i].pos.y, m_blockForward[i].pos.z);
    }
    // couplingFunctionalSum *= -instance.zeta;
    // couplingFunctionalSum *= instance.zeta;
    // couplingFunctionalSum += cargoStep * m_blockNumber * instance.zeta;
    couplingFunctionalSum += cargoStep;
    // couplingFunctionalSum *= instance.dt;
    for (int i = 0; i < m_blockNumber; i++)
    {
        // m_blockForward[i].Z += couplingFunctionalSum;
        auto temporaryBlock = eulerStepParallel(m_blockForward[i], m_blockBackward[i],
                                                couplingFunctionalSum, zNoiseTerm, m_timeStep, i);
        m_blockBackward[i] = m_blockForward[i];
        m_blockForward[i] = temporaryBlock;
    }
    m_timeStep += 1;
    return;
}
void BlockGroup::outputBlockGroup(const int cargoId) const
{
    for (int i = 0; i < m_blockNumber; i++)
    {
        OutputBuffer::instance().insertBlock(cargoId, i, m_blockForward[i]);
        // outputFileTrajectory <<
        // m_timeStep<<";"<<i<<";"<<m_blockForward[i].X<<";"<<m_blockForward[i].Y<<";"<<m_blockForward[i].Z<<";"<<m_blockForward[i].S<<
        // std::endl;
    }
}
void BlockGroup::accumulateBlockGroup(std::vector<Block> &sumBlockGroup) const
{
    for (int i = 0; i < m_blockNumber; i++)
    {
        sumBlockGroup[i].pos += m_blockForward[i].pos;
    }
}

Block BlockGroup::eulerStepParallel(const Block &currentBlock, const Block &currentBackwardBlock,
                                    const double zzz, const double zNoiseTerm, Integer &time, const Integer index) const
{
    Config &instance = Config::instance();

    static FND fnd(0, 1); // fast normal distribution

    // values near the mean are the most likely
    // standard deviation affects the dispersion of generated values from the mean
    Block block; // instance of empty block
    // check the definition instance.alpha instance.beta instance.zeta
    block.pos.x =
        currentBlock.pos.x +
        m_model->xDerivativeGSoftDevice(currentBlock.pos.x, currentBlock.pos.y, currentBlock.pos.z,
                                        currentBackwardBlock.pos.x, currentBackwardBlock.pos.y, currentBackwardBlock.pos.z,
                                        currentBlock.S, time * instance.dt, instance.phaseSpace[index], instance.phaseTime[index]) *
            instance.dt +
        sqrt(2 * instance.xD * instance.alpha * instance.dt) * fnd();
    block.pos.y =
        currentBlock.pos.y +
        m_model->yDerivativeGSoftDevice(currentBlock.pos.x, currentBlock.pos.y, currentBlock.pos.z,
                                        currentBackwardBlock.pos.x, currentBackwardBlock.pos.y, currentBackwardBlock.pos.z,
                                        currentBackwardBlock.S, time * instance.dt, instance.phaseTime[index]) *
            instance.dt +
        sqrt(2 * instance.yD * instance.beta * instance.dt) * fnd();
    block.pos.z = currentBlock.pos.z + zzz * instance.zeta * instance.dt +
                  sqrt(2 * instance.zD * instance.zeta * instance.dt) * zNoiseTerm;
    return block;
}