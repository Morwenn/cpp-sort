/*
 * Copyright (c) 2015-2021 Morwenn
 * SPDX-License-Identifier: MIT
 */
#include <algorithm>
#include <forward_list>
#include <functional>
#include <iterator>
#include <list>
#include <vector>
#include <catch2/catch.hpp>
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
        CHECK( std::is_sorted(std::begin(vec), std::end(vec)) );
    }

    SECTION( "sort with random-access iterable and compare" )
    {
        constexpr void(*sorter)(std::vector<int>&, std::greater<>) = cppsort::default_sorter();

        sorter(vec, std::greater<>{});
        CHECK( std::is_sorted(std::begin(vec), std::end(vec), std::greater<>{}) );
    }

    SECTION( "sort with random-access iterable and projection" )
    {
        constexpr void(*sorter)(std::vector<int>&, decltype(projection)) = cppsort::default_sorter();

        sorter(vec, projection);
        CHECK( std::is_sorted(std::begin(vec), std::end(vec), std::greater<>{}) );
    }

    SECTION( "sort with random-access iterable, compare and projection" )
    {
        constexpr void(*sorter)(std::vector<int>&,
                                std::greater<>,
                                decltype(projection))
            = cppsort::default_sorter();

        sorter(vec, std::greater<>{}, projection);
        CHECK( std::is_sorted(std::begin(vec), std::end(vec)) );
    }

    SECTION( "sort with random-access iterators" )
    {
        constexpr void(*sorter)(std::vector<int>::iterator,
                                std::vector<int>::iterator)
            = cppsort::default_sorter();

        sorter(std::begin(vec), std::end(vec));
        CHECK( std::is_sorted(std::begin(vec), std::end(vec)) );
    }

    SECTION( "sort with random-access iterators and compare" )
    {
        constexpr void(*sorter)(std::vector<int>::iterator,
                                std::vector<int>::iterator,
                                std::greater<>)
            = cppsort::default_sorter();

        sorter(std::begin(vec), std::end(vec), std::greater<>{});
        CHECK( std::is_sorted(std::begin(vec), std::end(vec), std::greater<>{}) );
    }

    SECTION( "sort with random-access iterators and projection" )
    {
        constexpr void(*sorter)(std::vector<int>::iterator,
                                std::vector<int>::iterator,
                                decltype(projection))
            = cppsort::default_sorter();

        sorter(std::begin(vec), std::end(vec), projection);
        CHECK( std::is_sorted(std::begin(vec), std::end(vec), std::greater<>{}) );
    }

    SECTION( "sort with random-access iterators, compare and projection" )
    {
        constexpr void(*sorter)(std::vector<int>::iterator,
                                std::vector<int>::iterator,
                                std::greater<>,
                                decltype(projection))
            = cppsort::default_sorter();

        sorter(std::begin(vec), std::end(vec), std::greater<>{}, projection);
        CHECK( std::is_sorted(std::begin(vec), std::end(vec)) );
    }

    SECTION( "sort with bidirectional iterators" )
    {
        constexpr void(*sorter)(std::list<int>::iterator,
                                std::list<int>::iterator)
            = cppsort::default_sorter();

        std::list<int> li(std::begin(vec), std::end(vec));
        sorter(std::begin(li), std::end(li));
        CHECK( std::is_sorted(std::begin(li), std::end(li)) );
    }

    SECTION( "sort with bidirectional iterators and compare" )
    {
        constexpr void(*sorter)(std::list<int>::iterator,
                                std::list<int>::iterator,
                                std::greater<>)
            = cppsort::default_sorter();

        std::list<int> li(std::begin(vec), std::end(vec));
        sorter(std::begin(li), std::end(li), std::greater<>{});
        CHECK( std::is_sorted(std::begin(li), std::end(li), std::greater<>{}) );
    }

    SECTION( "sort with bidirectional iterators and projection" )
    {
        constexpr void(*sorter)(std::list<int>::iterator,
                                std::list<int>::iterator,
                                decltype(projection))
            = cppsort::default_sorter();

        std::list<int> li(std::begin(vec), std::end(vec));
        sorter(std::begin(li), std::end(li), projection);
        CHECK( std::is_sorted(std::begin(li), std::end(li), std::greater<>{}) );
    }

    SECTION( "sort with bidirectional iterators, compare and projection" )
    {
        constexpr void(*sorter)(std::list<int>::iterator,
                                std::list<int>::iterator,
                                std::greater<>,
                      decltype(projection))
            = cppsort::default_sorter();

        std::list<int> li(std::begin(vec), std::end(vec));
        sorter(std::begin(li), std::end(li), std::greater<>{}, projection);
        CHECK( std::is_sorted(std::begin(li), std::end(li)) );
    }

    SECTION( "sort with forward iterators" )
    {
        constexpr void(*sorter)(std::forward_list<int>::iterator,
                                std::forward_list<int>::iterator)
            = cppsort::default_sorter();

        std::forward_list<int> li(std::begin(vec), std::end(vec));
        sorter(std::begin(li), std::end(li));
        CHECK( std::is_sorted(std::begin(li), std::end(li)) );
    }

    SECTION( "sort with forward iterators and compare" )
    {
        constexpr void(*sorter)(std::forward_list<int>::iterator,
                                std::forward_list<int>::iterator,
                                std::greater<>)
            = cppsort::default_sorter();

        std::forward_list<int> li(std::begin(vec), std::end(vec));
        sorter(std::begin(li), std::end(li), std::greater<>{});
        CHECK( std::is_sorted(std::begin(li), std::end(li), std::greater<>{}) );
    }

    SECTION( "sort with forward iterators and projection" )
    {
        constexpr void(*sorter)(std::forward_list<int>::iterator,
                                std::forward_list<int>::iterator,
                                decltype(projection))
            = cppsort::default_sorter();

        std::forward_list<int> li(std::begin(vec), std::end(vec));
        sorter(std::begin(li), std::end(li), projection);
        CHECK( std::is_sorted(std::begin(li), std::end(li), std::greater<>{}) );
    }

    SECTION( "sort with forward iterators and projection" )
    {
        constexpr void(*sorter)(std::forward_list<int>::iterator,
                                std::forward_list<int>::iterator,
                                std::greater<>,
                                decltype(projection))
            = cppsort::default_sorter();

        std::forward_list<int> li(std::begin(vec), std::end(vec));
        sorter(std::begin(li), std::end(li), std::greater<>{}, projection);
        CHECK( std::is_sorted(std::begin(li), std::end(li)) );
    }

    SECTION( "sort with self-sortable iterable" )
    {
        constexpr void(*sorter)(std::list<int>&) = cppsort::default_sorter();

        std::list<int> li(std::begin(vec), std::end(vec));
        sorter(li);
        CHECK( std::is_sorted(std::begin(li), std::end(li)) );
    }

    SECTION( "sort with self-sortable iterable and compare" )
    {
        constexpr void(*sorter)(std::forward_list<int>&, std::greater<>) = cppsort::default_sorter();

        std::forward_list<int> li(std::begin(vec), std::end(vec));
        sorter(li, std::greater<>{});
        CHECK( std::is_sorted(std::begin(li), std::end(li), std::greater<>{}) );
    }
}
