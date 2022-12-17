/*
 * Copyright (c) 2015-2022 Morwenn
 * SPDX-License-Identifier: MIT
 */

// Range v3 library
//
//  Copyright Eric Niebler 2014
//
//  Use, modification and distribution is subject to the
//  Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
// Project home: https://github.com/ericniebler/range-v3
//
// Implementation based on the code in libc++
//   http://http://libcxx.llvm.org/

#ifndef CPPSORT_DETAIL_MINMAX_ELEMENT_H_
#define CPPSORT_DETAIL_MINMAX_ELEMENT_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <iterator>
#include <utility>
#include <cpp-sort/utility/as_function.h>
#include "config.h"

namespace cppsort::detail
{
    template<typename Iterator>
    struct minmax_result_type
    {
        Iterator min, max, last;
    };

    template<typename ForwardIterator, typename Sentinel, typename Compare, typename Projection>
    constexpr auto unchecked_minmax_element(ForwardIterator first, Sentinel last,
                                            Compare compare, Projection projection)
        -> minmax_result_type<ForwardIterator>
    {
        // Same as minmax_element, except that it assumes that the collection
        // contains at least two elements
        CPPSORT_ASSUME(first != last);
        CPPSORT_ASSUME(std::next(first) != last);

        auto&& comp = utility::as_function(compare);
        auto&& proj = utility::as_function(projection);
        minmax_result_type<ForwardIterator> result = {first, first, {}};

        while (++first != last) {
            auto tmp = first;
            if (++first == last) {
                if (comp(proj(*tmp), proj(*result.min))) {
                    result.min = tmp;
                } else if(not comp(proj(*tmp), proj(*result.max))) {
                    result.max = tmp;
                }
                break;
            } else {
                if (comp(proj(*first), proj(*tmp))) {
                    if (comp(proj(*first), proj(*result.min))) {
                        result.min = first;
                    } if (not comp(proj(*tmp), proj(*result.max))) {
                        result.max = tmp;
                    }
                } else {
                    if (comp(proj(*tmp), proj(*result.min))) {
                        result.min = tmp;
                    } if(not comp(proj(*first), proj(*result.max))) {
                        result.max = first;
                    }
                }
            }
        }

        result.last = first;
        return result;
    }

    template<typename ForwardIterator, typename Sentinel, typename Compare, typename Projection>
    constexpr auto minmax_element(ForwardIterator first, Sentinel last,
                                  Compare compare, Projection projection)
        -> minmax_result_type<ForwardIterator>
    {
        minmax_result_type<ForwardIterator> result{first, first, first};
        if (first == last) {
            return result;
        }
        auto next = std::next(first);
        if (next == last) {
            result.last = next;
            return result;
        }
        return unchecked_minmax_element(std::move(first), std::move(last),
                                        std::move(compare), std::move(projection));
    }
}

#endif // CPPSORT_DETAIL_MINMAX_ELEMENT_H_
