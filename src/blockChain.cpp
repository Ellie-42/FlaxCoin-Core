#include "blockChain.h"
#include "mineAlgo.h"
#include <memory>
#include <botan/hash.h>
#include <botan/base58.h>
#include <botan/secmem.h>
#include <iostream>
#include <math.h>
#include "utils.h"
#include "txStructs.h"

/** \brief Initializes the BlockChain
* details This function sets up the the block chain and generates the genesis block.
*/
blockChain::blockChain()
{
    genesisBlock = std::shared_ptr<block>(new block(0,"933ec8bc34e21924fa75f560edc22df1d1c",0,std::vector<std::string>(),0));
    mining = mineAlgo(genesisBlock);
    std::cout<<"Test1"<<std::endl;
    mining.setBlockData(mining.hashGen(genNonce));
    std::cout<<"Test2"<<std::endl;
    this->chain.push_back(genesisBlock);
}

/** \brief Uses the last block on the chain to validate the newest block onto the chain. Returns if the block is valid or not.
 *
 * \param std::shared_ptr<block> prevBlock This is the last block in the chain to validate the next block
 * \param curBlock std::shared_ptr<block> This is the block that is being validated.
 * \return bool
 *
 */
bool blockChain::validateBlock(std::shared_ptr<block> prevBlock, std::shared_ptr<block> curBlock)
{
    if(curBlock->getIndex() == 0)
    {
        return true; //I know I need to compare against internals...
        std::cout<<"Its the initial block!"<<std::endl;
    }

    if(curBlock->getPrevHash() != prevBlock->getCurHash()){std::cout<<"Validation failed at hash check."<<std::endl;return false;}

    std::string r1 = mineAlgo::round_01[mineAlgo::numbList[std::to_string(curBlock->getIndex()).back()]];
    std::string r2 = mineAlgo::round_02[mineAlgo::numbList[mineAlgo::findBase58Val(curBlock->getPrevHash().back())]];
    std::string r3_1 = mineAlgo::round_03_1;
    std::string r3_2 = mineAlgo::round_03_2;

    std::unique_ptr<Botan::HashFunction> hash_01(Botan::HashFunction::create(r1));
    std::unique_ptr<Botan::HashFunction> hash_02(Botan::HashFunction::create(r2));//
    std::unique_ptr<Botan::HashFunction> hash_03_1(Botan::HashFunction::create(r3_1));
    std::unique_ptr<Botan::HashFunction> hash_03_2(Botan::HashFunction::create(r3_2));

    std::string completeBlockData = curBlock->digest(curBlock->nonce );
    Botan::secure_vector<uint8_t> hashData01;

    hash_01->update(completeBlockData);
    hash_01->final(hashData01);
    std::vector<uint8_t> tempDigest_1 = Botan::unlock(hashData01);

    hash_03_2->update(hashData01);
    hash_03_2->final(hashData01);
    std::vector<uint8_t> tempDigest_2 = Botan::unlock(hashData01);

    hash_03_1->update(hashData01);
    hash_03_1->final(hashData01);

    miningData retVal;

    std::vector<uint8_t> hashDigest = Botan::unlock(hashData01);

    tempDigest_2.insert(tempDigest_2.end(),tempDigest_1.begin(),tempDigest_1.end());

    hash_02->update(tempDigest_2);
    hash_02->update(completeBlockData);
    hash_02->final(hashData01);

    tempDigest_1 = Botan::unlock(hashData01);

    for(int i = 1 ; i < 15; i++)
    {
        hashDigest.insert(hashDigest.begin()+i+1,tempDigest_1.at(i));
    }

    std::string checkDat = Botan::base58_encode(hashDigest);
    if(checkDat == curBlock->getCurHash())
    {
        return true;
    }
    return false;
}

/** \brief Helper function to get the difficulty adjustment, returning the new amount for the next block
 * Adjusts the difficulty for the next block. Should be run in to generate the new block difficulty.
 * \return int8_t
 *
 */
uint8_t blockChain::diffAdjust()
{
    std::shared_ptr<block> lastBlock = chain.back();
    std::shared_ptr<block> oldBlock = chain.at(chain.size() - 6);
    if(chain.size() % 10 != 0){return lastBlock->getDiff();}

    if((lastBlock->getTimestamp() - oldBlock->getTimestamp()) > block_gen_interval * block_adj_interval)
    {
        return lastBlock->getDiff() + 1;
    }
    else if((lastBlock->getTimestamp() - oldBlock->getTimestamp()) < block_gen_interval * block_adj_interval)
    {
        return lastBlock->getDiff() - 1;
    }
    else
    {
        return lastBlock->getDiff();
    }
}

/** \brief Adds a block to the chain, as long as it validates.
 *
 * \param newBlock std::shared_ptr<block> The block that is being added to the chain
 * \return int Returns a value indicating if the block addition was valid. 0 means it passed. -1 is the only current error code. This is left open to permit additional error codes to aid debugging solutions
 *
 */
int blockChain::addBlock(std::shared_ptr<block> newBlock)
{
    this->combined_diff += pow(2, newBlock->getDiff());
    std::shared_ptr<block> lastBlock = chain.at(newBlock->getIndex()-1);
    std::cout<<"\n\nCurrent block data: " << *newBlock;
    std::cout<<"\n\nValidating...\nPrevious block data: \n" << *lastBlock;
    if(validateBlock(lastBlock,newBlock))
    {
        chain.push_back(newBlock);
        return 0;
    }
    else
    {
        return -1;
    }
}


/** \brief Compares the difficulty in an easy function to aid the highest difficulty chain
 *
 * \param chain01 const blockChain&
 * \param chain02 const blockChain&
 * \return bool
 *
 */
bool operator>(const blockChain& chain01, const blockChain& chain02)
{
    if(chain01.combined_diff > chain02.combined_diff)
    {
        return true;
    }
    return false;
}


/** \brief Check if the chains are of equal difficulty and equal length, and the last block on the chain to see if they are the same by checking the hash.
 *
 * \param chain01 const blockChain&
 * \param chain02 const blockChain&
 * \return bool
 *
 */
bool operator==(const blockChain& chain01, const blockChain& chain02)
{
    if(chain01.combined_diff == chain02.combined_diff && chain01.chain.size() == chain02.chain.size() && chain01.chain.back()->getCurHash() == chain02.chain.back()->getCurHash())
    {
        return true;
    }
    return false;
}

/** \brief Scan each block as it is loaded in to scan for tx data for corresponding public addresses
* \param The shared pointer to the current block that is being checked
* \param The public address to search for
* \return a transactions object. If the tx did not have a key, the isEmpty flag of the struct will be set to true.
*/
tDat::transaction blockChain::scanBlock(std::shared_ptr<block> checkBlock, std::string pubAddr)
{
    tDat::transaction tData;
    tData.isEmpty = true;
    for(std::string dat : checkBlock ->getData())
    {

    }
    return tData;

}

