/*
 * Copyright (c) 2019-2022 Morwenn
 * SPDX-License-Identifier: MIT
 */
#include <algorithm>
#include <forward_list>
#include <iterator>
#include <list>
#include <vector>
#include <catch2/catch_template_test_macros.hpp>
#include <cpp-sort/sorters.h>
#include <testing-tools/distributions.h>
#include <testing-tools/memory_exhaustion.h>

//
// Specific tests to check that a selection of sorters still run fine
// even when there isn't any extra heap memory available
//
// These tests shouldn't be part of the main test suite executable
//

TEMPLATE_TEST_CASE( "heap exhaustion for random-access sorters", "[sorters][heap_exhaustion]",
                    cppsort::grail_sorter<>,
                    cppsort::heap_sorter,
                    cppsort::insertion_sorter,
                    cppsort::merge_sorter,
                    cppsort::pdq_sorter,
                    cppsort::quick_merge_sorter,
                    cppsort::quick_sorter,
                    cppsort::selection_sorter,
                    cppsort::ska_sorter,
                    cppsort::smooth_sorter,
                    cppsort::split_sorter,
                    cppsort::std_sorter,
                    cppsort::wiki_sorter<> )
{
    std::vector<int> collection; collection.reserve(491);
    auto distribution = dist::shuffled{};
    distribution(std::back_inserter(collection), 491, -125);

    using sorter = TestType;
    {
        scoped_memory_exhaustion _;
        sorter{}(collection);
    }
    CHECK( std::is_sorted(std::begin(collection), std::end(collection)) );
}

TEMPLATE_TEST_CASE( "heap exhaustion for bidirectional sorters", "[sorters][heap_exhaustion]",
                    cppsort::insertion_sorter,
                    cppsort::merge_sorter,
                    cppsort::quick_merge_sorter,
                    cppsort::quick_sorter,
                    cppsort::selection_sorter )
{
    std::list<int> collection;
    auto distribution = dist::shuffled{};
    distribution(std::back_inserter(collection), 491, -125);

    using sorter = TestType;
    {
        scoped_memory_exhaustion _;
        sorter{}(collection);
    }
    CHECK( std::is_sorted(std::begin(collection), std::end(collection)) );
}

TEMPLATE_TEST_CASE( "heap exhaustion for forward sorters", "[sorters][heap_exhaustion]",
                    cppsort::merge_sorter,
                    cppsort::quick_merge_sorter,
                    cppsort::quick_sorter,
                    cppsort::selection_sorter )
{
    std::forward_list<int> collection;
    auto distribution = dist::shuffled{};
    distribution(std::front_inserter(collection), 491, -125);

    using sorter = TestType;
    {
        scoped_memory_exhaustion _;
        sorter{}(collection);
    }
    CHECK( std::is_sorted(std::begin(collection), std::end(collection)) );
}
