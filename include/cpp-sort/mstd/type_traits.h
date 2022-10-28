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

#if defined(__SIZEOF_INT128__) && !defined(_LIBCPP_VERSION)
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
#else
    using std::is_integral;
    using std::is_signed;
    using std::is_unsigned;
    using std::is_integral_v;
    using std::is_signed_v;
    using std::is_unsigned_v;
#endif
}

#endif // CPPSORT_MSTD_TYPE_TRAITS_H_
