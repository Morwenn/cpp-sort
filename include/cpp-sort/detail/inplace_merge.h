// -*- C++ -*-
//===-------------------------- algorithm ---------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is dual licensed under the MIT and the University of Illinois Open
// Source Licenses. See LICENSE.TXT for details.
//
// //  Modified in 2015 by Morwenn for inclusion into cpp-sort
//
//===----------------------------------------------------------------------===//
#ifndef CPPSORT_DETAIL_INPLACE_MERGE_H_
#define CPPSORT_DETAIL_INPLACE_MERGE_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <algorithm>
#include <cstddef>
#include <functional>
#include <iterator>
#include <memory>
#include <type_traits>
#include <utility>
#include "lower_bound.h"
#include "upper_bound.h"

namespace cppsort
{
namespace detail
{
    struct temporary_buffer_deleter
    {
        template<typename T>
        auto operator()(T* pointer) const
            -> void
        {
            if (pointer)
            {
                std::return_temporary_buffer(pointer);
            }
        }
    };

    struct destruct_n
    {
    private:
        std::size_t size;

        template <class Tp>
        void process(Tp* p, std::false_type) noexcept
            {for (std::size_t i = 0; i < size; ++i, ++p) p->~Tp();}

        template <class Tp>
        void process(Tp*, std::true_type) noexcept
            {}

        void incr(std::false_type) noexcept
            {++size;}
        void incr(std::true_type) noexcept
            {}

        void set(std::size_t s, std::false_type) noexcept
            {size = s;}
        void set(std::size_t, std::true_type) noexcept
            {}
    public:
        explicit destruct_n(std::size_t s) noexcept
            : size(s) {}

        template <class Tp>
        void incr(Tp*) noexcept
            {incr(std::integral_constant<bool, std::is_trivially_destructible<Tp>::value>());}

        template <class Tp>
        void set(std::size_t s, Tp*) noexcept
            {set(s, std::integral_constant<bool, std::is_trivially_destructible<Tp>::value>());}

        template <class Tp>
        void operator()(Tp* p) noexcept
            {process(p, std::integral_constant<bool, std::is_trivially_destructible<Tp>::value>());}
    };

    template <class Predicate>
    class negate
    {
    private:
        Predicate pred;
    public:
        negate() {}

        explicit negate(Predicate p) : pred(p) {}

        template <class T1>
        bool operator()(const T1& x) {return !pred(x);}

        template <class T1, class T2>
        bool operator()(const T1& x, const T2& y) {return !pred(x, y);}
    };

    template <class Compare, class InputIterator1, class InputIterator2,
              class OutputIterator, class Projection>
    void half_inplace_merge(InputIterator1 first1, InputIterator1 last1,
                            InputIterator2 first2, InputIterator2 last2,
                            OutputIterator result, Compare comp, Projection projection)
    {
        auto&& proj = utility::as_function(projection);

        for (; first1 != last1; ++result)
        {
            if (first2 == last2)
            {
                std::move(first1, last1, result);
                return;
            }

            if (comp(proj(*first2), proj(*first1)))
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
        // first2 through last2 are already in the right spot.
    }

    template <class Compare, class BidirectionalIterator, class Projection>
    auto buffered_inplace_merge(BidirectionalIterator first, BidirectionalIterator middle,
                                BidirectionalIterator last, Compare comp, Projection projection,
                                typename std::iterator_traits<BidirectionalIterator>::difference_type len1,
                                typename std::iterator_traits<BidirectionalIterator>::difference_type len2,
                                typename std::iterator_traits<BidirectionalIterator>::value_type* buff)
        -> void
    {
        using value_type = typename std::iterator_traits<BidirectionalIterator>::value_type;
        destruct_n d(0);
        std::unique_ptr<value_type, destruct_n&> h2(buff, d);
        if (len1 <= len2)
        {
            value_type* p = buff;
            for (BidirectionalIterator i = first; i != middle; d.incr((value_type*)nullptr), (void) ++i, ++p)
                ::new(p) value_type(std::move(*i));
            half_inplace_merge(buff, p, middle, last, first, comp, projection);
        }
        else
        {
            value_type* p = buff;
            for (BidirectionalIterator i = middle; i != last; d.incr((value_type*)nullptr), (void) ++i, ++p)
                ::new(p) value_type(std::move(*i));
            typedef std::reverse_iterator<BidirectionalIterator> RBi;
            typedef std::reverse_iterator<value_type*> Rv;
            half_inplace_merge(Rv(p), Rv(buff),
                               RBi(middle), RBi(first), RBi(last),
                               negate<Compare>(comp), projection);
        }
    }

    template <class Compare, class BidirectionalIterator, class Projection>
    auto inplace_merge_impl(BidirectionalIterator first, BidirectionalIterator middle, BidirectionalIterator last,
                            Compare comp, Projection projection,
                            typename std::iterator_traits<BidirectionalIterator>::difference_type len1,
                            typename std::iterator_traits<BidirectionalIterator>::difference_type len2,
                            typename std::iterator_traits<BidirectionalIterator>::value_type* buff,
                            std::ptrdiff_t buff_size)
        -> void
    {
        typedef typename std::iterator_traits<BidirectionalIterator>::difference_type difference_type;
        auto&& proj = utility::as_function(projection);

        while (true)
        {
            // if middle == last, we're done
            if (len2 == 0)
                return;
            if (len1 <= buff_size || len2 <= buff_size)
                return buffered_inplace_merge<Compare>
                       (first, middle, last, comp, projection, len1, len2, buff);
            // shrink [first, middle) as much as possible (with no moves), returning if it shrinks to 0
            for (; true; ++first, (void) --len1)
            {
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
            if (len1 < len2)
            {   // len >= 1, len2 >= 2
                len21 = len2 / 2;
                m2 = middle;
                std::advance(m2, len21);
                m1 = upper_bound(first, middle, proj(*m2), comp, projection);
                len11 = std::distance(first, m1);
            }
            else
            {
                if (len1 == 1)
                {   // len1 >= len2 && len2 > 0, therefore len2 == 1
                    // It is known *first > *middle
                    using std::swap;
                    swap(*first, *middle);
                    return;
                }
                // len1 >= 2, len2 >= 1
                len11 = len1 / 2;
                m1 = first;
                std::advance(m1, len11);
                m2 = lower_bound(middle, last, proj(*m1), comp, projection);
                len21 = std::distance(middle, m2);
            }
            difference_type len12 = len1 - len11;  // distance(m1, middle)
            difference_type len22 = len2 - len21;  // distance(m2, last)
            // [first, m1) [m1, middle) [middle, m2) [m2, last)
            // swap middle two partitions
            middle = std::rotate(m1, middle, m2);
            // len12 and len21 now have swapped meanings
            // merge smaller range with recurisve call and larger with tail recursion elimination
            if (len11 + len21 < len12 + len22)
            {
                inplace_merge_impl<Compare>(first, m1, middle, comp, projection,
                                            len11, len21, buff, buff_size);
    //          inplace_merge_impl<Compare>(middle, m2, last, comp, projection,
    //                                      len12, len22, buff, buff_size);
                first = middle;
                middle = m2;
                len1 = len12;
                len2 = len22;
            }
            else
            {
                inplace_merge_impl<Compare>(middle, m2, last, comp, projection,
                                            len12, len22, buff, buff_size);
    //          inplace_merge_impl<Compare>(first, m1, middle, comp, projection,
    //                                      len11, len21, buff, buff_size);
                last = middle;
                middle = m1;
                len1 = len11;
                len2 = len21;
            }
        }
    }

    template<typename BidirectionalIterator, typename Compare, typename Projection>
    auto inplace_merge(BidirectionalIterator first, BidirectionalIterator middle,
                       BidirectionalIterator last, Compare comp, Projection projection)
        -> void
    {
        typedef typename std::iterator_traits<BidirectionalIterator>::value_type value_type;
        typedef typename std::iterator_traits<BidirectionalIterator>::difference_type difference_type;
        difference_type len1 = std::distance(first, middle);
        difference_type len2 = std::distance(middle, last);
        difference_type buf_size = std::min(len1, len2);
        std::pair<value_type*, std::ptrdiff_t> buf = std::get_temporary_buffer<value_type>(buf_size);
        std::unique_ptr<value_type, temporary_buffer_deleter> h(buf.first);

        typedef typename std::add_lvalue_reference<Compare>::type Comp_ref;
        return inplace_merge_impl<Comp_ref>(first, middle, last, comp, projection,
                                            len1, len2, buf.first, buf.second);
    }
}}

#endif // CPPSORT_DETAIL_INPLACE_MERGE_H_
