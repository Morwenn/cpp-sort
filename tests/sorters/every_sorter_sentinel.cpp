/*
 * Copyright (c) 2022 Morwenn
 * SPDX-License-Identifier: MIT
 */
#include <algorithm>
#include <iterator>
#include <vector>
#include <catch2/catch_template_test_macros.hpp>
#include <cpp-sort/sorters.h>
#include <cpp-sort/utility/buffer.h>
#include <testing-tools/distributions.h>
#include <testing-tools/end_sentinel.h>
#include <testing-tools/old_default_sorter.h>
#include <testing-tools/random.h>

TEMPLATE_TEST_CASE( "test every sorter with a sentinel", "[sorters][sentinel]",
                    cppsort::cartesian_tree_sorter,
                    cppsort::counting_sorter,
                    cppsort::insertion_sorter,
                    cppsort::mel_sorter,
                    cppsort::selection_sorter )
{
    std::vector<int> collection;
    auto distribution = dist::shuffled{};
    distribution(std::back_inserter(collection), 1000, -100);

    TestType sorter;
    sorter(collection.begin(), end_sentinel(collection.end()));
    CHECK( std::is_sorted(collection.begin(), collection.end()) );
}
