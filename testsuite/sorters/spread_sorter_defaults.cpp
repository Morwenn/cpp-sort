/*
 * Copyright (c) 2015-2022 Morwenn
 * SPDX-License-Identifier: MIT
 */
#include <algorithm>
#include <functional>
#include <iterator>
#include <numeric>
#include <vector>
#include <catch2/catch_test_macros.hpp>
#include <cpp-sort/sorters/spread_sorter.h>
#include <cpp-sort/utility/functional.h>
#include <testing-tools/random.h>

TEST_CASE( "spread_sorter generate overloads",
           "[spread_sorter][sorter_facade]" )
{
    // These tests use spread_sorter as a real-world Guinea pig
    // to make sure that sorter_facade generates the expected
    // operator() overloads for non-comparison sorters

    SECTION( "default operator() with std::less<>" )
    {
        std::vector<int> vec(100'000);
        std::iota(std::begin(vec), std::end(vec), 0);

        std::shuffle(std::begin(vec), std::end(vec), hasard::engine());
        cppsort::spread_sort(vec, std::less<>{});
        CHECK( std::is_sorted(std::begin(vec), std::end(vec), std::less<>{}) );

        std::shuffle(std::begin(vec), std::end(vec), hasard::engine());
        cppsort::spread_sort(std::begin(vec), std::end(vec), std::less<>{});
        CHECK( std::is_sorted(std::begin(vec), std::end(vec), std::less<>{}) );
    }

    SECTION( "default operator() with utility::identity" )
    {
        std::vector<int> vec(100'000);
        std::iota(std::begin(vec), std::end(vec), 0);

        std::shuffle(std::begin(vec), std::end(vec), hasard::engine());
        cppsort::spread_sort(vec, cppsort::utility::identity{});
        CHECK( std::is_sorted(std::begin(vec), std::end(vec)) );

        std::shuffle(std::begin(vec), std::end(vec), hasard::engine());
        cppsort::spread_sort(std::begin(vec), std::end(vec), cppsort::utility::identity{});
        CHECK( std::is_sorted(std::begin(vec), std::end(vec)) );
    }

    SECTION( "default operator() with both" )
    {
        std::vector<int> vec(100'000);
        std::iota(std::begin(vec), std::end(vec), 0);

        std::shuffle(std::begin(vec), std::end(vec), hasard::engine());
        cppsort::spread_sort(vec, std::less<>{}, cppsort::utility::identity{});
        CHECK( std::is_sorted(std::begin(vec), std::end(vec)) );

        std::shuffle(std::begin(vec), std::end(vec), hasard::engine());
        cppsort::spread_sort(std::begin(vec), std::end(vec), std::less<>{}, cppsort::utility::identity{});
        CHECK( std::is_sorted(std::begin(vec), std::end(vec)) );
    }
}
