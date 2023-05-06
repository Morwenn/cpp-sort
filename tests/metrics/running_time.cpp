/*
 * Copyright (c) 2023 Morwenn
 * SPDX-License-Identifier: MIT
 */
#include <algorithm>
#include <chrono>
#include <iterator>
#include <list>
#include <catch2/catch_test_macros.hpp>
#include <cpp-sort/metrics/running_time.h>
#include <cpp-sort/sorters/splay_sorter.h>
#include <testing-tools/distributions.h>

TEST_CASE( "basic metrics::running_time tests", "[metrics]" )
{
    using namespace std::chrono_literals;

    std::list<int> collection;
    auto distribution = dist::shuffled{};
    distribution(std::back_inserter(collection), 102);

    SECTION( "with default duration type" )
    {
        cppsort::metrics::running_time<
            cppsort::splay_sorter
        > sorter;

        auto res = sorter(collection);
        CHECK( res > 0s );
        CHECK( std::is_sorted(collection.begin(), collection.end()) );
    }

    SECTION( "with explicit duration type" )
    {
        cppsort::metrics::running_time<
            cppsort::splay_sorter,
            std::chrono::microseconds
        > sorter;

        auto res = sorter(collection);
        CHECK( res > 0s );
        CHECK( res < 1s );
        CHECK( std::is_sorted(collection.begin(), collection.end()) );
    }

}
