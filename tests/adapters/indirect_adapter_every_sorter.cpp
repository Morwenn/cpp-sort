/*
 * Copyright (c) 2016-2023 Morwenn
 * SPDX-License-Identifier: MIT
 */
#include <algorithm>
#include <forward_list>
#include <iterator>
#include <list>
#include <vector>
#include <catch2/catch_template_test_macros.hpp>
#include <cpp-sort/adapters/indirect_adapter.h>
#include <cpp-sort/adapters/stable_adapter.h>
#include <cpp-sort/sorters.h>
#include <testing-tools/distributions.h>
#include <testing-tools/old_sorters.h>

TEMPLATE_TEST_CASE( "every random-access sorter with indirect adapter", "[indirect_adapter]",
                    old_default_sorter,
                    cppsort::adaptive_shivers_sorter,
                    cppsort::cartesian_tree_sorter,
                    cppsort::d_ary_heap_sorter<7>,
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
                    cppsort::ska_sorter,
                    cppsort::slab_sorter,
                    cppsort::smooth_sorter,
                    cppsort::spin_sorter,
                    cppsort::splay_sorter,
                    old_split_sorter,
                    cppsort::spread_sorter,
                    cppsort::std_sorter,
                    cppsort::stable_adapter<cppsort::std_sorter>,
                    cppsort::tim_sorter,
                    old_verge_sorter,
                    cppsort::wiki_sorter<> )
{
    std::vector<double> collection; collection.reserve(412);
    auto distribution = dist::shuffled{};
    distribution.call<double>(std::back_inserter(collection), 412, -125);

    cppsort::indirect_adapter<TestType> sorter;
    sorter(collection);
    CHECK( std::is_sorted(collection.begin(), collection.end()) );
}

TEMPLATE_TEST_CASE( "every bidirectional sorter with indirect_adapter", "[indirect_adapter]",
                    cppsort::cartesian_tree_sorter,
                    old_drop_merge_sorter,
                    cppsort::insertion_sorter,
                    cppsort::mel_sorter,
                    cppsort::merge_sorter,
                    cppsort::pdq_sorter, // Check extended support
                    cppsort::quick_merge_sorter,
                    cppsort::quick_sorter,
                    cppsort::selection_sorter,
                    cppsort::slab_sorter,
                    cppsort::splay_sorter,
                    old_verge_sorter )
{
    std::list<double> collection;
    auto distribution = dist::shuffled{};
    distribution.call<double>(std::back_inserter(collection), 412, -125);

    cppsort::indirect_adapter<TestType> sorter;
    sorter(collection);
    CHECK( std::is_sorted(collection.begin(), collection.end()) );
}

TEMPLATE_TEST_CASE( "every forward sorter with with indirect_adapter", "[indirect_adapter]",
                    cppsort::cartesian_tree_sorter,
                    cppsort::mel_sorter,
                    cppsort::merge_sorter,
                    cppsort::pdq_sorter, // Check extended support
                    cppsort::quick_merge_sorter,
                    cppsort::quick_sorter,
                    cppsort::selection_sorter,
                    cppsort::splay_sorter )
{
    std::forward_list<double> collection;
    auto distribution = dist::shuffled{};
    distribution.call<double>(std::front_inserter(collection), 412, -125);

    cppsort::indirect_adapter<TestType> sorter;
    sorter(collection);
    CHECK( std::is_sorted(collection.begin(), collection.end()) );
}
