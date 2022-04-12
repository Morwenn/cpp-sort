/*
 * Copyright (c) 2015-2022 Morwenn
 * SPDX-License-Identifier: MIT
 */
#include <algorithm>
#include <forward_list>
#include <functional>
#include <iterator>
#include <list>
#include <vector>
#include <catch2/catch_test_macros.hpp>
#include <cpp-sort/sorters/default_sorter.h>
#include <cpp-sort/sort.h>
#include <testing-tools/distributions.h>

TEST_CASE( "default sorter tests", "[default_sorter]" )
{
    // Collection to sort
    std::vector<int> vec; vec.reserve(80);
    auto distribution = dist::shuffled{};
    distribution(std::back_inserter(vec), 80, 0);

    SECTION( "sort with random-access iterable" )
    {
        cppsort::sort(vec);
        CHECK( std::is_sorted(vec.begin(), vec.end()) );
    }

    SECTION( "sort with random-access iterable and compare" )
    {
        cppsort::sort(vec, std::greater<>{});
        CHECK( std::is_sorted(vec.begin(), vec.end(), std::greater<>{}) );
    }

    SECTION( "sort with random-access iterators" )
    {
        cppsort::sort(vec.begin(), vec.end());
        CHECK( std::is_sorted(vec.begin(), vec.end()) );
    }

    SECTION( "sort with random-access iterators and compare" )
    {
        cppsort::sort(vec.begin(), vec.end(), std::greater<>{});
        CHECK( std::is_sorted(vec.begin(), vec.end(), std::greater<>{}) );
    }

    SECTION( "sort with bidirectional iterators" )
    {
        std::list<int> li(vec.begin(), vec.end());
        cppsort::sort(li.begin(), li.end());
        CHECK( std::is_sorted(li.begin(), li.end()) );
    }

    SECTION( "sort with bidirectional iterators and compare" )
    {
        std::list<int> li(vec.begin(), vec.end());
        cppsort::sort(li.begin(), li.end(), std::greater<>{});
        CHECK( std::is_sorted(li.begin(), li.end(), std::greater<>{}) );
    }

    SECTION( "sort with forward iterators" )
    {
        std::forward_list<int> li(vec.begin(), vec.end());
        cppsort::sort(li.begin(), li.end());
        CHECK( std::is_sorted(li.begin(), li.end()) );
    }

    SECTION( "sort with forward iterators and compare" )
    {
        std::forward_list<int> li(vec.begin(), vec.end());
        cppsort::sort(li.begin(), li.end(), std::greater<>{});
        CHECK( std::is_sorted(li.begin(), li.end(), std::greater<>{}) );
    }

    SECTION( "sort with self-sortable iterable" )
    {
        std::list<int> li(vec.begin(), vec.end());
        cppsort::sort(li);
        CHECK( std::is_sorted(li.begin(), li.end()) );
    }

    SECTION( "sort with self-sortable iterable and compare" )
    {
        std::forward_list<int> li(vec.begin(), vec.end());
        cppsort::sort(li, std::greater<>{});
        CHECK( std::is_sorted(li.begin(), li.end(), std::greater<>{}) );
    }
}
