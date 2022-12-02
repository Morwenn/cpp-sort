/*
 * Copyright (c) 2016-2022 Morwenn
 * SPDX-License-Identifier: MIT
 */
#include <functional>
#include <iterator>
#include <vector>
#include <catch2/catch_test_macros.hpp>
#include <cpp-sort/sorters/merge_insertion_sorter.h>
#include <testing-tools/algorithm.h>
#include <testing-tools/distributions.h>
#include <testing-tools/wrapper.h>

TEST_CASE( "merge_insertion_sorter tests with projections",
           "[merge_insertion_sorter][projection]" )
{
    // Wrapper to hide the integer
    using wrapper = generic_wrapper<int>;

    // Collection to sort
    std::vector<wrapper> vec;
    auto distribution = dist::shuffled{};
    distribution(std::back_inserter(vec), 80);

    SECTION( "sort with random-access range" )
    {
        cppsort::merge_insertion_sort(vec, &wrapper::value);
        CHECK( helpers::is_sorted(vec.begin(), vec.end(), std::less<>{}, &wrapper::value) );
    }

    SECTION( "sort with random-access range and compare" )
    {
        cppsort::merge_insertion_sort(vec, std::greater<>{}, &wrapper::value);
        CHECK( helpers::is_sorted(vec.begin(), vec.end(), std::greater<>{}, &wrapper::value) );
    }

    SECTION( "sort with random-access iterators" )
    {
        cppsort::merge_insertion_sort(vec.begin(), vec.end(), &wrapper::value);
        CHECK( helpers::is_sorted(vec.begin(), vec.end(), std::less<>{}, &wrapper::value) );
    }

    SECTION( "sort with random-access iterators and compare" )
    {
        cppsort::merge_insertion_sort(vec.begin(), vec.end(),
                                      std::greater<>{}, &wrapper::value);
        CHECK( helpers::is_sorted(vec.begin(), vec.end(), std::greater<>{}, &wrapper::value) );
    }
}
