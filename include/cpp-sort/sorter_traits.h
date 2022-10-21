/*
 * Copyright (c) 2015-2022 Morwenn
 * SPDX-License-Identifier: MIT
 */
#ifndef CPPSORT_SORTER_TRAITS_H_
#define CPPSORT_SORTER_TRAITS_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <cstddef>
#include <functional>
#include <iterator>
#include <type_traits>
#include <utility>
#include <cpp-sort/mstd/ranges.h>
#include "detail/raw_checkers.h"
#include "detail/type_traits.h"

namespace cppsort
{
    ////////////////////////////////////////////////////////////
    // Whether a type is a projection

    namespace detail
    {
        template<typename Projection, typename Range, typename Compare>
        using is_projection_t = std::invoke_result_t<Compare,
            std::invoke_result_t<Projection, decltype(*mstd::begin(std::declval<Range&>()))>,
            std::invoke_result_t<Projection, decltype(*mstd::end(std::declval<Range&>()))>
        >;

        template<typename Projection, typename Iterator, typename Compare>
        using is_projection_iterator_t = std::invoke_result_t<Compare,
            std::invoke_result_t<Projection, decltype(*std::declval<Iterator&>())>,
            std::invoke_result_t<Projection, decltype(*std::declval<Iterator&>())>
        >;
    }

    template<
        typename Projection,
        typename Range,
        typename Compare = std::less<>
    >
    struct is_projection:
        detail::is_detected<detail::is_projection_t, Projection, Range, Compare>
    {};

    template<typename Projection, typename Range, typename Compare=std::less<>>
    inline constexpr bool is_projection_v
        = is_projection<Projection, Range, Compare>::value;

    template<
        typename Projection,
        typename Iterator,
        typename Compare = std::less<>
    >
    struct is_projection_iterator:
        detail::is_detected<detail::is_projection_iterator_t, Projection, Iterator, Compare>
    {};

    template<typename Projection, typename Iterator, typename Compare=std::less<>>
    inline constexpr bool is_projection_iterator_v
        = is_projection_iterator<Projection, Iterator, Compare>::value;

    ////////////////////////////////////////////////////////////
    // Sorter type categories

    namespace detail
    {
        template<typename Sorter, typename Range>
        struct has_sort:
            std::is_invocable<Sorter, Range&>
        {};

        template<typename Sorter, typename Range, typename Compare>
        struct has_comparison_sort:
            std::conjunction<
                std::is_invocable<Sorter, Range&, Compare>,
                is_projection<std::identity, Range, Compare>
            >
        {};

        template<typename Sorter, typename Range, typename Projection>
        struct has_projection_sort:
            std::conjunction<
                std::is_invocable<Sorter, Range&, Projection>,
                is_projection<Projection, Range>
            >
        {};

        template<typename Sorter, typename Range, typename Compare, typename Projection>
        struct has_comparison_projection_sort:
            std::conjunction<
                std::is_invocable<Sorter, Range&, Compare, Projection>,
                is_projection<Projection, Range, Compare>
            >
        {};

        template<typename Sorter, typename Iterator>
        struct has_sort_iterator:
            std::is_invocable<Sorter, Iterator, Iterator>
        {};

        template<typename Sorter, typename Iterator, typename Compare>
        struct has_comparison_sort_iterator:
            std::conjunction<
                std::is_invocable<Sorter, Iterator, Iterator, Compare>,
                is_projection_iterator<std::identity, Iterator, Compare>
            >
        {};

        template<typename Sorter, typename Iterator, typename Projection>
        struct has_projection_sort_iterator:
            std::conjunction<
                std::is_invocable<Sorter, Iterator, Iterator, Projection>,
                is_projection_iterator<Projection, Iterator>
            >
        {};

        template<typename Sorter, typename Iterator, typename Compare, typename Projection>
        struct has_comparison_projection_sort_iterator:
            std::conjunction<
                std::is_invocable<Sorter, Iterator, Iterator, Compare, Projection>,
                is_projection_iterator<Projection, Iterator, Compare>
            >
        {};
    }

    template<typename Sorter, typename Range>
    struct is_sorter:
        detail::has_sort<Sorter, Range>
    {};

    template<typename Sorter, typename Range>
    inline constexpr bool is_sorter_v
        = is_sorter<Sorter, Range>::value;

    template<typename Sorter, typename Range, typename Compare>
    struct is_comparison_sorter:
        detail::has_comparison_sort<Sorter, Range, Compare>
    {};

    template<typename Sorter, typename Range, typename Compare>
    inline constexpr bool is_comparison_sorter_v
        = is_comparison_sorter<Sorter, Range, Compare>::value;

    template<typename Sorter, typename Range, typename Projection>
    struct is_projection_sorter:
        detail::has_projection_sort<Sorter, Range, Projection>
    {};

    template<typename Sorter, typename Range, typename Projection>
    inline constexpr bool is_projection_sorter_v
        = is_projection_sorter<Sorter, Range, Projection>::value;

    template<typename Sorter, typename Range, typename Compare, typename Projection>
    struct is_comparison_projection_sorter:
        detail::has_comparison_projection_sort<Sorter, Range, Compare, Projection>
    {};

    template<typename Sorter, typename Range, typename Compare, typename Projection>
    inline constexpr bool is_comparison_projection_sorter_v
        = is_comparison_projection_sorter<Sorter, Range, Compare, Projection>::value;

    template<typename Sorter, typename Iterator>
    struct is_sorter_iterator:
        detail::has_sort_iterator<Sorter, Iterator>
    {};

    template<typename Sorter, typename Iterator>
    inline constexpr bool is_sorter_iterator_v
        = is_sorter_iterator<Sorter, Iterator>::value;

    template<typename Sorter, typename Iterator, typename Compare>
    struct is_comparison_sorter_iterator:
        detail::has_comparison_sort_iterator<Sorter, Iterator, Compare>
    {};

    template<typename Sorter, typename Iterator, typename Compare>
    inline constexpr bool is_comparison_sorter_iterator_v
        = is_comparison_sorter_iterator<Sorter, Iterator, Compare>::value;

    template<typename Sorter, typename Iterator, typename Projection>
    struct is_projection_sorter_iterator:
        detail::has_projection_sort_iterator<Sorter, Iterator, Projection>
    {};

    template<typename Sorter, typename Iterator, typename Projection>
    inline constexpr bool is_projection_sorter_iterator_v
        = is_projection_sorter_iterator<Sorter, Iterator, Projection>::value;

    template<typename Sorter, typename Iterator, typename Compare, typename Projection>
    struct is_comparison_projection_sorter_iterator:
        detail::has_comparison_projection_sort_iterator<Sorter, Iterator, Compare, Projection>
    {};

    template<typename Sorter, typename Iterator, typename Compare, typename Projection>
    inline constexpr bool is_comparison_projection_sorter_iterator_v
        = is_comparison_projection_sorter_iterator<Sorter, Iterator, Compare, Projection>::value;

    ////////////////////////////////////////////////////////////
    // Sorter traits

    // This trait class is a bit different than usual traits
    // classes: the goal is to decrease the coupling between
    // the different traits and to make programs using one of
    // the traits valid even if the other traits don't exist

    template<typename Sorter>
    struct sorter_traits:
        detail::raw_check_iterator_category<Sorter>,
        detail::raw_check_is_always_stable<Sorter>
    {};

    template<typename Sorter>
    using iterator_category = typename sorter_traits<Sorter>::iterator_category;

    template<typename Sorter>
    using is_always_stable = typename sorter_traits<Sorter>::is_always_stable;

    template<typename Sorter>
    inline constexpr bool is_always_stable_v = is_always_stable<Sorter>::value;

    ////////////////////////////////////////////////////////////
    // Whether a sorter is stable when called with parameter of
    // specific types

    template<typename>
    struct is_stable;

    template<typename Sorter, typename... Args>
    struct is_stable<Sorter(Args...)>:
        sorter_traits<Sorter>::is_always_stable
    {};

    template<typename Arg>
    inline constexpr bool is_stable_v = is_stable<Arg>::value;

    ////////////////////////////////////////////////////////////
    // Fixed-size sorter traits

    template<template<std::size_t> typename FixedSizeSorter>
    struct fixed_sorter_traits
    {
        // Empty for SFINAE-friendliness
    };

    ////////////////////////////////////////////////////////////
    // Sorter traits modifiers

    template<typename Sorter, typename Category>
    struct rebind_iterator_category:
        Sorter
    {
        static_assert(
            std::is_base_of<
                cppsort::iterator_category<Sorter>,
                Category
            >::value,
            "the new iterator category should be more specific"
        );

        // New category
        using iterator_category = Category;
    };
}

#endif // CPPSORT_SORTER_TRAITS_H_
