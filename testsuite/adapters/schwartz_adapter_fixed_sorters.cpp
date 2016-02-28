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
#include <array>
#include <ctime>
#include <iostream>
#include <iterator>
#include <random>
#include <catch.hpp>
#include <cpp-sort/adapters/schwartz_adapter.h>
#include <cpp-sort/adapters/small_array_adapter.h>
#include <cpp-sort/fixed_sorters.h>
#include <cpp-sort/sort.h>
#include "../algorithm.h"

TEST_CASE( "Schwartzian transform adapter with fixed-size sorters",
           "[schwartz_adapter]" )
{
    struct wrapper { double value; };

    auto&& low_comparisons_sort = cppsort::schwartz_adapter<
        cppsort::small_array_adapter<
            cppsort::low_comparisons_sorter
        >
    >{};
    auto&& low_moves_sort = cppsort::schwartz_adapter<
        cppsort::small_array_adapter<
            cppsort::low_moves_sorter
        >
    >{};
    auto&& sorting_network_sort = cppsort::schwartz_adapter<
        cppsort::small_array_adapter<
            cppsort::sorting_network_sorter
        >
    >{};

    std::mt19937 engine(std::time(nullptr));

    SECTION( "size 0" )
    {
        std::array<wrapper, 0> collection;

        cppsort::sort(collection, low_comparisons_sort, &wrapper::value);
        cppsort::sort(collection, low_moves_sort, &wrapper::value);
        cppsort::sort(collection, sorting_network_sort, &wrapper::value);
    }

    SECTION( "size 1" )
    {
        std::array<wrapper, 1> collection;

        cppsort::sort(collection, low_comparisons_sort, &wrapper::value);
        cppsort::sort(collection, low_moves_sort, &wrapper::value);
        cppsort::sort(collection, sorting_network_sort, &wrapper::value);
    }

    SECTION( "size 2" )
    {
        std::array<wrapper, 2> collection;
        helpers::iota(std::begin(collection), std::end(collection), -10.0, &wrapper::value);

        std::shuffle(std::begin(collection), std::end(collection), engine);
        cppsort::sort(collection, low_comparisons_sort, &wrapper::value);
        CHECK( helpers::is_sorted(std::begin(collection), std::end(collection),
                                  std::less<>{}, &wrapper::value) );

        std::shuffle(std::begin(collection), std::end(collection), engine);
        cppsort::sort(collection, low_moves_sort, &wrapper::value);
        CHECK( helpers::is_sorted(std::begin(collection), std::end(collection),
                                  std::less<>{}, &wrapper::value) );

        std::shuffle(std::begin(collection), std::end(collection), engine);
        cppsort::sort(collection, sorting_network_sort, &wrapper::value);
        CHECK( helpers::is_sorted(std::begin(collection), std::end(collection),
                                  std::less<>{}, &wrapper::value) );
    }

    SECTION( "size 3" )
    {
        std::array<wrapper, 3> collection;
        helpers::iota(std::begin(collection), std::end(collection), -10.0, &wrapper::value);

        std::shuffle(std::begin(collection), std::end(collection), engine);
        cppsort::sort(collection, low_comparisons_sort, &wrapper::value);
        CHECK( helpers::is_sorted(std::begin(collection), std::end(collection),
                                  std::less<>{}, &wrapper::value) );

        std::shuffle(std::begin(collection), std::end(collection), engine);
        cppsort::sort(collection, low_moves_sort, &wrapper::value);
        CHECK( helpers::is_sorted(std::begin(collection), std::end(collection),
                                  std::less<>{}, &wrapper::value) );

        std::shuffle(std::begin(collection), std::end(collection), engine);
        cppsort::sort(collection, sorting_network_sort, &wrapper::value);
        CHECK( helpers::is_sorted(std::begin(collection), std::end(collection),
                                  std::less<>{}, &wrapper::value) );
    }

    SECTION( "size 4" )
    {
        std::array<wrapper, 4> collection;
        helpers::iota(std::begin(collection), std::end(collection), -10.0, &wrapper::value);

        std::shuffle(std::begin(collection), std::end(collection), engine);
        cppsort::sort(collection, low_comparisons_sort, &wrapper::value);
        CHECK( helpers::is_sorted(std::begin(collection), std::end(collection),
                                  std::less<>{}, &wrapper::value) );

        std::shuffle(std::begin(collection), std::end(collection), engine);
        cppsort::sort(collection, low_moves_sort, &wrapper::value);
        CHECK( helpers::is_sorted(std::begin(collection), std::end(collection),
                                  std::less<>{}, &wrapper::value) );

        std::shuffle(std::begin(collection), std::end(collection), engine);
        cppsort::sort(collection, sorting_network_sort, &wrapper::value);
        CHECK( helpers::is_sorted(std::begin(collection), std::end(collection),
                                  std::less<>{}, &wrapper::value) );
    }

    SECTION( "size 5" )
    {
        std::array<wrapper, 5> collection;
        helpers::iota(std::begin(collection), std::end(collection), -10.0, &wrapper::value);

        std::shuffle(std::begin(collection), std::end(collection), engine);
        cppsort::sort(collection, low_comparisons_sort, &wrapper::value);
        CHECK( helpers::is_sorted(std::begin(collection), std::end(collection),
                                  std::less<>{}, &wrapper::value) );

        std::shuffle(std::begin(collection), std::end(collection), engine);
        cppsort::sort(collection, low_moves_sort, &wrapper::value);
        CHECK( helpers::is_sorted(std::begin(collection), std::end(collection),
                                  std::less<>{}, &wrapper::value) );

        std::shuffle(std::begin(collection), std::end(collection), engine);
        cppsort::sort(collection, sorting_network_sort, &wrapper::value);
        CHECK( helpers::is_sorted(std::begin(collection), std::end(collection),
                                  std::less<>{}, &wrapper::value) );
    }

    SECTION( "size 6" )
    {
        std::array<wrapper, 6> collection;
        helpers::iota(std::begin(collection), std::end(collection), -10.0, &wrapper::value);

        std::shuffle(std::begin(collection), std::end(collection), engine);
        cppsort::sort(collection, low_comparisons_sort, &wrapper::value);
        CHECK( helpers::is_sorted(std::begin(collection), std::end(collection),
                                  std::less<>{}, &wrapper::value) );

        std::shuffle(std::begin(collection), std::end(collection), engine);
        cppsort::sort(collection, low_moves_sort, &wrapper::value);
        CHECK( helpers::is_sorted(std::begin(collection), std::end(collection),
                                  std::less<>{}, &wrapper::value) );

        std::shuffle(std::begin(collection), std::end(collection), engine);
        cppsort::sort(collection, sorting_network_sort, &wrapper::value);
        CHECK( helpers::is_sorted(std::begin(collection), std::end(collection),
                                  std::less<>{}, &wrapper::value) );
    }
}
