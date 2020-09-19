/*
 * Copyright (c) 2015-2018 Morwenn
 * SPDX-License-Identifier: MIT
 */
#include <algorithm>
#include <forward_list>
#include <functional>
#include <iterator>
#include <list>
#include <random>
#include <vector>
#include <catch2/catch.hpp>
#include <cpp-sort/sorters/default_sorter.h>
#include <cpp-sort/sort.h>
#include <testing-tools/algorithm.h>

TEST_CASE( "default sorter tests with projections",
           "[default_sorter][projection]" )
{
    // Pseudo-random number engine
    std::mt19937_64 engine(Catch::rngSeed());

    // Wrapper to hide the integer
    struct wrapper { int value; };

    // Collection to sort
    std::vector<wrapper> vec(80);
    helpers::iota(std::begin(vec), std::end(vec), 0, &wrapper::value);
    std::shuffle(std::begin(vec), std::end(vec), engine);

    SECTION( "sort with random-access iterable" )
    {
        cppsort::sort(vec, &wrapper::value);
        CHECK( helpers::is_sorted(std::begin(vec), std::end(vec), std::less<>{}, &wrapper::value) );
    }

    SECTION( "sort with random-access iterable and compare" )
    {
        cppsort::sort(vec, std::greater<>{}, &wrapper::value);
        CHECK( helpers::is_sorted(std::begin(vec), std::end(vec), std::greater<>{}, &wrapper::value) );
    }

    SECTION( "sort with random-access iterators" )
    {
        cppsort::sort(std::begin(vec), std::end(vec), &wrapper::value);
        CHECK( helpers::is_sorted(std::begin(vec), std::end(vec), std::less<>{}, &wrapper::value) );
    }

    SECTION( "sort with random-access iterators and compare" )
    {
        cppsort::sort(std::begin(vec), std::end(vec), std::greater<>{}, &wrapper::value);
        CHECK( helpers::is_sorted(std::begin(vec), std::end(vec), std::greater<>{}, &wrapper::value) );
    }

    SECTION( "sort with bidirectional iterators" )
    {
        std::list<wrapper> li(std::begin(vec), std::end(vec));
        cppsort::sort(std::begin(li), std::end(li), &wrapper::value);
        CHECK( helpers::is_sorted(std::begin(li), std::end(li), std::less<>{}, &wrapper::value) );
    }

    SECTION( "sort with bidirectional iterators and compare" )
    {
        std::list<wrapper> li(std::begin(vec), std::end(vec));
        cppsort::sort(std::begin(li), std::end(li), std::greater<>{}, &wrapper::value);
        CHECK( helpers::is_sorted(std::begin(li), std::end(li), std::greater<>{}, &wrapper::value) );
    }

    SECTION( "sort with forward iterators" )
    {
        std::forward_list<wrapper> li(std::begin(vec), std::end(vec));
        cppsort::sort(std::begin(li), std::end(li), &wrapper::value);
        CHECK( helpers::is_sorted(std::begin(li), std::end(li), std::less<>{}, &wrapper::value) );
    }

    SECTION( "sort with forward iterators and compare" )
    {
        std::forward_list<wrapper> li(std::begin(vec), std::end(vec));
        cppsort::sort(std::begin(li), std::end(li), std::greater<>{}, &wrapper::value);
        CHECK( helpers::is_sorted(std::begin(li), std::end(li), std::greater<>{}, &wrapper::value) );
    }
}
