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
#include <cpp-sort/sorters/merge_sorter.h>
#include <cpp-sort/sort.h>

template<typename Iterator, typename Compare, typename Projection>
auto is_sorted(Iterator first, Iterator last, Compare compare, Projection projection)
    -> bool
{
    auto&& proj = cppsort::detail::as_function(projection);
    for (auto it = std::next(first) ; it != last ; ++it)
    {
        if (compare(proj(it), proj(first)))
        {
            return false;
        }
    }
    return true;
}

TEST_CASE( "merge_sorter tests with projections",
           "[merge_sorter][projection]" )
{
    // Pseudo-random number engine
    std::mt19937_64 engine(std::time(nullptr));

    // Wrapper to hide the integer
    struct wrapper { int value; };

    // Collection to sort
    std::vector<wrapper> vec(80);
    int value = 0;
    for (auto& elem: vec)
    {
        elem.value = value++;
    }

    SECTION( "sort with random-access iterable" )
    {
        std::shuffle(std::begin(vec), std::end(vec), engine);
        cppsort::sort(vec, cppsort::merge_sorter{}, &wrapper::value);
        CHECK( is_sorted(std::begin(vec), std::end(vec), std::less<>{}, &wrapper::value) );
    }

    SECTION( "sort with random-access iterable and compare" )
    {
        std::shuffle(std::begin(vec), std::end(vec), engine);
        cppsort::sort(vec, cppsort::merge_sorter{}, std::greater<>{}, &wrapper::value);
        CHECK( is_sorted(std::begin(vec), std::end(vec), std::greater<>{}, &wrapper::value) );
    }

    SECTION( "sort with random-access iterators" )
    {
        std::shuffle(std::begin(vec), std::end(vec), engine);
        cppsort::sort(std::begin(vec), std::end(vec), cppsort::merge_sorter{}, &wrapper::value);
        CHECK( is_sorted(std::begin(vec), std::end(vec), std::less<>{}, &wrapper::value) );
    }

    SECTION( "sort with random-access iterators and compare" )
    {
        std::shuffle(std::begin(vec), std::end(vec), engine);
        cppsort::sort(std::begin(vec), std::end(vec), cppsort::merge_sorter{}, std::greater<>{}, &wrapper::value);
        CHECK( is_sorted(std::begin(vec), std::end(vec), std::greater<>{}, &wrapper::value) );
    }

    SECTION( "sort with bidirectional iterators" )
    {
        std::shuffle(std::begin(vec), std::end(vec), engine);
        std::list<wrapper> li(std::begin(vec), std::end(vec));
        cppsort::sort(std::begin(li), std::end(li), cppsort::merge_sorter{}, &wrapper::value);
        CHECK( is_sorted(std::begin(li), std::end(li), std::less<>{}, &wrapper::value) );
    }

    SECTION( "sort with bidirectional iterators and compare" )
    {
        std::shuffle(std::begin(vec), std::end(vec), engine);
        std::list<wrapper> li(std::begin(vec), std::end(vec));
        cppsort::sort(std::begin(li), std::end(li), cppsort::merge_sorter{}, std::greater<>{}, &wrapper::value);
        CHECK( is_sorted(std::begin(li), std::end(li), std::greater<>{}, &wrapper::value) );
    }

    SECTION( "sort with forward iterators" )
    {
        std::shuffle(std::begin(vec), std::end(vec), engine);
        std::forward_list<wrapper> li(std::begin(vec), std::end(vec));
        cppsort::sort(std::begin(li), std::end(li), cppsort::merge_sorter{}, &wrapper::value);
        CHECK( is_sorted(std::begin(li), std::end(li), std::less<>{}, &wrapper::value) );
    }

    SECTION( "sort with forward iterators and compare" )
    {
        std::shuffle(std::begin(vec), std::end(vec), engine);
        std::forward_list<wrapper> li(std::begin(vec), std::end(vec));
        cppsort::sort(std::begin(li), std::end(li), cppsort::merge_sorter{}, std::greater<>{}, &wrapper::value);
        CHECK( is_sorted(std::begin(li), std::end(li), std::greater<>{}, &wrapper::value) );
    }
}
