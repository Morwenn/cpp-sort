/*
 * Copyright (c) 2020-2021 Morwenn
 * SPDX-License-Identifier: MIT
 */
#include <iterator>
#include <vector>
#include <catch2/catch.hpp>
#include <cpp-sort/probes.h>
#include <testing-tools/distributions.h>
#include <testing-tools/functional_checks.h>

TEMPLATE_TEST_CASE( "every probe with comparison function altered by move", "[probe]",
                    decltype(cppsort::probe::dis),
                    decltype(cppsort::probe::enc),
                    decltype(cppsort::probe::exc),
                    decltype(cppsort::probe::ham),
                    decltype(cppsort::probe::inv),
                    decltype(cppsort::probe::max),
                    decltype(cppsort::probe::mono),
                    decltype(cppsort::probe::osc),
                    decltype(cppsort::probe::par),
                    decltype(cppsort::probe::rem),
                    decltype(cppsort::probe::runs),
                    decltype(cppsort::probe::sus) )
{
    std::vector<int> collection; collection.reserve(491);
    auto distribution = dist::shuffled{};
    distribution(std::back_inserter(collection), 491, -125);

    using probe = TestType;
    auto mop = probe{}(collection, move_check_compare{});
    CHECK( mop >= 0 );
}

TEMPLATE_TEST_CASE( "every probe with projection function altered by move", "[probe]",
                    decltype(cppsort::probe::dis),
                    decltype(cppsort::probe::enc),
                    decltype(cppsort::probe::exc),
                    decltype(cppsort::probe::ham),
                    decltype(cppsort::probe::inv),
                    decltype(cppsort::probe::max),
                    decltype(cppsort::probe::mono),
                    decltype(cppsort::probe::osc),
                    decltype(cppsort::probe::par),
                    decltype(cppsort::probe::rem),
                    decltype(cppsort::probe::runs),
                    decltype(cppsort::probe::sus) )
{
    std::vector<int> collection; collection.reserve(491);
    auto distribution = dist::shuffled{};
    distribution(std::back_inserter(collection), 491, -125);

    using probe = TestType;
    auto mop = probe{}(collection, move_check_projection{});
    CHECK( mop >= 0 );
}
