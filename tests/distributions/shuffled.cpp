/*
 * Copyright (c) 2017-2024 Morwenn
 * SPDX-License-Identifier: MIT
 */
#include <deque>
#include <forward_list>
#include <list>
#include <vector>
#include <catch2/catch_template_test_macros.hpp>
#include <cpp-sort/sorters.h>
#include <cpp-sort/utility/buffer.h>
#include <cpp-sort/utility/functional.h>
#include <testing-tools/distributions.h>
#include "test_distribution.h"

TEMPLATE_TEST_CASE( "test random-access sorters with shuffled distribution", "[distributions]",
                    cppsort::adaptive_shivers_sorter,
                    cppsort::cartesian_tree_sorter,
                    cppsort::counting_sorter,
                    cppsort::d_ary_heap_sorter<2>,
                    cppsort::d_ary_heap_sorter<3>,
                    cppsort::d_ary_heap_sorter<4>,
                    cppsort::d_ary_heap_sorter<5>,
                    cppsort::d_ary_heap_sorter<6>,
                    cppsort::d_ary_heap_sorter<7>,
                    cppsort::d_ary_heap_sorter<8>,
                    cppsort::d_ary_heap_sorter<9>,
                    cppsort::drop_merge_sorter,
                    cppsort::grail_sorter<>,
                    cppsort::grail_sorter<
                        cppsort::utility::dynamic_buffer<cppsort::utility::sqrt>
                    >,
                    cppsort::heap_sorter,
                    cppsort::insertion_sorter,
                    cppsort::mel_sorter,
                    cppsort::merge_sorter,
                    cppsort::merge_insertion_sorter,
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
                    cppsort::split_sorter,
                    cppsort::spread_sorter,
                    cppsort::std_sorter,
                    cppsort::tim_sorter,
                    cppsort::verge_sorter,
                    cppsort::wiki_sorter<>,
                    cppsort::wiki_sorter<
                        cppsort::utility::dynamic_buffer<cppsort::utility::half>
                    > )
{
    SECTION( "with std::vector" )
    {
        std::vector<int> collection;
        helpers::test_distribution<TestType>(collection, 10'000, dist::shuffled{}, -2500);
    }

    SECTION( "with std::deque" )
    {
        std::deque<int> collection;
        helpers::test_distribution<TestType>(collection, 10'000, dist::shuffled{}, -2500);
    }
}

TEMPLATE_TEST_CASE( "test bidirectional sorters with shuffled distribution", "[distributions]",
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
                    cppsort::splay_sorter,
                    cppsort::verge_sorter )
{
    std::list<int> collection;
    helpers::test_distribution<TestType>(collection, 2500, dist::shuffled{}, -1000);
}

TEMPLATE_TEST_CASE( "test forward sorters with shuffled distribution", "[distributions]",
                    cppsort::cartesian_tree_sorter,
                    cppsort::counting_sorter,
                    cppsort::mel_sorter,
                    cppsort::merge_sorter,
                    cppsort::quick_merge_sorter,
                    cppsort::quick_sorter,
                    cppsort::selection_sorter,
                    cppsort::splay_sorter )
{
    std::forward_list<int> collection;
    helpers::test_distribution<TestType>(collection, 2500, dist::shuffled{}, -1000);
}
