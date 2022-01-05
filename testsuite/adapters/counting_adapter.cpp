/*
 * Copyright (c) 2015-2022 Morwenn
 * SPDX-License-Identifier: MIT
 */
#include <algorithm>
#include <cstddef>
#include <functional>
#include <iterator>
#include <list>
#include <vector>
#include <catch2/catch_test_macros.hpp>
#include <cpp-sort/adapters/counting_adapter.h>
#include <cpp-sort/sorters/selection_sorter.h>
#include <cpp-sort/sorters/std_sorter.h>
#include <testing-tools/algorithm.h>
#include <testing-tools/distributions.h>
#include <testing-tools/span.h>
#include <testing-tools/wrapper.h>

using wrapper = generic_wrapper<int>;

TEST_CASE( "basic counting_adapter tests",
           "[counting_adapter][selection_sorter]" )
{
    // Selection sort always makes the same number of comparisons
    // for a given size of arrays, allowing to deterministically
    // check that number of comparisons
    cppsort::counting_adapter<
        cppsort::selection_sorter
    > sorter;

    SECTION( "without projections" )
    {
        // Fill the collection
        std::list<int> collection;
        auto distribution = dist::shuffled{};
        distribution(std::back_inserter(collection), 65);

        // Sort and check it's sorted
        std::size_t res = sorter(collection);
        CHECK( res == 2080 );
        CHECK( std::is_sorted(collection.begin(), collection.end()) );
    }

    SECTION( "with projections" )
    {
        // Fill the collection
        std::list<wrapper> collection;
        auto distribution = dist::shuffled{};
        distribution(std::back_inserter(collection), 80);

        // Sort and check it's sorted
        std::size_t res = sorter(collection, &wrapper::value);
        CHECK( res == 3160 );
        CHECK( helpers::is_sorted(collection.begin(), collection.end(),
                                  std::less<>{}, &wrapper::value) );
    }
}

TEST_CASE( "counting_adapter tests with std_sorter",
           "[counting_adapter][std_sorter]" )
{
    cppsort::counting_adapter<
        cppsort::std_sorter
    > sorter;

    SECTION( "without projections" )
    {
        // Fill the collection
        std::vector<int> collection; collection.reserve(65);
        auto distribution = dist::shuffled{};
        distribution(std::back_inserter(collection), 65, 0);

        // Sort and check it's sorted
        sorter(collection);
        CHECK( std::is_sorted(collection.begin(), collection.end()) );
    }
}

TEST_CASE( "counting_adapter with span",
           "[counting_adapter][span][selection_sorter]" )
{
    cppsort::counting_adapter<
        cppsort::selection_sorter
    > sorter;

    SECTION( "without projections" )
    {
        // Fill the collection
        std::list<int> collection;
        auto distribution = dist::shuffled{};
        distribution(std::back_inserter(collection), 65, 0);

        // Sort and check it's sorted
        std::size_t res = sorter(make_span(collection));
        CHECK( res == 2080 );
        CHECK( std::is_sorted(collection.begin(), collection.end()) );
    }

    SECTION( "with projections" )
    {
        // Fill the collection
        std::list<wrapper> collection;
        auto distribution = dist::shuffled{};
        distribution(std::back_inserter(collection), 80, 0);

        // Sort and check it's sorted
        std::size_t res = sorter(make_span(collection), &wrapper::value);
        CHECK( res == 3160 );
        CHECK( helpers::is_sorted(collection.begin(), collection.end(),
                                  std::less<>{}, &wrapper::value) );
    }
}
