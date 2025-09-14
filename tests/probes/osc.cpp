/*
 * Copyright (c) 2016-2025 Morwenn
 * SPDX-License-Identifier: MIT
 */
#include <algorithm>
#include <forward_list>
#include <numeric>
#include <vector>
#include <catch2/catch_test_macros.hpp>
#include <rapidcheck.h>
#include <rapidcheck/catch.h>
#include <cpp-sort/probes/osc.h>
#include <cpp-sort/utility/size.h>
#include <testing-tools/internal_compare.h>
#include <testing-tools/wrapper.h>

TEST_CASE( "measure of disorder: osc", "[probe][osc]" )
{
    using cppsort::probe::osc;

    SECTION( "simple test" )
    {
        // Example from the paper Adaptive Heapsort
        // by Levcopoulos and Petersson

        const std::forward_list<int> li = { 6, 3, 9, 8, 4, 7, 1, 11 };
        CHECK( osc(li) == 17 );
        CHECK( osc(li.begin(), li.end()) == 17 );

        std::vector<internal_compare<int>> tricky(li.begin(), li.end());
        CHECK( osc(tricky, &internal_compare<int>::compare_to) == 17 );
    }

    SECTION( "upper bound" )
    {
        // Example from the paper Adaptive Heapsort
        // by Levcopoulos and Petersson, the upper bound
        // should be:
        // - When size is even: (size * (size - 2)) / 2
        // - When size is odd: (size * (size - 2) - 1) / 2

        // Even number of elements
        {
            const std::forward_list<int> li = { 8, 5, 10, 3, 12, 1, 13, 2, 11, 4, 9, 6 };
            auto max_n = osc.max_for_size(cppsort::utility::size(li));
            CHECK( max_n == 60 );
            CHECK( osc(li) == max_n );
            CHECK( osc(li.begin(), li.end()) == max_n );
        }

        // Odd number of elements
        {
            const std::forward_list<int> li = { 8, 5, 10, 3, 12, 1, 13, 2, 11, 4, 9, 6, 7 };
            auto max_n = osc.max_for_size(cppsort::utility::size(li));
            CHECK( max_n == 71 );
            CHECK( osc(li) == max_n );
            CHECK( osc(li.begin(), li.end()) == max_n );
        }
    }

    SECTION( "regressions" )
    {
        using wrapper = generic_wrapper<generic_wrapper<int>>;
        const std::vector<wrapper> vec = { {{6}}, {{3}}, {{9}}, {{8}}, {{4}}, {{7}}, {{1}}, {{11}} };
        auto comp = [](generic_wrapper<int> const& lhs, generic_wrapper<int> const& rhs) {
            return lhs.value < rhs.value;
        };
        CHECK( osc(vec, comp, &wrapper::value) == 17 );
    }

    rc::prop("Osc(Reversed(X)) = Osc(X)", [](std::vector<int> sequence) {
        auto osc_x = cppsort::probe::osc(sequence);
        std::reverse(sequence.begin(), sequence.end());
        return cppsort::probe::osc(sequence) == osc_x;
    });

    SECTION( "Sorting and Measures of Disorder, Theorem 3.18" )
    {
        int size = 1000;

        std::vector<int> vec(size, 0);
        vec[0] = size - 1;
        std::iota(vec.begin() + 1, vec.end(), 0);
        CHECK( osc(vec) <= size );
    }
}
