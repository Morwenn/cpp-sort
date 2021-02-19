/*
 * Copyright (c) 2018-2021 Morwenn
 * SPDX-License-Identifier: MIT
 */
#include <forward_list>
#include <vector>
#include <catch2/catch.hpp>
#include <cpp-sort/probes/mono.h>
#include <testing-tools/internal_compare.h>

TEST_CASE( "presortedness measure: mono", "[probe][mono]" )
{
    SECTION( "simple test" )
    {
        const std::forward_list<int> li = { 48, 43, 96, 44, 42, 34, 42, 57, 68, 69 };
        CHECK( cppsort::probe::mono(li) == 2 );
        CHECK( cppsort::probe::mono(li.begin(), li.end()) == 2 );

        std::vector<internal_compare<int>> tricky(li.begin(), li.end());
        CHECK( cppsort::probe::mono(tricky, &internal_compare<int>::compare_to) == 2 );
    }

    SECTION( "lower bound" )
    {
        const std::forward_list<int> li1 = { 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0 };
        CHECK( cppsort::probe::mono(li1) == 0 );
    }

    SECTION( "upper bound" )
    {
        // The upper bound should correspond to:
        // size / 2

        const std::forward_list<int> li = { 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0 };
        CHECK( cppsort::probe::mono(li) == 5 );
        CHECK( cppsort::probe::mono(li.begin(), li.end()) == 5 );
    }

    SECTION( "equal neighbours in the sequence" )
    {
        const std::forward_list<int> li = { 0, 0, 0, 1, 2, 3, 4, 6, 5, 3 };
        CHECK( cppsort::probe::mono(li) == 1 );

        const std::forward_list<int> li1 = { 6, 5, 4, 3, 2, 2, 2, 2 };
        CHECK( cppsort::probe::mono(li1) == 0 );

        const std::forward_list<int> li2 = { 1, 1, 2, 8, 3, 3, 2, 1, 1, 5, 6 };
        CHECK( cppsort::probe::mono(li2) == 2 );
    }
}
