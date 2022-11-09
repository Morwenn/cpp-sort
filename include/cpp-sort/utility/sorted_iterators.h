/*
 * Copyright (c) 2022 Morwenn
 * SPDX-License-Identifier: MIT
 */
#ifndef CPPSORT_UTILITY_SORTED_ITERATORS_H_
#define CPPSORT_UTILITY_SORTED_ITERATORS_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <functional>
#include <iterator>
#include <type_traits>
#include <utility>
#include <vector>
#include <cpp-sort/sorter_facade.h>
#include <cpp-sort/sorter_traits.h>
#include <cpp-sort/utility/adapter_storage.h>
#include <cpp-sort/utility/functional.h>
#include <cpp-sort/utility/size.h>
#include "../detail/checkers.h"
#include "../detail/iterator_traits.h"
#include "../detail/type_traits.h"

namespace cppsort
{
namespace utility
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
                                      cppsort::detail::difference_type_t<Iterator> size,
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
                typename ForwardIterable,
                typename Compare = std::less<>,
                typename Projection = utility::identity,
                typename = cppsort::detail::enable_if_t<
                    is_projection_v<Projection, ForwardIterable, Compare>
                >
            >
            auto operator()(ForwardIterable&& iterable, Compare compare={}, Projection projection={}) const
                -> std::vector<cppsort::detail::remove_cvref_t<decltype(std::begin(iterable))>>
            {
                using category = cppsort::detail::iterator_category_t<decltype(std::begin(iterable))>;
                static_assert(
                    std::is_base_of<iterator_category, category>::value,
                    "sorted_iterators requires at least forward iterators"
                );

                auto dist = cppsort::utility::size(iterable);
                return compute_sorted_iterators(this->get(), std::begin(iterable), std::end(iterable),
                                                dist, std::move(compare), std::move(projection));
            }

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
                -> std::vector<ForwardIterator>
            {
                using category = cppsort::detail::iterator_category_t<ForwardIterator>;
                static_assert(
                    std::is_base_of<iterator_category, category>::value,
                    "sorted_iterators requires at least forward iterators"
                );

                auto dist = std::distance(first, last);
                return compute_sorted_iterators(this->get(), std::move(first), std::move(last),
                                                dist, std::move(compare), std::move(projection));
            }

            ////////////////////////////////////////////////////////////
            // Sorter traits

            using iterator_category = std::forward_iterator_tag;
        };
    }

    template<typename Sorter>
    struct sorted_iterators:
        sorter_facade<detail::sorted_iterators_impl<Sorter>>
    {
        sorted_iterators() = default;

        constexpr explicit sorted_iterators(Sorter sorter):
            sorter_facade<detail::sorted_iterators_impl<Sorter>>(std::move(sorter))
        {}
    };
}}

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
