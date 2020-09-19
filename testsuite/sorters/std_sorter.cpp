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
    struct wrapper { int value; };

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
