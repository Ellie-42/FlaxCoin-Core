#ifndef B_RNG_H
#define B_RNG_H

#include "api.h"
#include <botan/rng.h>
#include <botan/auto_rng.h>

class b_rng : public sphincs_plus::random
{
    public:
        sphincs_plus::success_flag randFuncWrap(unsigned char * target, size_t num_bytes);
        sphincs_plus::random_return randFunc(unsigned char *target,size_t num_bytes);


    protected:

    private:
};

#endif // B_RNG_H
