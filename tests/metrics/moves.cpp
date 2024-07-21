/*
 * Copyright (c) 2023-2024 Morwenn
 * SPDX-License-Identifier: MIT
 */
#include <algorithm>
#include <cstddef>
#include <functional>
#include <iterator>
#include <list>
#include <span>
#include <vector>
#include <catch2/catch_test_macros.hpp>
#include <cpp-sort/adapters/hybrid_adapter.h>
#include <cpp-sort/metrics/moves.h>
#include <cpp-sort/sorters/heap_sorter.h>
#include <cpp-sort/sorters/insertion_sorter.h>
#include <testing-tools/algorithm.h>
#include <testing-tools/distributions.h>
#include <testing-tools/wrapper.h>

using wrapper = generic_wrapper<int>;

TEST_CASE( "basic metrics::moves tests",
           "[metrics][heap_sorter]" )
{
    cppsort::metrics::moves<cppsort::heap_sorter> sorter;

    SECTION( "without projections" )
    {
        std::vector<int> collection;
        auto distribution = dist::descending_plateau{};
        distribution(std::back_inserter(collection), 65);

        auto res = sorter(collection);
        CHECK( res == 463 );
        CHECK( std::is_sorted(collection.begin(), collection.end()) );
    }

    SECTION( "with projections" )
    {
        std::vector<wrapper> collection;
        auto distribution = dist::descending_plateau{};
        distribution(std::back_inserter(collection), 80);

        auto res = sorter(collection, &wrapper::value);
        CHECK( res == 573 );
        CHECK( helpers::is_sorted(collection.begin(), collection.end(),
                                  std::less<>{}, &wrapper::value) );
    }
}

TEST_CASE( "metrics::moves with span",
           "[metrics][span][heap_sorter]" )
{
    cppsort::metrics::moves<cppsort::heap_sorter> sorter;

    SECTION( "without projections" )
    {
        std::vector<int> collection;
        auto distribution = dist::descending_plateau{};
        distribution(std::back_inserter(collection), 65);

        auto res = sorter(std::span(collection));
        CHECK( res == 463 );
        CHECK( std::is_sorted(collection.begin(), collection.end()) );
    }

    SECTION( "with projections" )
    {
        std::vector<wrapper> collection;
        auto distribution = dist::descending_plateau{};
        distribution(std::back_inserter(collection), 80);

        auto res = sorter(std::span(collection), &wrapper::value);
        CHECK( res == 573 );
        CHECK( helpers::is_sorted(collection.begin(), collection.end(),
                                  std::less<>{}, &wrapper::value) );
    }
}

TEST_CASE( "metrics::moves over iterator-category-sensitive sorters",
           "[metrics][hybriad_adapter][heap_sorter][insertion_sorter]" )
{
    cppsort::metrics::moves<
        cppsort::hybrid_adapter<
            cppsort::insertion_sorter,
            cppsort::heap_sorter
        >
    > sorter;
    auto distribution = dist::descending_plateau{};

    std::vector<int> vec;
    distribution(std::back_inserter(vec), 65);
    std::list<int> li(vec.begin(), vec.end());

    {
        auto res = sorter(vec);
        CHECK( res == 463 );
        CHECK( std::is_sorted(vec.begin(), vec.end()) );
    }
    {
        auto res = sorter(li);
        CHECK( res == 1977 );
        CHECK( std::is_sorted(li.begin(), li.end()) );
    }
}
