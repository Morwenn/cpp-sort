/*
 * Copyright (c) 2024 Morwenn
 * SPDX-License-Identifier: MIT
 */
#include <algorithm>
#include <forward_list>
#include <iterator>
#include <list>
#include <vector>
#include <catch2/catch_template_test_macros.hpp>
#include <cpp-sort/adapters.h>
#include <cpp-sort/sorters/mel_sorter.h>
#include <cpp-sort/sorters/quick_merge_sorter.h>
#include <testing-tools/distributions.h>
#include <testing-tools/memory_exhaustion.h>

//
// Specific tests to check that a selection of adapters still run fine
// even when there isn't any extra heap memory available
//
// These tests shouldn't be part of the main test suite executable
//

using Sorter = cppsort::quick_merge_sorter;
using HybridSorter = cppsort::hybrid_adapter<Sorter, cppsort::mel_sorter>;

TEMPLATE_TEST_CASE( "heap exhaustion for random-access adapters", "[adapters][heap_exhaustion]",
                    cppsort::container_aware_adapter<Sorter>,
                    cppsort::counting_adapter<Sorter>,
                    HybridSorter,
                    cppsort::self_sort_adapter<Sorter>,
                    cppsort::split_adapter<Sorter> )
{
    std::vector<int> collection; collection.reserve(3000);
    auto distribution = dist::shuffled{};
    distribution(std::back_inserter(collection), 491, -521);

    using sorter = TestType;
    {
        scoped_memory_exhaustion _;
        sorter{}(collection);
    }
    CHECK( std::is_sorted(collection.begin(), collection.end()) );
}

TEMPLATE_TEST_CASE( "heap exhaustion for bidirectional adapters", "[adapters][heap_exhaustion]",
                    cppsort::container_aware_adapter<Sorter>,
                    cppsort::counting_adapter<Sorter>,
                    HybridSorter,
                    cppsort::self_sort_adapter<Sorter>,
                    cppsort::split_adapter<Sorter> )
{
    std::list<int> collection;
    auto distribution = dist::shuffled{};
    distribution(std::back_inserter(collection), 491, -521);

    using sorter = TestType;
    {
        scoped_memory_exhaustion _;
        sorter{}(collection);
    }
    CHECK( std::is_sorted(collection.begin(), collection.end()) );
}

TEMPLATE_TEST_CASE( "heap exhaustion for forward adapters", "[adapters][heap_exhaustion]",
                    cppsort::container_aware_adapter<Sorter>,
                    cppsort::counting_adapter<Sorter>,
                    HybridSorter,
                    cppsort::self_sort_adapter<Sorter> )
{
    std::forward_list<int> collection;
    auto distribution = dist::shuffled{};
    distribution(std::front_inserter(collection), 491, -521);

    using sorter = TestType;
    {
        scoped_memory_exhaustion _;
        sorter{}(collection);
    }
    CHECK( std::is_sorted(collection.begin(), collection.end()) );
}
