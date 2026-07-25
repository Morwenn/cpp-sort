/*
 * Copyright (c) 2026 Morwenn
 * SPDX-License-Identifier: MIT
 */
#ifndef CPPSORT_PROBES_REVE_H_
#define CPPSORT_PROBES_REVE_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <functional>
#include <iterator>
#include <cpp-sort/sorter_facade.h>
#include <cpp-sort/sorter_traits.h>
#include <cpp-sort/utility/as_function.h>
#include <cpp-sort/utility/functional.h>
#include "../detail/iterator_traits.h"
#include "../detail/type_traits.h"

namespace cppsort::probe
{
    namespace detail
    {
        struct reve_impl
        {
            template<
                typename ForwardIterator,
                typename Compare = std::less<>,
                typename Projection = utility::identity,
                typename = cppsort::detail::enable_if_t<
                    is_projection_iterator_v<Projection, ForwardIterator, Compare>
                >
            >
            auto operator()(ForwardIterator first, ForwardIterator last,
                            Compare compare={}, Projection projection={}) const
                -> cppsort::detail::difference_type_t<ForwardIterator>
            {
                using difference_type = cppsort::detail::difference_type_t<ForwardIterator>;
                auto&& comp = utility::as_function(compare);
                auto&& proj = utility::as_function(projection);

                if (first == last || std::next(first) == last) {
                    return 0;
                }

                // Current "direction" of the run:
                // -1: we are in a descending run
                // 0: initial value, we don't know the direction of the first run yet
                // 1: we are in an ascending run
                int direction = 0;

                auto current = first;
                auto next = std::next(first);

                difference_type count = 0; // Number of "reversals" in the sequence
                for (;next != last; ++current, ++next) {
                    if (comp(proj(*current), proj(*next))) {
                        if (direction == -1) {
                            ++count;
                        }
                        direction = 1;
                    } else if (comp(proj(*next), proj(*current))) {
                        if (direction == 1) {
                            ++count;
                        }
                        direction = -1;
                    }
                    // When neighbors compare equivalent, the direction does not change
                }
                return count;
            }

            template<typename Integer>
            static constexpr auto max_for_size(Integer n)
                -> Integer
            {
                return n < 2 ? 0 : n - 2;
            }
        };
    }

    inline constexpr sorter_facade<detail::reve_impl> reve{};
}

#endif // CPPSORT_PROBES_REVE_H_
