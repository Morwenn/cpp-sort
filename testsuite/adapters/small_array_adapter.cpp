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
#include <array>
#include <cstddef>
#include <functional>
#include <iterator>
#include <utility>
#include <catch.hpp>
#include <cpp-sort/adapters/hybrid_adapter.h>
#include <cpp-sort/adapters/small_array_adapter.h>
#include <cpp-sort/sort.h>
#include <cpp-sort/sorter_facade.h>

template<std::size_t N>
struct fixed_sorter:
    cppsort::sorter_facade<fixed_sorter<N>>
{
    using cppsort::sorter_facade<fixed_sorter<N>>::operator();

    template<
        typename RandomAccessIterator,
        typename Compare = std::less<>
    >
    auto operator()(RandomAccessIterator first, RandomAccessIterator last, Compare={}) const
        -> bool
    {
        return std::distance(first, last) == N;
    }
};

struct regular_sorter:
    cppsort::sorter_facade<regular_sorter>
{
    using cppsort::sorter_facade<regular_sorter>::operator();

    template<
        typename RandomAccessIterator,
        typename Compare = std::less<>
    >
    auto operator()(RandomAccessIterator, RandomAccessIterator, Compare={}) const
        -> bool
    {
        return false;
    }
};

namespace cppsort
{
    template<std::size_t N>
    struct sorter_traits<fixed_sorter<N>>
    {
        using iterator_category = std::random_access_iterator_tag;
    };

    template<>
    struct sorter_traits<regular_sorter>
    {
        using iterator_category = std::random_access_iterator_tag;
    };
}

TEST_CASE( "small array adapter",
           "[small_array_adapter][hybrid_adapter]" )
{
    std::array<int, 6u> array;

    SECTION( "without indices" )
    {
        using sorter = cppsort::small_array_adapter<fixed_sorter>;

        CHECK( cppsort::sort(array, sorter{}) );
    }

    SECTION( "with indices" )
    {
        using sorter = cppsort::small_array_adapter<
            fixed_sorter,
            std::make_index_sequence<14u>
        >;

        CHECK( cppsort::sort(array, sorter{}) );
    }

    SECTION( "with indices in hybrid_adapter" )
    {
        std::array<int, 25u> big_array;

        using sorter = cppsort::hybrid_adapter<
            cppsort::small_array_adapter<
                fixed_sorter,
                std::make_index_sequence<14u>
            >,
            regular_sorter
        >;

        CHECK( cppsort::sort(array, sorter{}) );
        CHECK( not cppsort::sort(big_array, sorter{}) );
    }
}
