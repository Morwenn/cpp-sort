/*
 * Copyright (c) 2016-2022 Morwenn
 * SPDX-License-Identifier: MIT
 */
#ifndef CPPSORT_SORTERS_COUNTING_SORTER_H_
#define CPPSORT_SORTERS_COUNTING_SORTER_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <functional>
#include <iterator>
#include <type_traits>
#include <utility>
#include <cpp-sort/sorter_facade.h>
#include "../detail/counting_sort.h"
#include "../detail/iterator_traits.h"
#include "../detail/type_traits.h"

namespace cppsort
{
    ////////////////////////////////////////////////////////////
    // Sorter

    namespace detail
    {
        struct counting_sorter_impl
        {
            template<typename ForwardIterator>
            auto operator()(ForwardIterator first, ForwardIterator last) const
                -> detail::enable_if_t<
                    detail::is_integral<value_type_t<ForwardIterator>>::value
                >
            {
                static_assert(
                    std::is_base_of<
                        iterator_category,
                        iterator_category_t<ForwardIterator>
                    >::value,
                    "counting_sorter requires at least forward iterators"
                );

                counting_sort(std::move(first), std::move(last));
            }

            template<typename ForwardIterator>
            auto operator()(ForwardIterator first, ForwardIterator last, std::greater<>) const
                -> detail::enable_if_t<
                    detail::is_integral<value_type_t<ForwardIterator>>::value
                >
            {
                static_assert(
                    std::is_base_of<
                        iterator_category,
                        iterator_category_t<ForwardIterator>
                    >::value,
                    "counting_sorter requires at least forward iterators"
                );

                reverse_counting_sort(std::move(first), std::move(last));
            }

            template<typename ForwardIterator>
            auto operator()(ForwardIterator first, ForwardIterator last, std::ranges::greater) const
                -> detail::enable_if_t<
                    detail::is_integral<value_type_t<ForwardIterator>>::value
                >
            {
                static_assert(
                    std::is_base_of<
                        iterator_category,
                        iterator_category_t<ForwardIterator>
                    >::value,
                    "counting_sorter requires at least forward iterators"
                );

                reverse_counting_sort(std::move(first), std::move(last));
            }

            ////////////////////////////////////////////////////////////
            // Sorter traits

            using iterator_category = std::forward_iterator_tag;
            using is_always_stable = std::false_type;
        };
    }

    struct counting_sorter:
        sorter_facade<detail::counting_sorter_impl>
    {};

    ////////////////////////////////////////////////////////////
    // Sort function

    inline constexpr counting_sorter counting_sort{};
}

#endif // CPPSORT_SORTERS_COUNTING_SORTER_H_
