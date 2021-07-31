/*
 * Copyright (c) 2016-2021 Morwenn
 * SPDX-License-Identifier: MIT
 */
#include <forward_list>
#include <vector>
#include <catch2/catch.hpp>
#include <cpp-sort/probes/dis.h>
#include <cpp-sort/utility/size.h>
#include <testing-tools/internal_compare.h>

TEST_CASE( "presortedness measure: dis", "[probe][dis]" )
{
    using cppsort::probe::dis;

    SECTION( "simple tests" )
    {
        {
            std::forward_list<int> li = { 47, 53, 46, 41, 59, 81, 74, 97, 100, 45 };
            CHECK( dis(li) == 9 );
            CHECK( dis(li.begin(), li.end()) == 9 );

            std::vector<internal_compare<int>> tricky(li.begin(), li.end());
            CHECK( dis(tricky, &internal_compare<int>::compare_to) == 9 );
        }
        {
            const std::forward_list<int> li = { 48, 43, 96, 44, 42, 34, 42, 57, 68, 69 };
            CHECK( dis(li) == 7 );
            CHECK( dis(li.begin(), li.end()) == 7 );

            std::vector<internal_compare<int>> tricky(li.begin(), li.end());
            CHECK( dis(tricky, &internal_compare<int>::compare_to) == 7 );
        }
    }

    SECTION( "roughly sorting test" )
    {
        // Example from *Roughly Sorting: Sequential and Parallel Approach*
        // by T. Altman and Y. Igarashi

        const std::forward_list<int> li = {
            2, 3, 5, 1, 4, 2, 6,
            8, 7, 9, 8, 11, 6, 13,
            12, 16, 15, 17, 18,
            20, 18, 19, 21, 19
        };
        CHECK( dis(li) == 5 );
    }

    SECTION( "upper bound" )
    {
        // The upper bound should correspond to the size of
        // the input sequence minus one

        std::forward_list<int> li = { 10, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0 };
        auto max_n = dis.max_for_size(cppsort::utility::size(li));
        CHECK( max_n == 10 );
        CHECK( dis(li) == max_n );
        CHECK( dis(li.begin(), li.end()) == max_n );
    }
}
