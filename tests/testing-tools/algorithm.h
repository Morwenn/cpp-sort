/*
 * Copyright (c) 2015-2021 Morwenn
 * SPDX-License-Identifier: MIT
 */
#ifndef CPPSORT_TESTSUITE_ALGORITHM_H_
#define CPPSORT_TESTSUITE_ALGORITHM_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <functional>
#include <cpp-sort/utility/as_function.h>
#include <cpp-sort/utility/functional.h>

namespace helpers
{
    template<
        typename Iterator,
        typename Compare = std::less<>,
        typename Projection = cppsort::utility::identity
    >
    constexpr auto is_sorted(Iterator first, Iterator last,
                             Compare compare={}, Projection projection={})
        -> bool
    {
        auto&& comp = cppsort::utility::as_function(compare);
        auto&& proj = cppsort::utility::as_function(projection);

        if (first == last) {
            return true;
        }

        auto next = first;
        while (++next != last) {
            if (comp(proj(*next), proj(*first))) {
                return false;
            }
            ++first;
        }
        return true;
    }

    template<
        typename ForwardIterator,
        typename T,
        typename Projection = cppsort::utility::identity
    >
    auto iota(ForwardIterator first, ForwardIterator last,
              T value, Projection projection={})
        -> void
    {
        auto&& proj = cppsort::utility::as_function(projection);

        while (first != last)
        {
            proj(*first++) = value;
            ++value;
        }
    }
}

#endif // CPPSORT_TESTSUITE_ALGORITHM_H_
