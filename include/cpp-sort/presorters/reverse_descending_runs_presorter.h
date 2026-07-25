/*
 * Copyright (c) 2026 Morwenn
 * SPDX-License-Identifier: MIT
 */
#ifndef CPPSORT_PRESORTERS_REVERSE_DESCENDING_RUNS_PRESORTER_H_
#define CPPSORT_PRESORTERS_REVERSE_DESCENDING_RUNS_PRESORTER_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <algorithm>
#include <functional>
#include <iterator>
#include <type_traits>
#include <utility>
#include <cpp-sort/sorter_facade.h>
#include <cpp-sort/sorter_traits.h>
#include <cpp-sort/utility/functional.h>
#include "../detail/heapsort.h"
#include "../detail/iterator_traits.h"
#include "../detail/type_traits.h"

namespace cppsort
{
    ////////////////////////////////////////////////////////////
    // Sorter

    namespace detail
    {
        struct reverse_descending_runs_presorter_impl
        {
            template<
                typename BidirectionalIterator,
                typename Compare = std::less<>,
                typename Projection = utility::identity,
                typename = detail::enable_if_t<
                    is_projection_iterator_v<Projection, BidirectionalIterator, Compare>
                >
            >
            auto operator()(BidirectionalIterator first, BidirectionalIterator last,
                            Compare compare={}, Projection projection={}) const
                -> void
            {
                static_assert(
                    std::is_base_of_v<
                        iterator_category,
                        iterator_category_t<BidirectionalIterator>
                    >,
                    "reverse_descending_runs_presorter requires at least bidirectional iterators"
                );

                auto&& comp = utility::as_function(compare);
                auto&& proj = utility::as_function(projection);

                if (first == last) return;
                auto next = std::next(first);
                if (next == last) return;

                while (next != last) {
                    if (comp(proj(*next), proj(*first))) {
                        // Consume a descending run
                        auto begin_run = first;
                        do {
                            first = next;
                            ++next;
                            if (next == last) {
                                std::reverse(begin_run, next);
                                return;
                            }
                        } while (comp(proj(*next), proj(*first)));
                        std::reverse(begin_run, next);
                    } else {
                        // Consume a non-descending run
                        do {
                            first = next;
                            ++next;
                            if (next == last) return;
                        } while (not comp(proj(*next), proj(*first)));
                    }

                    first = next;
                    ++next;
                }
            }

            ////////////////////////////////////////////////////////////
            // Sorter traits

            using iterator_category = std::bidirectional_iterator_tag;
            using is_always_stable = std::true_type;
        };
    }

    struct reverse_descending_runs_presorter:
        sorter_facade<detail::reverse_descending_runs_presorter_impl>
    {};

    ////////////////////////////////////////////////////////////
    // Sort function

    inline constexpr reverse_descending_runs_presorter reverse_descending_runs_presort{};
}

#endif // CPPSORT_PRESORTERS_REVERSE_DESCENDING_RUNS_PRESORTER_H_
