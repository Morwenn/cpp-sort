// -*- C++ -*-
//===-------------------------- algorithm ---------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is dual licensed under the MIT and the University of Illinois Open
// Source Licenses. See LICENSE.TXT for details.
//
// Modified in 2015-2020 by Morwenn for inclusion into cpp-sort
//
//===----------------------------------------------------------------------===//

// Copyright (c) 2009 Alexander Stepanov and Paul McJones
// Modified in 2015-2020 by Morwenn for inclusion into cpp-sort
//
// Permission to use, copy, modify, distribute and sell this software
// and its documentation for any purpose is hereby granted without
// fee, provided that the above copyright notice appear in all copies
// and that both that copyright notice and this permission notice
// appear in supporting documentation. The authors make no
// representations about the suitability of this software for any
// purpose. It is provided "as is" without express or implied
// warranty.

// Algorithms from
// Elements of Programming
// by Alexander Stepanov and Paul McJones
// Addison-Wesley Professional, 2009

#ifndef CPPSORT_DETAIL_INPLACE_MERGE_H_
#define CPPSORT_DETAIL_INPLACE_MERGE_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <algorithm>
#include <cstddef>
#include <iterator>
#include <memory>
#include <utility>
#include <cpp-sort/utility/as_function.h>
#include <cpp-sort/utility/iter_move.h>
#include "config.h"
#include "iterator_traits.h"
#include "lower_bound.h"
#include "memory.h"
#include "move.h"
#include "rotate.h"
#include "type_traits.h"
#include "upper_bound.h"

namespace cppsort
{
namespace detail
{
    ////////////////////////////////////////////////////////////
    // Blind merge with a buffer

    template<typename InputIterator1, typename InputIterator2,
             typename OutputIterator, typename Size,
             typename Compare, typename Projection>
    auto half_inplace_merge(InputIterator1 first1, InputIterator1 last1,
                            InputIterator2 first2, InputIterator2 last2,
                            OutputIterator result, Size min_len,
                            Compare compare, Projection projection)
        -> void
    {
        using utility::iter_move;
        auto&& comp = utility::as_function(compare);
        auto&& proj = utility::as_function(projection);

        for (; min_len != 0 ; --min_len) {
            CPPSORT_ASSUME(first1 != last1);
            CPPSORT_ASSUME(first2 != last2);
            if (comp(proj(*first2), proj(*first1))) {
                *result = iter_move(first2);
                ++first2;
            } else {
                *result = iter_move(first1);
                ++first1;
            }
            ++result;
        }

        if (first1 == last1) {
            // first2 through last2 are already in the right spot
            return;
        }

        if (first2 == last2) {
            detail::move(first1, last1, result);
            return;
        }

        while (true) {
            CPPSORT_ASSUME(first1 != last1);
            CPPSORT_ASSUME(first2 != last2);

            if (comp(proj(*first2), proj(*first1))) {
                *result = iter_move(first2);
                ++first2;
                if (first2 == last2) {
                    detail::move(first1, last1, ++result);
                    return;
                }
            } else {
                *result = iter_move(first1);
                ++first1;
                if (first1 == last1) {
                    // first2 through last2 are already in the right spot
                    return;
                }
            }
            ++result;
        }
    }

    ////////////////////////////////////////////////////////////
    // Prepare the buffer prior to the blind merge (only for
    // bidirectional iterator)

    template<typename Predicate>
    class invert
    {
        private:

            Predicate predicate;

        public:

            invert() {}

            explicit invert(Predicate predicate):
                predicate(std::move(predicate))
            {}

            template<typename T1, typename T2>
            auto operator()(T1&& x, T2&& y)
                -> bool
            {
                auto&& pred = utility::as_function(predicate);
                return pred(std::forward<T2>(y), std::forward<T1>(x));
            }

            template<typename T1, typename T2>
            auto operator()(T1&& x, T2&& y) const
                -> bool
            {
                auto&& pred = utility::as_function(predicate);
                return pred(std::forward<T2>(y), std::forward<T1>(x));
            }
    };

    template<typename BidirectionalIterator, typename RandomAccessIterator,
             typename Compare, typename Projection>
    auto buffered_inplace_merge(BidirectionalIterator first, BidirectionalIterator middle,
                                BidirectionalIterator last,
                                Compare compare, Projection projection,
                                difference_type_t<BidirectionalIterator> len1,
                                difference_type_t<BidirectionalIterator> len2,
                                RandomAccessIterator buff)
        -> void
    {
        using utility::iter_move;
        using rvalue_reference = remove_cvref_t<rvalue_reference_t<BidirectionalIterator>>;
        destruct_n<rvalue_reference> d(0);
        std::unique_ptr<rvalue_reference, destruct_n<rvalue_reference>&> h2(buff, d);
        if (len1 <= len2) {
            auto ptr = uninitialized_move(first, middle, buff, d);
            half_inplace_merge(buff, ptr, middle, last, first, len1,
                               std::move(compare), std::move(projection));
        } else {
            auto ptr = uninitialized_move(middle, last, buff, d);
            using RBi = std::reverse_iterator<BidirectionalIterator>;
            using Rv = std::reverse_iterator<rvalue_reference*>;
            half_inplace_merge(Rv(ptr), Rv(buff),
                               RBi(middle), RBi(first),
                               RBi(last), len2,
                               invert<Compare>(compare), std::move(projection));
        }
    }

    ////////////////////////////////////////////////////////////
    // Forward algorithm using recmerge

    template<typename ForwardIterator, typename Compare, typename Projection>
    auto merge_n_step_0(ForwardIterator f0, difference_type_t<ForwardIterator> n0,
                        ForwardIterator f1, difference_type_t<ForwardIterator> n1,
                        Compare compare, Projection projection,
                        ForwardIterator& f0_0, difference_type_t<ForwardIterator>& n0_0,
                        ForwardIterator& f0_1, difference_type_t<ForwardIterator>& n0_1,
                        ForwardIterator& f1_0, difference_type_t<ForwardIterator>& n1_0,
                        ForwardIterator& f1_1, difference_type_t<ForwardIterator>& n1_1)
        -> void
    {
        auto&& proj = utility::as_function(projection);

        f0_0 = std::move(f0);
        n0_0 = n0 / 2;
        f0_1 = std::next(f0_0, n0_0);
        f1_1 = lower_bound(f1, std::next(f1, n1), proj(*f0_1),
                           std::move(compare), std::move(projection));
        f1_0 = detail::rotate(f0_1, std::move(f1), f1_1);
        n0_1 = std::distance(f0_1, f1_0);
        ++f1_0;
        n1_0 = n0 - n0_0 - 1;
        n1_1 = n1 - n0_1;
    }

    template<typename ForwardIterator, typename Compare, typename Projection>
    auto merge_n_step_1(ForwardIterator f0, difference_type_t<ForwardIterator> n0,
                        ForwardIterator f1, difference_type_t<ForwardIterator> n1,
                        Compare compare, Projection projection,
                        ForwardIterator& f0_0, difference_type_t<ForwardIterator>& n0_0,
                        ForwardIterator& f0_1, difference_type_t<ForwardIterator>& n0_1,
                        ForwardIterator& f1_0, difference_type_t<ForwardIterator>& n1_0,
                        ForwardIterator& f1_1, difference_type_t<ForwardIterator>& n1_1)
        -> void
    {
        auto&& proj = utility::as_function(projection);

        f0_0 = f0;
        n0_1 = n1 / 2;
        f1_1 = std::next(f1, n0_1);
        f0_1 = upper_bound(f0, std::next(f0, n0), proj(*f1_1),
                           std::move(compare), std::move(projection));
        ++f1_1;
        f1_0 = detail::rotate(f0_1, std::move(f1), f1_1);
        n0_0 = std::distance(f0_0, f0_1);
        n1_0 = n0 - n0_0;
        n1_1 = n1 - n0_1 - 1;
    }

    template<typename ForwardIterator, typename RandomAccessIterator,
             typename Compare, typename Projection>
    auto merge_n_adaptative(ForwardIterator f0, difference_type_t<ForwardIterator> n0,
                            ForwardIterator f1, difference_type_t<ForwardIterator> n1,
                            RandomAccessIterator buffer, std::ptrdiff_t buff_size,
                            Compare compare, Projection projection)
        -> void
    {
        using rvalue_reference = remove_cvref_t<rvalue_reference_t<ForwardIterator>>;
        using difference_type = difference_type_t<ForwardIterator>;

        if (n0 == 0 || n1 == 0) return;

        if (n0 <= buff_size) {
            destruct_n<rvalue_reference> d(0);
            std::unique_ptr<
                rvalue_reference,
                destruct_n<rvalue_reference>&
            > h2(buffer, d);
            auto buff_ptr = uninitialized_move(f0, f1, buffer, d);

            half_inplace_merge(
                buffer, buff_ptr, f1, std::next(f1, n1), f0,
                n0, std::move(compare), std::move(projection)
            );
            return;
        }

        ForwardIterator f0_0, f0_1, f1_0, f1_1;
        difference_type n0_0, n0_1, n1_0, n1_1;

        if (n0 < n1) {
            merge_n_step_0(
                std::move(f0), n0, std::move(f1), n1,
                compare, projection,
                f0_0, n0_0, f0_1, n0_1,
                f1_0, n1_0, f1_1, n1_1
            );
        } else {
            merge_n_step_1(
                std::move(f0), n0, std::move(f1), n1,
                compare, projection,
                f0_0, n0_0, f0_1, n0_1,
                f1_0, n1_0, f1_1, n1_1
            );
        }

        merge_n_adaptative(f0_0, n0_0, f0_1, n0_1,
                           buffer, buff_size,
                           compare, projection);
        merge_n_adaptative(f1_0, n1_0, f1_1, n1_1,
                           buffer, buff_size,
                           std::move(compare), std::move(projection));
    }

    template<typename ForwardIterator, typename RandomAccessIterator,
             typename Compare, typename Projection>
    auto inplace_merge(ForwardIterator first, ForwardIterator middle, ForwardIterator,
                       Compare compare, Projection projection,
                       difference_type_t<ForwardIterator> len1,
                       difference_type_t<ForwardIterator> len2,
                       RandomAccessIterator buff, std::ptrdiff_t buff_size,
                       std::forward_iterator_tag)
        -> void
    {
        merge_n_adaptative(std::move(first), len1, std::move(middle), len2,
                           buff, buff_size,
                           std::move(compare), std::move(projection));
    }

    ////////////////////////////////////////////////////////////
    // Bidirectional algorithm using recmerge

    template<typename BidirectionalIterator, typename RandomAccessIterator,
             typename Compare, typename Projection>
    auto inplace_merge(BidirectionalIterator first, BidirectionalIterator middle,
                       BidirectionalIterator last,
                       Compare compare, Projection projection,
                       difference_type_t<BidirectionalIterator> len1,
                       difference_type_t<BidirectionalIterator> len2,
                       RandomAccessIterator buff, std::ptrdiff_t buff_size,
                       std::bidirectional_iterator_tag tag)
        -> void
    {
        using difference_type = difference_type_t<BidirectionalIterator>;
        auto&& comp = utility::as_function(compare);
        auto&& proj = utility::as_function(projection);

        while (true) {
            // if middle == last, we're done
            if (len2 == 0)
                return;
            if (len1 <= buff_size || len2 <= buff_size) {
                buffered_inplace_merge(first, middle, last,
                                       std::move(compare), std::move(projection),
                                       len1, len2, buff);
                return;
            }
            // shrink [first, middle) as much as possible (with no moves), returning if it shrinks to 0
            for (; true; ++first, (void) --len1) {
                if (len1 == 0)
                    return;
                if (comp(proj(*middle), proj(*first)))
                    break;
            }
            // first < middle < last
            // *first > *middle
            // partition [first, m1) [m1, middle) [middle, m2) [m2, last) such that
            //     all elements in:
            //         [first, m1)  <= [middle, m2)
            //         [middle, m2) <  [m1, middle)
            //         [m1, middle) <= [m2, last)
            //     and m1 or m2 is in the middle of its range
            BidirectionalIterator m1;  // "median" of [first, middle)
            BidirectionalIterator m2;  // "median" of [middle, last)
            difference_type len11;      // distance(first, m1)
            difference_type len21;      // distance(middle, m2)
            // binary search smaller range
            if (len1 < len2) {
                // len >= 1, len2 >= 2
                len21 = len2 / 2;
                m2 = middle;
                std::advance(m2, len21);
                m1 = upper_bound(first, middle, proj(*m2), compare, projection);
                len11 = std::distance(first, m1);
            } else {
                if (len1 == 1) {
                    // len1 >= len2 && len2 > 0, therefore len2 == 1
                    // It is known *first > *middle
                    using utility::iter_swap;
                    iter_swap(first, middle);
                    return;
                }
                // len1 >= 2, len2 >= 1
                len11 = len1 / 2;
                m1 = first;
                std::advance(m1, len11);
                m2 = lower_bound(middle, last, proj(*m1), compare, projection);
                len21 = std::distance(middle, m2);
            }
            difference_type len12 = len1 - len11;  // distance(m1, middle)
            difference_type len22 = len2 - len21;  // distance(m2, last)
            // [first, m1) [m1, middle) [middle, m2) [m2, last)
            // swap middle two partitions
            middle = detail::rotate(m1, middle, m2);
            // len12 and len21 now have swapped meanings
            // merge smaller range with recursive call and larger with tail recursion elimination
            if (len11 + len21 < len12 + len22) {
                inplace_merge(first, m1, middle, compare, projection,
                              len11, len21, buff, buff_size, tag);
    //          inplace_merge_impl(middle, m2, last, compare, projection,
    //                             len12, len22, buff, buff_size);
                first = middle;
                middle = m2;
                len1 = len12;
                len2 = len22;
            } else {
                inplace_merge(middle, m2, last, compare, projection,
                              len12, len22, buff, buff_size, tag);
    //          inplace_merge_impl(first, m1, middle, compare, projection,
    //                             len11, len21, buff, buff_size);
                last = middle;
                middle = m1;
                len1 = len11;
                len2 = len21;
            }
        }
    }

    ////////////////////////////////////////////////////////////
    // Random-access algorithm using symmerge

    template<typename RandomAccessIterator, typename Compare, typename Projection>
    auto symmerge_bsearch(RandomAccessIterator arr,
                          difference_type_t<RandomAccessIterator> l,
                          difference_type_t<RandomAccessIterator> r,
                          difference_type_t<RandomAccessIterator> p,
                          Compare compare, Projection projection)
        -> difference_type_t<RandomAccessIterator>
    {
        auto&& comp = utility::as_function(compare);
        auto&& proj = utility::as_function(projection);

        while (l < r) {
            auto m = l + (r - l) / 2;
            if (not comp(proj(arr[p-m]), proj(arr[m]))) {
                l = m + 1;
            } else {
                r = m;
            }
        }
        return l;
    }

    template<typename RandomAccessIterator1, typename RandomAccessIterator2,
             typename Compare, typename Projection>
    auto symmerge(RandomAccessIterator1 arr,
                  difference_type_t<RandomAccessIterator1> first,
                  difference_type_t<RandomAccessIterator1> middle,
                  difference_type_t<RandomAccessIterator1> last,
                  Compare compare, Projection projection,
                  difference_type_t<RandomAccessIterator1> len1,
                  difference_type_t<RandomAccessIterator1> len2,
                  RandomAccessIterator2 buff, std::ptrdiff_t buff_size)
        -> void
    {
        while (len1 > 0 && len2 > 0)  {

            if (len1 <= buff_size || len2 <= buff_size) {
                buffered_inplace_merge(arr + first, arr + middle, arr + last,
                                       std::move(compare), std::move(projection),
                                       len1, len2, buff);
                return;
            }

            auto m = first + (last - first) / 2;
            auto n = m + middle;
            auto start = middle > m ?
                symmerge_bsearch(arr, n - last, m, n - 1, compare, projection) :
                symmerge_bsearch(arr, first, middle, n - 1, compare, projection);
            auto end = n - start;

            detail::rotate(arr + start, arr + middle, arr + end);
            symmerge(arr, first, start, m, compare, projection,
                     start - first, m - start, buff, buff_size);

            // Unrolled tail recursion, last doesn't change
            first = m;
            middle = end;
            len1 = middle - first;
            len2 = last - middle;
        }
    }

    template<typename RandomAccessIterator1, typename RandomAccessIterator2,
             typename Compare, typename Projection>
    auto inplace_merge(RandomAccessIterator1 first, RandomAccessIterator1 middle,
                       RandomAccessIterator1 last,
                       Compare compare, Projection projection,
                       difference_type_t<RandomAccessIterator1> len1,
                       difference_type_t<RandomAccessIterator1> len2,
                       RandomAccessIterator2 buff, std::ptrdiff_t buff_size,
                       std::random_access_iterator_tag)
        -> void
    {
        symmerge(first, 0, middle - first, last - first,
                 std::move(compare), std::move(projection),
                 len1, len2, buff, buff_size);
    }

    ////////////////////////////////////////////////////////////
    // Intermediate functions allocating the memory buffer if
    // one wasn't passed

    template<typename ForwardIterator, typename Compare, typename Projection>
    auto inplace_merge(ForwardIterator first, ForwardIterator middle,
                       ForwardIterator last, Compare compare, Projection projection,
                       std::forward_iterator_tag)
        -> void
    {
        using rvalue_reference = remove_cvref_t<rvalue_reference_t<ForwardIterator>>;
        auto&& comp = as_function(compare);
        auto&& proj = as_function(projection);

        // Shrink the problem size on the left side
        while (first != middle && not comp(proj(*middle), proj(*first))) {
            ++first;
        }

        if (first == middle) return;

        auto n0 = std::distance(first, middle);
        auto n1 = std::distance(middle, last);

        auto buffer = temporary_buffer<rvalue_reference>(std::max(n0, n1));
        merge_n_adaptative(std::move(first), n0, std::move(middle), n1,
                           buffer.data(), buffer.size(),
                           std::move(compare), std::move(projection));
    }

    template<typename BidirectionalIterator, typename Compare, typename Projection>
    auto inplace_merge(BidirectionalIterator first, BidirectionalIterator middle,
                       BidirectionalIterator last, Compare compare, Projection projection,
                       std::bidirectional_iterator_tag)
        -> void
    {
        using rvalue_reference = remove_cvref_t<rvalue_reference_t<BidirectionalIterator>>;

        auto len1 = std::distance(first, middle);
        auto len2 = std::distance(middle, last);
        temporary_buffer<rvalue_reference> buffer(std::min(len1, len2));

        using category = iterator_category_t<BidirectionalIterator>;
        inplace_merge(std::move(first), std::move(middle), std::move(last),
                      std::move(compare), std::move(projection),
                      len1, len2, buffer.data(), buffer.size(),
                      category{});
    }

    ////////////////////////////////////////////////////////////
    // Generic inplace_merge interfaces

    // Unbuffered overload, defers the buffer allocation to a specific
    // function depending on the iterator category

    template<typename ForwardIterator, typename Compare, typename Projection>
    auto inplace_merge(ForwardIterator first, ForwardIterator middle, ForwardIterator last,
                       Compare compare, Projection projection)
        -> void
    {
        using category = iterator_category_t<ForwardIterator>;
        inplace_merge(std::move(first), std::move(middle), std::move(last),
                      std::move(compare), std::move(projection),
                      category{});
    }

    // Buffered overload, which also happens to take the length of the
    // subranges, allowing not to cross the whole range to compute them
    // from time to time

    template<typename ForwardIterator, typename RandomAccessIterator,
             typename Compare, typename Projection>
    auto inplace_merge(ForwardIterator first, ForwardIterator middle, ForwardIterator last,
                       Compare compare, Projection projection,
                       difference_type_t<ForwardIterator> len1,
                       difference_type_t<ForwardIterator> len2,
                       RandomAccessIterator buff, std::ptrdiff_t buff_size)
        -> void
    {
        using category = iterator_category_t<ForwardIterator>;
        inplace_merge(std::move(first), std::move(middle), std::move(last),
                      std::move(compare), std::move(projection),
                      len1, len2, buff, buff_size,
                      category{});
    }
}}

#endif // CPPSORT_DETAIL_INPLACE_MERGE_H_
