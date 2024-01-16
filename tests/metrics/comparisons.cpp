/*
 * Copyright (c) 2023-2024 Morwenn
 * SPDX-License-Identifier: MIT
 */
#include <algorithm>
#include <cstddef>
#include <functional>
#include <iterator>
#include <list>
#include <ranges>
#include <catch2/catch_test_macros.hpp>
#include <cpp-sort/metrics/comparisons.h>
#include <cpp-sort/sorters/selection_sorter.h>
#include <testing-tools/algorithm.h>
#include <testing-tools/distributions.h>
#include <testing-tools/wrapper.h>

using wrapper = generic_wrapper<int>;

TEST_CASE( "basic metrics::comparisons tests",
           "[metrics][selection_sorter]" )
{
    // Selection sort always makes the same number of comparisons
    // for a given size of arrays, allowing to deterministically
    // check that number of comparisons
    cppsort::metrics::comparisons<
        cppsort::selection_sorter
    > sorter;

    SECTION( "without projections" )
    {
        std::list<int> collection;
        auto distribution = dist::shuffled{};
        distribution(std::back_inserter(collection), 65);

        auto res = sorter(collection);
        CHECK( res == 2080 );
        CHECK( std::is_sorted(collection.begin(), collection.end()) );
    }

    SECTION( "with projections" )
    {
        std::list<wrapper> collection;
        auto distribution = dist::shuffled{};
        distribution(std::back_inserter(collection), 80);

        auto res = sorter(collection, &wrapper::value);
        CHECK( res == 3160 );
        CHECK( helpers::is_sorted(collection.begin(), collection.end(),
                                  std::less<>{}, &wrapper::value) );
    }
}

TEST_CASE( "metrics::comparisons with temporary range",
           "[metrics][subrange][selection_sorter]" )
{
    cppsort::metrics::comparisons<
        cppsort::selection_sorter
    > sorter;

    SECTION( "without projections" )
    {
        std::list<int> collection;
        auto distribution = dist::shuffled{};
        distribution(std::back_inserter(collection), 65, 0);

        auto res = sorter(std::ranges::subrange(collection));
        CHECK( res == 2080 );
        CHECK( std::is_sorted(collection.begin(), collection.end()) );
    }

    SECTION( "with projections" )
    {
        std::list<wrapper> collection;
        auto distribution = dist::shuffled{};
        distribution(std::back_inserter(collection), 80, 0);

        auto res = sorter(std::ranges::subrange(collection), &wrapper::value);
        CHECK( res == 3160 );
        CHECK( helpers::is_sorted(collection.begin(), collection.end(),
                                  std::less<>{}, &wrapper::value) );
    }
}
