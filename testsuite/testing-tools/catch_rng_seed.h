/*
 * Copyright (c) 2022 Morwenn
 * SPDX-License-Identifier: MIT
 */
#ifndef CPPSORT_TESTSUITE_CATCH_RNG_SEED_H_
#define CPPSORT_TESTSUITE_CATCH_RNG_SEED_H_

namespace Catch
{
    // This functions is only available in an internal header that
    // drags a lot of dependencies, it's cheaper to just declare
    // it ourselves in this wrapper
    unsigned int rngSeed();
}

#endif // CPPSORT_TESTSUITE_CATCH_RNG_SEED_H_
