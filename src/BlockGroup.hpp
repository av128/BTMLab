#pragma once

#include "Block.hpp"

class BlockGroup
{
public:
    explicit BlockGroup(const Integer blockNumber, const std::string model);  // constructor
    ~BlockGroup();  // destructor '~' by convention, usually done automatically. We have to
                    // declare it.
    void initializeBlockGroup();
    void forwardBlockGroupStep(const double cargoStep);
    void outputBlockGroup(const int cargoId) const;  // can't change the class variables
    void accumulateBlockGroup(std::vector<Block>& sumBlockGroup) const;

    Block eulerStepParallel(const Block& currentBlock, const Block& currentBackwardBlock,
        const double zzz, const double zNoiseTerm, Integer& time, const Integer index) const;

private:
    std::vector<Block> m_blockForward;
    std::vector<Block> m_blockBackward;
    Integer m_blockNumber;
    Integer m_timeStep;
    std::shared_ptr<IModelMethods> m_model;
};