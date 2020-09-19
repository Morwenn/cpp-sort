/*
 * Copyright (c) 2018 Morwenn
 * SPDX-License-Identifier: MIT
 */
#include <functional>
#include <catch2/catch.hpp>
#include <cpp-sort/stable_sort.h>
#include <cpp-sort/sorters/selection_sorter.h>
#include <cpp-sort/utility/functional.h>
#include <testing-tools/algorithm.h>

TEST_CASE( "test stable sorting C arrays", "[sort]" )
{
    int arr[] = { 8, 1, 6, 7, 3, 5, 4, 9, 2 };

    SECTION( "without anything" )
    {
        cppsort::stable_sort(arr);
        CHECK( helpers::is_sorted(std::begin(arr), std::end(arr)) );
    }

    SECTION( "with comparison function" )
    {
        cppsort::stable_sort(arr, std::greater<>{});
        CHECK( helpers::is_sorted(std::begin(arr), std::end(arr), std::greater<>{}) );
    }

    SECTION( "with projection function" )
    {
        cppsort::stable_sort(arr, std::negate<>{});
        CHECK( helpers::is_sorted(std::begin(arr), std::end(arr), std::greater<>{}) );
    }

    SECTION( "with comparison and projection functions" )
    {
        cppsort::stable_sort(arr, std::greater<>{}, std::negate<>{});
        CHECK( helpers::is_sorted(std::begin(arr), std::end(arr)) );
    }

    SECTION( "with sorter" )
    {
        cppsort::stable_sort(cppsort::selection_sort, arr);
        CHECK( helpers::is_sorted(std::begin(arr), std::end(arr)) );
    }

    SECTION( "with sorter and comparison function" )
    {
        cppsort::stable_sort(cppsort::selection_sort, arr, std::greater<>{});
        CHECK( helpers::is_sorted(std::begin(arr), std::end(arr), std::greater<>{}) );
    }

    SECTION( "with sorter and projection function" )
    {
        cppsort::stable_sort(cppsort::selection_sort, arr, std::negate<>{});
        CHECK( helpers::is_sorted(std::begin(arr), std::end(arr), std::greater<>{}) );
    }

    SECTION( "with sorter, comparison and projection function" )
    {
        cppsort::stable_sort(cppsort::selection_sort, arr, std::greater<>{}, std::negate<>{});
        CHECK( helpers::is_sorted(std::begin(arr), std::end(arr)) );
    }
}
