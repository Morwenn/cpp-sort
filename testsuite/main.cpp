/*
 * Copyright (c) 2015-2021 Morwenn
 * SPDX-License-Identifier: MIT
 */
#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>
#include <cpp-sort/detail/random.h>
#include <testing-tools/distributions.h>

thread_local cppsort::detail::rand_bit_generator<std::mt19937_64> dist::gen{
    std::mt19937_64(Catch::rngSeed())
};
