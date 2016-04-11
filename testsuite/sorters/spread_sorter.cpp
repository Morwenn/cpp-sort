/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2015-2016 Morwenn
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
#include <iterator>
#include <numeric>
#include <random>
#include <string>
#include <vector>
#include <catch.hpp>
#include <cpp-sort/sorters/spread_sorter.h>
#include <cpp-sort/sort.h>

TEST_CASE( "spread_sorter tests", "[spread_sorter]" )
{
    // Pseudo-random number engine
    std::mt19937_64 engine(std::time(nullptr));

    SECTION( "sort with int iterable" )
    {
        std::vector<int> vec(100'000);
        std::iota(std::begin(vec), std::end(vec), 0);
        std::shuffle(std::begin(vec), std::end(vec), engine);
        cppsort::sort(cppsort::spread_sorter{}, vec);
        CHECK( std::is_sorted(std::begin(vec), std::end(vec)) );
    }

    SECTION( "sort with unsigned int iterators" )
    {
        std::vector<unsigned> vec(100'000);
        std::iota(std::begin(vec), std::end(vec), 0u);
        std::shuffle(std::begin(vec), std::end(vec), engine);
        cppsort::sort(cppsort::spread_sorter{}, std::begin(vec), std::end(vec));
        CHECK( std::is_sorted(std::begin(vec), std::end(vec)) );
    }

    SECTION( "sort with float iterable" )
    {
        std::vector<float> vec(100'000);
        std::iota(std::begin(vec), std::end(vec), 0.0f);
        std::shuffle(std::begin(vec), std::end(vec), engine);
        cppsort::sort(cppsort::spread_sorter{}, vec);
        CHECK( std::is_sorted(std::begin(vec), std::end(vec)) );
    }

    SECTION( "sort with double iterators" )
    {
        std::vector<double> vec(100'000);
        std::iota(std::begin(vec), std::end(vec), 0.0);
        std::shuffle(std::begin(vec), std::end(vec), engine);
        cppsort::sort(cppsort::spread_sorter{}, std::begin(vec), std::end(vec));
        CHECK( std::is_sorted(std::begin(vec), std::end(vec)) );
    }

    SECTION( "sort with std::string" )
    {
        std::vector<std::string> vec;
        for (int i = 0 ; i < 100'000 ; ++i)
        {
            vec.push_back(std::to_string(i));
        }

        std::shuffle(std::begin(vec), std::end(vec), engine);
        cppsort::sort(cppsort::spread_sorter{}, vec);
        CHECK( std::is_sorted(std::begin(vec), std::end(vec)) );

        std::shuffle(std::begin(vec), std::end(vec), engine);
        cppsort::sort(cppsort::spread_sorter{}, std::begin(vec), std::end(vec));
        CHECK( std::is_sorted(std::begin(vec), std::end(vec)) );
    }

    SECTION( "reverse sort with std::string" )
    {
        std::vector<std::string> vec;
        for (int i = 0 ; i < 100'000 ; ++i)
        {
            vec.push_back(std::to_string(i));
        }

        std::shuffle(std::begin(vec), std::end(vec), engine);
        cppsort::sort(cppsort::spread_sorter{}, vec, std::greater<>{});
        CHECK( std::is_sorted(std::begin(vec), std::end(vec), std::greater<>{}) );

        std::shuffle(std::begin(vec), std::end(vec), engine);
        cppsort::sort(cppsort::spread_sorter{}, std::begin(vec), std::end(vec), std::greater<>{});
        CHECK( std::is_sorted(std::begin(vec), std::end(vec), std::greater<>{}) );
    }
}
