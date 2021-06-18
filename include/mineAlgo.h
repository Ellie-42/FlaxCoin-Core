#ifndef MINEALGO_H
#define MINEALGO_H

#include "block.h"
#include <memory>
#include <string>
#include <bitset>

struct miningData{
    std::string hashData;// = "12345679123456789123456789123456789123456789123456789";
    int nonce;
    std::string hashBits;
};

class mineAlgo
{
    public:
        /** Default constructor */
        mineAlgo(std::shared_ptr<block> blockData);
        mineAlgo();
        mineAlgo(const mineAlgo &cpy);

        void setCurBlock(std::shared_ptr<block> blockData)
        {
            this->blockData = blockData;
        }

        miningData mine();

        miningData hashGen(int nonce);

        std::shared_ptr<block> getBlock(){return blockData;};
        std::string getBlockTxData(){return blockTxData;}

        bool validateBlockTx();
        bool validateBlockCoinBaseTx();

        bool signTxIn();

        bool verifyTxOut();

        void setBlockData(miningData data);

        static uint8_t findBase58Val(char val);

        static const std::string round_01 [4];
        static const std::string round_02 [4];
        static const std::string round_03_1;
        static const std::string round_03_2;
        static const char base58List [58];
        static const int8_t numbList [58];

    private:
        std::shared_ptr<block> blockData;
        std::string blockTxData;
};

#endif // MINEALGO_H
