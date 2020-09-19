/*
 * Copyright (c) 2015-2017 Morwenn
 * SPDX-License-Identifier: MIT
 */
#ifndef CPPSORT_DETAIL_IS_SORTED_UNTIL_H_
#define CPPSORT_DETAIL_IS_SORTED_UNTIL_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <cpp-sort/utility/as_function.h>

namespace cppsort
{
namespace detail
{
    template<typename ForwardIterator, typename Compare, typename Projection>
    auto is_sorted_until(ForwardIterator first, ForwardIterator last,
                         Compare compare, Projection projection)
        -> ForwardIterator
    {
        if (first != last)
        {
            auto&& comp = utility::as_function(compare);
            auto&& proj = utility::as_function(projection);

            ForwardIterator next = first;
            while (++next != last)
            {
                if (comp(proj(*next), proj(*first)))
                {
                    return next;
                }
                first = next;
            }
        }
        return last;
    }
}}

#endif // CPPSORT_DETAIL_IS_SORTED_UNTIL_H_
