/*
 * Copyright (c) 2016-2020 Morwenn
 * SPDX-License-Identifier: MIT
 */
#include <algorithm>
#include <iterator>
#include <random>
#include <vector>
#include <catch2/catch.hpp>
#include <cpp-sort/sorters.h>
#include <cpp-sort/utility/buffer.h>
#include <testing-tools/distributions.h>
#include <testing-tools/move_only.h>

TEMPLATE_TEST_CASE( "test every sorter with move-only types", "[sorters]",
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
    // General test to make sure that every sorter compiles fine with
    // move-only types, additionally checking that no read-after-move
    // operation is performed and that no self-move is performed

    std::vector<double> numbers;
    numbers.reserve(491);
    auto distribution = dist::shuffled{};
    distribution(std::back_inserter(numbers), 491, -125);

    std::mt19937 engine(Catch::rngSeed());
    std::shuffle(std::begin(numbers), std::end(numbers), engine);
    std::vector<move_only<double>> collection(std::begin(numbers), std::end(numbers));

    TestType sorter;
    sorter(collection);
    CHECK( std::is_sorted(std::begin(collection), std::end(collection)) );
}
