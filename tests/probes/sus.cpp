/*
 * Copyright (c) 2021-2022 Morwenn
 * SPDX-License-Identifier: MIT
 */
#include <forward_list>
#include <vector>
#include <catch2/catch_test_macros.hpp>
#include <cpp-sort/probes/sus.h>
#include <testing-tools/internal_compare.h>

TEST_CASE( "presortedness measure: sus", "[probe][sus]" )
{
    using cppsort::probe::sus;

    SECTION( "simple test" )
    {
        std::forward_list<int> li = { 6, 9, 79, 41, 44, 49, 11, 16, 69, 15 };
        CHECK( sus(li) == 3 );
        CHECK( sus(li.begin(), li.end()) == 3 );

        std::vector<internal_compare<int>> tricky(li.begin(), li.end());
        CHECK( sus(tricky, &internal_compare<int>::compare_to) == 3 );
    }

    SECTION( "upper bound" )
    {
        // The upper bound should correspond to the size of
        // the input sequence minus one

        std::forward_list<int> li = { 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0 };
        auto max_n = sus.max_for_size(cppsort::mstd::distance(li));
        CHECK( max_n == 10 );
        CHECK( sus(li) == max_n );
        CHECK( sus(li.begin(), li.end()) == max_n );
    }
}
