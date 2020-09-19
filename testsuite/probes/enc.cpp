/*
 * Copyright (c) 2016-2018 Morwenn
 * SPDX-License-Identifier: MIT
 */
#include <forward_list>
#include <iterator>
#include <vector>
#include <catch2/catch.hpp>
#include <cpp-sort/probes/enc.h>
#include <testing-tools/internal_compare.h>

TEST_CASE( "presortedness measure: enc", "[probe][enc]" )
{
    SECTION( "simple test" )
    {
        std::forward_list<int> li = { 4, 6, 5, 2, 9, 1, 3, 8, 0, 7 };
        CHECK( cppsort::probe::enc(li) == 2 );
        CHECK( cppsort::probe::enc(std::begin(li), std::end(li)) == 2 );

        std::vector<internal_compare<int>> tricky(li.begin(), li.end());
        CHECK( cppsort::probe::enc(tricky, &internal_compare<int>::compare_to) == 2 );
    }

    SECTION( "lower bound" )
    {
        std::forward_list<int> li = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
        CHECK( cppsort::probe::enc(li) == 0 );
        CHECK( cppsort::probe::enc(std::begin(li), std::end(li)) == 0 );
    }

    SECTION( "upper bound" )
    {
        // The upper bound should correspond to half the size
        // of the input sequence minus one

        std::forward_list<int> li = { 0, 9, 1, 8, 2, 7, 3, 6, 4, 5 };
        CHECK( cppsort::probe::enc(li) == 4 );
        CHECK( cppsort::probe::enc(std::begin(li), std::end(li)) == 4 );
    }
}
