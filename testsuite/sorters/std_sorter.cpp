/*
 * Copyright (c) 2016-2020 Morwenn
 * SPDX-License-Identifier: MIT
 */
#include <algorithm>
#include <functional>
#include <iterator>
#include <vector>
#include <catch2/catch.hpp>
#include <cpp-sort/sorters/std_sorter.h>
#include <testing-tools/algorithm.h>
#include <testing-tools/distributions.h>
#include <testing-tools/wrapper.h>

TEST_CASE( "std_sorter tests", "[std_sorter]" )
{
    std::vector<int> vec;
    auto distribution = dist::shuffled{};
    distribution(std::back_inserter(vec), 80);

    SECTION( "sort with iterable" )
    {
        cppsort::std_sort(vec);
        CHECK( std::is_sorted(vec.begin(), vec.end()) );
    }

    SECTION( "sort with iterable and compare" )
    {
        cppsort::std_sort(vec, std::greater<>{});
        CHECK( std::is_sorted(vec.begin(), vec.end(), std::greater<>{}) );
    }

    SECTION( "sort with iterators" )
    {
        cppsort::std_sort(vec.begin(), vec.end());
        CHECK( std::is_sorted(vec.begin(), vec.end()) );
    }

    SECTION( "sort with iterators and compare" )
    {
        cppsort::std_sort(vec.begin(), vec.end(), std::greater<>{});
        CHECK( std::is_sorted(vec.begin(), vec.end(), std::greater<>{}) );
    }
}

TEST_CASE( "std_sorter tests with projections",
           "[std_sorter][projection]" )
{
    // Wrapper to hide the integer
    using wrapper = generic_wrapper<int>;

    std::vector<wrapper> vec;
    auto distribution = dist::shuffled{};
    distribution(std::back_inserter(vec), 80);

    SECTION( "sort with iterable" )
    {
        cppsort::std_sort(vec, &wrapper::value);
        CHECK( helpers::is_sorted(vec.begin(), vec.end(), std::less<>{}, &wrapper::value) );
    }

    SECTION( "sort with iterable and compare" )
    {
        cppsort::std_sort(vec, std::greater<>{}, &wrapper::value);
        CHECK( helpers::is_sorted(vec.begin(), vec.end(), std::greater<>{}, &wrapper::value) );
    }

    SECTION( "sort with iterators" )
    {
        cppsort::std_sort(vec.begin(), vec.end(), &wrapper::value);
        CHECK( helpers::is_sorted(vec.begin(), vec.end(), std::less<>{}, &wrapper::value) );
    }

    SECTION( "sort with iterators and compare" )
    {
        cppsort::std_sort(vec.begin(), vec.end(), std::greater<>{}, &wrapper::value);
        CHECK( helpers::is_sorted(vec.begin(), vec.end(), std::greater<>{}, &wrapper::value) );
    }
}

TEST_CASE( "stable_adapter<std_sorter> tests",
           "[std_sorter][stable_adapter]" )
{
    std::vector<int> vec;
    auto distribution = dist::shuffled{};
    distribution(std::back_inserter(vec), 80);

    auto sort = cppsort::stable_adapter<cppsort::std_sorter>(cppsort::std_sort);
    sort(vec);
    CHECK( std::is_sorted(vec.begin(), vec.end()) );
}
