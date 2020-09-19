/*
 * Copyright (c) 2016-2020 Morwenn
 * SPDX-License-Identifier: MIT
 */
#include <algorithm>
#include <functional>
#include <iterator>
#include <vector>
#include <catch2/catch.hpp>
#include <cpp-sort/adapters/indirect_adapter.h>
#include <cpp-sort/sorters/quick_sorter.h>
#include <testing-tools/algorithm.h>
#include <testing-tools/distributions.h>
#include <testing-tools/span.h>

TEST_CASE( "basic tests with indirect_adapter",
           "[indirect_adapter]" )
{
    std::vector<int> vec; vec.reserve(221);
    auto distribution = dist::shuffled{};
    distribution(std::back_inserter(vec), 221, -32);

    // Working shuffled copy
    auto collection = vec;

    cppsort::indirect_adapter<
        cppsort::quick_sorter
    > sorter;

    SECTION( "with comparison" )
    {
        sorter(collection, std::greater<>{});
        CHECK( std::is_sorted(std::begin(collection), std::end(collection), std::greater<>{}) );

        collection = vec;
        sorter(std::begin(collection), std::end(collection), std::greater<>{});
        CHECK( std::is_sorted(std::begin(collection), std::end(collection), std::greater<>{}) );
    }

    SECTION( "with projection" )
    {
        sorter(collection, std::negate<>{});
        CHECK( helpers::is_sorted(std::begin(collection), std::end(collection),
                                  std::less<>{}, std::negate<>{}) );

        collection = vec;
        sorter(std::begin(collection), std::end(collection), std::negate<>{});
        CHECK( helpers::is_sorted(std::begin(collection), std::end(collection),
                                  std::less<>{}, std::negate<>{}) );
    }

    SECTION( "with comparison and projection" )
    {
        sorter(collection, std::greater<>{}, std::negate<>{});
        CHECK( std::is_sorted(std::begin(collection), std::end(collection)) );

        collection = vec;
        sorter(std::begin(collection), std::end(collection), std::greater<>{}, std::negate<>{});
        CHECK( std::is_sorted(std::begin(collection), std::end(collection)) );
    }
}

TEST_CASE( "indirect_adapter with temporary span",
           "[indirect_adapter][span]" )
{
    std::vector<int> collection; collection.reserve(221);
    auto distribution = dist::shuffled{};
    distribution(std::back_inserter(collection), 221, -32);

    cppsort::indirect_adapter<
        cppsort::quick_sorter
    > sorter;

    SECTION( "with comparison" )
    {
        sorter(make_span(collection), std::greater<>{});
        CHECK( std::is_sorted(std::begin(collection), std::end(collection), std::greater<>{}) );
    }

    SECTION( "with projection" )
    {
        sorter(make_span(collection), std::negate<>{});
        CHECK( helpers::is_sorted(std::begin(collection), std::end(collection),
                                  std::less<>{}, std::negate<>{}) );
    }

    SECTION( "with comparison and projection" )
    {
        sorter(make_span(collection), std::greater<>{}, std::negate<>{});
        CHECK( std::is_sorted(std::begin(collection), std::end(collection)) );
    }
}
