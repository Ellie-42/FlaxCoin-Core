#ifndef BLOCKCHAIN_H
#define BLOCKCHAIN_H

#include "block.h"
#include "mineAlgo.h"
#include "transactionData.h"
#include "txStructs.h"
#include <vector>

const int block_gen_interval = 60;
const int block_adj_interval = 5;
const int genNonce = 1231;
const std::string coin_name="FlaxCoin";
const std::string coin_symb="FLAX";
class blockChain
{
    public:

        blockChain();
        int addBlock(std::shared_ptr<block> newBlock); //made this an int to permit addition of error codes later

        std::vector<std::shared_ptr<block>> getChain(){return chain;}
        unsigned int getChainLength(){return chain.size();};

        bool validateBlock(std::shared_ptr<block> prevBlock, std::shared_ptr<block> curBlock);

        std::shared_ptr<block> getBlockAtIndex(unsigned int i){return chain.at(i);}

        tDat::transaction scanBlock(std::shared_ptr<block> checkBlock, std::string pubAddr);

        uint8_t diffAdjust();

    friend bool operator>(const blockChain& chain01, const blockChain& chain02);
    friend bool operator==(const blockChain& chain01, const blockChain& chain02);


    protected:
        std::vector<std::shared_ptr<block>> chain;

    private:
        long long unsigned int combined_diff;
        std::shared_ptr<block> genesisBlock;
        mineAlgo mining;
};

#endif // BLOCKCHAIN_H
