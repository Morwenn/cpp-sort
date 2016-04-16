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
#include <forward_list>
#include <functional>
#include <iterator>
#include <random>
#include <vector>
#include <catch.hpp>
#include <cpp-sort/adapters/container_aware_adapter.h>
#include <cpp-sort/sort.h>
#include <cpp-sort/sorters/selection_sorter.h>

TEST_CASE( "container_aware_adapter and std::forward_list",
           "[container_aware_adapter]" )
{
    // Tests for the sorters that have container-aware
    // overloads for std::forward_list

    using sorter = cppsort::container_aware_adapter<
        cppsort::selection_sorter
    >;

    std::vector<double> vec(187);
    std::iota(std::begin(vec), std::end(vec), -25);
    std::mt19937 engine(std::time(nullptr));
    std::shuffle(std::begin(vec), std::end(vec), engine);

    SECTION( "selection_sorter" )
    {
        std::forward_list<double> collection(std::begin(vec), std::end(vec));

        collection = { std::begin(vec), std::end(vec) };
        sorter{}(collection, std::greater<>{});
        CHECK( std::is_sorted(std::begin(collection), std::end(collection), std::greater<>{}) );

        collection = { std::begin(vec), std::end(vec) };
        cppsort::sort(sorter{}, collection, std::greater<>{});
        CHECK( std::is_sorted(std::begin(collection), std::end(collection), std::greater<>{}) );

        collection = { std::begin(vec), std::end(vec) };
        sorter{}(collection, std::negate<>{});
        CHECK( std::is_sorted(std::begin(collection), std::end(collection), std::greater<>{}) );

        collection = { std::begin(vec), std::end(vec) };
        cppsort::sort(sorter{}, collection, std::negate<>{});
        CHECK( std::is_sorted(std::begin(collection), std::end(collection), std::greater<>{}) );

        collection = { std::begin(vec), std::end(vec) };
        sorter{}(collection, std::greater<>{}, std::negate<>{});
        CHECK( std::is_sorted(std::begin(collection), std::end(collection)) );

        collection = { std::begin(vec), std::end(vec) };
        cppsort::sort(sorter{}, collection, std::greater<>{}, std::negate<>{});
        CHECK( std::is_sorted(std::begin(collection), std::end(collection)) );
    }
}
