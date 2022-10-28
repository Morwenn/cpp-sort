/*
 * Copyright (c) 2022 Morwenn
 * SPDX-License-Identifier: MIT
 */

//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef CPPSORT_MSTD_ITERATOR_H_
#define CPPSORT_MSTD_ITERATOR_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <concepts>
#include <iterator>
#include <type_traits>
#include <cpp-sort/mstd/concepts.h>
#include <cpp-sort/mstd/type_traits.h>

namespace cppsort::mstd
{
    namespace detail
    {
        ////////////////////////////////////////////////////////////
        // is_primary_template
        //
        // Check whether a type is an instantiation of the primary
        // template of very specific standard library classes that
        // are "blessed" by the standard for theis kinds of check.
        //
        // This utility requires standard library cooperation and
        // can not be fully reimplemented by hand, so we reuse guts
        // of various standard library implementations to make it
        // work.

#if defined(__GLIBCXX__)
        template<typename Iterator>
        concept is_iter_traits_primary
            = std::__detail::__primary_traits_iter<Iterator>;
#elif defined(_LIBCPP_VERSION)
        template<typename Iterator>
        conept is_iter_traits_primary
            = std::__is_primary_template<std::iterator_traits<Iterator>>::value;
#elif defined(_MSC_VER)
        template<typename Iterator>
        concept is_iter_traits_primary
            = std::_Is_from_primary<std::iterator_traits<Iterator>>;
#else
#       error "cpp-sort is not compatible with this standard library implementation"
#endif

        ////////////////////////////////////////////////////////////
        // iter_concept
        //
        // This utility requires standard library cooperation and
        // can not be fully reimplemented by hand, so we reuse guts
        // of various standard library implementations to make it
        // work.

#if defined(__GLIBCXX__)
        template<typename Iterator>
        using iter_concept = typename std::__detail::__iter_concept_impl<Iterator>::type;
#elif defined(_LIBCPP_VERSION)
        template<typename Iterator>
        using iter_concept = typename __iter_concept_cache<Iterator>::type::template _Apply<Iterator>;
#elif defined(_MSC_VER)
        template<typename Iterator>
        using iter_concept = std::_Iter_concept<Iterator>;
#else
#       error "cpp-sort is not compatible with this standard library implementation"
#endif

        ////////////////////////////////////////////////////////////
        // can_reference

        template<typename T>
        using with_reference = T&;

        template<typename T>
        concept can_reference = requires {
            typename with_reference<T>;
        };

        ////////////////////////////////////////////////////////////
        // boolean_testable

        template<typename T>
        concept boolean_testable_impl =
            std::convertible_to<T, bool>;

        template<typename T>
        concept boolean_testable =
            boolean_testable_impl<T> &&
            requires(T&& value) {
                { not std::forward<T>(value) } -> boolean_testable_impl;
            };

        ////////////////////////////////////////////////////////////
        // weakly_equality_comparable_with

        template<typename T, typename U>
        concept weakly_equality_comparable_with =
            requires(const std::remove_reference_t<T>& t, const std::remove_reference_t<U>& u) {
                { t == u } -> boolean_testable;
                { t != u } -> boolean_testable;
                { u == t } -> boolean_testable;
                { u != t } -> boolean_testable;
            };
    }

    ////////////////////////////////////////////////////////////
    // indirectly_readable
    //
    // Compared to std::indirectly_readable, this concept has no
    // constraint on the type returned by iter_move, and thus no
    // constraint on the associated rvalue-reference type. Some
    // parts of cpp-sort are too clever in this regard and go
    // further than the standard iterator model.

    namespace detail
    {
        template<typename Indirect>
        concept indirectly_readable_impl =
            requires(const Indirect in) {
                typename std::iter_value_t<Indirect>;
                typename std::iter_reference_t<Indirect>;
                typename std::iter_rvalue_reference_t<Indirect>;
                { *in } -> std::same_as<std::iter_reference_t<Indirect>>;
            } &&
            std::common_reference_with<
                std::iter_reference_t<Indirect>&&, std::iter_value_t<Indirect>&
            >;
    }

    template<typename Indirect>
    concept indirectly_readable =
        detail::indirectly_readable_impl<std::remove_cvref_t<Indirect>>;

    ////////////////////////////////////////////////////////////
    // incrementable_traits

#if defined(__SIZEOF_INT128__) && !defined(_LIBCPP_VERSION)
    template<typename T>
    struct incrementable_traits:
        std::incrementable_traits<T>
    {};

    template<typename T>
        requires (!requires { typename T::difference_type; }) &&
        requires(const T& a, const T& b) { { a - b } -> mstd::integral; }
    struct incrementable_traits<T>
    {
        using difference_type = mstd::make_signed_t<
            decltype(std::declval<T>() - std::declval<T>())
        >;
    };
#else
    using std::incrementable_traits;
#endif

    ////////////////////////////////////////////////////////////
    // iter_difference_t

#if defined(__SIZEOF_INT128__) && !defined(_LIBCPP_VERSION)
    template<typename Iterator>
    using iter_difference_t = typename mstd::conditional_t<
        detail::is_iter_traits_primary<std::remove_cvref_t<Iterator>>,
        incrementable_traits<std::remove_cvref_t<Iterator>>,
        std::iterator_traits<std::remove_cvref_t<Iterator>>
    >::difference_type;
#else
    using std::iter_difference_t;
#endif

    ////////////////////////////////////////////////////////////
    // weakly_incrementable
    //
    // Unlike the standard iterator model, cpp-sort does not
    // require iterators to provide a postfix operator++ at all.

    template<typename Iterator>
    concept weakly_incrementable =
        std::movable<Iterator> &&
        requires(Iterator it) {
            typename iter_difference_t<Iterator>;
            requires mstd::signed_integral<iter_difference_t<Iterator>>;
            { ++it } -> std::same_as<Iterator&>;
        };

    ////////////////////////////////////////////////////////////
    // incrementable
    //
    // Unlike the standard iterator model, cpp-sort does not
    // require iterators to provide a postfix operator++ at all.

    template<typename Iterator>
    concept incrementable =
        std::regular<Iterator> &&
        weakly_incrementable<Iterator>;

    ////////////////////////////////////////////////////////////
    // input_or_output_iterator

    template<typename Iterator>
    concept input_or_output_iterator =
        requires(Iterator it) {
            { *it } -> detail::can_reference;
        } &&
        weakly_incrementable<Iterator>;

    ////////////////////////////////////////////////////////////
    // sentinel_for

    template<typename Sentinel, typename Iterator>
    concept sentinel_for =
        std::semiregular<Sentinel> &&
        input_or_output_iterator<Iterator> &&
        detail::weakly_equality_comparable_with<Sentinel, Iterator>;

    ////////////////////////////////////////////////////////////
    // sized_sentinel_for

    template<typename Sentinel, typename Iterator>
    concept sized_sentinel_for =
        sentinel_for<Sentinel, Iterator> &&
        !std::disable_sized_sentinel_for<std::remove_cv_t<Sentinel>, std::remove_cv_t<Iterator>> &&
        requires(const Iterator& it, const Sentinel& s) {
            { s - it } -> std::same_as<iter_difference_t<Iterator>>;
            { it - s } -> std::same_as<iter_difference_t<Iterator>>;
        };

    ////////////////////////////////////////////////////////////
    // input_iterator

    template<typename Iterator>
    concept input_iterator =
        input_or_output_iterator<Iterator> &&
        indirectly_readable<Iterator> &&
        requires { typename detail::iter_concept<Iterator>; } &&
        std::derived_from<detail::iter_concept<Iterator>, std::input_iterator_tag>;

    ////////////////////////////////////////////////////////////
    // forward_iterator

    template<typename Iterator>
    concept forward_iterator =
        input_iterator<Iterator> &&
        std::derived_from<detail::iter_concept<Iterator>, std::forward_iterator_tag> &&
        incrementable<Iterator> &&
        sentinel_for<Iterator, Iterator>;

    ////////////////////////////////////////////////////////////
    // bidirectional_iterator

    template<typename Iterator>
    concept bidirectional_iterator =
        forward_iterator<Iterator> &&
        std::derived_from<detail::iter_concept<Iterator>, std::bidirectional_iterator_tag> &&
        requires(Iterator it) {
            { --it } -> std::same_as<Iterator&>;
        };

    ////////////////////////////////////////////////////////////
    // random_access_iterator

    template<typename Iterator>
    concept random_access_iterator =
        bidirectional_iterator<Iterator> &&
        std::derived_from<detail::iter_concept<Iterator>, std::random_access_iterator_tag> &&
        std::totally_ordered<Iterator> &&
        sized_sentinel_for<Iterator, Iterator> &&
        requires(Iterator it1, const Iterator it2, const iter_difference_t<Iterator> n) {
            { it1 += n } -> std::same_as<Iterator&>;
            { it2 +  n } -> std::same_as<Iterator>;
            { n + it2 } -> std::same_as<Iterator>;
            { it1 -= n } -> std::same_as<Iterator&>;
            { it2 - n } -> std::same_as<Iterator>;
            { it2[n] } -> std::same_as<std::iter_reference_t<Iterator>>;
        };

    ////////////////////////////////////////////////////////////
    // contiguous_iterator

    template<typename Iterator>
    concept contiguous_iterator =
        random_access_iterator<Iterator> &&
        std::derived_from<detail::iter_concept<Iterator>, std::contiguous_iterator_tag> &&
        std::is_lvalue_reference_v<std::iter_reference_t<Iterator>> &&
        std::same_as<
            std::iter_value_t<Iterator>,
            std::remove_cvref_t<std::iter_reference_t<Iterator>>
        > &&
        requires(const Iterator& it) {
            { std::to_address(it) }
                -> std::same_as<std::add_pointer_t<std::iter_reference_t<Iterator>>>;
        };
}

#endif // CPPSORT_MSTD_ITERATOR_H_
