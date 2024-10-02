/*
 * Copyright (c) 2021-2024 Morwenn
 * SPDX-License-Identifier: MIT
 */
#include <forward_list>
#include <iterator>
#include <vector>
#include <catch2/catch_test_macros.hpp>
#include <cpp-sort/probes/block.h>
#include <testing-tools/distributions.h>
#include <testing-tools/internal_compare.h>

TEST_CASE( "presortedness measure: block", "[probe][block]" )
{
    using cppsort::probe::block;

    SECTION( "simple test" )
    {
        const std::forward_list<int> li = { 74, 59, 62, 23, 86, 69, 18, 52, 77, 68 };
        CHECK( block(li) == 8 );
        CHECK( block(li.begin(), li.end()) == 8 );

        std::vector<internal_compare<int>> tricky(li.begin(), li.end());
        CHECK( block(tricky, &internal_compare<int>::compare_to) == 8 );
    }

    SECTION( "upper bound" )
    {
        // The upper bound should correspond to the size of
        // the input sequence minus one

        const std::forward_list<int> li = { 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0 };
        auto max_n = block.max_for_size(cppsort::mstd::distance(li));
        CHECK( max_n == 10 );
        CHECK( block(li) == max_n );
        CHECK( block(li.begin(), li.end()) == max_n );
    }
}
