/*
 * Copyright (c) 2016-2021 Morwenn
 * SPDX-License-Identifier: MIT
 */
#include <algorithm>
#include <iterator>
#include <vector>
#include <catch2/catch.hpp>
#include <cpp-sort/adapters/indirect_adapter.h>
#include <cpp-sort/adapters/stable_adapter.h>
#include <cpp-sort/sorters.h>
#include <testing-tools/distributions.h>

TEMPLATE_TEST_CASE( "every random-access sorter with indirect adapter", "[indirect_adapter]",
                    cppsort::cartesian_tree_sorter,
                    cppsort::default_sorter,
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
                    cppsort::ska_sorter,
                    cppsort::slab_sorter,
                    cppsort::smooth_sorter,
                    cppsort::spin_sorter,
                    cppsort::split_sorter,
                    cppsort::spread_sorter,
                    cppsort::std_sorter,
                    cppsort::stable_adapter<cppsort::std_sorter>,
                    cppsort::tim_sorter,
                    cppsort::verge_sorter,
                    cppsort::wiki_sorter<> )
{
    std::vector<double> collection; collection.reserve(412);
    auto distribution = dist::shuffled{};
    distribution.call<double>(std::back_inserter(collection), 412, -125);

    cppsort::indirect_adapter<TestType> sorter;
    sorter(collection);
    CHECK( std::is_sorted(std::begin(collection), std::end(collection)) );
}

TEMPLATE_TEST_CASE( "every bidirectional sorter with indirect_adapter", "[indirect_adapter]",
                    cppsort::cartesian_tree_sorter,
                    cppsort::drop_merge_sorter,
                    cppsort::insertion_sorter,
                    cppsort::mel_sorter,
                    cppsort::merge_sorter,
                    cppsort::pdq_sorter, // Check extended support
                    cppsort::quick_merge_sorter,
                    cppsort::quick_sorter,
                    cppsort::selection_sorter,
                    cppsort::slab_sorter,
                    cppsort::verge_sorter )
{
    std::list<double> collection;
    auto distribution = dist::shuffled{};
    distribution.call<double>(std::back_inserter(collection), 412, -125);

    cppsort::indirect_adapter<TestType> sorter;
    sorter(collection);
    CHECK( std::is_sorted(std::begin(collection), std::end(collection)) );
}

TEMPLATE_TEST_CASE( "every forward sorter with with indirect_adapter", "[indirect_adapter]",
                    cppsort::cartesian_tree_sorter,
                    cppsort::mel_sorter,
                    cppsort::merge_sorter,
                    cppsort::pdq_sorter, // Check extended support
                    cppsort::quick_merge_sorter,
                    cppsort::quick_sorter,
                    cppsort::selection_sorter )
{
    std::forward_list<double> collection;
    auto distribution = dist::shuffled{};
    distribution.call<double>(std::front_inserter(collection), 412, -125);

    cppsort::indirect_adapter<TestType> sorter;
    sorter(collection);
    CHECK( std::is_sorted(std::begin(collection), std::end(collection)) );
}
