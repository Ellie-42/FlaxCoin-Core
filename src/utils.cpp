#include <string>
#include <vector>
#include <algorithm>
#include <botan/hash.h>
#include <botan/auto_rng.h>
#include <botan/rng.h>
#include <iostream>
#include <sstream>
#include <bitset>

#include "utils.h"


/** \brief Convert the bip39 vector into a string
* \param The Bip39 vector list
* \return The stringifie Bip39 seed
*/
std::string stringBipVec(std::vector<std::string> bipVec)
{
    std::string retVal;
    for(std::string str : bipVec)
    {
        retVal += str + " ";
    }
    retVal.pop_back();
    return retVal;
}

/** \brief Generates a Bip39 phrase, incorporating the given password.
* \param The user defined password to secure the seed
* \return The Bip39 seed in vector format. Last word is the checksum
*/
std::vector<std::string> genBip39(std::string password)
{
    std::vector<std::string> phrase;
    std::unique_ptr<Botan::RandomNumberGenerator> rng;
    rng.reset(new Botan::AutoSeeded_RNG);
    uint8_t entBytes[2];
    for(int i = 0; i < 11; i++)
    {
        uint16_t testByte;
        while(true)
        {
            rng->randomize(entBytes, 2);
            testByte = static_cast<uint16_t>(entBytes[1]);
            testByte = testByte + (entBytes[0] << 3);
            if(testByte < 2048)
            {
                break;
            }
        }
        std::cout << "Word: " << US_EN_Wordlist[testByte] << " Index:" << testByte << std::endl;
        phrase.push_back(US_EN_Wordlist[testByte]);
    }
    std::string tempPhrase;
    for(std::string word : phrase)
    {
        tempPhrase += word;
        std::cout<<word<<" ";
    }
    std::cout<<std::endl;
    tempPhrase += password;
    std::cout<<tempPhrase<<std::endl;
    Botan::secure_vector<uint8_t> hashVal;
    std::unique_ptr<Botan::HashFunction> checksumHashFunc(Botan::HashFunction::create("Skein-512(64)"));
    checksumHashFunc->update(tempPhrase);
    hashVal = checksumHashFunc->final();
    std::vector<uint8_t> checksum = Botan::unlock(hashVal);
    uint16_t check_01 = static_cast<uint16_t>(checksum.at(5));
    uint16_t check_02 = static_cast<uint16_t>(checksum.at(7));
    check_01 = check_02 + (check_01 << 5);
    check_01 = check_01/4;
    std::cout<<check_01<<std::endl;
    phrase.push_back(US_EN_Wordlist[check_01]);
    return phrase;
}

/** \brief Verifies the provided Bip39 string utilizing the given password
* \param The Bip39 phrase in string format
* \param The user defined password
* \param Sets the length of the Bip39 seed to permit longer mnemonic seeds
* \return True if the seed is valid
*/

std::vector<std::string> vectorizeBip39(std::string bipString)
{
    std::vector<std::string> bipList;
    std::istringstream ss(bipString);
    std::string word;
    while(ss >> word)
    {
        bipList.push_back(word);
    }
    return bipList;
}

bool verifyBip39(std::string phrase, std::string password, unsigned int wordlength)
{
    std::string newPhrase;
    std::string curWord;
    std::istringstream ss(phrase);
    unsigned int index = 0;
    while(ss >> curWord)
    {
        if(index == wordlength-1)
        {
            std::unique_ptr<Botan::HashFunction> checksumHashFunc(Botan::HashFunction::create("Skein-512(64)"));
            Botan::secure_vector<uint8_t> checksum = checksumHashFunc->process(newPhrase + password);
            uint16_t check_01 = static_cast<uint16_t>(checksum.at(5));
            uint16_t check_02 = static_cast<uint16_t>(checksum.at(7));
            check_01 = check_02 + (check_01 << 5);
            check_01 = check_01/4;
            //std::cout<<"Value: " << check_01 << std::endl;
            if(curWord == US_EN_Wordlist[check_01])
            {
                return true;
            }
            else
            {
                return false;
            }
        }
        else
        {
            newPhrase += curWord;
        }
        index++;
    }
    return false;
}

/** \brief Creates entropy from the given Bip39 seed, and password
* \param The bip39 seed vector
* \param The password for the Bip39 seed
* \return Returns an empty entropy vector if the password does not match the given seed, otherwise returns a repeatable entropy value for the RNG
*/

std::vector<uint8_t> entropyGen(std::vector<std::string> bip39Phrase,std::string password)
{
    std::vector<uint8_t> ret;
    if(!verifyBip39(stringBipVec(bip39Phrase),password))
        {return ret;}
    std::string digest;
    for(std::string temp : bip39Phrase)
    {
        digest+=temp;
    }
    digest+=password;
    std::unique_ptr<Botan::HashFunction> entropyFunc(Botan::HashFunction::create("SHAKE-256"));
    return Botan::unlock(entropyFunc->process(digest));
}

uint8_t bitsetToUint8(std::bitset<64> bits)
{
    //std::cout<<"Beginning conversion."<<std::endl;
    uint8_t retVal = 0;
    retVal += bits[7];
    //std::cout<<"Cur bit: " << bits[7] << std::endl;
    //std::cout<<std::bitset<8>{retVal}<<std::endl;
    //std::cout<<"Round: "<< 7 <<std::endl;
    for(int i = 6; i >= 0; i--)
    {
        retVal = retVal << 1;
        retVal += bits[i];
        //std::cout<<"Cur bit: " << bits[i] << std::endl;
        //std::cout<<std::bitset<8>{retVal}<<std::endl;
        //std::cout<<"Round: "<<i<<std::endl;

    }
    return retVal;
}
