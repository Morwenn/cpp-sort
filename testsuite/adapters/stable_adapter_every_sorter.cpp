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
#include <cstddef>
#include <forward_list>
#include <list>
#include <random>
#include <vector>
#include <catch2/catch.hpp>
#include <cpp-sort/adapters/stable_adapter.h>
#include <cpp-sort/sorters.h>
#include <cpp-sort/utility/buffer.h>
#include <testing-tools/algorithm.h>

namespace
{
    struct wrapper
    {
        int value;
        int order;
    };

    auto operator<(const wrapper& lhs, const wrapper& rhs)
        -> bool
    {
        if (lhs.value < rhs.value) {
            return true;
        }
        if (rhs.value < lhs.value) {
            return false;
        }
        return lhs.order < rhs.order;
    }
}

TEMPLATE_TEST_CASE( "every random-access sorter with stable_adapter", "[stable_adapter]",
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
                    cppsort::std_sorter,
                    cppsort::tim_sorter,
                    cppsort::verge_sorter )
{
    std::vector<wrapper> collection(412);
    std::size_t count = 0;
    for (wrapper& wrap: collection) {
        wrap.value = count++ % 17;
    }
    std::mt19937 engine(Catch::rngSeed());
    std::shuffle(collection.begin(), collection.end(), engine);
    helpers::iota(collection.begin(), collection.end(), 0, &wrapper::order);

    cppsort::stable_adapter<TestType> sorter;
    sorter(collection, &wrapper::value);
    CHECK( std::is_sorted(collection.begin(), collection.end()) );
}

TEMPLATE_TEST_CASE( "every bidirectional sorter with stable_adapter", "[stable_adapter]",
                    cppsort::drop_merge_sorter,
                    cppsort::insertion_sorter,
                    cppsort::merge_sorter,
                    cppsort::quick_merge_sorter,
                    cppsort::quick_sorter,
                    cppsort::selection_sorter,
                    cppsort::verge_sorter )
{
    std::vector<wrapper> collection(412);
    std::size_t count = 0;
    for (wrapper& wrap: collection) {
        wrap.value = count++ % 17;
    }
    std::mt19937 engine(Catch::rngSeed());
    std::shuffle(collection.begin(), collection.end(), engine);
    helpers::iota(collection.begin(), collection.end(), 0, &wrapper::order);

    std::list<wrapper> li(collection.begin(), collection.end());
    cppsort::stable_adapter<TestType> sorter;
    sorter(li, &wrapper::value);
    CHECK( std::is_sorted(li.begin(), li.end()) );
}

TEMPLATE_TEST_CASE( "every forward sorter with with stable_adapter", "[stable_adapter]",
                    cppsort::merge_sorter,
                    cppsort::quick_merge_sorter,
                    cppsort::quick_sorter,
                    cppsort::selection_sorter )
{
    std::vector<wrapper> collection(412);
    std::size_t count = 0;
    for (wrapper& wrap: collection) {
        wrap.value = count++ % 17;
    }
    std::mt19937 engine(Catch::rngSeed());
    std::shuffle(collection.begin(), collection.end(), engine);
    helpers::iota(collection.begin(), collection.end(), 0, &wrapper::order);

    std::forward_list<wrapper> fli(collection.begin(), collection.end());
    cppsort::stable_adapter<TestType> sorter;
    sorter(fli, &wrapper::value);
    CHECK( std::is_sorted(fli.begin(), fli.end()) );
}
