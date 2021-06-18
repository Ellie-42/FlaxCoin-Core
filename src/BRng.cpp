#include "BRng.h"
#include <botan/hash.h>
#include <botan/auto_rng.h>
#include <botan/rng.h>
#include <botan/base58.h>
#include <iostream>
#include <sstream>
#include <bitset>

#include "ISAAC64.h"


sphincs_plus::success_flag b_rng::randFuncWrap(unsigned char * target, size_t num_bytes)
{
    std::unique_ptr<Botan::RandomNumberGenerator> rng;
    rng.reset(new Botan::AutoSeeded_RNG);
    uint8_t entBytes[16];
    rng->randomize(entBytes,16);
    std::vector<uint8_t> entropy = Botan::base58_decode(Botan::base58_encode(entBytes,16));
    std::seed_seq seed(entropy.begin(),entropy.end());
    isaac64_engine rngEngine;
    rngEngine.seed(seed);
    std::string hex01;
    std::stringstream hexSS01;
    for(int i = 0; i < num_bytes; i++)
    {
        hexSS01<<std::hex<<rngEngine();
    }
    hex01 = hexSS01.str();
    memcpy(target, hex01.c_str(), num_bytes);
    return true;
}

sphincs_plus::random_return b_rng::randFunc(unsigned char * target, size_t num_bytes)
{
    randFuncWrap(target,num_bytes);
    return sphincs_plus::random_success;
}
