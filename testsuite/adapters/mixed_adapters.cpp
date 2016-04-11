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
#include <random>
#include <vector>
#include <catch.hpp>
#include <cpp-sort/adapters/indirect_adapter.h>
#include <cpp-sort/adapters/schwartz_adapter.h>
#include <cpp-sort/sort.h>
#include <cpp-sort/sorters/selection_sorter.h>
#include "../algorithm.h"

//
// Tests several adapters at once
//

TEST_CASE( "indirect sort with Schwartzian transform",
           "[indirect_adapter][schwartz_adapter]" )
{
    struct wrapper { short value; };

    std::vector<wrapper> collection(334);
    helpers::iota(std::begin(collection), std::end(collection), -93, &wrapper::value);
    std::mt19937 engine(std::time(nullptr));
    std::shuffle(std::begin(collection), std::end(collection), engine);

    SECTION( "schwartz_adapter over indirect_adapter" )
    {
        using sorter = cppsort::schwartz_adapter<
            cppsort::indirect_adapter<
                cppsort::selection_sorter
            >
        >;

        cppsort::sort(sorter{}, collection, &wrapper::value);
        CHECK( helpers::is_sorted(std::begin(collection), std::end(collection),
                                  std::less<>{}, &wrapper::value) );
    }

    SECTION( "indirect_adapter over schwartz_adapter" )
    {
        using sorter = cppsort::indirect_adapter<
            cppsort::schwartz_adapter<
                cppsort::selection_sorter
            >
        >;

        cppsort::sort(sorter{}, collection, &wrapper::value);
        CHECK( helpers::is_sorted(std::begin(collection), std::end(collection),
                                  std::less<>{}, &wrapper::value) );
    }

    SECTION( "schwartz_adapter over schwartz_adapter" )
    {
        using sorter = cppsort::schwartz_adapter<
            cppsort::schwartz_adapter<
                cppsort::selection_sorter
            >
        >;

        cppsort::sort(sorter{}, collection, &wrapper::value);
        CHECK( helpers::is_sorted(std::begin(collection), std::end(collection),
                                  std::less<>{}, &wrapper::value) );
    }

    SECTION( "indirect_adapter over indirect_adapter" )
    {
        using sorter = cppsort::indirect_adapter<
            cppsort::indirect_adapter<
                cppsort::selection_sorter
            >
        >;

        cppsort::sort(sorter{}, collection, &wrapper::value);
        CHECK( helpers::is_sorted(std::begin(collection), std::end(collection),
                                  std::less<>{}, &wrapper::value) );
    }
}
