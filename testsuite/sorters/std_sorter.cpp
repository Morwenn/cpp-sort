/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2016 Morwenn
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
#include <functional>
#include <iterator>
#include <numeric>
#include <random>
#include <vector>
#include <catch.hpp>
#include <cpp-sort/sorters/std_sorter.h>
#include <cpp-sort/sort.h>
#include "../algorithm.h"

TEST_CASE( "std_sorter tests", "[std_sorter]" )
{
    // Pseudo-random number engine
    std::mt19937_64 engine(std::time(nullptr));

    // Collection to sort
    std::vector<int> vec(80);
    std::iota(std::begin(vec), std::end(vec), 0);
    std::shuffle(std::begin(vec), std::end(vec), engine);

    SECTION( "sort with iterable" )
    {
        cppsort::sort(cppsort::std_sorter{}, vec);
        CHECK( std::is_sorted(std::begin(vec), std::end(vec)) );
    }

    SECTION( "sort with iterable and compare" )
    {
        cppsort::sort(cppsort::std_sorter{}, vec, std::greater<>{});
        CHECK( std::is_sorted(std::begin(vec), std::end(vec), std::greater<>{}) );
    }

    SECTION( "sort with iterators" )
    {
        cppsort::sort(cppsort::std_sorter{}, std::begin(vec), std::end(vec));
        CHECK( std::is_sorted(std::begin(vec), std::end(vec)) );
    }

    SECTION( "sort with iterators and compare" )
    {
        cppsort::sort(cppsort::std_sorter{}, std::begin(vec), std::end(vec), std::greater<>{});
        CHECK( std::is_sorted(std::begin(vec), std::end(vec), std::greater<>{}) );
    }
}

TEST_CASE( "std_sorter tests with projections",
           "[std_sorter][projection]" )
{
    // Pseudo-random number engine
    std::mt19937_64 engine(std::time(nullptr));

    // Wrapper to hide the integer
    struct wrapper { int value; };

    // Collection to sort
    std::vector<wrapper> vec(80);
    helpers::iota(std::begin(vec), std::end(vec), 0, &wrapper::value);
    std::shuffle(std::begin(vec), std::end(vec), engine);

    SECTION( "sort with iterable" )
    {
        cppsort::sort(cppsort::std_sorter{}, vec, &wrapper::value);
        CHECK( helpers::is_sorted(std::begin(vec), std::end(vec), std::less<>{}, &wrapper::value) );
    }

    SECTION( "sort with iterable and compare" )
    {
        cppsort::sort(cppsort::std_sorter{}, vec, std::greater<>{}, &wrapper::value);
        CHECK( helpers::is_sorted(std::begin(vec), std::end(vec), std::greater<>{}, &wrapper::value) );
    }

    SECTION( "sort with iterators" )
    {
        cppsort::sort(cppsort::std_sorter{}, std::begin(vec), std::end(vec), &wrapper::value);
        CHECK( helpers::is_sorted(std::begin(vec), std::end(vec), std::less<>{}, &wrapper::value) );
    }

    SECTION( "sort with iterators and compare" )
    {
        cppsort::sort(cppsort::std_sorter{}, std::begin(vec), std::end(vec), std::greater<>{}, &wrapper::value);
        CHECK( helpers::is_sorted(std::begin(vec), std::end(vec), std::greater<>{}, &wrapper::value) );
    }
}
