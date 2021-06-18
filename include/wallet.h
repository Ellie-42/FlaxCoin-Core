#ifndef WALLET_H
#define WALLET_H

#include <botan/secmem.h>
#include "txStructs.h"
#include <random>
#include "ISAAC64.h"
#include "api.h"
#include <vector>
#include <memory>
#include "utils.h"
#include <botan/base58.h>
#include "transactionData.h"

/**< A variable to enable outputs in debug mode */
const bool DEBUG = true;

/**< The default amount of keys a wallet should generate for a new wallet */
const int defaultKeyAmnt = 16;

struct transactGen
{
    std::shared_ptr<transData::transaction> output;
    bool success = false;
};

struct txToAddr
{
    std::vector<tDat::unspentTxOut> _uTxOuts;
    std::string _privateKey;
    std::string _publicAddr;

    void makePubAddr(uint8_t * pubKey, size_t length)
    {
        std::vector<uint8_t> publicKey = Botan::base58_decode(Botan::base58_encode(pubKey, length));
        publicKey.insert(publicKey.begin(), 42); //addrprefix_1
        publicKey.insert(publicKey.begin()+1, 20); //addrprefix_2
        publicKey.insert(publicKey.begin()+2, 21); //addrprefix_3
        _publicAddr = Botan::base58_encode(publicKey); //encode the address
    }

    void setPrivAddr(uint8_t * pubKey, size_t length)
    {
          _privateKey = Botan::base58_encode(pubKey, length);
    }

    bool convertFromFullKey(std::shared_ptr<sphincs_plus::key_shake256_256f_simple> key)
    {
        if(key->get_private_key() == 0 ){return false;}
        setPrivAddr(const_cast<unsigned char *>(key->get_private_key()), key->len_private_key());
        makePubAddr(const_cast<unsigned char *>(key->get_public_key()), key->len_public_key());
        return true;
    }

};

class wallet
{
    public:

        /**\brief Generates a wallet, securing with the given password
        *\param The password. Strength checks should be implemented at the front end.
        *\return The mnemonic seed that is secured with the given password
        */
        std::string generateWallet(std::string password = ""); //takes in a password (not required, but recommended) and returns a mnemonic seed

        /**\brief Loads a wallet from the given seed and password. Will have it load TX data in a later version
        *\param The mnemonic seed used to generate the keys
        *\param The password used to secure the seed. Without the right password, the seed will not verify, and certainly will not generate the correct keys
        *\return Returns true if the load was successful.
        */
        bool loadWalletFromSeed(std::string phrase, std::string password = ""); //loads a wallet from mnemonic seed, and use the applicable password, returns true if successful

        /**\brief Loads a seed and tx data from the specified file, decrypting witht he given password.
        *\param The wallet file
        *\param The file's password
        *\return Returns true if the wallet loading was successful.
        */
        bool loadWalletFromFile(std::string filePath, std::string password = ""); //loads wallet from file, returns true if successful

        /**\brief Save the seed and corresponding tx data to the specified file. Really should encrypt these but given there are people that don't want to implement security, the option is there. But dont use that option. This should be ran soon after the wallet is made, as wallets do not persist a local copy of the seed.
        *\param The file to encrypt the seed to
        *\param The mnemonic seed
        *\param The corresponding password
        *\return Returns true if the save was successful
        */
        bool saveWalletToFile(std::string filePath,std::string seed,std::string password="");

        /**\brief Returns the key at the given index in the wallet
        *\param The index, it should be below 16 for a default wallet.
        *\return A shared_ptr to the key
        */

        transactGen generateTransaction(float send);

        bool scanPubAddr(std::string pAddr);

        txToAddr getKeyAtIndex(int i){return walletData.at(i);}

    private:
        /**< Unsure, look to future version. I think it will be useful. Unsure at the moment.*/
        std::shared_ptr<sphincs_plus::key_shake256_256f_simple>  loadPrivateKey(std::string privateKey); //loads a private key and returns it

        /**< Generates a new kew. Not implemented yet, unsure if it will be useful. */
        std::shared_ptr<sphincs_plus::key_shake256_256f_simple>  generateKeyPair(); //generates a new key

        /**< The actual wallet and keys */
        std::vector<txToAddr> walletData; //the keys of the wallet

        /**< Specifies if the wallet only contains keys with valid public keys */
        bool isWatchOnly; //defines if the keys in the wallet are watch-only
};

#endif // WALLET_H
