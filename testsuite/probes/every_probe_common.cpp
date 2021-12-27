/*
 * Copyright (c) 2021 Morwenn
 * SPDX-License-Identifier: MIT
 */
#include <numeric>
#include <type_traits>
#include <vector>
#include <catch2/catch.hpp>
#include <cpp-sort/probes.h>

//
// Common tests for measures of presortedness
//

TEMPLATE_TEST_CASE( "test every probe with all_equal distribution", "[probe]",
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
                    decltype(cppsort::probe::runs),
                    decltype(cppsort::probe::sms),
                    decltype(cppsort::probe::sus) )
{
    // Ensure that all measures of presortedness return 0 when
    // given a collection where all elements are equal

    std::vector<int> collection(50, 5);
    std::decay_t<TestType> mop;
    auto presortedness = mop(collection);
    CHECK( presortedness == 0 );
}

TEMPLATE_TEST_CASE( "test every probe with a sorted collection", "[probe]",
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
                    decltype(cppsort::probe::runs),
                    decltype(cppsort::probe::sms),
                    decltype(cppsort::probe::sus) )
{
    // Ensure that all measures of presortedness return 0 when
    // given a collection where all elements are sorted

    std::vector<int> collection(50);
    std::iota(collection.begin(), collection.end(), 0);
    std::decay_t<TestType> mop;
    auto presortedness = mop(collection);
    CHECK( presortedness == 0 );
}

TEMPLATE_TEST_CASE( "test every probe with a 0 or 1 element", "[probe]",
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
                    decltype(cppsort::probe::runs),
                    decltype(cppsort::probe::sms),
                    decltype(cppsort::probe::sus) )
{
    // Ensure that all measures of presortedness return 0 when
    // given a collection with 0 or 1 element

    std::decay_t<TestType> mop;

    SECTION( "empty collection" )
    {
        std::vector<int> collection;
        auto presortedness = mop(collection);
        CHECK( presortedness == 0 );
    }

    SECTION( "one-element collection" )
    {
        std::vector<int> collection = { 42 };
        auto presortedness = mop(collection);
        CHECK( presortedness == 0 );
    }
}
