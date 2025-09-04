/*
 * Copyright (c) 2021-2025 Morwenn
 * SPDX-License-Identifier: MIT
 */
#include <algorithm>
#include <numeric>
#include <random>
#include <type_traits>
#include <vector>
#include <catch2/catch_template_test_macros.hpp>
#include <rapidcheck.h>
#include <rapidcheck/catch.h>
#include <cpp-sort/probes.h>
#include "testing-tools/random.h"

//
// Test common properties of measures of presortedness
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
    // Ensure that all measures of disorder return 0 when
    // given a collection where all elements are equal

    const std::vector<int> collection(50, 5);
    std::decay_t<TestType> measure;
    auto disorder = measure(collection);
    CHECK( disorder == 0 );
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
    // Ensure that all measures of disorder return 0 when
    // given a collection where all elements are sorted

    std::vector<int> collection(50);
    std::iota(collection.begin(), collection.end(), 0);
    std::decay_t<TestType> measure;
    auto disorder = measure(collection);
    CHECK( disorder == 0 );
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
    // Ensure that all measures of disorder return 0 when
    // given a collection with 0 or 1 element

    std::decay_t<TestType> measure;

    SECTION( "empty collection" )
    {
        const std::vector<int> collection;
        auto disorder = measure(collection);
        CHECK( disorder == 0 );
    }

    SECTION( "one-element collection" )
    {
        const std::vector<int> collection = { 42 };
        auto disorder = measure(collection);
        CHECK( disorder == 0 );
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
    // is the same, then the measure of disorder returns the same
    // result

    rc::prop("order isomorphism", [](std::vector<int> sequence1) {
        std::decay_t<TestType> measure;

        for (int& elem: sequence1) {
            elem /= 2;
        }

        auto sequence2 = sequence1;
        for (int& elem: sequence2) {
            elem *= 2;
        }

        return measure(sequence1) == measure(sequence2);
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
        std::decay_t<TestType> measure;

        auto size = static_cast<std::vector<int>::difference_type>(sequence.size());
        if (size <= 2) {
            return true;
        }

        return measure(sequence) <= (size - 1)
             + measure(sequence.begin() + 1, sequence.end());
    });
}

TEMPLATE_TEST_CASE( "test prefix monotonicity", "[probe]",
                    decltype(cppsort::probe::dis),
                    decltype(cppsort::probe::enc),
                    decltype(cppsort::probe::exc),
                    decltype(cppsort::probe::ham),
                    decltype(cppsort::probe::inv),
                    decltype(cppsort::probe::max),
                    decltype(cppsort::probe::rem),
                    decltype(cppsort::probe::runs),
                    decltype(cppsort::probe::spear),
                    decltype(cppsort::probe::sus) )
{
    // Property formalized by Estivill-Castro in *Sorting and Measures of Disorder*
    // The following probes don't satisfy it: Block, Mono, Osc

    // Note: the original paper claims that Osc also satisfies this property,
    // but it fails for X=⟨3, 0⟩ Y=⟨⟩ Z=⟨4, 2⟩

    rc::prop("prefix monoticity", [](std::vector<int> sequence) {
        using diff_t = std::vector<int>::difference_type;
        using param_t = std::uniform_int_distribution<diff_t>::param_type;

        auto size = static_cast<diff_t>(sequence.size());
        if (size < 3) {
            return true;
        }

        // Split the sequence into three consequent subsequences X, Y and Z
        std::uniform_int_distribution<diff_t> dist;
        auto x_begin = sequence.begin();
        auto y_begin = x_begin + dist(hasard::engine(), param_t{0, size - 1});
        auto z_begin = x_begin + dist(hasard::engine(), param_t{y_begin - x_begin, size - 1});

        // Ensure that all elements of Z are greater than all elements of X and Y
        std::nth_element(x_begin, z_begin, sequence.end());

        std::decay_t<TestType> measure;
        auto disorder_x = measure(x_begin, y_begin);
        auto disorder_y = measure(y_begin, z_begin);
        auto disorder_yz = measure(y_begin, sequence.end());
        auto new_x_begin = std::rotate(x_begin, y_begin, z_begin);
        auto disorder_xz = measure(new_x_begin, sequence.end());

        return disorder_x <= disorder_y
            ? disorder_xz <= disorder_yz
            : disorder_yz <= disorder_xz;
    });
}

TEMPLATE_TEST_CASE( "test M(XY) = M(X) + M(Y) if X <= Y for most probes M", "[probe]",
                    decltype(cppsort::probe::exc),
                    decltype(cppsort::probe::ham),
                    decltype(cppsort::probe::inv),
                    decltype(cppsort::probe::rem),
                    decltype(cppsort::probe::runs),
                    decltype(cppsort::probe::spear) )
{
    // Property formalized by Estivill-Castro in *Sorting and Measures of Disorder*
    // Not all measures of presortedness satisfy it, but a lot do

    rc::prop("M(XY) = M(X) + M(Y) if X ≤ Y", [](std::vector<int> sequence) {
        using diff_t = std::vector<int>::difference_type;
        using param_t = std::uniform_int_distribution<diff_t>::param_type;

        // Split the sequence into two consequent subsequences X and Y
        auto size = static_cast<diff_t>(sequence.size());
        std::uniform_int_distribution<diff_t> dist;
        auto x_begin = sequence.begin();
        auto y_begin = x_begin + dist(hasard::engine(), param_t{0, size});
        std::nth_element(x_begin, y_begin, sequence.end());

        std::decay_t<TestType> measure;
        return measure(sequence) == measure(x_begin, y_begin) + measure(y_begin, sequence.end());
    });
}
