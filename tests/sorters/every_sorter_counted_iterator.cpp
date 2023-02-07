/*
 * Copyright (c) 2023 Morwenn
 * SPDX-License-Identifier: MIT
 */
#include <algorithm>
#include <forward_list>
#include <iterator>
#include <list>
#include <vector>
#include <catch2/catch_template_test_macros.hpp>
#include <cpp-sort/adapters/split_adapter.h>
#include <cpp-sort/adapters/verge_adapter.h>
#include <cpp-sort/sorters.h>
#include <testing-tools/distributions.h>
#include <testing-tools/old_sorters.h>
#include <testing-tools/random.h>

TEMPLATE_TEST_CASE( "test every random-access sorter with counted_iterator",
                    "[sorters][sentinel][counted_iterator]",
                    old_default_sorter,
                    cppsort::adaptive_shivers_sorter,
                    cppsort::cartesian_tree_sorter,
                    cppsort::counting_sorter,
                    cppsort::d_ary_heap_sorter<5>,
                    old_drop_merge_sorter,
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
                    cppsort::splay_sorter,
                    old_split_sorter,
                    cppsort::spread_sorter,
                    cppsort::std_sorter,
                    cppsort::tim_sorter,
                    cppsort::verge_adapter<cppsort::slab_sorter>,
                    old_verge_sorter,
                    cppsort::wiki_sorter<> )
{
    std::vector<int> collection;
    auto distribution = dist::shuffled{};
    distribution(std::back_inserter(collection), 1000, -100);

    TestType sorter;
    auto begin = std::counted_iterator(collection.begin(), 1000);
    auto end = sorter(begin, std::default_sentinel);
    CHECK( end.base() == collection.end() );
    CHECK( std::is_sorted(collection.begin(), collection.end()) );
}

TEMPLATE_TEST_CASE( "test every bidirectional sorter with counted_iterator",
                    "[sorters][sentinel][counted_iterator]",
                    old_default_sorter,
                    cppsort::cartesian_tree_sorter,
                    cppsort::counting_sorter,
                    old_drop_merge_sorter,
                    cppsort::insertion_sorter,
                    cppsort::mel_sorter,
                    cppsort::merge_sorter,
                    cppsort::quick_merge_sorter,
                    cppsort::quick_sorter,
                    cppsort::selection_sorter,
                    cppsort::slab_sorter,
                    cppsort::splay_sorter,
                    cppsort::split_adapter<cppsort::quick_merge_sorter> )
{
    std::vector<int> vec;
    auto distribution = dist::shuffled{};
    distribution(std::back_inserter(vec), 1000, -100);
    std::list<int> collection(vec.begin(), vec.end());

    TestType sorter;
    auto begin = std::counted_iterator(collection.begin(), 1000);
    auto end = sorter(begin, std::default_sentinel);
    CHECK( end.base() == collection.end() );
    CHECK( std::is_sorted(collection.begin(), collection.end()) );
}

TEMPLATE_TEST_CASE( "test every forward sorter with counted_iterator",
                    "[sorters][sentinel][counted_iterator]",
                    old_default_sorter,
                    cppsort::cartesian_tree_sorter,
                    cppsort::counting_sorter,
                    cppsort::mel_sorter,
                    cppsort::merge_sorter,
                    cppsort::quick_merge_sorter,
                    cppsort::quick_sorter,
                    cppsort::selection_sorter,
                    cppsort::splay_sorter )
{
    std::vector<int> vec;
    auto distribution = dist::shuffled{};
    distribution(std::back_inserter(vec), 1000, -100);
    std::forward_list<int> collection(vec.begin(), vec.end());

    TestType sorter;
    auto begin = std::counted_iterator(collection.begin(), 1000);
    auto end = sorter(begin, std::default_sentinel);
    CHECK( end.base() == collection.end() );
    CHECK( std::is_sorted(collection.begin(), collection.end()) );
}
