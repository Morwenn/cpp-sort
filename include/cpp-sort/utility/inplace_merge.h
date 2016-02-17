/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2015-2016 Morwenn
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

// Copyright (c) 2009 Alexander Stepanov and Paul McJones
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

#ifndef CPPSORT_UTILITY_INPLACE_MERGE_H_
#define CPPSORT_UTILITY_INPLACE_MERGE_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <algorithm>
#include <functional>
#include <iterator>
#include <memory>
#include <utility>
#include <cpp-sort/utility/as_function.h>
#include <cpp-sort/utility/functional.h>
#include "../detail/inplace_merge.h"
#include "../detail/iterator_traits.h"

namespace cppsort
{
namespace utility
{
    namespace detail
    {
        template<typename ForwardIterator, typename Compare, typename Projection>
        auto merge_n_step_0(ForwardIterator f0, cppsort::detail::difference_type_t<ForwardIterator> n0,
                            ForwardIterator f1, cppsort::detail::difference_type_t<ForwardIterator> n1,
                            Compare compare, Projection projection,
                            ForwardIterator& f0_0, cppsort::detail::difference_type_t<ForwardIterator>& n0_0,
                            ForwardIterator& f0_1, cppsort::detail::difference_type_t<ForwardIterator>& n0_1,
                            ForwardIterator& f1_0, cppsort::detail::difference_type_t<ForwardIterator>& n1_0,
                            ForwardIterator& f1_1, cppsort::detail::difference_type_t<ForwardIterator>& n1_1)
            -> void
        {
            auto&& proj = utility::as_function(projection);

            f0_0 = f0;
            n0_0 = n0 / 2;
            f0_1 = std::next(f0_0, n0_0);
            f1_1 = cppsort::detail::lower_bound(f1, std::next(f1, n1), proj(*f0_1), compare, projection);
            f1_0 = std::rotate(f0_1, f1, f1_1);
            n0_1 = std::distance(f0_1, f1_0);
            ++f1_0;
            n1_0 = n0 - n0_0 - 1;
            n1_1 = n1 - n0_1;
        }

        template<typename ForwardIterator, typename Compare, typename Projection>
        auto merge_n_step_1(ForwardIterator f0, cppsort::detail::difference_type_t<ForwardIterator> n0,
                            ForwardIterator f1, cppsort::detail::difference_type_t<ForwardIterator> n1,
                            Compare compare, Projection projection,
                            ForwardIterator& f0_0, cppsort::detail::difference_type_t<ForwardIterator>& n0_0,
                            ForwardIterator& f0_1, cppsort::detail::difference_type_t<ForwardIterator>& n0_1,
                            ForwardIterator& f1_0, cppsort::detail::difference_type_t<ForwardIterator>& n1_0,
                            ForwardIterator& f1_1, cppsort::detail::difference_type_t<ForwardIterator>& n1_1)
            -> void
        {
            auto&& proj = utility::as_function(projection);

            f0_0 = f0;
            n0_1 = n1 / 2;
            f1_1 = std::next(f1, n0_1);
            f0_1 = cppsort::detail::upper_bound(f0, std::next(f0, n0), proj(*f1_1), compare, projection);
            ++f1_1;
            f1_0 = std::rotate(f0_1, f1, f1_1);
            n0_0 = std::distance(f0_0, f0_1);
            n1_0 = n0 - n0_0;
            n1_1 = n1 - n0_1 - 1;
        }

        template<typename ForwardIterator, typename RandomAccessIterator,
                 typename Compare, typename Projection>
        auto merge_n_adaptative(ForwardIterator f0, cppsort::detail::difference_type_t<ForwardIterator> n0,
                                ForwardIterator f1, cppsort::detail::difference_type_t<ForwardIterator> n1,
                                RandomAccessIterator buffer, std::ptrdiff_t buff_size,
                                Compare compare, Projection projection)
            -> void
        {
            using value_type = cppsort::detail::value_type_t<ForwardIterator>;
            using difference_type = cppsort::detail::difference_type_t<ForwardIterator>;

            if (n0 == 0 || n1 == 0) return;

            if (n0 <= buff_size)
            {
                cppsort::detail::destruct_n d(0);
                std::unique_ptr<value_type, cppsort::detail::destruct_n&> h2(buffer, d);

                value_type* buff_it = buffer;
                for (auto it = f0 ; it != f1 ; d.incr((value_type*)nullptr), (void) ++it)
                {
                    ::new(buff_it) value_type(std::move(*it));
                    ++buff_it;
                }

                cppsort::detail::half_inplace_merge(
                    buffer, buff_it, f1, std::next(f1, n1), f0,
                    compare, projection
                );
                return;
            }

            ForwardIterator f0_0, f0_1, f1_0, f1_1;
            difference_type n0_0, n0_1, n1_0, n1_1;

            if (n0 < n1)
            {
                merge_n_step_0(
                    f0, n0, f1, n1,
                    compare, projection,
                    f0_0, n0_0, f0_1, n0_1,
                    f1_0, n1_0, f1_1, n1_1
                );
            }
            else
            {
                merge_n_step_1(
                    f0, n0, f1, n1,
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
                               compare, projection);
        }

        template<typename ForwardIterator, typename Compare, typename Projection>
        auto inplace_merge(ForwardIterator first, ForwardIterator middle,
                           ForwardIterator last, Compare compare, Projection projection,
                           std::forward_iterator_tag)
            -> void
        {
            using value_type = cppsort::detail::value_type_t<ForwardIterator>;
            auto&& proj = as_function(projection);

            // Shrink the problem size on the left side
            while (first != middle && not compare(proj(*middle), proj(*first)))
            {
                ++first;
            }

            if (first == middle) return;

            auto n0 = std::distance(first, middle);
            auto n1 = std::distance(middle, last);
            ForwardIterator f0 = first;
            ForwardIterator f1 = middle;

            auto buffer = std::get_temporary_buffer<value_type>(std::max(n0, n1));
            std::unique_ptr<value_type, temporary_buffer_deleter> ptr(buffer.first);

            merge_n_adaptative(f0, n0, f1, n1,
                               buffer.first, buffer.second,
                               compare, projection);
        }

        template<typename BidirectionalIterator, typename Compare, typename Projection>
        auto inplace_merge(BidirectionalIterator first, BidirectionalIterator middle,
                           BidirectionalIterator last, Compare compare, Projection projection,
                           std::bidirectional_iterator_tag)
            -> void
        {
            cppsort::detail::inplace_merge(first, middle, last, compare, projection);
        }
    }

    template<
        typename ForwardIterator,
        typename Compare = std::less<>,
        typename Projection = utility::identity
    >
    auto inplace_merge(ForwardIterator first, ForwardIterator middle,
                       ForwardIterator last, Compare compare={}, Projection projection={})
        -> void
    {
        using category = cppsort::detail::iterator_category_t<ForwardIterator>;
        detail::inplace_merge(first, middle, last, compare, projection, category{});
    }
}}

#endif // CPPSORT_UTILITY_INPLACE_MERGE_H_
