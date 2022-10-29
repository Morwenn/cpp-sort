/*
 * Copyright (c) 2016-2022 Morwenn
 * SPDX-License-Identifier: MIT
 */
#include <forward_list>
#include <vector>
#include <catch2/catch_test_macros.hpp>
#include <cpp-sort/probes/osc.h>
#include <testing-tools/internal_compare.h>
#include <testing-tools/wrapper.h>

TEST_CASE( "presortedness measure: osc", "[probe][osc]" )
{
    using cppsort::probe::osc;

    SECTION( "simple test" )
    {
        // Example from the paper Adaptive Heapsort
        // by Levcopoulos and Petersson

        std::forward_list<int> li = { 6, 3, 9, 8, 4, 7, 1, 11 };
        CHECK( osc(li) == 17 );
        CHECK( osc(li.begin(), li.end()) == 17 );

        std::vector<internal_compare<int>> tricky(li.begin(), li.end());
        CHECK( osc(tricky, &internal_compare<int>::compare_to) == 17 );
    }

    SECTION( "upper bound" )
    {
        // Example from the paper Adaptive Heapsort
        // by Levcopoulos and Petersson, the upper bound
        // should be (size * (size - 2) - 1) / 2

        std::forward_list<int> li = { 8, 5, 10, 3, 12, 1, 13, 2, 11, 4, 9, 6, 7 };
        auto max_n = osc.max_for_size(cppsort::mstd::distance(li));
        CHECK( max_n == 71 );
        CHECK( osc(li) == max_n );
        CHECK( osc(li.begin(), li.end()) == max_n );
    }

    SECTION( "regressions" )
    {
        using wrapper = generic_wrapper<generic_wrapper<int>>;
        std::vector<wrapper> vec = { {{6}}, {{3}}, {{9}}, {{8}}, {{4}}, {{7}}, {{1}}, {{11}} };
        auto comp = [](generic_wrapper<int> const& lhs, generic_wrapper<int> const& rhs) {
            return lhs.value < rhs.value;
        };
        CHECK( osc(vec, comp, &wrapper::value) == 17 );
    }
}
