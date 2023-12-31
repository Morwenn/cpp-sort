/*
 * Copyright (c) 2023 Morwenn
 * SPDX-License-Identifier: MIT
 */
#include <algorithm>
#include <cstddef>
#include <functional>
#include <iterator>
#include <list>
#include <vector>
#include <catch2/catch_test_macros.hpp>
#include <cpp-sort/metrics/projections.h>
#include <cpp-sort/sorters/selection_sorter.h>
#include <cpp-sort/sorters/std_sorter.h>
#include <testing-tools/algorithm.h>
#include <testing-tools/distributions.h>
#include <testing-tools/span.h>
#include <testing-tools/wrapper.h>

using wrapper = generic_wrapper<int>;

TEST_CASE( "basic metrics::projections tests",
           "[metrics][selection_sorter]" )
{
    // Selection sort always makes the same number of projections
    // for a given size of arrays, allowing to deterministically
    // check that number
    cppsort::metrics::projections<
        cppsort::selection_sorter
    > sorter;

    SECTION( "without projections" )
    {
        std::list<int> collection;
        auto distribution = dist::shuffled{};
        distribution(std::back_inserter(collection), 65);

        auto res = sorter(collection);
        CHECK( res == 4160 );
        CHECK( std::is_sorted(collection.begin(), collection.end()) );
    }

    SECTION( "with projections" )
    {
        std::list<wrapper> collection;
        auto distribution = dist::shuffled{};
        distribution(std::back_inserter(collection), 80);

        auto res = sorter(collection, &wrapper::value);
        CHECK( res == 6320 );
        CHECK( helpers::is_sorted(collection.begin(), collection.end(),
                                  std::less<>{}, &wrapper::value) );
    }
}

TEST_CASE( "metrics::projections with span",
           "[metrics][span][selection_sorter]" )
{
    cppsort::metrics::projections<
        cppsort::selection_sorter
    > sorter;

    SECTION( "without projections" )
    {
        std::list<int> collection;
        auto distribution = dist::shuffled{};
        distribution(std::back_inserter(collection), 65, 0);

        auto res = sorter(make_span(collection));
        CHECK( res == 4160 );
        CHECK( std::is_sorted(collection.begin(), collection.end()) );
    }

    SECTION( "with projections" )
    {
        std::list<wrapper> collection;
        auto distribution = dist::shuffled{};
        distribution(std::back_inserter(collection), 80, 0);

        auto res = sorter(make_span(collection), &wrapper::value);
        CHECK( res == 6320 );
        CHECK( helpers::is_sorted(collection.begin(), collection.end(),
                                  std::less<>{}, &wrapper::value) );
    }
}
