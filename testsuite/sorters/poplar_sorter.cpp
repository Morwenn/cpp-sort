/*
 * Copyright (c) 2017-2022 Morwenn
 * SPDX-License-Identifier: MIT
 */
#include <algorithm>
#include <iterator>
#include <vector>
#include <catch2/catch_test_macros.hpp>
#include <cpp-sort/sorters/poplar_sorter.h>
#include <testing-tools/distributions.h>

TEST_CASE( "poplar_sorter tests", "[poplar_sorter]" )
{
    // Distribution used to generate the data to sort
    auto distribution = dist::shuffled{};

    SECTION( "tricky test case 131073" )
    {
        // With signed 32-bit difference_type for an iterator,
        // sorting a collection of 131073 elements triggered a
        // problem with the underlying bit tricks that eventually
        // made the algorithm fail

        auto size = 131073; // This part matters the most
        std::vector<int> vec; vec.reserve(size);
        distribution(std::back_inserter(vec), size, -1568);
        cppsort::poplar_sort(vec);
        CHECK( std::is_sorted(vec.begin(), vec.end()) );
    }
}
