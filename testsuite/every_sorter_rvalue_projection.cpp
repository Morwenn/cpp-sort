/*
 * Copyright (c) 2021 Morwenn
 * SPDX-License-Identifier: MIT
 */
#include <algorithm>
#include <deque>
#include <forward_list>
#include <functional>
#include <iterator>
#include <list>
#include <vector>
#include <catch2/catch.hpp>
#include <cpp-sort/sorters.h>
#include <testing-tools/distributions.h>

TEMPLATE_TEST_CASE( "random-access sorters with a projection returning an rvalue", "[sorters][projection]",
                    cppsort::block_sorter<>,
                    cppsort::cartesian_tree_sorter,
                    cppsort::drop_merge_sorter,
                    cppsort::grail_sorter<>,
                    cppsort::heap_sorter,
                    cppsort::insertion_sorter,
                    cppsort::mel_sorter,
                    cppsort::merge_insertion_sorter,
                    cppsort::merge_sorter,
                    cppsort::pdq_sorter,
                    cppsort::poplar_sorter,
                    cppsort::quick_merge_sorter,
                    cppsort::quick_sorter,
                    cppsort::selection_sorter,
                    cppsort::slab_sorter,
                    cppsort::ska_sorter,
                    cppsort::smooth_sorter,
                    cppsort::spin_sorter,
                    cppsort::split_sorter,
                    cppsort::std_sorter,
                    cppsort::tim_sorter,
                    cppsort::verge_sorter )
{
    // This test is meant to check that sorters can correctly handle
    // projections that return an rvalue, we use std::negate<int> as
    // the projection for simplicity

    auto distribution = dist::shuffled{};

    SECTION( "std::vector" )
    {
        std::vector<int> collection;
        distribution(std::back_inserter(collection), 50);

        TestType sorter;
        sorter(collection, std::negate<>{});
        CHECK( std::is_sorted(collection.begin(), collection.end(), std::greater<>{}) );
    }

    SECTION( "std::deque" )
    {
        std::deque<int> collection;
        distribution(std::back_inserter(collection), 50);

        TestType sorter;
        sorter(collection, std::negate<>{});
        CHECK( std::is_sorted(collection.begin(), collection.end(), std::greater<>{}) );
    }
}

TEMPLATE_TEST_CASE( "bidirectional sorters with a projection returning an rvalue", "[sorters][projection]",
                    cppsort::drop_merge_sorter,
                    cppsort::insertion_sorter,
                    cppsort::mel_sorter,
                    cppsort::merge_sorter,
                    cppsort::quick_merge_sorter,
                    cppsort::quick_sorter,
                    cppsort::selection_sorter,
                    cppsort::verge_sorter )
{
    SECTION( "std::list" )
    {
        std::list<int> collection;
        auto distribution = dist::shuffled{};
        distribution(std::back_inserter(collection), 50);

        TestType sorter;
        sorter(collection, std::negate<>{});
        CHECK( std::is_sorted(collection.begin(), collection.end(), std::greater<>{}) );
    }
}

TEMPLATE_TEST_CASE( "forward sorters with a projection returning an rvalue", "[sorters][projection]",
                    cppsort::mel_sorter,
                    cppsort::merge_sorter,
                    cppsort::quick_merge_sorter,
                    cppsort::quick_sorter,
                    cppsort::selection_sorter )
{
    SECTION( "std::forward_list" )
    {
        std::forward_list<int> collection;
        auto distribution = dist::shuffled{};
        distribution(std::front_inserter(collection), 50);

        TestType sorter;
        sorter(collection, std::negate<>{});
        CHECK( std::is_sorted(collection.begin(), collection.end(), std::greater<>{}) );
    }
}
