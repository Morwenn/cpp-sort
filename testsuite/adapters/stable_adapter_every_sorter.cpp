/*
 * Copyright (c) 2016-2020 Morwenn
 * SPDX-License-Identifier: MIT
 */
#include <algorithm>
#include <forward_list>
#include <iterator>
#include <list>
#include <vector>
#include <catch2/catch.hpp>
#include <cpp-sort/adapters/stable_adapter.h>
#include <cpp-sort/sorters.h>
#include <cpp-sort/utility/buffer.h>
#include <testing-tools/algorithm.h>
#include <testing-tools/distributions.h>
#include <testing-tools/wrapper.h>

using wrapper = generic_stable_wrapper<int>;

TEMPLATE_TEST_CASE( "every random-access sorter with stable_adapter", "[stable_adapter]",
                    cppsort::block_sorter<cppsort::utility::fixed_buffer<0>>,
                    cppsort::default_sorter,
                    cppsort::drop_merge_sorter,
                    cppsort::grail_sorter<>,
                    cppsort::heap_sorter,
                    cppsort::insertion_sorter,
                    cppsort::merge_insertion_sorter,
                    cppsort::merge_sorter,
                    cppsort::pdq_sorter,
                    cppsort::poplar_sorter,
                    cppsort::quick_merge_sorter,
                    cppsort::quick_sorter,
                    cppsort::selection_sorter,
                    cppsort::smooth_sorter,
                    cppsort::spin_sorter,
                    cppsort::split_sorter,
                    cppsort::std_sorter,
                    cppsort::tim_sorter,
                    cppsort::verge_sorter )
{
    std::vector<wrapper> collection;
    auto distribution = dist::shuffled_16_values{};
    distribution(std::back_inserter(collection), 412);
    helpers::iota(collection.begin(), collection.end(), 0, &wrapper::order);

    cppsort::stable_t<TestType> sorter;
    sorter(collection, &wrapper::value);
    CHECK( std::is_sorted(collection.begin(), collection.end()) );
}

TEMPLATE_TEST_CASE( "every bidirectional sorter with stable_adapter", "[stable_adapter]",
                    cppsort::drop_merge_sorter,
                    cppsort::insertion_sorter,
                    cppsort::merge_sorter,
                    cppsort::quick_merge_sorter,
                    cppsort::quick_sorter,
                    cppsort::selection_sorter,
                    cppsort::verge_sorter )
{
    std::list<wrapper> collection;
    auto distribution = dist::shuffled_16_values{};
    distribution(std::back_inserter(collection), 412);
    helpers::iota(collection.begin(), collection.end(), 0, &wrapper::order);

    cppsort::stable_t<TestType> sorter;
    sorter(collection, &wrapper::value);
    CHECK( std::is_sorted(collection.begin(), collection.end()) );
}

TEMPLATE_TEST_CASE( "every forward sorter with with stable_adapter", "[stable_adapter]",
                    cppsort::merge_sorter,
                    cppsort::quick_merge_sorter,
                    cppsort::quick_sorter,
                    cppsort::selection_sorter )
{
    std::forward_list<wrapper> collection;
    auto distribution = dist::shuffled_16_values{};
    distribution(std::front_inserter(collection), 412);
    helpers::iota(collection.begin(), collection.end(), 0, &wrapper::order);

    cppsort::stable_t<TestType> sorter;
    sorter(collection, &wrapper::value);
    CHECK( std::is_sorted(collection.begin(), collection.end()) );
}
