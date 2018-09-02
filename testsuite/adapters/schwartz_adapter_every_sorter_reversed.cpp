/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2017-2018 Morwenn
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
#include <deque>
#include <iterator>
#include <random>
#include <string>
#include <catch2/catch.hpp>
#include <cpp-sort/adapters/schwartz_adapter.h>
#include <cpp-sort/sort.h>
#include <cpp-sort/sorters.h>
#include <cpp-sort/utility/buffer.h>
#include "../algorithm.h"

namespace
{
    template<typename T=double>
    struct wrapper
    {
        T value;
    };
}

TEST_CASE( "every sorter with Schwartzian transform adapter and reverse iterators",
           "[schwartz_adapter][reverse_iterator]" )
{
    std::deque<wrapper<>> collection(412);
    helpers::iota(std::begin(collection), std::end(collection), -125, &wrapper<>::value);
    std::mt19937 engine(Catch::rngSeed());
    std::shuffle(std::begin(collection), std::end(collection), engine);

    SECTION( "block_sorter" )
    {
        using namespace cppsort;

        using sorter = schwartz_adapter<block_sorter<
            utility::fixed_buffer<0>
        >>;
        cppsort::sort(sorter{}, std::rbegin(collection), std::rend(collection), &wrapper<>::value);
        CHECK( helpers::is_sorted(std::begin(collection), std::end(collection),
                                  std::greater<>{}, &wrapper<>::value) );
    }

    SECTION( "default_sorter" )
    {
        using sorter = cppsort::schwartz_adapter<cppsort::default_sorter>;
        cppsort::sort(sorter{}, std::rbegin(collection), std::rend(collection), &wrapper<>::value);
        CHECK( helpers::is_sorted(std::begin(collection), std::end(collection),
                                  std::greater<>{}, &wrapper<>::value) );
    }

    SECTION( "drop_merge_sorter" )
    {
        using sorter = cppsort::schwartz_adapter<cppsort::drop_merge_sorter>;
        cppsort::sort(sorter{}, std::rbegin(collection), std::rend(collection), &wrapper<>::value);
        CHECK( helpers::is_sorted(std::begin(collection), std::end(collection),
                                  std::greater<>{}, &wrapper<>::value) );
    }

    SECTION( "grail_sorter" )
    {
        using sorter = cppsort::schwartz_adapter<cppsort::grail_sorter<>>;
        cppsort::sort(sorter{}, std::rbegin(collection), std::rend(collection), &wrapper<>::value);
        CHECK( helpers::is_sorted(std::begin(collection), std::end(collection),
                                  std::greater<>{}, &wrapper<>::value) );
    }

    SECTION( "heap_sorter" )
    {
        using sorter = cppsort::schwartz_adapter<cppsort::heap_sorter>;
        cppsort::sort(sorter{}, std::rbegin(collection), std::rend(collection), &wrapper<>::value);
        CHECK( helpers::is_sorted(std::begin(collection), std::end(collection),
                                  std::greater<>{}, &wrapper<>::value) );
    }

    SECTION( "insertion_sorter" )
    {
        using sorter = cppsort::schwartz_adapter<cppsort::insertion_sorter>;
        cppsort::sort(sorter{}, std::rbegin(collection), std::rend(collection), &wrapper<>::value);
        CHECK( helpers::is_sorted(std::begin(collection), std::end(collection),
                                  std::greater<>{}, &wrapper<>::value) );
    }

    SECTION( "merge_insertion_sorter" )
    {
        using sorter = cppsort::schwartz_adapter<cppsort::merge_insertion_sorter>;
        cppsort::sort(sorter{}, std::rbegin(collection), std::rend(collection), &wrapper<>::value);
        CHECK( helpers::is_sorted(std::begin(collection), std::end(collection),
                                  std::greater<>{}, &wrapper<>::value) );
    }

    SECTION( "merge_sorter" )
    {
        using sorter = cppsort::schwartz_adapter<cppsort::merge_sorter>;
        cppsort::sort(sorter{}, std::rbegin(collection), std::rend(collection), &wrapper<>::value);
        CHECK( helpers::is_sorted(std::begin(collection), std::end(collection),
                                  std::greater<>{}, &wrapper<>::value) );
    }

    SECTION( "pdq_sorter" )
    {
        using sorter = cppsort::schwartz_adapter<cppsort::pdq_sorter>;
        cppsort::sort(sorter{}, std::rbegin(collection), std::rend(collection), &wrapper<>::value);
        CHECK( helpers::is_sorted(std::begin(collection), std::end(collection),
                                  std::greater<>{}, &wrapper<>::value) );
    }

    SECTION( "poplar_sorter" )
    {
        using sorter = cppsort::schwartz_adapter<cppsort::poplar_sorter>;
        cppsort::sort(sorter{}, std::rbegin(collection), std::rend(collection), &wrapper<>::value);
        CHECK( helpers::is_sorted(std::begin(collection), std::end(collection),
                                  std::greater<>{}, &wrapper<>::value) );
    }

    SECTION( "quick_merge_sorter" )
    {
        using sorter = cppsort::schwartz_adapter<cppsort::quick_merge_sorter>;
        cppsort::sort(sorter{}, std::rbegin(collection), std::rend(collection), &wrapper<>::value);
        CHECK( helpers::is_sorted(std::begin(collection), std::end(collection),
                                  std::greater<>{}, &wrapper<>::value) );
    }

    SECTION( "quick_sorter" )
    {
        using sorter = cppsort::schwartz_adapter<cppsort::quick_sorter>;
        cppsort::sort(sorter{}, std::rbegin(collection), std::rend(collection), &wrapper<>::value);
        CHECK( helpers::is_sorted(std::begin(collection), std::end(collection),
                                  std::greater<>{}, &wrapper<>::value) );
    }

    SECTION( "selection_sorter" )
    {
        using sorter = cppsort::schwartz_adapter<cppsort::selection_sorter>;
        cppsort::sort(sorter{}, std::rbegin(collection), std::rend(collection), &wrapper<>::value);
        CHECK( helpers::is_sorted(std::begin(collection), std::end(collection),
                                  std::greater<>{}, &wrapper<>::value) );
    }

    SECTION( "ska_sorter" )
    {
        using sorter = cppsort::schwartz_adapter<cppsort::ska_sorter>;

        std::deque<wrapper<int>> collection2(412);
        helpers::iota(std::begin(collection2), std::end(collection2), -125, &wrapper<int>::value);
        std::deque<wrapper<std::string>> collection3;
        for (int i = -125 ; i < 287 ; ++i) { collection3.push_back({std::to_string(i)}); }

        cppsort::sort(sorter{}, std::rbegin(collection), std::rend(collection), &wrapper<>::value);
        CHECK( helpers::is_sorted(std::begin(collection), std::end(collection),
                                  std::greater<>{}, &wrapper<>::value) );

        std::shuffle(std::begin(collection2), std::end(collection2), engine);
        cppsort::sort(sorter{}, std::rbegin(collection2), std::rend(collection2), &wrapper<int>::value);
        CHECK( helpers::is_sorted(std::begin(collection2), std::end(collection2),
                                  std::greater<>{}, &wrapper<int>::value) );

        std::shuffle(std::begin(collection3), std::end(collection3), engine);
        cppsort::sort(sorter{}, std::rbegin(collection3), std::rend(collection3), &wrapper<std::string>::value);
        CHECK( helpers::is_sorted(std::begin(collection3), std::end(collection3),
                                  std::greater<>{}, &wrapper<std::string>::value) );
    }

    SECTION( "smooth_sorter" )
    {
        using sorter = cppsort::schwartz_adapter<cppsort::smooth_sorter>;
        cppsort::sort(sorter{}, std::rbegin(collection), std::rend(collection), &wrapper<>::value);
        CHECK( helpers::is_sorted(std::begin(collection), std::end(collection),
                                  std::greater<>{}, &wrapper<>::value) );
    }

    SECTION( "spread_sorter" )
    {
        using sorter = cppsort::schwartz_adapter<cppsort::spread_sorter>;

        std::deque<wrapper<int>> collection2(412);
        helpers::iota(std::begin(collection2), std::end(collection2), -125, &wrapper<int>::value);
        std::deque<wrapper<std::string>> collection3;
        for (int i = -125 ; i < 287 ; ++i) { collection3.push_back({std::to_string(i)}); }

        cppsort::sort(sorter{}, std::rbegin(collection), std::rend(collection), &wrapper<>::value);
        CHECK( helpers::is_sorted(std::begin(collection), std::end(collection),
                                  std::greater<>{}, &wrapper<>::value) );

        std::shuffle(std::begin(collection2), std::end(collection2), engine);
        cppsort::sort(sorter{}, std::rbegin(collection2), std::rend(collection2), &wrapper<int>::value);
        CHECK( helpers::is_sorted(std::begin(collection2), std::end(collection2),
                                  std::greater<>{}, &wrapper<int>::value) );

        std::shuffle(std::begin(collection3), std::end(collection3), engine);
        cppsort::sort(sorter{}, std::rbegin(collection3), std::rend(collection3), &wrapper<std::string>::value);
        CHECK( helpers::is_sorted(std::begin(collection3), std::end(collection3),
                                  std::greater<>{}, &wrapper<std::string>::value) );

        std::shuffle(std::begin(collection3), std::end(collection3), engine);
        cppsort::sort(sorter{}, std::rbegin(collection3), std::rend(collection3),
                      std::greater<>{}, &wrapper<std::string>::value);
        CHECK( helpers::is_sorted(std::begin(collection3), std::end(collection3),
                                  std::less<>{}, &wrapper<std::string>::value) );
    }

    SECTION( "tim_sorter" )
    {
        using sorter = cppsort::schwartz_adapter<cppsort::tim_sorter>;
        cppsort::sort(sorter{}, std::rbegin(collection), std::rend(collection), &wrapper<>::value);
        CHECK( helpers::is_sorted(std::begin(collection), std::end(collection),
                                  std::greater<>{}, &wrapper<>::value) );
    }

    SECTION( "verge_sorter" )
    {
        using sorter = cppsort::schwartz_adapter<cppsort::verge_sorter>;
        cppsort::sort(sorter{}, std::rbegin(collection), std::rend(collection), &wrapper<>::value);
        CHECK( helpers::is_sorted(std::begin(collection), std::end(collection),
                                  std::greater<>{}, &wrapper<>::value) );
    }
}
