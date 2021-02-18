/*
 * Copyright (c) 2021 Morwenn
 * SPDX-License-Identifier: MIT
 */
#include <forward_list>
#include <vector>
#include <catch2/catch.hpp>
#include <cpp-sort/probes/sus.h>
#include <testing-tools/internal_compare.h>

TEST_CASE( "presortedness measure: sus", "[probe][sus]" )
{
    SECTION( "simple test" )
    {
        std::forward_list<int> li = { 6, 9, 79, 41, 44, 49, 11, 16, 69, 15 };
        CHECK( cppsort::probe::sus(li) == 3 );
        CHECK( cppsort::probe::sus(li.begin(), li.end()) == 3 );

        std::vector<internal_compare<int>> tricky(li.begin(), li.end());
        CHECK( cppsort::probe::sus(tricky, &internal_compare<int>::compare_to) == 3 );
    }

    SECTION( "lower bound" )
    {
        std::forward_list<int> li = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
        CHECK( cppsort::probe::sus(li) == 0 );
        CHECK( cppsort::probe::sus(li.begin(), li.end()) == 0 );
    }

    SECTION( "upper bound" )
    {
        // The upper bound should correspond to the size of
        // the input sequence minus one

        std::forward_list<int> li = { 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0 };
        CHECK( cppsort::probe::sus(li) == 10 );
        CHECK( cppsort::probe::sus(li.begin(), li.end()) == 10 );
    }
}
