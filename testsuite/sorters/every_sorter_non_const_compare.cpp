/*
 * Copyright (c) 2020-2022 Morwenn
 * SPDX-License-Identifier: MIT
 */
#include <algorithm>
#include <iterator>
#include <vector>
#include <catch2/catch_template_test_macros.hpp>
#include <cpp-sort/sorters.h>
#include <cpp-sort/utility/buffer.h>
#include <testing-tools/distributions.h>

TEMPLATE_TEST_CASE( "test extended compatibility with LWG 3031", "[sorters]",
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
                    cppsort::slab_sorter,
                    cppsort::smooth_sorter,
                    cppsort::spin_sorter,
                    cppsort::split_sorter,
                    cppsort::std_sorter,
                    cppsort::tim_sorter,
                    cppsort::verge_sorter,
                    cppsort::wiki_sorter<cppsort::utility::fixed_buffer<0>> )
{
    // LWG3031 allows algorithms taking a predicate to work correctly when
    // said predicate takes non-const references on either side as long as
    // it doesn't modify its parameters

    std::vector<int> collection;
    collection.reserve(50);
    auto distribution = dist::shuffled{};
    distribution(std::back_inserter(collection), 50, -25);

    TestType sorter;
    sorter(collection, [](int& lhs, int& rhs) { return lhs < rhs; });
    CHECK( std::is_sorted(collection.begin(), collection.end()) );
}
