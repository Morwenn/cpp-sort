/*
 * Copyright (c) 2021 Morwenn
 * SPDX-License-Identifier: MIT
 */
#include <catch2/catch.hpp>
#include "random.h"

namespace hasard
{
    auto engine()
        -> xoshiro256ss&
    {
        thread_local xoshiro256ss res(Catch::rngSeed());
        return res;
    }

    auto bit_gen()
        -> rand_bit_generator<xoshiro256ss>&
    {
        thread_local rand_bit_generator<xoshiro256ss> res{
            xoshiro256ss(Catch::rngSeed())
        };
        return res;
    }
}
