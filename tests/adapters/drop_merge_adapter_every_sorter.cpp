/*
 * Copyright (c) 2022 Morwenn
 * SPDX-License-Identifier: MIT
 */
#include <algorithm>
#include <iterator>
#include <list>
#include <vector>
#include <catch2/catch_template_test_macros.hpp>
#include <cpp-sort/adapters/drop_merge_adapter.h>
#include <cpp-sort/sorters.h>
#include <cpp-sort/utility/buffer.h>
#include <testing-tools/distributions.h>

TEMPLATE_TEST_CASE( "every random-access sorter with drop_merge_adapter", "[drop_merge_adapter]",
                    cppsort::adaptive_shivers_sorter,
                    cppsort::cartesian_tree_sorter,
                    cppsort::d_ary_heap_sorter<6>,
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
                    cppsort::ska_sorter,
                    cppsort::smooth_sorter,
                    cppsort::spin_sorter,
                    cppsort::split_sorter,
                    cppsort::spread_sorter,
                    cppsort::std_sorter,
                    cppsort::tim_sorter,
                    cppsort::wiki_sorter<cppsort::utility::fixed_buffer<0>> )
{
    std::vector<double> collection;
    collection.reserve(1'000);
    auto distribution = dist::inversions(0.2); // Big enough merges
    distribution.call<double>(std::back_inserter(collection), 1'000);

    cppsort::drop_merge_adapter<TestType> sorter;
    sorter(collection);
    CHECK( std::is_sorted(collection.begin(), collection.end()) );
}

TEMPLATE_TEST_CASE( "every bidirectional sorter with drop_merge_adapter", "[drop_merge_adapter]",
                    cppsort::adaptive_shivers_sorter,
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
                    cppsort::ska_sorter,
                    cppsort::smooth_sorter,
                    cppsort::spin_sorter,
                    cppsort::split_sorter,
                    cppsort::spread_sorter,
                    cppsort::std_sorter,
                    cppsort::tim_sorter,
                    cppsort::wiki_sorter<cppsort::utility::fixed_buffer<0>> )
{
    std::list<double> collection;
    auto distribution = dist::inversions(0.2); // Big enough merges
    distribution.call<double>(std::back_inserter(collection), 1'000);

    cppsort::drop_merge_adapter<TestType> sorter;
    sorter(collection);
    CHECK( std::is_sorted(collection.begin(), collection.end()) );
}
