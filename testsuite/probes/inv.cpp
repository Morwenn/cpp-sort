/*
 * Copyright (c) 2016-2018 Morwenn
 * SPDX-License-Identifier: MIT
 */
#include <forward_list>
#include <iterator>
#include <vector>
#include <catch2/catch.hpp>
#include <cpp-sort/probes/inv.h>
#include <testing-tools/internal_compare.h>

TEST_CASE( "presortedness measure: inv", "[probe][inv]" )
{
    SECTION( "simple test" )
    {
        const std::forward_list<int> li = { 48, 43, 96, 44, 42, 34, 42, 57, 68, 69 };
        CHECK( cppsort::probe::inv(li) == 19 );
        CHECK( cppsort::probe::inv(std::begin(li), std::end(li)) == 19 );

        std::vector<internal_compare<int>> tricky(li.begin(), li.end());
        CHECK( cppsort::probe::inv(tricky, &internal_compare<int>::compare_to) == 19 );
    }

    SECTION( "lower bound" )
    {
        const std::forward_list<int> li = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
        CHECK( cppsort::probe::inv(li) == 0 );
        CHECK( cppsort::probe::inv(std::begin(li), std::end(li)) == 0 );
    }

    SECTION( "upper bound" )
    {
        // The upper bound should correspond to:
        // size * (size - 1) / 2

        const std::forward_list<int> li = { 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0 };
        CHECK( cppsort::probe::inv(li) == 55 );
        CHECK( cppsort::probe::inv(std::begin(li), std::end(li)) == 55 );
    }
}
