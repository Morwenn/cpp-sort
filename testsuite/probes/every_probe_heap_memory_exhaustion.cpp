/*
 * Copyright (c) 2020 Morwenn
 * SPDX-License-Identifier: MIT
 */
#include <algorithm>
#include <forward_list>
#include <iterator>
#include <list>
#include <vector>
#include <catch2/catch.hpp>
#include <cpp-sort/probes.h>
#include <testing-tools/distributions.h>
#include <testing-tools/memory_exhaustion.h>

//
// Specific tests to check that a selection of probes still run fine
// even when there isn't any extra heap memory available
//
// These tests shouldn't be part of the main test suite executable
//

TEMPLATE_TEST_CASE( "heap exhaustion for random-access probes", "[probe][heap_exhaustion]",
                    decltype(cppsort::probe::dis),
                    decltype(cppsort::probe::mono),
                    decltype(cppsort::probe::osc),
                    decltype(cppsort::probe::par),
                    decltype(cppsort::probe::runs) )
{
    std::vector<int> collection; collection.reserve(491);
    auto distribution = dist::shuffled{};
    distribution(std::back_inserter(collection), 491, -125);

    std::vector<int>::difference_type mop;
    using probe = TestType;
    {
        scoped_memory_exhaustion _;
        mop = probe{}(collection);
    }
    CHECK( mop >= 0 );
}

TEMPLATE_TEST_CASE( "heap exhaustion for bidirectional probes", "[probe][heap_exhaustion]",
                    decltype(cppsort::probe::dis),
                    decltype(cppsort::probe::mono),
                    decltype(cppsort::probe::osc),
                    decltype(cppsort::probe::par),
                    decltype(cppsort::probe::runs) )
{
    std::list<int> collection;
    auto distribution = dist::shuffled{};
    distribution(std::back_inserter(collection), 491, -125);

    std::list<int>::difference_type mop;
    using probe = TestType;
    {
        scoped_memory_exhaustion _;
        mop = probe{}(collection);
    }
    CHECK( mop >= 0 );
}

TEMPLATE_TEST_CASE( "heap exhaustion for forward probes", "[probe][heap_exhaustion]",
                    decltype(cppsort::probe::dis),
                    decltype(cppsort::probe::mono),
                    decltype(cppsort::probe::osc),
                    decltype(cppsort::probe::par),
                    decltype(cppsort::probe::runs) )
{
    std::forward_list<int> collection;
    auto distribution = dist::shuffled{};
    distribution(std::front_inserter(collection), 491, -125);

    std::forward_list<int>::difference_type mop;
    using probe = TestType;
    {
        scoped_memory_exhaustion _;
        mop = probe{}(collection);
    }
    CHECK( mop >= 0 );
}
