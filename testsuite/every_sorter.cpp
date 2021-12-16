/*
 * Copyright (c) 2016-2021 Morwenn
 * SPDX-License-Identifier: MIT
 */
#include <algorithm>
#include <deque>
#include <forward_list>
#include <iterator>
#include <list>
#include <vector>
#include <catch2/catch.hpp>
#include <cpp-sort/sorters.h>
#include <cpp-sort/utility/buffer.h>
#include <cpp-sort/utility/functional.h>
#include <testing-tools/distributions.h>

TEMPLATE_TEST_CASE( "test every random-access sorter", "[sorters]",
                    cppsort::cartesian_tree_sorter,
                    cppsort::counting_sorter,
                    cppsort::drop_merge_sorter,
                    cppsort::grail_sorter<>,
                    cppsort::grail_sorter<
                        cppsort::utility::dynamic_buffer<cppsort::utility::sqrt>
                    >,
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
                    cppsort::ska_sorter,
                    cppsort::slab_sorter,
                    cppsort::smooth_sorter,
                    cppsort::spin_sorter,
                    cppsort::split_sorter,
                    cppsort::std_sorter,
                    cppsort::tim_sorter,
                    cppsort::verge_sorter,
                    cppsort::wiki_sorter<>,
                    cppsort::wiki_sorter<
                        cppsort::utility::dynamic_buffer<cppsort::utility::half>
                    > )
{
    // General test to make sure that every sorter compiles fine
    // and is able to sort a vector of numbers. spread_sorter is
    // already tested in-depth somewhere else and needs specific
    // tests, so it's not included here.

    auto distribution = dist::shuffled{};

    SECTION( "with std::vector" )
    {
        std::vector<int> collection; collection.reserve(491);
        distribution(std::back_inserter(collection), 491, -125);

        TestType sorter;
        sorter(collection);
        CHECK( std::is_sorted(collection.begin(), collection.end()) );
    }

    SECTION( "with std::deque" )
    {
        std::deque<int> collection;
        distribution(std::back_inserter(collection), 491, -125);

        TestType sorter;
        sorter(collection);
        CHECK( std::is_sorted(collection.begin(), collection.end()) );
    }
}

TEMPLATE_TEST_CASE( "test every bidirectional sorter", "[sorters]",
                    cppsort::cartesian_tree_sorter,
                    cppsort::counting_sorter,
                    cppsort::drop_merge_sorter,
                    cppsort::insertion_sorter,
                    cppsort::mel_sorter,
                    cppsort::merge_sorter,
                    cppsort::quick_merge_sorter,
                    cppsort::quick_sorter,
                    cppsort::selection_sorter,
                    cppsort::slab_sorter,
                    cppsort::verge_sorter )
{
    std::list<int> collection;
    auto distribution = dist::shuffled{};
    distribution(std::back_inserter(collection), 491, -125);

    TestType sorter;
    sorter(collection);
    CHECK( std::is_sorted(collection.begin(), collection.end()) );
}

TEMPLATE_TEST_CASE( "test every forward sorter", "[sorters]",
                    cppsort::cartesian_tree_sorter,
                    cppsort::counting_sorter,
                    cppsort::mel_sorter,
                    cppsort::merge_sorter,
                    cppsort::quick_merge_sorter,
                    cppsort::quick_sorter,
                    cppsort::selection_sorter )
{
    std::forward_list<int> collection;
    auto distribution = dist::shuffled{};
    distribution(std::front_inserter(collection), 491, -125);

    TestType sorter;
    sorter(collection);
    CHECK( std::is_sorted(collection.begin(), collection.end()) );
}
