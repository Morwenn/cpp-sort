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
#include <functional>
#include <iterator>
#include <numeric>
#include <random>
#include <vector>
#include <catch.hpp>
#include <cpp-sort/sort.h>
#include <cpp-sort/sorters/spread_sorter.h>
#include <cpp-sort/utility/functional.h>

TEST_CASE( "spread_sorter generate overloads",
           "[spread_sorter][sorter_facade]" )
{
    // These tests use spread_sorter as a real-world Guinea pig
    // to make sure that sorter_facade generates the expected
    // operator() overloads for non-comparison sorters

    // Pseudo-random number engine
    std::mt19937_64 engine(std::time(nullptr));

    SECTION( "default operator() with std::less<>" )
    {
        std::vector<int> vec(100'000);
        std::iota(std::begin(vec), std::end(vec), 0);

        std::shuffle(std::begin(vec), std::end(vec), engine);
        cppsort::sort(vec, cppsort::spread_sorter{}, std::less<>{});
        CHECK( std::is_sorted(std::begin(vec), std::end(vec), std::less<>{}) );

        std::shuffle(std::begin(vec), std::end(vec), engine);
        cppsort::sort(std::begin(vec), std::end(vec), cppsort::spread_sorter{}, std::less<>{});
        CHECK( std::is_sorted(std::begin(vec), std::end(vec), std::less<>{}) );
    }

    SECTION( "default operator() with utility::identity" )
    {
        std::vector<int> vec(100'000);
        std::iota(std::begin(vec), std::end(vec), 0);

        std::shuffle(std::begin(vec), std::end(vec), engine);
        cppsort::sort(vec, cppsort::spread_sorter{}, cppsort::utility::identity{});
        CHECK( std::is_sorted(std::begin(vec), std::end(vec)) );

        std::shuffle(std::begin(vec), std::end(vec), engine);
        cppsort::sort(std::begin(vec), std::end(vec), cppsort::spread_sorter{}, cppsort::utility::identity{});
        CHECK( std::is_sorted(std::begin(vec), std::end(vec)) );
    }

    SECTION( "default operator() with both" )
    {
        std::vector<int> vec(100'000);
        std::iota(std::begin(vec), std::end(vec), 0);

        std::shuffle(std::begin(vec), std::end(vec), engine);
        cppsort::sort(vec, cppsort::spread_sorter{}, std::less<>{}, cppsort::utility::identity{});
        CHECK( std::is_sorted(std::begin(vec), std::end(vec)) );

        std::shuffle(std::begin(vec), std::end(vec), engine);
        cppsort::sort(std::begin(vec), std::end(vec), cppsort::spread_sorter{}, std::less<>{}, cppsort::utility::identity{});
        CHECK( std::is_sorted(std::begin(vec), std::end(vec)) );
    }
}
