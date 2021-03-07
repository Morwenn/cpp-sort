/*
 * Copyright (c) 2020-2021 Morwenn
 * SPDX-License-Identifier: MIT
 */
#include <algorithm>
#include <iterator>
#include <vector>
#include <catch2/catch.hpp>
#include <cpp-sort/sorters.h>
#include <cpp-sort/utility/buffer.h>
#include <cpp-sort/utility/functional.h>
#include <testing-tools/distributions.h>

TEMPLATE_TEST_CASE( "test random-access sorters with median_of_3_killer distribution", "[distributions]",
                    cppsort::cartesian_tree_sorter,
                    cppsort::block_sorter<>,
                    cppsort::block_sorter<
                        cppsort::utility::dynamic_buffer<cppsort::utility::half>
                    >,
                    cppsort::drop_merge_sorter,
                    cppsort::grail_sorter<>,
                    cppsort::grail_sorter<
                        cppsort::utility::dynamic_buffer<cppsort::utility::sqrt>
                    >,
                    cppsort::heap_sorter,
                    cppsort::mel_sorter,
                    cppsort::merge_sorter,
                    cppsort::pdq_sorter,
                    cppsort::poplar_sorter,
                    cppsort::quick_merge_sorter,
                    cppsort::quick_sorter,
                    cppsort::ska_sorter,
                    cppsort::smooth_sorter,
                    cppsort::spin_sorter,
                    cppsort::split_sorter,
                    cppsort::spread_sorter,
                    cppsort::std_sorter,
                    cppsort::tim_sorter,
                    cppsort::verge_sorter )
{
    std::vector<int> collection;
    collection.reserve(1000);
    auto distribution = dist::median_of_3_killer{};
    distribution(std::back_inserter(collection), 1000);

    TestType sorter;
    sorter(collection);
    CHECK( std::is_sorted(collection.begin(), collection.end()) );
}

TEMPLATE_TEST_CASE( "test bidirectional sorters with median_of_3_killer distribution", "[distributions]",
                    cppsort::drop_merge_sorter,
                    cppsort::merge_sorter,
                    cppsort::quick_merge_sorter,
                    cppsort::quick_sorter,
                    cppsort::verge_sorter )
{
    std::list<int> collection;
    auto distribution = dist::median_of_3_killer{};
    distribution(std::back_inserter(collection), 1000);

    TestType sorter;
    sorter(collection);
    CHECK( std::is_sorted(collection.begin(), collection.end()) );
}
