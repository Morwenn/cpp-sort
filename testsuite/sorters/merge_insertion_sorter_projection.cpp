/*
 * Copyright (c) 2016-2020 Morwenn
 * SPDX-License-Identifier: MIT
 */
#include <algorithm>
#include <functional>
#include <iterator>
#include <random>
#include <vector>
#include <catch2/catch.hpp>
#include <cpp-sort/sorters/merge_insertion_sorter.h>
#include <testing-tools/algorithm.h>
#include <testing-tools/wrapper.h>

TEST_CASE( "merge_insertion_sorter tests with projections",
           "[merge_insertion_sorter][projection]" )
{
    // Pseudo-random number engine
    std::mt19937_64 engine(Catch::rngSeed());

    // Wrapper to hide the integer
    using wrapper = generic_wrapper<int>;

    // Collection to sort
    std::vector<wrapper> vec(80);
    helpers::iota(std::begin(vec), std::end(vec), 0, &wrapper::value);
    std::shuffle(std::begin(vec), std::end(vec), engine);

    SECTION( "sort with random-access iterable" )
    {
        cppsort::merge_insertion_sort(vec, &wrapper::value);
        CHECK( helpers::is_sorted(std::begin(vec), std::end(vec), std::less<>{}, &wrapper::value) );
    }

    SECTION( "sort with random-access iterable and compare" )
    {
        cppsort::merge_insertion_sort(vec, std::greater<>{}, &wrapper::value);
        CHECK( helpers::is_sorted(std::begin(vec), std::end(vec), std::greater<>{}, &wrapper::value) );
    }

    SECTION( "sort with random-access iterators" )
    {
        cppsort::merge_insertion_sort(std::begin(vec), std::end(vec), &wrapper::value);
        CHECK( helpers::is_sorted(std::begin(vec), std::end(vec), std::less<>{}, &wrapper::value) );
    }

    SECTION( "sort with random-access iterators and compare" )
    {
        cppsort::merge_insertion_sort(std::begin(vec), std::end(vec),
                                      std::greater<>{}, &wrapper::value);
        CHECK( helpers::is_sorted(std::begin(vec), std::end(vec), std::greater<>{}, &wrapper::value) );
    }
}
