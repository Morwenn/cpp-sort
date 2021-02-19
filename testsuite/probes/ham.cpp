/*
 * Copyright (c) 2016-2021 Morwenn
 * SPDX-License-Identifier: MIT
 */
#include <forward_list>
#include <iterator>
#include <vector>
#include <catch2/catch.hpp>
#include <cpp-sort/probes/ham.h>
#include <testing-tools/distributions.h>
#include <testing-tools/internal_compare.h>

TEST_CASE( "presortedness measure: ham", "[probe][ham]" )
{
    SECTION( "simple test" )
    {
        std::forward_list<int> li = { 34, 43, 96, 42, 44, 48, 57, 42, 68, 69 };
        CHECK( cppsort::probe::ham(li) == 6 );
        CHECK( cppsort::probe::ham(std::begin(li), std::end(li)) == 6 );

        std::vector<internal_compare<int>> tricky(li.begin(), li.end());
        CHECK( cppsort::probe::ham(tricky, &internal_compare<int>::compare_to) == 6 );
    }

    SECTION( "upper bound" )
    {
        // The upper bound should correspond to the size of
        // the input sequence

        std::forward_list<int> li = { 10, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
        CHECK( cppsort::probe::ham(li) == 11 );
        CHECK( cppsort::probe::ham(std::begin(li), std::end(li)) == 11 );
    }

    SECTION( "regressions" )
    {
        std::vector<int> collection;
        collection.reserve(100);
        auto distribution = dist::ascending_sawtooth{};
        distribution(std::back_inserter(collection), 100);

        std::sort(std::begin(collection), std::end(collection));
        CHECK( cppsort::probe::ham(collection) == 0 );
    }
}
