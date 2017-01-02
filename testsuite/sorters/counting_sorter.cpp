/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2016-2017 Morwenn
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
#include <forward_list>
#include <iterator>
#include <list>
#include <vector>
#include <catch.hpp>
#include <cpp-sort/sorters/counting_sorter.h>
#include <cpp-sort/sort.h>
#include "../distributions.h"

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
        cppsort::sort(cppsort::counting_sorter{}, vec);
        CHECK( std::is_sorted(std::begin(vec), std::end(vec)) );
    }

    SECTION( "sort with unsigned int iterators" )
    {
        std::list<unsigned> li;;
        distribution(std::back_inserter(li), size, 0u);
        cppsort::sort(cppsort::counting_sorter{}, std::begin(li), std::end(li));
        CHECK( std::is_sorted(std::begin(li), std::end(li)) );
    }

    SECTION( "reverse sort with long long iterable" )
    {
        std::vector<long long> vec; vec.reserve(size);
        distribution(std::back_inserter(vec), size, 1568);
        cppsort::sort(cppsort::counting_sorter{}, vec);
        CHECK( std::is_sorted(std::begin(vec), std::end(vec)) );
    }

    SECTION( "reverse sort with unsigned long long iterators" )
    {
        std::forward_list<unsigned long long> li;
        distribution(std::front_inserter(li), size, 0ULL);
        cppsort::sort(cppsort::counting_sorter{}, std::begin(li), std::end(li));
        CHECK( std::is_sorted(std::begin(li), std::end(li)) );
    }
}
