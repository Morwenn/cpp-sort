/*
 * Copyright (c) 2016-2022 Morwenn
 * SPDX-License-Identifier: MIT
 */
#ifndef CPPSORT_UTILITY_ITER_MOVE_H_
#define CPPSORT_UTILITY_ITER_MOVE_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <iterator>
#include <ranges>
#include <type_traits>
#include <utility>
#include <cpp-sort/mstd/type_traits.h>

namespace cppsort
{
    namespace detail
    {
        ////////////////////////////////////////////////////////////
        // Check whether iter_move and iter_swap are available for
        // the given iterators. Having those checks outside of the
        // utility namespace avoids its iter_move and iter_swap
        // overloads getting in the way

        struct call_iter_move
        {
            template<typename Iterator>
            auto operator()(Iterator it) const
                -> decltype(iter_move(it));

            template<typename Iterator>
            auto operator()(std::move_iterator<Iterator> it) const
                -> decltype(iter_move(it.base()));

            template<typename Iterator>
            auto operator()(std::reverse_iterator<Iterator> it) const
                -> decltype(iter_move(it.base()));
        };

        struct call_iter_swap
        {
            template<typename Iterator>
            auto operator()(Iterator it1, Iterator it2) const
                -> decltype(iter_swap(it1, it2));

            template<typename Iterator>
            auto operator()(std::move_iterator<Iterator> it1, std::move_iterator<Iterator> it2) const
                -> decltype(iter_swap(it1.base(), it2.base()));

            template<typename Iterator>
            auto operator()(std::reverse_iterator<Iterator> it1, std::reverse_iterator<Iterator> it2) const
                -> decltype(iter_swap(it1.base(), it2.base()));
        };

        template<typename Iterator>
        inline constexpr bool has_iter_move_v
            = std::is_invocable_v<call_iter_move, Iterator>;

        template<typename Iterator>
        inline constexpr bool has_iter_swap_v
            = std::is_invocable_v<call_iter_swap, Iterator, Iterator>;

        ////////////////////////////////////////////////////////////
        // Result type of a non-specialized iter_move call

        template<typename Iterator>
        using iter_move_t = mstd::conditional_t<
            std::is_reference_v<typename std::iterator_traits<Iterator>::reference>,
            std::remove_reference_t<typename std::iterator_traits<Iterator>::reference>&&,
            std::decay_t<typename std::iterator_traits<Iterator>::reference>
        >;
    }

    namespace utility
    {
        ////////////////////////////////////////////////////////////
        // Generic iter_move and iter_swap

        using std::ranges::iter_move;

        template<typename Iterator>
            requires (
                not cppsort::detail::has_iter_swap_v<Iterator> &&
                cppsort::detail::has_iter_move_v<Iterator>
            )
        constexpr auto iter_swap(Iterator lhs, Iterator rhs)
            -> void
        {
            auto tmp = iter_move(lhs);
            *lhs = iter_move(rhs);
            *rhs = std::move(tmp);
        }

        template<typename Iterator>
            requires (
                not cppsort::detail::has_iter_swap_v<Iterator> &&
                not cppsort::detail::has_iter_move_v<Iterator>
            )
        constexpr auto iter_swap(Iterator lhs, Iterator rhs)
            -> void
        {
            // While this overload is not strictly needed, it
            // ensures that an ADL-found swap is used when the
            // iterator type does not have a dedicated iter_move
            // ADL-found overload
            using std::swap;
            swap(*lhs, *rhs);
        }

        ////////////////////////////////////////////////////////////
        // std::reverse_iterator overloads

        template<typename Iterator>
            requires (not cppsort::detail::has_iter_swap_v<std::reverse_iterator<Iterator>>)
        auto iter_swap(std::reverse_iterator<Iterator> lhs, std::reverse_iterator<Iterator> rhs)
            -> void
        {
            iter_swap(std::prev(lhs.base()), std::prev(rhs.base()));
        }

        ////////////////////////////////////////////////////////////
        // std::move_iterator overloads

        template<typename Iterator>
            requires (not cppsort::detail::has_iter_swap_v<std::move_iterator<Iterator>>)
        auto iter_swap(std::move_iterator<Iterator> lhs, std::move_iterator<Iterator> rhs)
            -> void
        {
            iter_swap(lhs.base(), rhs.base());
        }
    }
}

#endif // CPPSORT_UTILITY_ITER_MOVE_H_
