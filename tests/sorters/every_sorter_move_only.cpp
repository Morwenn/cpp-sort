/*
 * Copyright (c) 2016-2023 Morwenn
 * SPDX-License-Identifier: MIT
 */
#include <algorithm>
#include <iterator>
#include <vector>
#include <catch2/catch_template_test_macros.hpp>
#include <cpp-sort/sorters.h>
#include <cpp-sort/utility/buffer.h>
#include <testing-tools/distributions.h>
#include <testing-tools/move_only.h>
#include <testing-tools/old_sorters.h>

TEMPLATE_TEST_CASE( "test every sorter with move-only types", "[sorters]",
                    old_default_sorter,
                    cppsort::adaptive_shivers_sorter,
                    cppsort::cartesian_tree_sorter,
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
                    cppsort::smooth_sorter,
                    cppsort::spin_sorter,
                    cppsort::splay_sorter,
                    old_split_sorter,
                    cppsort::std_sorter,
                    cppsort::tim_sorter,
                    old_verge_sorter,
                    cppsort::wiki_sorter<cppsort::utility::fixed_buffer<0>> )
{
    // General test to make sure that every sorter compiles fine with
    // move-only types, additionally checking that no read-after-move
    // operation is performed and that no self-move is performed

    std::vector<move_only<double>> collection;
    collection.reserve(491);
    auto distribution = dist::shuffled{};
    distribution.call<double>(std::back_inserter(collection), 491, -125);

    TestType sorter;
    sorter(collection);
    CHECK( std::is_sorted(collection.begin(), collection.end()) );
}
