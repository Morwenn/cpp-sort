// -*- C++ -*-
//===-------------------------- algorithm ---------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is dual licensed under the MIT and the University of Illinois Open
// Source Licenses. See LICENSE.TXT for details.
//
// Modified in 2015 by Morwenn for inclusion into cpp-sort
//
//===----------------------------------------------------------------------===//
#ifndef CPPSORT_DETAIL_MERGE_MOVE_H_
#define CPPSORT_DETAIL_MERGE_MOVE_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <algorithm>
#include <utility>
#include <cpp-sort/utility/as_function.h>

namespace cppsort
{
namespace detail
{
    template<class InputIterator1, class InputIterator2, class OutputIterator,
             class Compare, class Projection1, class Projection2>
    auto merge_move(InputIterator1 first1, InputIterator1 last1,
                    InputIterator2 first2, InputIterator2 last2,
                    OutputIterator result, Compare comp={},
                    Projection1 projection1={}, Projection2 projection2={})
        -> OutputIterator
    {
        auto&& proj1 = utility::as_function(projection1);
        auto&& proj2 = utility::as_function(projection2);

        for (; first1 != last1; ++result)
        {
            if (first2 == last2)
                return std::move(first1, last1, result);
            if (comp(proj2(*first2), proj1(*first1)))
            {
                *result = std::move(*first2);
                ++first2;
            }
            else
            {
                *result = std::move(*first1);
                ++first1;
            }
        }
        return std::move(first2, last2, result);
    }
}}

#endif // CPPSORT_DETAIL_MERGE_MOVE_H_
