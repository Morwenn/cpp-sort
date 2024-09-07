/*
 * Copyright (c) 2024 Morwenn
 * SPDX-License-Identifier: MIT
 */
#include <algorithm>
#include <chrono>
#include <iterator>
#include <list>
#include <vector>
#include <catch2/catch_test_macros.hpp>
#include <cpp-sort/metrics.h>
#include <testing-tools/distributions.h>
#include <testing-tools/mutable_sorter.h>

TEST_CASE( "test mutable sorters with every metric",
           "[metrics][adapter_storage]" )
{
    std::vector<int> collection;
    collection.reserve(65);
    auto distribution = dist::shuffled{};
    distribution(std::back_inserter(collection), 65, 0);

    // Sorter that will be adapted by everything,
    // works with forward iterators
    auto original_sorter = mutable_sorter{};

    SECTION( "comparisons" )
    {
        cppsort::metrics::comparisons<mutable_sorter> sorter(original_sorter);

        std::list<int> li(collection.begin(), collection.end());
        auto res = sorter(li);
        CHECK( res > 0 );
        CHECK( std::is_sorted(li.begin(), li.end()) );
        CHECK( sorter.get().before_sort == mutable_state::modified );
        CHECK( sorter.get().after_sort == mutable_state::modified );
    }

    SECTION( "moves" )
    {
        cppsort::metrics::moves<mutable_sorter> sorter(original_sorter);

        auto res = sorter(collection);
        CHECK( res > 0 );
        CHECK( std::is_sorted(collection.begin(), collection.end()) );
        CHECK( sorter.get().before_sort == mutable_state::modified );
        CHECK( sorter.get().after_sort == mutable_state::modified );
    }

    SECTION( "projections" )
    {
        cppsort::metrics::projections<mutable_sorter> sorter(original_sorter);

        std::list<int> li(collection.begin(), collection.end());
        auto res = sorter(li);
        CHECK( res > 0 );
        CHECK( std::is_sorted(li.begin(), li.end()) );
        CHECK( sorter.get().before_sort == mutable_state::modified );
        CHECK( sorter.get().after_sort == mutable_state::modified );
    }

    SECTION( "running_time" )
    {
        using namespace std::chrono_literals;

        cppsort::metrics::running_time<mutable_sorter> sorter(original_sorter);

        auto res = sorter(collection);
        CHECK( res > 0s );
        CHECK( std::is_sorted(collection.begin(), collection.end()) );
        CHECK( sorter.get().before_sort == mutable_state::modified );
        CHECK( sorter.get().after_sort == mutable_state::modified );
    }
}