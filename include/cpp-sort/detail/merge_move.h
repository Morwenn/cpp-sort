/*
 * Copyright (c) 2019 Morwenn
 * SPDX-License-Identifier: MIT
 */
#ifndef CPPSORT_DETAIL_MERGE_MOVE_H_
#define CPPSORT_DETAIL_MERGE_MOVE_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <utility>
#include <cpp-sort/utility/as_function.h>
#include <cpp-sort/utility/iter_move.h>
#include "config.h"
#include "move.h"

namespace cppsort
{
namespace detail
{
    template<typename InputIterator1, typename InputIterator2, typename OutputIterator,
             typename Compare, typename Projection1, typename Projection2>
    auto merge_move(InputIterator1 first1, InputIterator1 last1,
                    InputIterator2 first2, InputIterator2 last2,
                    OutputIterator result, Compare compare,
                    Projection1 projection1, Projection2 projection2)
        -> OutputIterator
    {
        using utility::iter_move;
        auto&& comp = utility::as_function(compare);
        auto&& proj1 = utility::as_function(projection1);
        auto&& proj2 = utility::as_function(projection2);

        if (first1 == last1) {
            return detail::move(first2, last2, result);
        }

        if (first2 == last2) {
            return detail::move(first1, last1, result);
        }

        while (true) {
            CPPSORT_ASSUME(first1 != last1);
            CPPSORT_ASSUME(first2 != last2);

            if (comp(proj2(*first2), proj1(*first1))) {
                *result = iter_move(first2);
                ++first2;
                if (first2 == last2) {
                    return detail::move(first1, last1, ++result);
                }
            } else {
                *result = iter_move(first1);
                ++first1;
                if (first1 == last1) {
                    return detail::move(first2, last2, ++result);
                }
            }
            ++result;
        }
    }
}}

#endif // CPPSORT_DETAIL_MERGE_MOVE_H_
