/*
 * Copyright (c) 2016-2021 Morwenn
 * SPDX-License-Identifier: MIT
 */
#include <list>
#include <vector>
#include <catch2/catch.hpp>
#include <cpp-sort/probes/par.h>
#include <testing-tools/internal_compare.h>

TEST_CASE( "presortedness measure: par", "[probe][par]" )
{
    using cppsort::probe::par;

    SECTION( "simple test" )
    {
        const std::list<int> li = { 48, 43, 96, 44, 42, 34, 42, 57, 68, 69 };
        CHECK( par(li) == 7 );
        CHECK( par(li.begin(), li.end()) == 7 );

        std::vector<internal_compare<int>> tricky(li.begin(), li.end());
        CHECK( par(tricky, &internal_compare<int>::compare_to) == 7 );
    }

    SECTION( "roughly sorting test" )
    {
        // Example from *Roughly Sorting: Sequential and Parallel Approach*
        // by T. Altman and Y. Igarashi

        const std::list<int> li = {
            2, 3, 5, 1, 4, 2, 6,
            8, 7, 9, 8, 11, 6, 13,
            12, 16, 15, 17, 18,
            20, 18, 19, 21, 19
        };
        CHECK( par(li) == 5 );
    }

    SECTION( "upper bound" )
    {
        // The upper bound should correspond to the size of
        // the input sequence minus one

        const std::list<int> li = { 10, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0 };
        std::list<int>::difference_type max_n = par.max_for_size(li.size());
        CHECK( max_n == 10 );
        CHECK( par(li) == max_n );
        CHECK( par(li.begin(), li.end()) == max_n );
    }
}
