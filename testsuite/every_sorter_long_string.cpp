/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2019 Morwenn
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
#include <iterator>
#include <random>
#include <string>
#include <utility>
#include <vector>
#include <catch2/catch.hpp>
#include <cpp-sort/sort.h>
#include <cpp-sort/sorters.h>
#include <cpp-sort/utility/buffer.h>
#include <cpp-sort/utility/functional.h>
#include "distributions.h"

namespace
{
    struct shuffled_string:
        dist::distribution<shuffled_string>
    {
        template<typename OutputIterator, typename T=long long int>
        auto operator()(OutputIterator out, long long int size, T start=T(0)) const
            -> void
        {
            // Pseudo-random number generator
            thread_local std::mt19937 engine(Catch::rngSeed());

            std::vector<std::string> vec;
            vec.reserve(size);

            T end = start + size;
            for (auto i = start ; i < end ; ++i) {
                auto s = std::to_string(i);
                vec.push_back(std::string(100 - s.size(), '0') + std::move(s));
            }
            std::shuffle(std::begin(vec), std::end(vec), engine);
            std::move(std::begin(vec), std::end(vec), out);
        }
    };
}

TEMPLATE_TEST_CASE( "test every sorter with long std::string", "[sorters]",
                    cppsort::block_sorter<>,
                    cppsort::block_sorter<
                        cppsort::utility::dynamic_buffer<cppsort::utility::half>
                    >,
                    cppsort::default_sorter,
                    cppsort::drop_merge_sorter,
                    cppsort::grail_sorter<>,
                    cppsort::grail_sorter<
                        cppsort::utility::dynamic_buffer<cppsort::utility::sqrt>
                    >,
                    cppsort::heap_sorter,
                    cppsort::insertion_sorter,
                    cppsort::merge_insertion_sorter,
                    cppsort::merge_sorter,
                    cppsort::pdq_sorter,
                    cppsort::poplar_sorter,
                    cppsort::quick_merge_sorter,
                    cppsort::quick_sorter,
                    cppsort::selection_sorter,
                    cppsort::ska_sorter,
                    cppsort::smooth_sorter,
                    cppsort::split_sorter,
                    cppsort::std_sorter,
                    cppsort::tim_sorter,
                    cppsort::verge_sorter )
{
    // This test primarily exists to detect self-move issues in the library:
    // long std::string instances actually self-destruct on self-move and can
    // thus give incorrect results or even destroy the values of the collection
    //
    // Instead of checking that the result is sorted, we check that it is equal
    // to the same array is sorted exactly the same way with std::sort, which
    // is actually a stronger check and even requires a total order for the
    // comparison function

    std::vector<std::string> collection;
    collection.reserve(250);
    auto distribution = shuffled_string{};
    distribution(std::back_inserter(collection), 491, -125);

    auto copy = collection;
    std::sort(std::begin(copy), std::end(copy));

    using sorter = TestType;
    cppsort::sort(sorter{}, collection);
    CHECK( bool(collection == copy) );
}
