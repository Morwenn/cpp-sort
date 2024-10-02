/*
 * Copyright (c) 2022-2024 Morwenn
 * SPDX-License-Identifier: MIT
 */
#ifndef CPPSORT_UTILITY_SORTED_ITERATORS_H_
#define CPPSORT_UTILITY_SORTED_ITERATORS_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <functional>
#include <iterator>
#include <utility>
#include <vector>
#include <cpp-sort/mstd/iterator.h>
#include <cpp-sort/mstd/ranges.h>
#include <cpp-sort/sorter_facade.h>
#include <cpp-sort/sorter_traits.h>
#include <cpp-sort/utility/adapter_storage.h>
#include <cpp-sort/utility/functional.h>
#include "../detail/checkers.h"

namespace cppsort::utility
{
    namespace detail
    {
        template<
            typename Sorter,
            typename Iterator,
            typename Compare,
            typename Projection
        >
        auto compute_sorted_iterators(Sorter&& sorter, Iterator first, Iterator last,
                                      mstd::iter_difference_t<Iterator> size,
                                      Compare compare, Projection projection)
            -> std::vector<Iterator>
        {
            // Copy the iterators in a vector
            std::vector<Iterator> iterators;
            iterators.reserve(size);
            for (auto it = first; it != last; ++it) {
                iterators.emplace_back(it);
            }

            // Sort the iterators on pointed values
            std::forward<Sorter>(sorter)(iterators.begin(), iterators.end(),
                                         std::move(compare),
                                         utility::indirect{} | std::move(projection));

            return iterators;
        }

        template<typename Sorter>
        struct sorted_iterators_impl:
            utility::adapter_storage<Sorter>,
            cppsort::detail::check_is_always_stable<Sorter>
        {
            sorted_iterators_impl() = default;

            constexpr explicit sorted_iterators_impl(Sorter&& sorter):
                utility::adapter_storage<Sorter>(std::move(sorter))
            {}

            template<
                mstd::forward_range Range,
                typename Compare = std::less<>,
                typename Projection = std::identity
            >
                requires is_projection_v<Projection, Range, Compare>
            auto operator()(Range&& range, Compare compare={}, Projection projection={}) const
                -> std::vector<mstd::iterator_t<Range>>
            {
                auto first = mstd::begin(range);
                auto last = mstd::end(range);
                auto last_it = mstd::next(first, std::move(last));
                return compute_sorted_iterators(this->get(),
                                                std::move(first), std::move(last_it),
                                                mstd::distance(range),
                                                std::move(compare), std::move(projection));
            }

            template<
                mstd::forward_iterator Iterator,
                mstd::sentinel_for<Iterator> Sentinel,
                typename Compare = std::less<>,
                typename Projection = std::identity
            >
                requires is_projection_iterator_v<Projection, Iterator, Compare>
            auto operator()(Iterator first, Sentinel last,
                            Compare compare={}, Projection projection={}) const
                -> std::vector<Iterator>
            {
                auto dist = mstd::distance(first, last);
                auto last_it = mstd::next(first, std::move(last));
                return compute_sorted_iterators(this->get(),
                                                std::move(first), std::move(last_it), dist,
                                                std::move(compare), std::move(projection));
            }

            ////////////////////////////////////////////////////////////
            // Sorter traits

            using iterator_category = std::forward_iterator_tag;
        };
    }

    template<typename Sorter>
    struct sorted_iterators:
        sorter_facade_base<detail::sorted_iterators_impl<Sorter>>
    {
        sorted_iterators() = default;

        constexpr explicit sorted_iterators(Sorter sorter):
            sorter_facade_base<detail::sorted_iterators_impl<Sorter>>(std::move(sorter))
        {}
    };
}

namespace cppsort
{
    ////////////////////////////////////////////////////////////
    // is_stable specialization

    template<typename Sorter, typename... Args>
    struct is_stable<cppsort::utility::sorted_iterators<Sorter>(Args...)>:
        is_stable<Sorter(Args...)>
    {};
}

#endif // CPPSORT_UTILITY_SORTED_ITERATORS_H_
