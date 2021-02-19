/*
 * Copyright (c) 2016-2021 Morwenn
 * SPDX-License-Identifier: MIT
 */
#include <forward_list>
#include <vector>
#include <catch2/catch.hpp>
#include <cpp-sort/probes/rem.h>
#include <testing-tools/internal_compare.h>

TEST_CASE( "presortedness measure: rem", "[probe][rem]" )
{
    SECTION( "simple test" )
    {
        // Forward iterators
        std::forward_list<int> li = { 6, 9, 79, 41, 44, 49, 11, 16, 69, 15 };
        CHECK( cppsort::probe::rem(li) == 4 );
        CHECK( cppsort::probe::rem(li.begin(), li.end()) == 4 );

        // Random-access iterators
        std::vector<int> vec(li.begin(), li.end());
        CHECK( cppsort::probe::rem(vec) == 4 );
        CHECK( cppsort::probe::rem(vec.begin(), vec.end()) == 4 );

        std::vector<internal_compare<int>> tricky(li.begin(), li.end());
        CHECK( cppsort::probe::rem(tricky, &internal_compare<int>::compare_to) == 4 );
    }

    SECTION( "upper bound" )
    {
        // The upper bound should correspond to the size of
        // the input sequence minus one

        std::forward_list<int> li = { 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0 };
        CHECK( cppsort::probe::rem(li) == 10 );
        CHECK( cppsort::probe::rem(li.begin(), li.end()) == 10 );
    }
}
