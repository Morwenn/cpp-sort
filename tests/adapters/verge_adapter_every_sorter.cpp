/*
 * Copyright (c) 2017-2022 Morwenn
 * SPDX-License-Identifier: MIT
 */
#include <algorithm>
#include <iterator>
#include <vector>
#include <catch2/catch_template_test_macros.hpp>
#include <cpp-sort/adapters/stable_adapter.h>
#include <cpp-sort/adapters/verge_adapter.h>
#include <cpp-sort/sorters.h>
#include <cpp-sort/utility/buffer.h>
#include <testing-tools/algorithm.h>
#include <testing-tools/distributions.h>
#include <testing-tools/wrapper.h>

TEMPLATE_TEST_CASE( "every sorter with verge_adapter", "[verge_adapter]",
                    cppsort::adaptive_shivers_sorter,
                    cppsort::cartesian_tree_sorter,
                    cppsort::d_ary_heap_sorter<4>,
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
    std::vector<double> collection; collection.reserve(412);
    auto distribution = dist::shuffled{};
    distribution.call<double>(std::back_inserter(collection), 412, -125);

    cppsort::verge_adapter<TestType> sorter;
    sorter(collection);
    CHECK( std::is_sorted(collection.begin(), collection.end()) );
}

TEMPLATE_TEST_CASE( "every sorter with stable verge_adapter", "[verge_adapter][stable_adapter]",
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
                    cppsort::smooth_sorter,
                    cppsort::spin_sorter,
                    cppsort::split_sorter,
                    cppsort::std_sorter,
                    cppsort::tim_sorter,
                    cppsort::wiki_sorter<cppsort::utility::fixed_buffer<0>> )
{
    using wrapper = generic_stable_wrapper<int>;
    std::vector<wrapper> collection; collection.reserve(400);
    auto distribution = dist::descending_plateau{};
    distribution(std::back_inserter(collection), 400);
    helpers::iota(collection.begin(), collection.end(), 0, &wrapper::order);

    cppsort::stable_adapter<cppsort::verge_adapter<TestType>> sorter;
    sorter(collection, &wrapper::value);
    CHECK( std::is_sorted(collection.begin(), collection.end()) );
}
