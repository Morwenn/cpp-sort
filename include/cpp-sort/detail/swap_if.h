/*
 * Copyright (c) 2015-2020 Morwenn
 * SPDX-License-Identifier: MIT
 */
#ifndef CPPSORT_DETAIL_SWAP_IF_H_
#define CPPSORT_DETAIL_SWAP_IF_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <algorithm>
#include <functional>
#include <type_traits>
#include <utility>
#include <cpp-sort/utility/as_function.h>
#include <cpp-sort/utility/functional.h>
#include <cpp-sort/utility/iter_move.h>
#include "config.h"
#include "type_traits.h"

namespace cppsort
{
namespace detail
{
    ////////////////////////////////////////////////////////////
    // swap_if

    template<typename T, typename Compare, typename Projection>
    auto swap_if(T& lhs, T& rhs, Compare compare, Projection projection)
        -> void
    {
        auto&& comp = utility::as_function(compare);
        auto&& proj = utility::as_function(projection);

        if (comp(proj(rhs), proj(lhs))) {
            using std::swap;
            swap(lhs, rhs);
        }
    }

    template<typename T>
    auto swap_if(T& lhs, T& rhs)
        noexcept(noexcept(swap_if(lhs, rhs, std::less<>{}, utility::identity{})))
        -> void
    {
        swap_if(lhs, rhs, std::less<>{}, utility::identity{});
    }

    template<typename Integer>
    auto swap_if(Integer& x, Integer& y, std::less<>, utility::identity) noexcept
        -> std::enable_if_t<std::is_integral<Integer>::value>
    {
        Integer dx = x;
        x = std::min(x, y);
        y ^= dx ^ x;
    }

    template<typename Float>
    auto swap_if(Float& x, Float& y, std::less<>, utility::identity) noexcept
        -> std::enable_if_t<std::is_floating_point<Float>::value>
    {
        Float dx = x;
        x = std::min(x, y);
        y = std::max(dx, y);
    }

    template<typename Integer>
    auto swap_if(Integer& x, Integer& y, std::greater<>, utility::identity) noexcept
        -> std::enable_if_t<std::is_integral<Integer>::value>
    {
        Integer dx = x;
        x = std::max(x, y);
        y ^= dx ^ x;
    }

    template<typename Float>
    auto swap_if(Float& x, Float& y, std::greater<>, utility::identity) noexcept
        -> std::enable_if_t<std::is_floating_point<Float>::value>
    {
        Float dx = x;
        x = std::max(x, y);
        y = std::min(dx, y);
    }

#if CPPSORT_STD_IDENTITY_AVAILABLE
    template<typename Integer>
    auto swap_if(Integer& x, Integer& y, std::less<> comp, std::identity) noexcept
        -> std::enable_if_t<std::is_integral<Integer>::value>
    {
        return swap_if(x, y, comp, utility::identity{});
    }

    template<typename Float>
    auto swap_if(Float& x, Float& y, std::less<> comp, std::identity) noexcept
        -> std::enable_if_t<std::is_floating_point<Float>::value>
    {
        return swap_if(x, y, comp, utility::identity{});
    }

    template<typename Integer>
    auto swap_if(Integer& x, Integer& y, std::greater<> comp, std::identity) noexcept
        -> std::enable_if_t<std::is_integral<Integer>::value>
    {
        return swap_if(x, y, comp, utility::identity{});
    }

    template<typename Float>
    auto swap_if(Float& x, Float& y, std::greater<> comp, std::identity) noexcept
        -> std::enable_if_t<std::is_floating_point<Float>::value>
    {
        return swap_if(x, y, comp, utility::identity{});
    }
#endif

    ////////////////////////////////////////////////////////////
    // iter_swap_if

    template<
        typename Iterator,
        typename Compare,
        typename Projection,
        typename = std::enable_if_t<
            is_detected_v<utility::detail::has_iter_move_t, Iterator> ||
            is_detected_v<utility::detail::has_iter_swap_t, Iterator>
        >
    >
    auto iter_swap_if(Iterator lhs, Iterator rhs, Compare compare, Projection projection)
        -> void
    {
        auto&& comp = utility::as_function(compare);
        auto&& proj = utility::as_function(projection);

        if (comp(proj(*rhs), proj(*lhs))) {
            using utility::iter_swap;
            iter_swap(lhs, rhs);
        }
    }

    template<
        typename Iterator,
        typename Compare,
        typename Projection,
        typename = std::enable_if_t<
            not is_detected_v<utility::detail::has_iter_move_t, Iterator> &&
            not is_detected_v<utility::detail::has_iter_swap_t, Iterator>
        >,
        typename = void // dummy parameter for ODR
    >
    auto iter_swap_if(Iterator lhs, Iterator rhs, Compare compare, Projection projection)
        noexcept(noexcept(swap_if(*lhs, *rhs, std::move(compare), std::move(projection))))
        -> void
    {
        // Take advantage of the swap_if optimizations
        // only when the iterators don't have dedicated
        // iter_move or iter_swap ADL-found functions

        swap_if(*lhs, *rhs, std::move(compare), std::move(projection));
    }
}}

#endif // CPPSORT_DETAIL_SWAP_IF_H_
