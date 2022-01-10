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
#include <cpp-sort/sorters/merge_sorter.h>
#include <testing-tools/distributions.h>

TEST_CASE( "merge_sorter tests", "[merge_sorter]" )
{
    // Collection to sort
    std::vector<int> vec; vec.reserve(80);
    auto distribution = dist::shuffled{};
    distribution(std::back_inserter(vec), 80, 0);

    SECTION( "sort with random-access iterable" )
    {
        cppsort::merge_sort(vec);
        CHECK( std::is_sorted(vec.begin(), vec.end()) );
    }

    SECTION( "sort with random-access iterable and compare" )
    {
        cppsort::merge_sort(vec, std::greater<>{});
        CHECK( std::is_sorted(vec.begin(), vec.end(), std::greater<>{}) );
    }

    SECTION( "sort with random-access iterators" )
    {
        cppsort::merge_sort(vec.begin(), vec.end());
        CHECK( std::is_sorted(vec.begin(), vec.end()) );
    }

    SECTION( "sort with random-access iterators and compare" )
    {
        cppsort::merge_sort(vec.begin(), vec.end(), std::greater<>{});
        CHECK( std::is_sorted(vec.begin(), vec.end(), std::greater<>{}) );
    }

    SECTION( "sort with bidirectional iterators" )
    {
        std::list<int> li(vec.begin(), vec.end());
        cppsort::merge_sort(li.begin(), li.end());
        CHECK( std::is_sorted(li.begin(), li.end()) );
    }

    SECTION( "sort with bidirectional iterators and compare" )
    {
        std::list<int> li(vec.begin(), vec.end());
        cppsort::merge_sort(li.begin(), li.end(), std::greater<>{});
        CHECK( std::is_sorted(li.begin(), li.end(), std::greater<>{}) );
    }

    SECTION( "sort with forward iterators" )
    {
        std::forward_list<int> li(vec.begin(), vec.end());
        cppsort::merge_sort(li.begin(), li.end());
        CHECK( std::is_sorted(li.begin(), li.end()) );
    }

    SECTION( "sort with forward iterators and compare" )
    {
        std::forward_list<int> li(vec.begin(), vec.end());
        cppsort::merge_sort(li.begin(), li.end(), std::greater<>{});
        CHECK( std::is_sorted(li.begin(), li.end(), std::greater<>{}) );
    }
}
