/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2016-2020 Morwenn
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
#include <functional>
#include <list>
#include <random>
#include <string>
#include <vector>
#include <catch2/catch.hpp>
#include <cpp-sort/adapters/schwartz_adapter.h>
#include <cpp-sort/sorters.h>
#include <cpp-sort/utility/buffer.h>
#include <testing-tools/algorithm.h>

namespace
{
    // double segfaults with MinGW-w64 64 bits in Release mode
    template<typename T=float>
    struct wrapper
    {
        T value;
    };
}

TEMPLATE_TEST_CASE( "every random-access sorter with Schwartzian transform adapter", "[schwartz_adapter]",
                    cppsort::block_sorter<cppsort::utility::fixed_buffer<0>>,
                    cppsort::default_sorter,
                    cppsort::drop_merge_sorter,
                    cppsort::grail_sorter<>,
                    cppsort::heap_sorter,
                    cppsort::insertion_sorter,
                    cppsort::merge_insertion_sorter,
                    cppsort::merge_sorter,
                    cppsort::pdq_sorter,
                    cppsort::poplar_sorter,
                    cppsort::quick_merge_sorter,
                    cppsort::quick_sorter,
                    cppsort::selection_sorter,
                    cppsort::smooth_sorter,
                    cppsort::spin_sorter,
                    cppsort::split_sorter,
                    cppsort::tim_sorter,
                    cppsort::verge_sorter )
{
    std::vector<wrapper<>> collection(412);
    helpers::iota(collection.begin(), collection.end(), -125, &wrapper<>::value);
    std::mt19937 engine(Catch::rngSeed());
    std::shuffle(collection.begin(), collection.end(), engine);

    cppsort::schwartz_adapter<TestType> sorter;
    sorter(collection, &wrapper<>::value);
    CHECK( helpers::is_sorted(collection.begin(), collection.end(),
                              std::less<>{}, &wrapper<>::value) );
}

TEMPLATE_TEST_CASE( "every bidirectional sorter with Schwartzian transform adapter", "[schwartz_adapter]",
                    cppsort::drop_merge_sorter,
                    cppsort::insertion_sorter,
                    cppsort::merge_sorter,
                    cppsort::quick_merge_sorter,
                    cppsort::quick_sorter,
                    cppsort::selection_sorter,
                    cppsort::verge_sorter )
{
    std::vector<wrapper<>> collection(412);
    helpers::iota(collection.begin(), collection.end(), -125, &wrapper<>::value);
    std::mt19937 engine(Catch::rngSeed());
    std::shuffle(collection.begin(), collection.end(), engine);

    std::list<wrapper<>> li(collection.begin(), collection.end());
    cppsort::schwartz_adapter<TestType> sorter;
    sorter(li, &wrapper<>::value);
    CHECK( helpers::is_sorted(li.begin(), li.end(), std::less<>{}, &wrapper<>::value) );
}

TEMPLATE_TEST_CASE( "every forward sorter with Schwartzian transform adapter", "[schwartz_adapter]",
                    cppsort::merge_sorter,
                    cppsort::quick_merge_sorter,
                    cppsort::quick_sorter,
                    cppsort::selection_sorter )
{
    std::vector<wrapper<>> collection(412);
    helpers::iota(collection.begin(), collection.end(), -125, &wrapper<>::value);
    std::mt19937 engine(Catch::rngSeed());
    std::shuffle(collection.begin(), collection.end(), engine);

    std::forward_list<wrapper<>> fli(collection.begin(), collection.end());
    cppsort::schwartz_adapter<TestType> sorter;
    sorter(fli, &wrapper<>::value);
    CHECK( helpers::is_sorted(fli.begin(), fli.end(), std::less<>{}, &wrapper<>::value) );
}

TEST_CASE( "type-specific sorters with Schwartzian transform adapter", "[schwartz_adapter]" )
{
    std::vector<wrapper<>> collection(412);
    helpers::iota(collection.begin(), collection.end(), -125, &wrapper<>::value);
    std::vector<wrapper<int>> collection2(412);
    helpers::iota(collection2.begin(), collection2.end(), -125, &wrapper<int>::value);
    std::vector<wrapper<std::string>> collection3;
    for (int i = -125 ; i < 287 ; ++i) {
        collection3.push_back({std::to_string(i)});
    }

    std::mt19937 engine(Catch::rngSeed());
    std::shuffle(collection.begin(), collection.end(), engine);
    std::shuffle(collection2.begin(), collection2.end(), engine);
    std::shuffle(collection3.begin(), collection3.end(), engine);

    SECTION( "ska_sorter" )
    {
        cppsort::schwartz_adapter<cppsort::ska_sorter> sorter;

        sorter(collection, &wrapper<>::value);
        CHECK( helpers::is_sorted(collection.begin(), collection.end(),
                                  std::less<>{}, &wrapper<>::value) );

        sorter(collection2, &wrapper<int>::value);
        CHECK( helpers::is_sorted(collection2.begin(), collection2.end(),
                                  std::less<>{}, &wrapper<int>::value) );

        sorter(collection3, &wrapper<std::string>::value);
        CHECK( helpers::is_sorted(collection3.begin(), collection3.end(),
                                  std::less<>{}, &wrapper<std::string>::value) );
    }

    SECTION( "spread_sorter" )
    {
        cppsort::schwartz_adapter<cppsort::spread_sorter> sorter;

        sorter(collection, &wrapper<>::value);
        CHECK( helpers::is_sorted(collection.begin(), collection.end(),
                                  std::less<>{}, &wrapper<>::value) );

        sorter(collection2, &wrapper<int>::value);
        CHECK( helpers::is_sorted(collection2.begin(), collection2.end(),
                                  std::less<>{}, &wrapper<int>::value) );

        sorter(collection3, &wrapper<std::string>::value);
        CHECK( helpers::is_sorted(collection3.begin(), collection3.end(),
                                  std::less<>{}, &wrapper<std::string>::value) );

        std::shuffle(collection3.begin(), collection3.end(), engine);
        sorter(collection3, std::greater<>{}, &wrapper<std::string>::value);
        CHECK( helpers::is_sorted(collection3.begin(), collection3.end(),
                                  std::greater<>{}, &wrapper<std::string>::value) );
    }
}
