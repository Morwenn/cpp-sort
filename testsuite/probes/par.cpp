/*
 * Copyright (c) 2016-2021 Morwenn
 * SPDX-License-Identifier: MIT
 */
#include <vector>
#include <catch2/catch.hpp>
#include <cpp-sort/probes/par.h>
#include <testing-tools/internal_compare.h>

TEST_CASE( "presortedness measure: par", "[probe][par]" )
{
    SECTION( "simple test" )
    {
        const std::vector<int> vec = { 48, 43, 96, 44, 42, 34, 42, 57, 68, 69 };
        CHECK( cppsort::probe::par(vec) == 7 );
        CHECK( cppsort::probe::par(vec.begin(), vec.end()) == 7 );

        std::vector<internal_compare<int>> tricky(vec.begin(), vec.end());
        CHECK( cppsort::probe::par(tricky, &internal_compare<int>::compare_to) == 7 );
    }

    SECTION( "upper bound" )
    {
        // The upper bound should correspond to the size of
        // the input sequence minus one

        const std::vector<int> vec = { 10, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0 };
        CHECK( cppsort::probe::par(vec) == 10 );
        CHECK( cppsort::probe::par(vec.begin(), vec.end()) == 10 );
    }
}
