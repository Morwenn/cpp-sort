/*
 * Copyright (c) 2019-2022 Morwenn
 * SPDX-License-Identifier: MIT
 */
#include <algorithm>
#include <iterator>
#include <vector>
#include <catch2/catch_test_macros.hpp>
#include <cpp-sort/sorters/spin_sorter.h>
#include <testing-tools/distributions.h>

TEST_CASE( "spin_sorter tests", "[spin_sorter]" )
{
    // spin_sorter has different paths that can only be reached depending
    // on the size of the input that generic tests don't seem to catch,
    // hence those additional tests. The different paths depend on the
    // following criteria:
    // - Whether log2(size(input)) is even or odd
    // - TODO

    std::vector<int> collection;
    auto distribution = dist::shuffled{};

    SECTION( "even number of levels" )
    {
        const int size = 2800;
        collection.reserve(size);
        distribution(std::back_inserter(collection), size, 0);
        cppsort::spin_sort(collection);
        CHECK( std::is_sorted(collection.begin(), collection.end()) );
    }

    SECTION( "odd number of levels" )
    {
        const int size = 2300;
        collection.reserve(size);
        distribution(std::back_inserter(collection), size, 0);
        cppsort::spin_sort(collection);
        CHECK( std::is_sorted(collection.begin(), collection.end()) );
    }
}
