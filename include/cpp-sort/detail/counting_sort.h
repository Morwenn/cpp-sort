/*
 * Copyright (c) 2016-2022 Morwenn
 * SPDX-License-Identifier: MIT
 */
#ifndef CPPSORT_DETAIL_COUNTING_SORT_H_
#define CPPSORT_DETAIL_COUNTING_SORT_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <algorithm>
#include <functional>
#include <iterator>
#include "immovable_vector.h"
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
        using difference_type = difference_type_t<ForwardIterator>;

        auto info = minmax_element_and_is_sorted(first, last);
        if (info.is_sorted) return;

        auto min = *info.min;
        auto max = *info.max;
        difference_type value_range = max - min + 1;

        immovable_vector<difference_type> counts(value_range);
        for (difference_type n = 0; n < value_range; ++n) {
            counts.emplace_back(0);
        }

        for (auto it = first; it != last; ++it) {
            ++counts[*it - min];
        }

        for (auto count: counts) {
            first = std::fill_n(first, count, min++);
        }
    }

    template<typename ForwardIterator>
    auto reverse_counting_sort(ForwardIterator first, ForwardIterator last)
        -> void
    {
        using difference_type = difference_type_t<ForwardIterator>;

        auto info = minmax_element_and_is_sorted(first, last, std::greater<>{});
        if (info.is_sorted) return;

        auto min = *info.max;
        auto max = *info.min;
        difference_type value_range = max - min + 1;

        immovable_vector<difference_type> counts(value_range);
        for (difference_type n = 0; n < value_range; ++n) {
            counts.emplace_back(0);
        }

        for (auto it = first; it != last; ++it) {
            ++counts[*it - min];
        }

        for (auto rit = counts.end(); rit != counts.begin(); --rit) {
            auto count = *std::prev(rit);
            first = std::fill_n(first, count, max--);
        }
    }
}}

#endif // CPPSORT_DETAIL_COUNTING_SORT_H_
