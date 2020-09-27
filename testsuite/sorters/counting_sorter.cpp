/*
 * Copyright (c) 2016-2019 Morwenn
 * SPDX-License-Identifier: MIT
 */
#include <algorithm>
#include <forward_list>
#include <iterator>
#include <list>
#include <vector>
#include <catch2/catch.hpp>
#include <cpp-sort/sorters/counting_sorter.h>
#include <testing-tools/distributions.h>

TEST_CASE( "counting_sorter tests", "[counting_sorter]" )
{
    // Distribution used to generate the data to sort
    auto distribution = dist::shuffled{};
    // Size of the collections to sort
    auto size = 100'000;

    SECTION( "sort with int iterable" )
    {
        std::vector<int> vec; vec.reserve(size);
        distribution(std::back_inserter(vec), size, -1568);
        cppsort::counting_sort(vec);
        CHECK( std::is_sorted(std::begin(vec), std::end(vec)) );
    }

    SECTION( "sort with unsigned int iterators" )
    {
        std::list<unsigned> li;
        distribution(std::back_inserter(li), size, 0u);
        cppsort::counting_sort(std::begin(li), std::end(li));
        CHECK( std::is_sorted(std::begin(li), std::end(li)) );
    }

    SECTION( "reverse sort with long long iterable" )
    {
        std::vector<long long> vec; vec.reserve(size);
        distribution(std::back_inserter(vec), size, 1568);
        cppsort::counting_sort(vec);
        CHECK( std::is_sorted(std::begin(vec), std::end(vec)) );
    }

    SECTION( "reverse sort with unsigned long long iterators" )
    {
        std::forward_list<unsigned long long> li;
        distribution(std::front_inserter(li), size, 0ULL);
        cppsort::counting_sort(std::begin(li), std::end(li));
        CHECK( std::is_sorted(std::begin(li), std::end(li)) );
    }

#ifdef __SIZEOF_INT128__
    SECTION( "sort with unsigned int128 iterators" )
    {
        std::list<__uint128_t> li;;
        distribution(std::back_inserter(li), size, __uint128_t(0));
        cppsort::counting_sort(std::begin(li), std::end(li));
        CHECK( std::is_sorted(std::begin(li), std::end(li)) );
    }
#endif

    SECTION( "GitHub issue #103" )
    {
        // Specific bug in counting_sort due to another specific bug in
        // minmax_element_and_is_sorted due to another specific bug in
        // minmax_element... found while investigating GitHub issue #103;
        // better have a test

        std::vector<int> vec = { -47, -46, -45, -44, -43, -42, -41, -39, -40, -38 };
        cppsort::counting_sort(vec);
        CHECK( std::is_sorted(std::begin(vec), std::end(vec)) );
    }
}
