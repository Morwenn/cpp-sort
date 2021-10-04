/*
 * Copyright (c) 2015-2021 Morwenn
 * SPDX-License-Identifier: MIT
 */
#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>
#include <testing-tools/distributions.h>

thread_local std::mt19937_64 dist::distributions_prng(Catch::rngSeed());
