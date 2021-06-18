#include "wallet.h"
#include "api.h"
#include "ISRand.h"
#include "utils.h"

#include <string>
#include <random>
#include <iostream>
#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include <memory>

bool wallet::loadWalletFromSeed(std::string phrase, std::string password)
{
    if(verifyBip39(phrase,password))
    {
        std::vector<uint8_t> entropy = entropyGen(vectorizeBip39(phrase),password);
        for(int ent : entropy)
        {
            std::cout<<std::hex<<ent;
        }
        std::cout<<std::endl;
        std::seed_seq seed(entropy.begin(),entropy.end());
        std::shared_ptr<i_rng> rng(new i_rng(&seed));
        for(int i = 0; i < defaultKeyAmnt; i++)
        {
            std::shared_ptr<sphincs_plus::key_shake256_256f_simple> nKey(new sphincs_plus::key_shake256_256f_simple());
            if(nKey->generate_key_pair(rng) == false)
            {
                std::cout<<"Keygen failed."<<std::endl;
            }

            txToAddr keyToDat;
            keyToDat.convertFromFullKey(nKey);

            if(DEBUG) //only for debug purposes to ensure repeatability and such
            {
                std::cout<<"Private key: " << keyToDat._privateKey << std::endl;
                std::cout<<"Public key: " << keyToDat._publicAddr << std::endl;
            }

            this->walletData.push_back(keyToDat);
        }
        return true;
    }
    return false;
}

bool wallet::loadWalletFromFile(std::string filePath, std::string password)
{
    std::string seed; // this variable will get loaded to n a bit
    return false;
}

std::shared_ptr<sphincs_plus::key_shake256_256f_simple> loadPrivateKey(std::string privateKey)
{
    std::shared_ptr<sphincs_plus::key_shake256_256f_simple> loadKey;
    loadKey->set_private_key(reinterpret_cast<unsigned char *>(const_cast<char*>(privateKey.data())));
    return loadKey;
}

std::string wallet::generateWallet(std::string password)
{
    std::vector<std::string> mnemonic = genBip39(password);
    std::string mnemonicString = stringBipVec(mnemonic);
    if(loadWalletFromSeed(mnemonicString, password))
    {
        return mnemonicString;
    }
    return "";
}

