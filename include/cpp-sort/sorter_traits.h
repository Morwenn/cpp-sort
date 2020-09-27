/*
 * Copyright (c) 2015-2018 Morwenn
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
#include <cpp-sort/utility/functional.h>
#include "detail/raw_checkers.h"
#include "detail/type_traits.h"

namespace cppsort
{
    ////////////////////////////////////////////////////////////
    // Whether a type is a projection

    namespace detail
    {
        template<typename Projection, typename Iterable, typename Compare>
        using is_projection_t = invoke_result_t<Compare,
            invoke_result_t<Projection, decltype(*std::begin(std::declval<Iterable&>()))>,
            invoke_result_t<Projection, decltype(*std::end(std::declval<Iterable&>()))>
        >;

        template<typename Projection, typename Iterator, typename Compare>
        using is_projection_iterator_t = invoke_result_t<Compare,
            invoke_result_t<Projection, decltype(*std::declval<Iterator&>())>,
            invoke_result_t<Projection, decltype(*std::declval<Iterator&>())>
        >;
    }

    template<
        typename Projection,
        typename Iterable,
        typename Compare = std::less<>
    >
    struct is_projection:
        detail::is_detected<detail::is_projection_t, Projection, Iterable, Compare>
    {};

    template<typename Projection, typename Iterable, typename Compare=std::less<>>
    constexpr bool is_projection_v
        = is_projection<Projection, Iterable, Compare>::value;

    template<
        typename Projection,
        typename Iterator,
        typename Compare = std::less<>
    >
    struct is_projection_iterator:
        detail::is_detected<detail::is_projection_iterator_t, Projection, Iterator, Compare>
    {};

    template<typename Projection, typename Iterator, typename Compare=std::less<>>
    constexpr bool is_projection_iterator_v
        = is_projection_iterator<Projection, Iterator, Compare>::value;

    ////////////////////////////////////////////////////////////
    // Sorter type categories

    namespace detail
    {
        template<typename Sorter, typename Iterable>
        struct has_sort:
            is_invocable<Sorter, Iterable&>
        {};

        template<typename Sorter, typename Iterable, typename Compare>
        struct has_comparison_sort:
            conjunction<
                is_invocable<Sorter, Iterable&, Compare>,
                is_projection<utility::identity, Iterable, Compare>
            >
        {};

        template<typename Sorter, typename Iterable, typename Projection>
        struct has_projection_sort:
            conjunction<
                is_invocable<Sorter, Iterable&, Projection>,
                is_projection<Projection, Iterable>
            >
        {};

        template<typename Sorter, typename Iterable, typename Compare, typename Projection>
        struct has_comparison_projection_sort:
            conjunction<
                is_invocable<Sorter, Iterable&, Compare, Projection>,
                is_projection<Projection, Iterable, Compare>
            >
        {};

        template<typename Sorter, typename Iterator>
        struct has_sort_iterator:
            is_invocable<Sorter, Iterator, Iterator>
        {};

        template<typename Sorter, typename Iterator, typename Compare>
        struct has_comparison_sort_iterator:
            conjunction<
                is_invocable<Sorter, Iterator, Iterator, Compare>,
                is_projection_iterator<utility::identity, Iterator, Compare>
            >
        {};

        template<typename Sorter, typename Iterator, typename Projection>
        struct has_projection_sort_iterator:
            conjunction<
                is_invocable<Sorter, Iterator, Iterator, Projection>,
                is_projection_iterator<Projection, Iterator>
            >
        {};

        template<typename Sorter, typename Iterator, typename Compare, typename Projection>
        struct has_comparison_projection_sort_iterator:
            conjunction<
                is_invocable<Sorter, Iterator, Iterator, Compare, Projection>,
                is_projection_iterator<Projection, Iterator, Compare>
            >
        {};
    }

    template<typename Sorter, typename Iterable>
    struct is_sorter:
        detail::has_sort<Sorter, Iterable>
    {};

    template<typename Sorter, typename Iterable>
    constexpr bool is_sorter_v
        = is_sorter<Sorter, Iterable>::value;

    template<typename Sorter, typename Iterable, typename Compare>
    struct is_comparison_sorter:
        detail::has_comparison_sort<Sorter, Iterable, Compare>
    {};

    template<typename Sorter, typename Iterable, typename Compare>
    constexpr bool is_comparison_sorter_v
        = is_comparison_sorter<Sorter, Iterable, Compare>::value;

    template<typename Sorter, typename Iterable, typename Projection>
    struct is_projection_sorter:
        detail::has_projection_sort<Sorter, Iterable, Projection>
    {};

    template<typename Sorter, typename Iterable, typename Projection>
    constexpr bool is_projection_sorter_v
        = is_projection_sorter<Sorter, Iterable, Projection>::value;

    template<typename Sorter, typename Iterable, typename Compare, typename Projection>
    struct is_comparison_projection_sorter:
        detail::has_comparison_projection_sort<Sorter, Iterable, Compare, Projection>
    {};

    template<typename Sorter, typename Iterable, typename Compare, typename Projection>
    constexpr bool is_comparison_projection_sorter_v
        = is_comparison_projection_sorter<Sorter, Iterable, Compare, Projection>::value;

    template<typename Sorter, typename Iterator>
    struct is_sorter_iterator:
        detail::has_sort_iterator<Sorter, Iterator>
    {};

    template<typename Sorter, typename Iterator>
    constexpr bool is_sorter_iterator_v
        = is_sorter_iterator<Sorter, Iterator>::value;

    template<typename Sorter, typename Iterator, typename Compare>
    struct is_comparison_sorter_iterator:
        detail::has_comparison_sort_iterator<Sorter, Iterator, Compare>
    {};

    template<typename Sorter, typename Iterator, typename Compare>
    constexpr bool is_comparison_sorter_iterator_v
        = is_comparison_sorter_iterator<Sorter, Iterator, Compare>::value;

    template<typename Sorter, typename Iterator, typename Projection>
    struct is_projection_sorter_iterator:
        detail::has_projection_sort_iterator<Sorter, Iterator, Projection>
    {};

    template<typename Sorter, typename Iterator, typename Projection>
    constexpr bool is_projection_sorter_iterator_v
        = is_projection_sorter_iterator<Sorter, Iterator, Projection>::value;

    template<typename Sorter, typename Iterator, typename Compare, typename Projection>
    struct is_comparison_projection_sorter_iterator:
        detail::has_comparison_projection_sort_iterator<Sorter, Iterator, Compare, Projection>
    {};

    template<typename Sorter, typename Iterator, typename Compare, typename Projection>
    constexpr bool is_comparison_projection_sorter_iterator_v
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
    constexpr bool is_always_stable_v = is_always_stable<Sorter>::value;

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
    constexpr bool is_stable_v = is_stable<Arg>::value;

    ////////////////////////////////////////////////////////////
    // Fixed-size sorter traits

    template<template<std::size_t> class FixedSizeSorter>
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
