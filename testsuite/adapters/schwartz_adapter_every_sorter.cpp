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
#include <iterator>
#include <random>
#include <vector>
#include <catch.hpp>
#include <cpp-sort/adapters/schwartz_adapter.h>
#include <cpp-sort/sort.h>
#include <cpp-sort/sorters.h>
#include <cpp-sort/utility/buffer.h>
#include "../algorithm.h"

TEST_CASE( "every sorter with Schwartzian transform adapter",
           "[schwartz_adapter]" )
{
    struct wrapper { double value; };

    std::vector<wrapper> collection(412);
    helpers::iota(std::begin(collection), std::end(collection), -125, &wrapper::value);
    std::mt19937 engine(std::time(nullptr));
    std::shuffle(std::begin(collection), std::end(collection), engine);

    SECTION( "block_sorter" )
    {
        using namespace cppsort;

        using sorter = schwartz_adapter<block_sorter<
            utility::fixed_buffer<0>
        >>;
        cppsort::sort(collection, sorter{}, &wrapper::value);
        CHECK( helpers::is_sorted(std::begin(collection), std::end(collection),
                                  std::less<>{}, &wrapper::value) );
    }

    SECTION( "default_sorter" )
    {
        using sorter = cppsort::schwartz_adapter<cppsort::default_sorter>;
        cppsort::sort(collection, sorter{}, &wrapper::value);
        CHECK( helpers::is_sorted(std::begin(collection), std::end(collection),
                                  std::less<>{}, &wrapper::value) );
    }

    SECTION( "grail_sorter" )
    {
        using sorter = cppsort::schwartz_adapter<cppsort::grail_sorter<>>;
        cppsort::sort(collection, sorter{}, &wrapper::value);
        CHECK( helpers::is_sorted(std::begin(collection), std::end(collection),
                                  std::less<>{}, &wrapper::value) );
    }

    SECTION( "heap_sorter" )
    {
        using sorter = cppsort::schwartz_adapter<cppsort::heap_sorter>;
        cppsort::sort(collection, sorter{}, &wrapper::value);
        CHECK( helpers::is_sorted(std::begin(collection), std::end(collection),
                                  std::less<>{}, &wrapper::value) );
    }

    SECTION( "insertion_sorter" )
    {
        using sorter = cppsort::schwartz_adapter<cppsort::insertion_sorter>;
        cppsort::sort(collection, sorter{}, &wrapper::value);
        CHECK( helpers::is_sorted(std::begin(collection), std::end(collection),
                                  std::less<>{}, &wrapper::value) );
    }

    SECTION( "merge_insertion_sorter" )
    {
        using sorter = cppsort::schwartz_adapter<cppsort::merge_insertion_sorter>;
        cppsort::sort(collection, sorter{}, &wrapper::value);
        CHECK( helpers::is_sorted(std::begin(collection), std::end(collection),
                                  std::less<>{}, &wrapper::value) );
    }

    SECTION( "merge_sorter" )
    {
        using sorter = cppsort::schwartz_adapter<cppsort::merge_sorter>;
        cppsort::sort(collection, sorter{}, &wrapper::value);
        CHECK( helpers::is_sorted(std::begin(collection), std::end(collection),
                                  std::less<>{}, &wrapper::value) );
    }

    SECTION( "pdq_sorter" )
    {
        using sorter = cppsort::schwartz_adapter<cppsort::pdq_sorter>;
        cppsort::sort(collection, sorter{}, &wrapper::value);
        CHECK( helpers::is_sorted(std::begin(collection), std::end(collection),
                                  std::less<>{}, &wrapper::value) );
    }

    SECTION( "poplar_sorter" )
    {
        using sorter = cppsort::schwartz_adapter<cppsort::poplar_sorter>;
        cppsort::sort(collection, sorter{}, &wrapper::value);
        CHECK( helpers::is_sorted(std::begin(collection), std::end(collection),
                                  std::less<>{}, &wrapper::value) );
    }

    SECTION( "quick_sorter" )
    {
        using sorter = cppsort::schwartz_adapter<cppsort::quick_sorter>;
        cppsort::sort(collection, sorter{}, &wrapper::value);
        CHECK( helpers::is_sorted(std::begin(collection), std::end(collection),
                                  std::less<>{}, &wrapper::value) );
    }

    SECTION( "selection_sorter" )
    {
        using sorter = cppsort::schwartz_adapter<cppsort::selection_sorter>;
        cppsort::sort(collection, sorter{}, &wrapper::value);
        CHECK( helpers::is_sorted(std::begin(collection), std::end(collection),
                                  std::less<>{}, &wrapper::value) );
    }

    SECTION( "smooth_sorter" )
    {
        using sorter = cppsort::schwartz_adapter<cppsort::smooth_sorter>;
        cppsort::sort(collection, sorter{}, &wrapper::value);
        CHECK( helpers::is_sorted(std::begin(collection), std::end(collection),
                                  std::less<>{}, &wrapper::value) );
    }

    SECTION( "tim_sorter" )
    {
        using sorter = cppsort::schwartz_adapter<cppsort::tim_sorter>;
        cppsort::sort(collection, sorter{}, &wrapper::value);
        CHECK( helpers::is_sorted(std::begin(collection), std::end(collection),
                                  std::less<>{}, &wrapper::value) );
    }

    SECTION( "verge_sorter" )
    {
        using sorter = cppsort::schwartz_adapter<cppsort::verge_sorter>;
        cppsort::sort(collection, sorter{}, &wrapper::value);
        CHECK( helpers::is_sorted(std::begin(collection), std::end(collection),
                                  std::less<>{}, &wrapper::value) );
    }
}
