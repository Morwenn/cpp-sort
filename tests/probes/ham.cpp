/*
 * Copyright (c) 2016-2022 Morwenn
 * SPDX-License-Identifier: MIT
 */
#include <forward_list>
#include <iterator>
#include <vector>
#include <catch2/catch_test_macros.hpp>
#include <cpp-sort/probes/ham.h>
#include <testing-tools/distributions.h>
#include <testing-tools/internal_compare.h>

TEST_CASE( "presortedness measure: ham", "[probe][ham]" )
{
    using cppsort::probe::ham;

    SECTION( "simple test" )
    {
        std::forward_list<int> li = { 34, 43, 96, 42, 44, 48, 57, 42, 68, 69 };
        CHECK( ham(li) == 6 );
        CHECK( ham(li.begin(), li.end()) == 6 );

        std::vector<internal_compare<int>> tricky(li.begin(), li.end());
        CHECK( ham(tricky, &internal_compare<int>::compare_to) == 6 );
    }

    SECTION( "upper bound" )
    {
        // The upper bound should correspond to the size of
        // the input sequence

        std::forward_list<int> li = { 10, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
        auto max_n = ham.max_for_size(cppsort::mstd::distance(li));
        CHECK( max_n == 11 );
        CHECK( ham(li) == max_n );
        CHECK( ham(li.begin(), li.end()) == max_n );
    }

    SECTION( "regressions" )
    {
        std::vector<int> collection;
        collection.reserve(100);
        auto distribution = dist::ascending_duplicates{};
        distribution(std::back_inserter(collection), 100);

        CHECK( ham(collection) == 0 );
    }
}
