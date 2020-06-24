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
#ifndef CPPSORT_DETAIL_COUNTING_SORT_H_
#define CPPSORT_DETAIL_COUNTING_SORT_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <algorithm>
#include <functional>
#include <vector>
#include "iterator_traits.h"
#include "minmax_element_and_is_sorted.h"

namespace cppsort
{
namespace detail
{
    template<typename ForwardIterator>
    auto counting_sort(ForwardIterator first, ForwardIterator last)
        -> void
    {
        auto info = minmax_element_and_is_sorted(first, last);
        if (info.is_sorted) return;

        using difference_type = difference_type_t<ForwardIterator>;
        auto min = *info.min;
        auto max = *info.max;
        std::vector<difference_type> counts(max - min + 1, 0);

        for (auto it = first ; it != last ; ++it)
        {
            ++counts[*it - min];
        }

        for (auto count: counts)
        {
            first = std::fill_n(first, count, min++);
        }
    }

    template<typename ForwardIterator>
    auto reverse_counting_sort(ForwardIterator first, ForwardIterator last)
        -> void
    {
        auto info = minmax_element_and_is_sorted(first, last, std::greater<>{});
        if (info.is_sorted) return;

        using difference_type = difference_type_t<ForwardIterator>;
        auto min = *info.max;
        auto max = *info.min;
        std::vector<difference_type> counts(max - min + 1, 0);

        for (auto it = first ; it != last ; ++it)
        {
            ++counts[*it - min];
        }

        for (auto rit = counts.rbegin() ; rit != counts.rend() ; ++rit)
        {
            auto count = *rit;
            first = std::fill_n(first, count, max--);
        }
    }
}}

#endif // CPPSORT_DETAIL_COUNTING_SORT_H_
