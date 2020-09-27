/*
 * Copyright (c) 2016-2020 Morwenn
 * SPDX-License-Identifier: MIT
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
