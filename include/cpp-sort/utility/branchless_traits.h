/*
 * Copyright (c) 2017-2021 Morwenn
 * SPDX-License-Identifier: MIT
 */
#ifndef CPPSORT_UTILITY_BRANCHLESS_TRAITS_H_
#define CPPSORT_UTILITY_BRANCHLESS_TRAITS_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <functional>
#include <type_traits>
#include "../detail/config.h"
#include "../detail/type_traits.h"

namespace cppsort
{
namespace utility
{
    ////////////////////////////////////////////////////////////
    // Whether a comparison function is likely branchless when
    // called with instances of a specific type

    namespace detail
    {
        template<typename Compare, typename T>
        struct is_probably_branchless_comparison_impl:
            std::false_type
        {};

        template<typename T>
        struct is_probably_branchless_comparison_impl<std::less<>, T>:
            std::is_arithmetic<T>
        {};

#ifdef __cpp_lib_ranges
        template<typename T>
        struct is_probably_branchless_comparison_impl<std::ranges::less, T>:
            std::is_arithmetic<T>
        {};
#endif

        template<typename T>
        struct is_probably_branchless_comparison_impl<std::less<T>, T>:
            std::is_arithmetic<T>
        {};

        template<typename T>
        struct is_probably_branchless_comparison_impl<std::greater<>, T>:
            std::is_arithmetic<T>
        {};

#ifdef __cpp_lib_ranges
        template<typename T>
        struct is_probably_branchless_comparison_impl<std::ranges::greater, T>:
            std::is_arithmetic<T>
        {};
#endif

        template<typename T>
        struct is_probably_branchless_comparison_impl<std::greater<T>, T>:
            std::is_arithmetic<T>
        {};
    }

    // Strip types from cv and reference qualifications if needed

    template<typename Compare, typename T>
    struct is_probably_branchless_comparison:
        cppsort::detail::conditional_t<
            cppsort::detail::disjunction<
                std::is_reference<Compare>,
                std::is_const<Compare>,
                std::is_volatile<Compare>,
                std::is_reference<T>,
                std::is_const<T>,
                std::is_volatile<T>
            >::value,
            is_probably_branchless_comparison<
                cppsort::detail::remove_cvref_t<Compare>,
                cppsort::detail::remove_cvref_t<T>
            >,
            detail::is_probably_branchless_comparison_impl<Compare, T>
        >
    {};

    template<typename Compare, typename T>
    constexpr bool is_probably_branchless_comparison_v
        = is_probably_branchless_comparison<Compare, T>::value;

    ////////////////////////////////////////////////////////////
    // Whether a projection function is likely branchless when
    // called with an instance of a specific type

    namespace detail
    {
        template<typename Projection, typename T>
        struct is_probably_branchless_projection_impl:
            std::is_member_object_pointer<Projection>
        {};

#if defined(__GLIBCXX__) || defined(_MSC_VER)
template<typename Class, typename T, typename U>
struct is_probably_branchless_projection_impl<std::_Mem_fn<T Class::*>, U>:
    std::is_member_object_pointer<T Class::*>
{};
#elif defined(_LIBCPP_VERSION)
        template<typename Class, typename T, typename U>
        struct is_probably_branchless_projection_impl<std::__mem_fn<T Class::*>, U>:
            std::is_member_object_pointer<T Class::*>
        {};
#endif

#if CPPSORT_STD_IDENTITY_AVAILABLE
        template<typename T>
        struct is_probably_branchless_projection_impl<std::identity, T>:
            std::true_type
        {};
#endif
    }

    // Strip types from cv and reference qualifications if needed

    template<typename Projection, typename T>
    struct is_probably_branchless_projection:
        cppsort::detail::conditional_t<
            cppsort::detail::disjunction<
                std::is_reference<Projection>,
                std::is_const<Projection>,
                std::is_volatile<Projection>,
                std::is_reference<T>,
                std::is_const<T>,
                std::is_volatile<T>
            >::value,
            is_probably_branchless_projection<
                cppsort::detail::remove_cvref_t<Projection>,
                cppsort::detail::remove_cvref_t<T>
            >,
            detail::is_probably_branchless_projection_impl<Projection, T>
        >
    {};

    template<typename Projection, typename T>
    constexpr bool is_probably_branchless_projection_v
        = is_probably_branchless_projection<Projection, T>::value;
}}

#endif // CPPSORT_UTILITY_BRANCHLESS_TRAITS_H_
