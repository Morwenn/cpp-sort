/*
 * Copyright (c) 2024 Morwenn
 * SPDX-License-Identifier: MIT
 */
#include <algorithm>
#include <deque>
#include <iterator>
#include <ranges>
#include <span>
#include <vector>
#include <catch2/catch_template_test_macros.hpp>
#include <cpp-sort/sorters.h>
#include <cpp-sort/utility/buffer.h>
#include <cpp-sort/utility/functional.h>
#include <testing-tools/distributions.h>
#include <testing-tools/old_sorters.h>

TEMPLATE_TEST_CASE( "test every sorter with std::views::zip", "[sorters][zip]",
                    old_default_sorter,
                    cppsort::adaptive_shivers_sorter,
                    cppsort::cartesian_tree_sorter,
                    cppsort::d_ary_heap_sorter<2>,
                    old_drop_merge_sorter,
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
                    cppsort::slab_sorter,
                    cppsort::ska_sorter,
                    cppsort::smooth_sorter,
                    cppsort::spin_sorter,
                    cppsort::splay_sorter,
                    old_split_sorter,
                    cppsort::spread_sorter,
                    cppsort::std_sorter,
                    cppsort::tim_sorter,
                    old_verge_sorter,
                    cppsort::wiki_sorter<>,
                    cppsort::wiki_sorter<
                        cppsort::utility::dynamic_buffer<cppsort::utility::half>
                    > )
{
    // General test to make sure that every sorter compiles fine
    // and is able to sort two zipped collections, displaying
    // support for C++20 proxy iterators

    std::vector<int> collection1;
    collection1.reserve(491);
    auto distribution = dist::shuffled{};
    distribution(std::back_inserter(collection1), 491, -125);

    // Same shuffle₫ order as collection1, to make it easy to check
    // that sorting the zipped collections based on the first also
    // applies the same permutations to the second one
    std::deque<int> collection2(collection1.begin(), collection1.end());

    TestType sorter;
    sorter(
        std::views::zip(collection1, collection2),
        [](const auto& pair) { return std::get<0>(pair); }
    );
    CHECK( std::ranges::is_sorted(collection1) );
    CHECK( std::ranges::is_sorted(collection2) );
}
