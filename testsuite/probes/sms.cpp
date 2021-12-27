/*
 * Copyright (c) 2021 Morwenn
 * SPDX-License-Identifier: MIT
 */
#include <forward_list>
#include <vector>
#include <catch2/catch.hpp>
#include <cpp-sort/probes/sms.h>
#include <cpp-sort/utility/size.h>
#include <testing-tools/internal_compare.h>

TEST_CASE( "presortedness measure: sms", "[probe][sms]" )
{
    using cppsort::probe::sms;

    SECTION( "simple tests" )
    {
        std::forward_list<int> li = { 40, 49, 58, 99, 60, 70, 12, 87, 9, 8, 82, 91, 99, 67, 82, 92 };
        CHECK( sms(li) == 3 );
        CHECK( sms(li.begin(), li.end()) == 3 );

        std::forward_list<int> li2 = { 4, 2, 6, 5, 3, 1, 9, 7, 10, 8 };
        CHECK( sms(li2) == 2 );
        CHECK( sms(li2.begin(), li2.end()) == 2 );

        std::vector<internal_compare<int>> tricky(li.begin(), li.end());
        CHECK( sms(tricky, &internal_compare<int>::compare_to) == 3 );
    }

    SECTION( "test for equivalent elements" )
    {
        std::forward_list<int> li1 = { 1, 2, 3, 4, 4, 4, 4, 5, 6, 7 };
        CHECK( sms(li1) == 0 );

        std::forward_list<int> li2 = { 7, 6, 5, 4, 4, 4, 4, 3, 2, 1 };
        CHECK( sms(li2) == 0 );
    }

    SECTION( "upper bound" )
    {
        // The upper bound should correspond to the size of
        // the input sequence minus one

        std::forward_list<int> li = {
            8, 17, 27, 14, 1, 9, 11, 23, 13, 32, 15,
            25, 29, 19, 21, 5, 3, 31, 16, 6, 10, 12,
            30, 2, 22, 4, 20, 26, 7, 18, 28, 24
        };
        auto max_n = sms.max_for_size(cppsort::utility::size(li));
        CHECK( max_n == 5 );
        CHECK( sms(li) == max_n );
        CHECK( sms(li.begin(), li.end()) == max_n );
    }
}
