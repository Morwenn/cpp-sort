/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2018 Morwenn
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
#include <functional>
#include <catch.hpp>
#include <cpp-sort/sort.h>
#include <cpp-sort/sorters/selection_sorter.h>
#include <cpp-sort/utility/functional.h>
#include "algorithm.h"

TEST_CASE( "test sorting C arrays", "[sort]" )
{
    int arr[] = { 8, 1, 6, 7, 3, 5, 4, 9, 2 };

    SECTION( "without anything" )
    {
        cppsort::sort(arr);
        CHECK( helpers::is_sorted(std::begin(arr), std::end(arr)) );
    }

    SECTION( "with comparison function" )
    {
        cppsort::sort(arr, std::greater<>{});
        CHECK( helpers::is_sorted(std::begin(arr), std::end(arr), std::greater<>{}) );
    }

    SECTION( "with projection function" )
    {
        cppsort::sort(arr, std::negate<>{});
        CHECK( helpers::is_sorted(std::begin(arr), std::end(arr), std::greater<>{}) );
    }

    SECTION( "with comparison and projection functions" )
    {
        cppsort::sort(arr, std::greater<>{}, std::negate<>{});
        CHECK( helpers::is_sorted(std::begin(arr), std::end(arr)) );
    }

    SECTION( "with sorter" )
    {
        cppsort::sort(cppsort::selection_sort, arr);
        CHECK( helpers::is_sorted(std::begin(arr), std::end(arr)) );
    }

    SECTION( "with sorter and comparison function" )
    {
        cppsort::sort(cppsort::selection_sort, arr, std::greater<>{});
        CHECK( helpers::is_sorted(std::begin(arr), std::end(arr), std::greater<>{}) );
    }

    SECTION( "with sorter and projection function" )
    {
        cppsort::sort(cppsort::selection_sort, arr, std::negate<>{});
        CHECK( helpers::is_sorted(std::begin(arr), std::end(arr), std::greater<>{}) );
    }

    SECTION( "with sorter, comparison and projection function" )
    {
        cppsort::sort(cppsort::selection_sort, arr, std::greater<>{}, std::negate<>{});
        CHECK( helpers::is_sorted(std::begin(arr), std::end(arr)) );
    }
}
