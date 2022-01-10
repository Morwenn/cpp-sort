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
#include <testing-tools/distributions.h>

TEST_CASE( "default sorter function pointer tests",
           "[default_sorter][function_pointer]" )
{
    // Collection to sort
    std::vector<int> vec; vec.reserve(80);
    auto distribution = dist::shuffled{};
    distribution(std::back_inserter(vec), 80, 0);

    // Projection to sort in descending order
    auto projection = [](int n) { return -n; };

    SECTION( "sort with random-access iterable" )
    {
        constexpr void(*sorter)(std::vector<int>&) = cppsort::default_sorter();

        sorter(vec);
        CHECK( std::is_sorted(vec.begin(), vec.end()) );
    }

    SECTION( "sort with random-access iterable and compare" )
    {
        constexpr void(*sorter)(std::vector<int>&, std::greater<>) = cppsort::default_sorter();

        sorter(vec, std::greater<>{});
        CHECK( std::is_sorted(vec.begin(), vec.end(), std::greater<>{}) );
    }

    SECTION( "sort with random-access iterable and projection" )
    {
        constexpr void(*sorter)(std::vector<int>&, decltype(projection)) = cppsort::default_sorter();

        sorter(vec, projection);
        CHECK( std::is_sorted(vec.begin(), vec.end(), std::greater<>{}) );
    }

    SECTION( "sort with random-access iterable, compare and projection" )
    {
        constexpr void(*sorter)(std::vector<int>&,
                                std::greater<>,
                                decltype(projection))
            = cppsort::default_sorter();

        sorter(vec, std::greater<>{}, projection);
        CHECK( std::is_sorted(vec.begin(), vec.end()) );
    }

    SECTION( "sort with random-access iterators" )
    {
        constexpr void(*sorter)(std::vector<int>::iterator,
                                std::vector<int>::iterator)
            = cppsort::default_sorter();

        sorter(vec.begin(), vec.end());
        CHECK( std::is_sorted(vec.begin(), vec.end()) );
    }

    SECTION( "sort with random-access iterators and compare" )
    {
        constexpr void(*sorter)(std::vector<int>::iterator,
                                std::vector<int>::iterator,
                                std::greater<>)
            = cppsort::default_sorter();

        sorter(vec.begin(), vec.end(), std::greater<>{});
        CHECK( std::is_sorted(vec.begin(), vec.end(), std::greater<>{}) );
    }

    SECTION( "sort with random-access iterators and projection" )
    {
        constexpr void(*sorter)(std::vector<int>::iterator,
                                std::vector<int>::iterator,
                                decltype(projection))
            = cppsort::default_sorter();

        sorter(vec.begin(), vec.end(), projection);
        CHECK( std::is_sorted(vec.begin(), vec.end(), std::greater<>{}) );
    }

    SECTION( "sort with random-access iterators, compare and projection" )
    {
        constexpr void(*sorter)(std::vector<int>::iterator,
                                std::vector<int>::iterator,
                                std::greater<>,
                                decltype(projection))
            = cppsort::default_sorter();

        sorter(vec.begin(), vec.end(), std::greater<>{}, projection);
        CHECK( std::is_sorted(vec.begin(), vec.end()) );
    }

    SECTION( "sort with bidirectional iterators" )
    {
        constexpr void(*sorter)(std::list<int>::iterator,
                                std::list<int>::iterator)
            = cppsort::default_sorter();

        std::list<int> li(vec.begin(), vec.end());
        sorter(li.begin(), li.end());
        CHECK( std::is_sorted(li.begin(), li.end()) );
    }

    SECTION( "sort with bidirectional iterators and compare" )
    {
        constexpr void(*sorter)(std::list<int>::iterator,
                                std::list<int>::iterator,
                                std::greater<>)
            = cppsort::default_sorter();

        std::list<int> li(vec.begin(), vec.end());
        sorter(li.begin(), li.end(), std::greater<>{});
        CHECK( std::is_sorted(li.begin(), li.end(), std::greater<>{}) );
    }

    SECTION( "sort with bidirectional iterators and projection" )
    {
        constexpr void(*sorter)(std::list<int>::iterator,
                                std::list<int>::iterator,
                                decltype(projection))
            = cppsort::default_sorter();

        std::list<int> li(vec.begin(), vec.end());
        sorter(li.begin(), li.end(), projection);
        CHECK( std::is_sorted(li.begin(), li.end(), std::greater<>{}) );
    }

    SECTION( "sort with bidirectional iterators, compare and projection" )
    {
        constexpr void(*sorter)(std::list<int>::iterator,
                                std::list<int>::iterator,
                                std::greater<>,
                      decltype(projection))
            = cppsort::default_sorter();

        std::list<int> li(vec.begin(), vec.end());
        sorter(li.begin(), li.end(), std::greater<>{}, projection);
        CHECK( std::is_sorted(li.begin(), li.end()) );
    }

    SECTION( "sort with forward iterators" )
    {
        constexpr void(*sorter)(std::forward_list<int>::iterator,
                                std::forward_list<int>::iterator)
            = cppsort::default_sorter();

        std::forward_list<int> li(vec.begin(), vec.end());
        sorter(li.begin(), li.end());
        CHECK( std::is_sorted(li.begin(), li.end()) );
    }

    SECTION( "sort with forward iterators and compare" )
    {
        constexpr void(*sorter)(std::forward_list<int>::iterator,
                                std::forward_list<int>::iterator,
                                std::greater<>)
            = cppsort::default_sorter();

        std::forward_list<int> li(vec.begin(), vec.end());
        sorter(li.begin(), li.end(), std::greater<>{});
        CHECK( std::is_sorted(li.begin(), li.end(), std::greater<>{}) );
    }

    SECTION( "sort with forward iterators and projection" )
    {
        constexpr void(*sorter)(std::forward_list<int>::iterator,
                                std::forward_list<int>::iterator,
                                decltype(projection))
            = cppsort::default_sorter();

        std::forward_list<int> li(vec.begin(), vec.end());
        sorter(li.begin(), li.end(), projection);
        CHECK( std::is_sorted(li.begin(), li.end(), std::greater<>{}) );
    }

    SECTION( "sort with forward iterators and projection" )
    {
        constexpr void(*sorter)(std::forward_list<int>::iterator,
                                std::forward_list<int>::iterator,
                                std::greater<>,
                                decltype(projection))
            = cppsort::default_sorter();

        std::forward_list<int> li(vec.begin(), vec.end());
        sorter(li.begin(), li.end(), std::greater<>{}, projection);
        CHECK( std::is_sorted(li.begin(), li.end()) );
    }

    SECTION( "sort with self-sortable iterable" )
    {
        constexpr void(*sorter)(std::list<int>&) = cppsort::default_sorter();

        std::list<int> li(vec.begin(), vec.end());
        sorter(li);
        CHECK( std::is_sorted(li.begin(), li.end()) );
    }

    SECTION( "sort with self-sortable iterable and compare" )
    {
        constexpr void(*sorter)(std::forward_list<int>&, std::greater<>) = cppsort::default_sorter();

        std::forward_list<int> li(vec.begin(), vec.end());
        sorter(li, std::greater<>{});
        CHECK( std::is_sorted(li.begin(), li.end(), std::greater<>{}) );
    }
}
