/*
 * Copyright (c) 2021-2025 Morwenn
 * SPDX-License-Identifier: MIT
 */
#include <numeric>
#include <type_traits>
#include <vector>
#include <catch2/catch_template_test_macros.hpp>
#include <rapidcheck.h>
#include <rapidcheck/catch.h>
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
                    decltype(cppsort::probe::spear),
                    decltype(cppsort::probe::sus) )
{
    // First property formalized by Mannila
    // Ensure that all measures of presortedness return 0 when
    // given a collection where all elements are equal

    const std::vector<int> collection(50, 5);
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
                    decltype(cppsort::probe::spear),
                    decltype(cppsort::probe::sus) )
{
    // First property formalized by Mannila
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
                    decltype(cppsort::probe::spear),
                    decltype(cppsort::probe::sus) )
{
    // First property formalized by Mannila
    // Ensure that all measures of presortedness return 0 when
    // given a collection with 0 or 1 element

    std::decay_t<TestType> mop;

    SECTION( "empty collection" )
    {
        const std::vector<int> collection;
        auto presortedness = mop(collection);
        CHECK( presortedness == 0 );
    }

    SECTION( "one-element collection" )
    {
        const std::vector<int> collection = { 42 };
        auto presortedness = mop(collection);
        CHECK( presortedness == 0 );
    }
}

TEMPLATE_TEST_CASE( "test order isomorphism for every probe", "[probe]",
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
                    decltype(cppsort::probe::spear),
                    decltype(cppsort::probe::sus) )
{
    // Second property formalized by Mannila
    // Ensure that when the relative order of elements of two sequences
    // is the same, then the measure of presortedness returns the same
    // result

    rc::prop("order isomorphism", [](std::vector<int> sequence1) {
        std::decay_t<TestType> mop;

        for (int& elem: sequence1) {
            elem /= 2;
        }

        auto sequence2 = sequence1;
        for (int& elem: sequence2) {
            elem *= 2;
        }

        return mop(sequence1) == mop(sequence2);
    });
}

TEMPLATE_TEST_CASE( "test M(aX) <= |X| + M(X) for most probes M", "[probe]",
                    decltype(cppsort::probe::block),
                    decltype(cppsort::probe::dis),
                    decltype(cppsort::probe::enc),
                    decltype(cppsort::probe::exc),
                    decltype(cppsort::probe::inv),
                    decltype(cppsort::probe::max),
                    decltype(cppsort::probe::mono),
                    decltype(cppsort::probe::rem),
                    decltype(cppsort::probe::runs),
                    decltype(cppsort::probe::sus) )
{
    // Fifth property formalized by Mannila
    // The following probes don't satisfy it: ham, osc, spear

    rc::prop("M(⟨a⟩X) ≤ |X| + M(X)", [](const std::vector<int>& sequence) {
        std::decay_t<TestType> mop;
        auto size = static_cast<std::vector<int>::difference_type>(sequence.size());
        if (size <= 2) {
            return true;
        }
        return mop(sequence) <= (size - 1) + mop(sequence.begin() + 1, sequence.end());
    });
}
