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
    template<typename ForwardIterator, typename Sentinel, typename Compare, typename Projection>
    constexpr auto unchecked_minmax_element(ForwardIterator first, Sentinel last,
                                            Compare compare, Projection projection)
        -> std::pair<ForwardIterator, ForwardIterator>
    {
        // Same as minmax_element, except that it assumes that the collection
        // contains at least two elements
        CPPSORT_ASSUME(first != last);
        CPPSORT_ASSUME(std::next(first) != last);

        auto&& comp = utility::as_function(compare);
        auto&& proj = utility::as_function(projection);
        std::pair<ForwardIterator, ForwardIterator> result{first, first};

        if (comp(proj(*first), proj(*result.first))) {
            result.first = first;
        } else {
            result.second = first;
        }

        while (++first != last) {
            auto tmp = first;
            if (++first == last) {
                if (comp(proj(*tmp), proj(*result.first))) {
                    result.first = tmp;
                } else if(not comp(proj(*tmp), proj(*result.second))) {
                    result.second = tmp;
                }
                break;
            } else {
                if (comp(proj(*first), proj(*tmp))) {
                    if (comp(proj(*first), proj(*result.first))) {
                        result.first = first;
                    } if (not comp(proj(*tmp), proj(*result.second))) {
                        result.second = tmp;
                    }
                } else {
                    if (comp(proj(*tmp), proj(*result.first))) {
                        result.first = tmp;
                    } if(not comp(proj(*first), proj(*result.second))) {
                        result.second = first;
                    }
                }
            }
        }
        return result;
    }

    template<typename ForwardIterator, typename Sentinel, typename Compare, typename Projection>
    constexpr auto minmax_element(ForwardIterator first, Sentinel last,
                                  Compare compare, Projection projection)
        -> std::pair<ForwardIterator, ForwardIterator>
    {
        std::pair<ForwardIterator, ForwardIterator> result{first, first};
        if (first == last || std::next(first) == last) {
            return result;
        }
        return unchecked_minmax_element(std::move(first), std::move(last),
                                        std::move(compare), std::move(projection));
    }
}

#endif // CPPSORT_DETAIL_MINMAX_ELEMENT_H_
