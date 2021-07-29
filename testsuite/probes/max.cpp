/*
 * Copyright (c) 2016-2021 Morwenn
 * SPDX-License-Identifier: MIT
 */
#include <forward_list>
#include <iterator>
#include <vector>
#include <catch2/catch.hpp>
#include <cpp-sort/probes/max.h>
#include <cpp-sort/utility/size.h>
#include <testing-tools/distributions.h>
#include <testing-tools/internal_compare.h>

TEST_CASE( "presortedness measure: max", "[probe][max]" )
{
    using cppsort::probe::max;

    SECTION( "simple test" )
    {
        std::forward_list<int> li = { 12, 28, 17, 59, 13, 10, 39, 21, 31, 30 };
        CHECK( (max)(li) == 6 );
        CHECK( (max)(li.begin(), li.end()) == 6 );

        std::vector<internal_compare<int>> tricky(li.begin(), li.end());
        CHECK( (max)(tricky, &internal_compare<int>::compare_to) == 6 );
    }

    SECTION( "upper bound" )
    {
        // The upper bound should correspond to the size of
        // the input sequence minus one

        std::forward_list<int> li = { 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0 };
        auto max_n = (max).max_for_size(cppsort::utility::size(li));
        CHECK( max_n == 10 );
        CHECK( (max)(li) == max_n );
        CHECK( (max)(li.begin(), li.end()) == max_n );
    }

    SECTION( "regressions" )
    {
        std::vector<int> collection;
        collection.reserve(100);
        auto distribution = dist::ascending_duplicates{};
        distribution(std::back_inserter(collection), 100);

        CHECK( (max)(collection) == 0 );
    }
}
