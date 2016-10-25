// -*- C++ -*-
//===-------------------------- algorithm ---------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is dual licensed under the MIT and the University of Illinois Open
// Source Licenses. See LICENSE.TXT for details.
//
// Modified in 2015-2016 by Morwenn for inclusion into cpp-sort
//
//===----------------------------------------------------------------------===//
#ifndef CPPSORT_DETAIL_MERGE_MOVE_H_
#define CPPSORT_DETAIL_MERGE_MOVE_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <utility>
#include <cpp-sort/utility/as_function.h>
#include <cpp-sort/utility/iter_move.h>
#include "move.h"

namespace cppsort
{
namespace detail
{
    template<typename InputIterator1, typename InputIterator2, typename OutputIterator,
             typename Compare, typename Projection1, typename Projection2>
    auto merge_move(InputIterator1 first1, InputIterator1 last1,
                    InputIterator2 first2, InputIterator2 last2,
                    OutputIterator result, Compare comp={},
                    Projection1 projection1={}, Projection2 projection2={})
        -> OutputIterator
    {
        using utility::iter_move;
        auto&& proj1 = utility::as_function(projection1);
        auto&& proj2 = utility::as_function(projection2);

        for (; first1 != last1; ++result)
        {
            if (first2 == last2)
                return detail::move(first1, last1, result);
            if (comp(proj2(*first2), proj1(*first1)))
            {
                *result = iter_move(first2);
                ++first2;
            }
            else
            {
                *result = iter_move(first1);
                ++first1;
            }
        }
        return detail::move(std::move(first2), std::move(last2), std::move(result));
    }
}}

#endif // CPPSORT_DETAIL_MERGE_MOVE_H_
