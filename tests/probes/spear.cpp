/*
 * Copyright (c) 2025 Morwenn
 * SPDX-License-Identifier: MIT
 */
#include <forward_list>
#include <initializer_list>
#include <iterator>
#include <vector>
#include <catch2/catch_test_macros.hpp>
#include <cpp-sort/probes/spear.h>
#include <cpp-sort/utility/size.h>
#include <testing-tools/distributions.h>
#include <testing-tools/internal_compare.h>

TEST_CASE( "presortedness measure: spear", "[probe][max]" )
{
    using cppsort::probe::spear;

    SECTION( "simple test" )
    {
        std::forward_list<int> li = { 12, 28, 17, 59, 13, 10, 39, 21, 31, 30 };
        CHECK( spear(li) == 28 );
        CHECK( spear(li.begin(), li.end()) == 28 );

        std::vector<internal_compare<int>> tricky(li.begin(), li.end());
        CHECK( spear(tricky, &internal_compare<int>::compare_to) == 28 );
    }

    SECTION( "upper bound" )
    {
        // The upper bound should be n²/2 when the collection
        // is sorted in reverse order

        std::forward_list<int> li = { 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0 };
        auto max_n = spear.max_for_size(cppsort::utility::size(li));
        CHECK( max_n == 60 );
        CHECK( spear(li) == max_n );
        CHECK( spear(li.begin(), li.end()) == max_n );
    }

    SECTION( "exhaustive check for 4 distinct values" )
    {
        // Results from Spearman's Footrule as a Measure of Disarray
        // by Diaconis and Graham

        auto spear = [](std::vector<int> vec) {
            return cppsort::probe::spear(vec);
        };

        CHECK( spear({1, 2, 3, 4}) == 0 );
        CHECK( spear({1, 2, 4, 3}) == 2 );
        CHECK( spear({1, 3, 2, 4}) == 2 );
        CHECK( spear({1, 3, 4, 2}) == 4 );
        CHECK( spear({1, 4, 2, 3}) == 4 );
        CHECK( spear({1, 4, 3, 2}) == 4 );
        CHECK( spear({2, 1, 3, 4}) == 2 );
        CHECK( spear({2, 1, 4, 3}) == 4 );
        CHECK( spear({2, 3, 1, 4}) == 4 );
        CHECK( spear({2, 3, 4, 1}) == 6 );
        CHECK( spear({2, 4, 1, 3}) == 6 );
        CHECK( spear({2, 4, 3, 1}) == 6 );
        CHECK( spear({3, 1, 2, 4}) == 4 );
        CHECK( spear({3, 1, 4, 2}) == 6 );
        CHECK( spear({3, 2, 1, 4}) == 4 );
        CHECK( spear({3, 2, 4, 1}) == 6 );
        CHECK( spear({3, 4, 1, 2}) == 8 );
        CHECK( spear({3, 4, 2, 1}) == 8 );
        CHECK( spear({4, 1, 2, 3}) == 6 );
        CHECK( spear({4, 1, 3, 2}) == 6 );
        CHECK( spear({4, 2, 1, 3}) == 6 );
        CHECK( spear({4, 2, 3, 1}) == 6 );
        CHECK( spear({4, 3, 1, 2}) == 8 );
        CHECK( spear({4, 3, 2, 1}) == 8 );
    }

    SECTION( "regression test: some equal values" )
    {
        int arr[] = {2, 2, 0, -1};
        CHECK( spear(arr) == 8 );
    }
}
