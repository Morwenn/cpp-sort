/*
 * Copyright (c) 2016-2019 Morwenn
 * SPDX-License-Identifier: MIT
 */
#include <forward_list>
#include <iterator>
#include <vector>
#include <catch2/catch.hpp>
#include <cpp-sort/probes/exc.h>
#include <testing-tools/distributions.h>
#include <testing-tools/internal_compare.h>

TEST_CASE( "presortedness measure: exc", "[probe][exc]" )
{
    SECTION( "simple test" )
    {
        std::forward_list<int> li = { 74, 59, 62, 23, 86, 69, 18, 52, 77, 68 };
        CHECK( cppsort::probe::exc(li) == 7 );
        CHECK( cppsort::probe::exc(std::begin(li), std::end(li)) == 7 );

        std::forward_list<int> li2 = { 9, 8, 7, 6, 5, 4, 3, 2, 1, 0 };
        CHECK( cppsort::probe::exc(li2) == 5 );
        CHECK( cppsort::probe::exc(std::begin(li2), std::end(li2)) == 5 );

        std::vector<internal_compare<int>> tricky(li.begin(), li.end());
        CHECK( cppsort::probe::exc(tricky, &internal_compare<int>::compare_to) == 7 );
    }

    SECTION( "lower bound" )
    {
        std::forward_list<int> li = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
        CHECK( cppsort::probe::exc(li) == 0 );
        CHECK( cppsort::probe::exc(std::begin(li), std::end(li)) == 0 );
    }

    SECTION( "upper bound" )
    {
        // The upper bound should correspond to the size of
        // the input sequence minus one

        std::forward_list<int> li = { 10, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
        CHECK( cppsort::probe::exc(li) == 10 );
        CHECK( cppsort::probe::exc(std::begin(li), std::end(li)) == 10 );
    }

    SECTION( "regressions" )
    {
        std::vector<int> collection;
        collection.reserve(100);
        auto distribution = dist::ascending_sawtooth{};
        distribution(std::back_inserter(collection), 100);

        std::sort(std::begin(collection), std::end(collection));
        CHECK( cppsort::probe::exc(collection) == 0 );
    }
}
