/*
 * Copyright (c) 2016-2020 Morwenn
 * SPDX-License-Identifier: MIT
 */
#include <algorithm>
#include <functional>
#include <iterator>
#include <numeric>
#include <random>
#include <vector>
#include <catch2/catch.hpp>
#include <cpp-sort/sorters/std_sorter.h>
#include <testing-tools/algorithm.h>
#include <testing-tools/wrapper.h>

TEST_CASE( "std_sorter tests", "[std_sorter]" )
{
    // Pseudo-random number engine
    std::mt19937_64 engine(Catch::rngSeed());

    // Collection to sort
    std::vector<int> vec(80);
    std::iota(std::begin(vec), std::end(vec), 0);
    std::shuffle(std::begin(vec), std::end(vec), engine);

    SECTION( "sort with iterable" )
    {
        cppsort::std_sort(vec);
        CHECK( std::is_sorted(std::begin(vec), std::end(vec)) );
    }

    SECTION( "sort with iterable and compare" )
    {
        cppsort::std_sort(vec, std::greater<>{});
        CHECK( std::is_sorted(std::begin(vec), std::end(vec), std::greater<>{}) );
    }

    SECTION( "sort with iterators" )
    {
        cppsort::std_sort(std::begin(vec), std::end(vec));
        CHECK( std::is_sorted(std::begin(vec), std::end(vec)) );
    }

    SECTION( "sort with iterators and compare" )
    {
        cppsort::std_sort(std::begin(vec), std::end(vec), std::greater<>{});
        CHECK( std::is_sorted(std::begin(vec), std::end(vec), std::greater<>{}) );
    }
}

TEST_CASE( "std_sorter tests with projections",
           "[std_sorter][projection]" )
{
    // Pseudo-random number engine
    std::mt19937_64 engine(Catch::rngSeed());

    // Wrapper to hide the integer
    using wrapper = generic_wrapper<int>;

    // Collection to sort
    std::vector<wrapper> vec(80);
    helpers::iota(std::begin(vec), std::end(vec), 0, &wrapper::value);
    std::shuffle(std::begin(vec), std::end(vec), engine);

    SECTION( "sort with iterable" )
    {
        cppsort::std_sort(vec, &wrapper::value);
        CHECK( helpers::is_sorted(std::begin(vec), std::end(vec), std::less<>{}, &wrapper::value) );
    }

    SECTION( "sort with iterable and compare" )
    {
        cppsort::std_sort(vec, std::greater<>{}, &wrapper::value);
        CHECK( helpers::is_sorted(std::begin(vec), std::end(vec), std::greater<>{}, &wrapper::value) );
    }

    SECTION( "sort with iterators" )
    {
        cppsort::std_sort(std::begin(vec), std::end(vec), &wrapper::value);
        CHECK( helpers::is_sorted(std::begin(vec), std::end(vec), std::less<>{}, &wrapper::value) );
    }

    SECTION( "sort with iterators and compare" )
    {
        cppsort::std_sort(std::begin(vec), std::end(vec), std::greater<>{}, &wrapper::value);
        CHECK( helpers::is_sorted(std::begin(vec), std::end(vec), std::greater<>{}, &wrapper::value) );
    }
}

TEST_CASE( "stable_adapter<std_sorter> tests",
           "[std_sorter][stable_adapter]" )
{
    // Pseudo-random number engine
    std::mt19937_64 engine(Catch::rngSeed());

    // Collection to sort
    std::vector<int> vec(80);
    std::iota(vec.begin(), vec.end(), 0);
    std::shuffle(vec.begin(), vec.end(), engine);

    auto sort = cppsort::stable_adapter<cppsort::std_sorter>(cppsort::std_sort);
    sort(vec);
    CHECK( std::is_sorted(vec.begin(), vec.end()) );
}
