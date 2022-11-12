/*
 * Copyright (c) 2021-2022 Morwenn
 * SPDX-License-Identifier: MIT
 */
#include <algorithm>
#include <cstdint>
#include <iterator>
#include <catch2/catch_template_test_macros.hpp>
#include <cpp-sort/sorters.h>
#include <cpp-sort/utility/buffer.h>
#include <cpp-sort/utility/functional.h>
#include <testing-tools/distributions.h>
#include <testing-tools/test_vector.h>

TEMPLATE_TEST_CASE( "test every sorter with an int8_t difference_type", "[sorters]",
                    cppsort::adaptive_shivers_sorter,
                    cppsort::cartesian_tree_sorter,
                    cppsort::counting_sorter,
                    cppsort::d_ary_heap_sorter<4>,
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
    // Test that sorters work as expected when the iterator to sort has
    // a small difference_type, despite potential promotions to int

    test_vector<int, std::int8_t> collection(127);
    auto distribution = dist::shuffled{};
    distribution(std::back_inserter(collection), 127);

    TestType sorter;
    sorter(collection);
    CHECK( std::is_sorted(collection.begin(), collection.end()) );
}
