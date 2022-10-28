/*
 * Copyright (c) 2015-2022 Morwenn
 * SPDX-License-Identifier: MIT
 */
#ifndef CPPSORT_MSTD_TYPE_TRAITS_H_
#define CPPSORT_MSTD_TYPE_TRAITS_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <type_traits>
#include "../detail/config.h"

namespace cppsort::mstd
{
    ////////////////////////////////////////////////////////////
    // Alternative to std::conditional_t

    namespace detail
    {
        template<bool>
        struct conditional_impl
        {
            template<typename T, typename U>
            using type = T;
        };

        template<>
        struct conditional_impl<false>
        {
            template<typename T, typename U>
            using type = U;
        };
    }

    template<bool B, typename T, typename U>
    using conditional_t = typename detail::conditional_impl<B>::template type<T, U>;

    ////////////////////////////////////////////////////////////
    // Alternative to std::enable_if_t

    namespace detail
    {
        template<bool>
        struct enable_if_impl {};

        template<>
        struct enable_if_impl<true>
        {
            template<typename T>
            using type = T;
        };
    }

    template<bool B, typename T=void>
    using enable_if_t = typename detail::enable_if_impl<B>::template type<T>;

    ////////////////////////////////////////////////////////////
    // standard library isn't instrumented but the type is still
    // available:
    // * libstdc++ is instrumented in gnu++ mode only
    // * libc++ is always instrumented
    // * Microsoft STL is never instrumented

#if CPPSORT_INT128_REQUIRES_LIBRARY_SUPPORT
    namespace detail
    {
        template<typename T>
        struct is_integral_impl:
            std::is_integral<T>::type
        {};

        template<>
        struct is_integral_impl<__int128_t>:
            std::true_type
        {};

        template<>
        struct is_integral_impl<__uint128_t>:
            std::true_type
        {};

        template<typename T>
        struct is_signed_impl:
            std::is_signed<T>::type
        {};

        template<>
        struct is_signed_impl<__int128_t>:
            std::true_type
        {};

        template<typename T>
        struct is_unsigned_impl:
            std::is_unsigned<T>::type
        {};

        template<>
        struct is_unsigned_impl<__uint128_t>:
            std::true_type
        {};
    }

    ////////////////////////////////////////////////////////////
    // Type properties

    template<typename T>
    struct is_integral:
        detail::is_integral_impl<std::remove_cv_t<T>>
    {};

    template<typename T>
    struct is_signed:
        detail::is_signed_impl<std::remove_cv_t<T>>
    {};

    template<typename T>
    struct is_unsigned:
        detail::is_unsigned_impl<std::remove_cv_t<T>>
    {};

    template<typename T>
    inline constexpr bool is_integral_v = is_integral<T>::value;

    template<typename T>
    inline constexpr bool is_signed_v = is_signed<T>::value;

    template<typename T>
    inline constexpr bool is_unsigned_v = is_unsigned<T>::value;

    ////////////////////////////////////////////////////////////
    // Type modifications

    template<typename T>
    struct make_signed:
        mstd::conditional_t<
            mstd::is_signed_v<T>,
            std::type_identity<T>, // Handles __int128_t
            std::make_signed<T>
        >
    {};

    template<>
    struct make_signed<__uint128_t>
    {
        using type = __int128_t;
    };

    template<>
    struct make_signed<const __uint128_t>
    {
        using type = const __int128_t;
    };

    template<>
    struct make_signed<volatile __uint128_t>
    {
        using type = volatile __int128_t;
    };

    template<>
    struct make_signed<const volatile __uint128_t>
    {
        using type = const volatile __int128_t;
    };

    template<typename T>
    struct make_unsigned:
        mstd::conditional_t<
            mstd::is_unsigned_v<T>,
            std::type_identity<T>, // Handles __uint128_t
            std::make_unsigned<T>
        >
    {};

    template<>
    struct make_unsigned<__int128_t>
    {
        using type = __uint128_t;
    };

    template<>
    struct make_unsigned<const __int128_t>
    {
        using type = const __uint128_t;
    };

    template<>
    struct make_unsigned<volatile __int128_t>
    {
        using type = volatile __uint128_t;
    };

    template<>
    struct make_unsigned<const volatile __int128_t>
    {
        using type = const volatile __uint128_t;
    };

    template<typename T>
    using make_signed_t = typename make_signed<T>::type;

    template<typename T>
    using make_unsigned_t = typename make_unsigned<T>::type;
#else
    // Type properties
    using std::is_integral;
    using std::is_signed;
    using std::is_unsigned;
    using std::is_integral_v;
    using std::is_signed_v;
    using std::is_unsigned_v;
    // Type modifications
    using std::make_signed;
    using std::make_unsigned;
    using std::make_signed_t;
    using std::make_unsigned_t;
#endif
}

#endif // CPPSORT_MSTD_TYPE_TRAITS_H_
