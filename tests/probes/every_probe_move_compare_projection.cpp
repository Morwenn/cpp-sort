/*
 * Copyright (c) 2020-2026 Morwenn
 * SPDX-License-Identifier: MIT
 */
#include <iterator>
#include <vector>
#include <catch2/catch_template_test_macros.hpp>
#include <cpp-sort/probes.h>
#include <testing-tools/distributions.h>
#include <testing-tools/functional_checks.h>

TEMPLATE_TEST_CASE( "every probe with comparison function altered by move", "[probe]",
                    decltype(cppsort::probe::amp),
                    decltype(cppsort::probe::block),
                    decltype(cppsort::probe::dis),
                    decltype(cppsort::probe::enc),
                    decltype(cppsort::probe::exc),
                    decltype(cppsort::probe::ham),
                    decltype(cppsort::probe::inv),
                    decltype(cppsort::probe::max),
                    decltype(cppsort::probe::mono),
                    decltype(cppsort::probe::osc),
                    decltype(cppsort::probe::rem),
                    decltype(cppsort::probe::reve),
                    decltype(cppsort::probe::runs),
                    decltype(cppsort::probe::spear),
                    decltype(cppsort::probe::sus) )
{
    std::vector<int> collection; collection.reserve(491);
    auto distribution = dist::shuffled{};
    distribution(std::back_inserter(collection), 491, -125);

    using probe = TestType;
    auto disorder = probe{}(collection, move_check_compare{});
    CHECK( disorder >= 0 );
}

TEMPLATE_TEST_CASE( "every probe with projection function altered by move", "[probe]",
                    decltype(cppsort::probe::amp),
                    decltype(cppsort::probe::block),
                    decltype(cppsort::probe::dis),
                    decltype(cppsort::probe::enc),
                    decltype(cppsort::probe::exc),
                    decltype(cppsort::probe::ham),
                    decltype(cppsort::probe::inv),
                    decltype(cppsort::probe::max),
                    decltype(cppsort::probe::mono),
                    decltype(cppsort::probe::osc),
                    decltype(cppsort::probe::rem),
                    decltype(cppsort::probe::reve),
                    decltype(cppsort::probe::runs),
                    decltype(cppsort::probe::spear),
                    decltype(cppsort::probe::sus) )
{
    std::vector<int> collection; collection.reserve(491);
    auto distribution = dist::shuffled{};
    distribution(std::back_inserter(collection), 491, -125);

    using probe = TestType;
    auto disorder = probe{}(collection, move_check_projection{});
    CHECK( disorder >= 0 );
}
