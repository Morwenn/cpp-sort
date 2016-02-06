/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2015-2016 Morwenn
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
#ifndef CPPSORT_SORTERS_SPREAD_SORTER_FLOAT_SPREAD_SORTER_H_
#define CPPSORT_SORTERS_SPREAD_SORTER_FLOAT_SPREAD_SORTER_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <cstdint>
#include <iterator>
#include <limits>
#include <type_traits>
#include <cpp-sort/sorter_facade.h>
#include "../../detail/iterator_traits.h"
#include "../../detail/spreadsort/float_sort.h"

namespace cppsort
{
    ////////////////////////////////////////////////////////////
    // Sorter

    namespace detail
    {
        struct float_spread_sorter_impl
        {
            template<typename RandomAccessIterator>
            auto operator()(RandomAccessIterator first, RandomAccessIterator last) const
                -> std::enable_if_t<
                    std::numeric_limits<value_type_t<RandomAccessIterator>>::is_iec559 && (
                        sizeof(value_type_t<RandomAccessIterator>) == sizeof(std::uint32_t) ||
                        sizeof(value_type_t<RandomAccessIterator>) == sizeof(std::uint64_t)
                    )

                >
            {
                spreadsort::float_sort(first, last);
            }

            ////////////////////////////////////////////////////////////
            // Sorter traits

            using iterator_category = std::random_access_iterator_tag;
            using is_stable = std::false_type;
        };
    }

    struct float_spread_sorter:
        sorter_facade<detail::float_spread_sorter_impl>
    {};
}

#endif // CPPSORT_SORTERS_SPREAD_SORTER_FLOAT_SPREAD_SORTER_H_
