// -*- C++ -*-
//===-------------------------- algorithm ---------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is dual licensed under the MIT and the University of Illinois Open
// Source Licenses. See LICENSE.TXT for details.
//
// //  Modified in 2016 by Morwenn for inclusion into cpp-sort
//
//===----------------------------------------------------------------------===//
#ifndef CPPSORT_DETAIL_PARTITION_H_
#define CPPSORT_DETAIL_PARTITION_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <iterator>
#include <type_traits>
#include <utility>
#include <cpp-sort/utility/iter_move.h>
#include "iterator_traits.h"

namespace cppsort
{
namespace detail
{
    template<typename Predicate, typename ForwardIterator>
    auto partition_impl(ForwardIterator first, ForwardIterator last, Predicate pred,
                        std::forward_iterator_tag)
        -> ForwardIterator
    {
        while (true)
        {
            if (first == last)
                return first;
            if (!pred(*first))
                break;
            ++first;
        }
        for (ForwardIterator p = first; ++p != last;)
        {
            if (pred(*p))
            {
                using utility::iter_swap;
                iter_swap(first, p);
                ++first;
            }
        }
        return first;
    }

    template<typename Predicate, typename BidirectionalIterator>
    auto partition_impl(BidirectionalIterator first, BidirectionalIterator last, Predicate pred,
                        std::bidirectional_iterator_tag)
        -> BidirectionalIterator
    {
        while (true)
        {
            while (true)
            {
                if (first == last)
                    return first;
                if (!pred(*first))
                    break;
                ++first;
            }
            do
            {
                if (first == --last)
                    return first;
            } while (!pred(*last));
            using utility::iter_swap;
            iter_swap(first, last);
            ++first;
        }
    }

    template<typename ForwardIterator, typename Predicate>
    auto partition(ForwardIterator first, ForwardIterator last, Predicate pred)
        -> ForwardIterator
    {
        return partition_impl<std::add_lvalue_reference_t<Predicate>>(
            std::move(first), std::move(last), pred,
            iterator_category_t<ForwardIterator>{}
        );
    }
}}

#endif // CPPSORT_DETAIL_PARTITION_H_
