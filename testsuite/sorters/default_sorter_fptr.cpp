/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2015 Morwenn
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */
#include <algorithm>
#include <ctime>
#include <forward_list>
#include <functional>
#include <iterator>
#include <list>
#include <random>
#include <vector>
#include <catch.hpp>
#include <cpp-sort/sorters/default_sorter.h>

TEST_CASE( "default sorter function pointer tests",
           "[default_sorter][function_pointer]" )
{
    // Pseudo-random number engine
    std::mt19937_64 engine(std::time(nullptr));

    // Collection to sort
    std::vector<int> vec(80);
    std::iota(std::begin(vec), std::end(vec), 0);

    // Projection to sort in descending order
    auto projection = [](int n) { return -n; };

    SECTION( "sort with random-access iterable" )
    {
        void(*sorter)(std::vector<int>&) = cppsort::default_sorter();

        std::shuffle(std::begin(vec), std::end(vec), engine);
        sorter(vec);
        CHECK( std::is_sorted(std::begin(vec), std::end(vec)) );
    }

    SECTION( "sort with random-access iterable and compare" )
    {
        void(*sorter)(std::vector<int>&, std::greater<>) = cppsort::default_sorter();

        std::shuffle(std::begin(vec), std::end(vec), engine);
        sorter(vec, std::greater<>{});
        CHECK( std::is_sorted(std::begin(vec), std::end(vec), std::greater<>{}) );
    }

    SECTION( "sort with random-access iterable and projection" )
    {
        void(*sorter)(std::vector<int>&, decltype(projection)) = cppsort::default_sorter();

        std::shuffle(std::begin(vec), std::end(vec), engine);
        sorter(vec, projection);
        CHECK( std::is_sorted(std::begin(vec), std::end(vec), std::greater<>{}) );
    }

    SECTION( "sort with random-access iterable, compare and projection" )
    {
        void(*sorter)(std::vector<int>&,
                      std::greater<>,
                      decltype(projection))
            = cppsort::default_sorter();

        std::shuffle(std::begin(vec), std::end(vec), engine);
        sorter(vec, std::greater<>{}, projection);
        CHECK( std::is_sorted(std::begin(vec), std::end(vec)) );
    }

    SECTION( "sort with random-access iterators" )
    {
        void(*sorter)(std::vector<int>::iterator,
                      std::vector<int>::iterator)
            = cppsort::default_sorter();

        std::shuffle(std::begin(vec), std::end(vec), engine);
        sorter(std::begin(vec), std::end(vec));
        CHECK( std::is_sorted(std::begin(vec), std::end(vec)) );
    }

    SECTION( "sort with random-access iterators and compare" )
    {
        void(*sorter)(std::vector<int>::iterator,
                      std::vector<int>::iterator,
                      std::greater<>)
            = cppsort::default_sorter();

        std::shuffle(std::begin(vec), std::end(vec), engine);
        sorter(std::begin(vec), std::end(vec), std::greater<>{});
        CHECK( std::is_sorted(std::begin(vec), std::end(vec), std::greater<>{}) );
    }

    SECTION( "sort with random-access iterators and projection" )
    {
        void(*sorter)(std::vector<int>::iterator,
                      std::vector<int>::iterator,
                      decltype(projection))
            = cppsort::default_sorter();

        std::shuffle(std::begin(vec), std::end(vec), engine);
        sorter(std::begin(vec), std::end(vec), projection);
        CHECK( std::is_sorted(std::begin(vec), std::end(vec), std::greater<>{}) );
    }

    SECTION( "sort with random-access iterators, compare and projection" )
    {
        void(*sorter)(std::vector<int>::iterator,
                      std::vector<int>::iterator,
                      std::greater<>,
                      decltype(projection))
            = cppsort::default_sorter();

        std::shuffle(std::begin(vec), std::end(vec), engine);
        sorter(std::begin(vec), std::end(vec), std::greater<>{}, projection);
        CHECK( std::is_sorted(std::begin(vec), std::end(vec)) );
    }

    SECTION( "sort with bidirectional iterators" )
    {
        void(*sorter)(std::list<int>::iterator,
                      std::list<int>::iterator)
            = cppsort::default_sorter();

        std::shuffle(std::begin(vec), std::end(vec), engine);
        std::list<int> li(std::begin(vec), std::end(vec));
        sorter(std::begin(li), std::end(li));
        CHECK( std::is_sorted(std::begin(li), std::end(li)) );
    }

    SECTION( "sort with bidirectional iterators and compare" )
    {
        void(*sorter)(std::list<int>::iterator,
                      std::list<int>::iterator,
                      std::greater<>)
            = cppsort::default_sorter();

        std::shuffle(std::begin(vec), std::end(vec), engine);
        std::list<int> li(std::begin(vec), std::end(vec));
        sorter(std::begin(li), std::end(li), std::greater<>{});
        CHECK( std::is_sorted(std::begin(li), std::end(li), std::greater<>{}) );
    }

    SECTION( "sort with bidirectional iterators and projection" )
    {
        void(*sorter)(std::list<int>::iterator,
                      std::list<int>::iterator,
                      decltype(projection))
            = cppsort::default_sorter();

        std::shuffle(std::begin(vec), std::end(vec), engine);
        std::list<int> li(std::begin(vec), std::end(vec));
        sorter(std::begin(li), std::end(li), projection);
        CHECK( std::is_sorted(std::begin(li), std::end(li), std::greater<>{}) );
    }

    SECTION( "sort with bidirectional iterators, compare and projection" )
    {
        void(*sorter)(std::list<int>::iterator,
                      std::list<int>::iterator,
                      std::greater<>,
                      decltype(projection))
            = cppsort::default_sorter();

        std::shuffle(std::begin(vec), std::end(vec), engine);
        std::list<int> li(std::begin(vec), std::end(vec));
        sorter(std::begin(li), std::end(li), std::greater<>{}, projection);
        CHECK( std::is_sorted(std::begin(li), std::end(li)) );
    }

    SECTION( "sort with forward iterators" )
    {
        void(*sorter)(std::forward_list<int>::iterator,
                      std::forward_list<int>::iterator)
            = cppsort::default_sorter();

        std::shuffle(std::begin(vec), std::end(vec), engine);
        std::forward_list<int> li(std::begin(vec), std::end(vec));
        sorter(std::begin(li), std::end(li));
        CHECK( std::is_sorted(std::begin(li), std::end(li)) );
    }

    SECTION( "sort with forward iterators and compare" )
    {
        void(*sorter)(std::forward_list<int>::iterator,
                      std::forward_list<int>::iterator,
                      std::greater<>)
            = cppsort::default_sorter();

        std::shuffle(std::begin(vec), std::end(vec), engine);
        std::forward_list<int> li(std::begin(vec), std::end(vec));
        sorter(std::begin(li), std::end(li), std::greater<>{});
        CHECK( std::is_sorted(std::begin(li), std::end(li), std::greater<>{}) );
    }

    SECTION( "sort with forward iterators and projection" )
    {
        void(*sorter)(std::forward_list<int>::iterator,
                      std::forward_list<int>::iterator,
                      decltype(projection))
            = cppsort::default_sorter();

        std::shuffle(std::begin(vec), std::end(vec), engine);
        std::forward_list<int> li(std::begin(vec), std::end(vec));
        sorter(std::begin(li), std::end(li), projection);
        CHECK( std::is_sorted(std::begin(li), std::end(li), std::greater<>{}) );
    }

    SECTION( "sort with forward iterators and projection" )
    {
        void(*sorter)(std::forward_list<int>::iterator,
                      std::forward_list<int>::iterator,
                      std::greater<>,
                      decltype(projection))
            = cppsort::default_sorter();

        std::shuffle(std::begin(vec), std::end(vec), engine);
        std::forward_list<int> li(std::begin(vec), std::end(vec));
        sorter(std::begin(li), std::end(li), std::greater<>{}, projection);
        CHECK( std::is_sorted(std::begin(li), std::end(li)) );
    }

    SECTION( "sort with self-sortable iterable" )
    {
        void(*sorter)(std::list<int>&) = cppsort::default_sorter();

        std::shuffle(std::begin(vec), std::end(vec), engine);
        std::list<int> li(std::begin(vec), std::end(vec));
        sorter(li);
        CHECK( std::is_sorted(std::begin(li), std::end(li)) );
    }

    SECTION( "sort with self-sortable iterable and compare" )
    {
        void(*sorter)(std::forward_list<int>&, std::greater<>) = cppsort::default_sorter();

        std::shuffle(std::begin(vec), std::end(vec), engine);
        std::forward_list<int> li(std::begin(vec), std::end(vec));
        sorter(li, std::greater<>{});
        CHECK( std::is_sorted(std::begin(li), std::end(li), std::greater<>{}) );
    }
}
