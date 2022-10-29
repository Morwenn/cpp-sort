/*
 * Copyright (c) 2016-2022 Morwenn
 * SPDX-License-Identifier: MIT
 */
#include <forward_list>
#include <vector>
#include <catch2/catch_test_macros.hpp>
#include <cpp-sort/probes/enc.h>
#include <testing-tools/internal_compare.h>

TEST_CASE( "presortedness measure: enc", "[probe][enc]" )
{
    using cppsort::probe::enc;

    SECTION( "simple test" )
    {
        std::forward_list<int> li = { 4, 6, 5, 2, 9, 1, 3, 8, 0, 7 };
        CHECK( enc(li) == 2 );
        CHECK( enc(li.begin(), li.end()) == 2 );

        std::vector<internal_compare<int>> tricky(li.begin(), li.end());
        CHECK( enc(tricky, &internal_compare<int>::compare_to) == 2 );
    }

    SECTION( "upper bound" )
    {
        // The upper bound should correspond to half the size
        // of the input sequence minus one

        std::forward_list<int> li = { 10, 0, 9, 1, 8, 2, 7, 3, 6, 4, 5 };
        auto max_n = enc.max_for_size(cppsort::mstd::distance(li));
        CHECK( max_n == 5 );
        CHECK( enc(li) == max_n );
        CHECK( enc(li.begin(), li.end()) == max_n );
    }
}
