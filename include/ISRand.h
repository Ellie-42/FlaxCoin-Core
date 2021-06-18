#ifndef ISRAND_H
#define ISRAND_H

#include <random>
#include <iostream>
#include <bitset>
#include <sstream>


#include "ISAAC64.h"
#include "api.h"


class i_rng : public sphincs_plus::random
{
public:
    i_rng(std::seed_seq * i_seed)
    {

        rngEngine.seed(*i_seed);
    }

    sphincs_plus::success_flag randFuncWrap(unsigned char * target, size_t num_bytes)
    {
        std::string hex01;

        std::stringstream hexSS01;
        hexSS01<<std::hex<<rngEngine();
        hexSS01<<std::hex<<rngEngine();
        hexSS01<<std::hex<<rngEngine();
        hexSS01<<std::hex<<rngEngine();
        hexSS01<<std::hex<<rngEngine();
        hexSS01<<std::hex<<rngEngine();
        hexSS01<<std::hex<<rngEngine();
        hexSS01<<std::hex<<rngEngine();
        hexSS01<<std::hex<<rngEngine();
        hex01 = hexSS01.str();
        memcpy(target, hex01.c_str(), num_bytes);
        return true;
    }

    sphincs_plus::random_return randFunc(unsigned char *target,size_t num_bytes)
    {
        randFuncWrap(target,num_bytes);
        return sphincs_plus::random_success;
    }

private:
    isaac64_engine rngEngine;

};







#endif // ISRAND_H
