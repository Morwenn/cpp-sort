/*
 * Copyright (c) 2016-2021 Morwenn
 * SPDX-License-Identifier: MIT
 */
#include <forward_list>
#include <iterator>
#include <vector>
#include <catch2/catch.hpp>
#include <cpp-sort/probes/exc.h>
#include <cpp-sort/utility/size.h>
#include <testing-tools/distributions.h>
#include <testing-tools/internal_compare.h>

TEST_CASE( "presortedness measure: exc", "[probe][exc]" )
{
    using cppsort::probe::exc;

    SECTION( "simple test" )
    {
        std::forward_list<int> li = { 74, 59, 62, 23, 86, 69, 18, 52, 77, 68 };
        CHECK( exc(li) == 7 );
        CHECK( exc(li.begin(), li.end()) == 7 );

        std::forward_list<int> li2 = { 9, 8, 7, 6, 5, 4, 3, 2, 1, 0 };
        CHECK( exc(li2) == 5 );
        CHECK( exc(li2.begin(), li2.end()) == 5 );

        std::vector<internal_compare<int>> tricky(li.begin(), li.end());
        CHECK( exc(tricky, &internal_compare<int>::compare_to) == 7 );
    }

    SECTION( "upper bound" )
    {
        // The upper bound should correspond to the size of
        // the input sequence minus one

        std::forward_list<int> li = { 10, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
        auto max_n = exc.max_for_size(cppsort::utility::size(li));
        CHECK( max_n == 10 );
        CHECK( exc(li) == max_n );
        CHECK( exc(li.begin(), li.end()) == max_n );
    }

    SECTION( "regressions" )
    {
        std::vector<int> collection;
        collection.reserve(100);
        auto distribution = dist::ascending_duplicates{};
        distribution(std::back_inserter(collection), 100);

        CHECK( exc(collection) == 0 );
    }
}
