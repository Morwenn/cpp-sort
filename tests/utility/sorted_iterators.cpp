/*
 * Copyright (c) 2022 Morwenn
 * SPDX-License-Identifier: MIT
 */
#include <iterator>
#include <vector>
#include <catch2/catch_test_macros.hpp>
#include <cpp-sort/sorters/heap_sorter.h>
#include <cpp-sort/sorters/insertion_sorter.h>
#include <cpp-sort/utility/functional.h>
#include <cpp-sort/utility/sorted_iterators.h>
#include <testing-tools/algorithm.h>
#include <testing-tools/distributions.h>

TEST_CASE( "basic sorted_iterators test", "[utility][sorted_iterators]" )
{
    using cppsort::utility::indirect;

    SECTION( "simple case" )
    {
        auto get_sorted_iterators_for = cppsort::utility::sorted_iterators<cppsort::heap_sorter>{};
        const std::vector<int> vec = { 6, 4, 2, 1, 8, 7, 0, 9, 5, 3 };
        auto indices = get_sorted_iterators_for(vec.data(), vec.data() + vec.size());
        CHECK( helpers::is_sorted(indices.begin(), indices.end(), {}, indirect{}) );

        std::vector<const int*> expected = {
            vec.data() + 6, vec.data() + 3, vec.data() + 2,
            vec.data() + 9, vec.data() + 1, vec.data() + 8,
            vec.data() + 0, vec.data() + 5, vec.data() + 4,
            vec.data() + 7
        };
        CHECK( indices == expected );
    }

    SECTION( "empty collection" )
    {
        auto get_sorted_iterators_for = cppsort::utility::sorted_iterators<cppsort::heap_sorter>{};
        const std::vector<int> vec = {};
        auto indices = get_sorted_iterators_for(vec.data(), vec.data() + vec.size());
        CHECK( helpers::is_sorted(indices.begin(), indices.end(), {}, indirect{}) );

        std::vector<const int*> expected = {};
        CHECK( indices == expected );

    }

    SECTION( "all_equal" )
    {
        // Use a stable algorithm to get deterministic results for equivalent elements
        auto get_sorted_iterators_for = cppsort::utility::sorted_iterators<cppsort::insertion_sorter>{};
        std::vector<int> vec;
        auto distribution = dist::all_equal{};
        distribution(std::back_inserter(vec), 10);
        auto indices = get_sorted_iterators_for(vec.data(), vec.data() + vec.size());
        CHECK( helpers::is_sorted(indices.begin(), indices.end(), {}, indirect{}) );

        std::vector<int*> expected = {
            vec.data() + 0, vec.data() + 1, vec.data() + 2,
            vec.data() + 3, vec.data() + 4, vec.data() + 5,
            vec.data() + 6, vec.data() + 7, vec.data() + 8,
            vec.data() + 9
        };
        CHECK( indices == expected );
    }

    SECTION( "ascending" )
    {
        auto get_sorted_iterators_for = cppsort::utility::sorted_iterators<cppsort::heap_sorter>{};
        std::vector<int> vec;
        auto distribution = dist::ascending{};
        distribution(std::back_inserter(vec), 10);
        auto indices = get_sorted_iterators_for(vec.data(), vec.data() + vec.size());
        CHECK( helpers::is_sorted(indices.begin(), indices.end(), {}, indirect{}) );

        std::vector<int*> expected = {
            vec.data() + 0, vec.data() + 1, vec.data() + 2,
            vec.data() + 3, vec.data() + 4, vec.data() + 5,
            vec.data() + 6, vec.data() + 7, vec.data() + 8,
            vec.data() + 9
        };
        CHECK( indices == expected );
    }

    SECTION( "descending" )
    {
        auto get_sorted_iterators_for = cppsort::utility::sorted_iterators<cppsort::heap_sorter>{};
        std::vector<int> vec;
        auto distribution = dist::descending{};
        distribution(std::back_inserter(vec), 10);
        auto indices = get_sorted_iterators_for(vec.data(), vec.data() + vec.size());
        CHECK( helpers::is_sorted(indices.begin(), indices.end(), {}, indirect{}) );

        std::vector<int*> expected = {
            vec.data() + 9, vec.data() + 8, vec.data() + 7,
            vec.data() + 6, vec.data() + 5, vec.data() + 4,
            vec.data() + 3, vec.data() + 2, vec.data() + 1,
            vec.data() + 0
        };
        CHECK( indices == expected );
    }
}
