/*
 * Copyright (c) 2016-2022 Morwenn
 * SPDX-License-Identifier: MIT
 */
#include <forward_list>
#include <vector>
#include <catch2/catch_test_macros.hpp>
#include <cpp-sort/probes/rem.h>
#include <cpp-sort/utility/size.h>
#include <testing-tools/internal_compare.h>

TEST_CASE( "presortedness measure: rem", "[probe][rem]" )
{
    using cppsort::probe::rem;

    SECTION( "simple test" )
    {
        // Forward iterators
        std::forward_list<int> li = { 6, 9, 79, 41, 44, 49, 11, 16, 69, 15 };
        CHECK( rem(li) == 4 );
        CHECK( rem(li.begin(), li.end()) == 4 );

        // Random-access iterators
        std::vector<int> vec(li.begin(), li.end());
        CHECK( rem(vec) == 4 );
        CHECK( rem(vec.begin(), vec.end()) == 4 );

        std::vector<internal_compare<int>> tricky(li.begin(), li.end());
        CHECK( rem(tricky, &internal_compare<int>::compare_to) == 4 );
    }

    SECTION( "upper bound" )
    {
        // The upper bound should correspond to the size of
        // the input sequence minus one

        std::forward_list<int> li = { 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0 };
        auto max_n = rem.max_for_size(cppsort::utility::size(li));
        CHECK( max_n == 10 );
        CHECK( rem(li) == max_n );
        CHECK( rem(li.begin(), li.end()) == max_n );
    }
}
