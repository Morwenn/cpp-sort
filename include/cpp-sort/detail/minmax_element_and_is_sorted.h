/*
 * Copyright (c) 2016-2022 Morwenn
 * SPDX-License-Identifier: MIT
 */
#ifndef CPPSORT_DETAIL_MINMAX_ELEMENT_AND_IS_SORTED_H_
#define CPPSORT_DETAIL_MINMAX_ELEMENT_AND_IS_SORTED_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <functional>
#include <iterator>
#include <cpp-sort/utility/as_function.h>
#include "minmax_element.h"

namespace cppsort::detail
{
    template<
        typename ForwardIterator,
        typename Sentinel,
        typename Compare = std::less<>,
        typename Projection = std::identity
    >
    constexpr auto minmax_element_and_is_sorted(ForwardIterator first, Sentinel last,
                                                Compare compare={}, Projection projection={})
        -> decltype(auto)
    {
        auto&& comp = utility::as_function(compare);
        auto&& proj = utility::as_function(projection);

        // Function-local result type, only the names of the
        // data members matter
        struct result_type
        {
            ForwardIterator min;
            ForwardIterator max;
            ForwardIterator last;
            bool is_sorted;
        } result = { first, first, first, true };

        // 0 or 1 elements
        if (first == last) {
            return result;
        }
        auto next = std::next(first);
        if (next == last) {
            result.last = next;
            return result;
        }

        // While it is sorted, the min and max are obvious
        auto current = first;
        while (not comp(proj(*next), proj(*current))) {
            ++current;
            ++next;

            // The range is fully sorted
            if (next == last) {
                result.max = current;
                result.last = next;
                return result;
            }
        }

        // The range is not sorted, use a regular minmax_element algorithm
        result.is_sorted = false;
        result.min = first;
        result.max = current;

        auto tmp = minmax_element(next, last, compare, projection);
        if (comp(proj(*tmp.min), proj(*result.min))) {
            result.min = tmp.min;
        }
        if (not comp(proj(*tmp.max), proj(*result.max))) {
            result.max = tmp.max;
        }
        result.last = tmp.last;
        return result;
    }
}

#endif // CPPSORT_DETAIL_MINMAX_ELEMENT_AND_IS_SORTED_H_
