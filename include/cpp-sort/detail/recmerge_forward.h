/*
 * Copyright (c) 2017-2024 Morwenn
 * SPDX-License-Identifier: MIT
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

#ifndef CPPSORT_DETAIL_RECMERGE_FORWARD_H_
#define CPPSORT_DETAIL_RECMERGE_FORWARD_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <cstddef>
#include <memory>
#include <utility>
#include <cpp-sort/mstd/iterator.h>
#include <cpp-sort/mstd/ranges.h>
#include <cpp-sort/utility/as_function.h>
#include "buffered_inplace_merge.h"
#include "iterator_traits.h"
#include "lower_bound.h"
#include "memory.h"
#include "move.h"
#include "rotate.h"
#include "upper_bound.h"

namespace cppsort::detail
{
    ////////////////////////////////////////////////////////////
    // recmerge for forward iterators

    template<typename ForwardIterator, typename Compare, typename Projection>
    auto merge_n_step_0(ForwardIterator f0, mstd::iter_difference_t<ForwardIterator> n0,
                        ForwardIterator f1, mstd::iter_difference_t<ForwardIterator> n1,
                        Compare compare, Projection projection,
                        ForwardIterator& f0_0, mstd::iter_difference_t<ForwardIterator>& n0_0,
                        ForwardIterator& f0_1, mstd::iter_difference_t<ForwardIterator>& n0_1,
                        ForwardIterator& f1_0, mstd::iter_difference_t<ForwardIterator>& n1_0,
                        ForwardIterator& f1_1, mstd::iter_difference_t<ForwardIterator>& n1_1)
        -> void
    {
        auto&& proj = utility::as_function(projection);

        f0_0 = std::move(f0);
        n0_0 = n0 / 2;
        f0_1 = mstd::next(f0_0, n0_0);
        f1_1 = lower_bound_n(f1, n1, proj(*f0_1), std::move(compare), std::move(projection));
        f1_0 = detail::rotate(f0_1, std::move(f1), f1_1);
        n0_1 = mstd::distance(f0_1, f1_0);
        ++f1_0;
        n1_0 = n0 - n0_0 - 1;
        n1_1 = n1 - n0_1;
    }

    template<typename ForwardIterator, typename Compare, typename Projection>
    auto merge_n_step_1(ForwardIterator f0, mstd::iter_difference_t<ForwardIterator> n0,
                        ForwardIterator f1, mstd::iter_difference_t<ForwardIterator> n1,
                        Compare compare, Projection projection,
                        ForwardIterator& f0_0, mstd::iter_difference_t<ForwardIterator>& n0_0,
                        ForwardIterator& f0_1, mstd::iter_difference_t<ForwardIterator>& n0_1,
                        ForwardIterator& f1_0, mstd::iter_difference_t<ForwardIterator>& n1_0,
                        ForwardIterator& f1_1, mstd::iter_difference_t<ForwardIterator>& n1_1)
        -> void
    {
        auto&& proj = utility::as_function(projection);

        f0_0 = f0;
        n0_1 = n1 / 2;
        f1_1 = mstd::next(f1, n0_1);
        f0_1 = upper_bound_n(f0, n0, proj(*f1_1), std::move(compare), std::move(projection));
        ++f1_1;
        f1_0 = detail::rotate(f0_1, std::move(f1), f1_1);
        n0_0 = mstd::distance(f0_0, f0_1);
        n1_0 = n0 - n0_0;
        n1_1 = n1 - n0_1 - 1;
    }

    template<
        mstd::forward_iterator Iterator,
        typename T,
        typename Compare,
        typename Projection
    >
    auto recmerge(Iterator f0, mstd::iter_difference_t<Iterator> n0,
                  Iterator f1, mstd::iter_difference_t<Iterator> n1,
                  T* buffer, std::ptrdiff_t buff_size,
                  Compare compare, Projection projection)
        -> void
    {
        using rvalue_type = rvalue_type_t<Iterator>;
        using difference_type = mstd::iter_difference_t<Iterator>;

        if (n0 == 0 || n1 == 0) return;

        if (n0 <= buff_size) {
            destruct_n<rvalue_type> d(0);
            std::unique_ptr<
                rvalue_type,
                destruct_n<rvalue_type>&
            > h2(buffer, d);
            auto buff_ptr = uninitialized_move(f0, f1, buffer, d);

            half_inplace_merge(
                buffer, buff_ptr, f1, mstd::next(f1, n1), f0,
                n0, std::move(compare), std::move(projection)
            );
            return;
        }

        Iterator f0_0, f0_1, f1_0, f1_1;
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

        recmerge(f0_0, n0_0, f0_1, n0_1, buffer, buff_size,
                 compare, projection);
        recmerge(f1_0, n1_0, f1_1, n1_1, buffer, buff_size,
                 std::move(compare), std::move(projection));
    }
}

#endif // CPPSORT_DETAIL_RECMERGE_FORWARD_H_
