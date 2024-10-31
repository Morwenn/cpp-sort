/*
 * Copyright (c) 2024 Morwenn
 * SPDX-License-Identifier: MIT
 */

//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef CPPSORT_DETAIL_INVOKE_RESULT_H_
#define CPPSORT_DETAIL_INVOKE_RESULT_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <type_traits>
#include <utility>

namespace cppsort
{
namespace detail
{
namespace libcxx
{
    template<typename DecayedFp>
    struct member_pointer_class_type {};

    template<typename Ret, typename ClassType>
    struct member_pointer_class_type<Ret ClassType::*>
    {
        using type = ClassType;
    };

    template<
        typename Fp,
        typename A0,
        typename DecayFp = std::decay_t<Fp>,
        typename DecayA0 = std::decay_t<A0>,
        typename ClassT = typename member_pointer_class_type<DecayFp>::type
    >
    using enable_if_bullet1 = std::enable_if_t<
        std::is_member_function_pointer<DecayFp>::value &&
        (std::is_same<ClassT, DecayA0>::value || std::is_base_of<ClassT, DecayA0>::value)
    >;

    template<
        typename Fp,
        typename A0,
        typename DecayFp = std::decay_t<Fp>,
        typename DecayA0 = std::decay_t<A0>
    >
    using enable_if_bullet2 = std::enable_if_t<
        std::is_member_function_pointer<DecayFp>::value &&
        std::__is_reference_wrapper<DecayA0>::value
    >;

    template<
        typename Fp,
        typename A0,
        typename DecayFp = std::decay_t<Fp>,
        typename DecayA0 = std::decay_t<A0>,
        typename ClassT = typename member_pointer_class_type<DecayFp>::type
    >
    using enable_if_bullet3 = std::enable_if_t<
        std::is_member_function_pointer<DecayFp>::value &&
        not (std::is_same<ClassT, DecayA0>::value || std::is_base_of<ClassT, DecayA0>::value) &&
        not std::__is_reference_wrapper<DecayA0>::value
    >;

    template<
        typename Fp,
        typename A0,
        typename DecayFp = std::decay_t<Fp>,
        typename DecayA0 = std::decay_t<A0>,
        typename ClassT = typename member_pointer_class_type<DecayFp>::type
    >
    using enable_if_bullet4 = std::enable_if_t<
        std::is_member_object_pointer<DecayFp>::value &&
        (std::is_same<ClassT, DecayA0>::value || std::is_base_of<ClassT, DecayA0>::value)
    >;

    template<
        typename Fp,
        typename A0,
        typename DecayFp = std::decay_t<Fp>,
        typename DecayA0 = std::decay_t<A0>
    >
    using enable_if_bullet5 = std::enable_if_t<
        std::is_member_object_pointer<DecayFp>::value &&
        std::__is_reference_wrapper<DecayA0>::value
    >;

    template<
        typename Fp,
        typename A0,
        typename DecayFp = std::decay_t<Fp>,
        typename DecayA0 = std::decay_t<A0>,
        typename ClassT = typename member_pointer_class_type<DecayFp>::type
    >
    using enable_if_bullet6 = std::enable_if_t<
        std::is_member_object_pointer<DecayFp>::value &&
        not (std::is_same<ClassT, DecayA0>::value || std::is_base_of<ClassT, DecayA0>::value) &&
        not std::__is_reference_wrapper<DecayA0>::value
    >;

    // do_invoke forward declarations

    // fall back - none of the bullets

    template<typename... Args>
    std::__nat do_invoke(Args&&... args);

    // bullets 1, 2 and 3

    template<
        typename Fp,
        typename A0,
        typename... Args,
        typename = enable_if_bullet1<Fp, A0>
    >
    constexpr auto do_invoke(Fp&& f, A0&& a0, Args&&... args)
        -> decltype((std::declval<A0>().*std::declval<Fp>())(std::declval<Args>()...))
    {
        return (static_cast<A0&&>(a0).*f)(static_cast<Args&&>(args)...);
    }

    template<
        typename Fp,
        typename A0,
        typename... Args,
        typename = enable_if_bullet2<Fp, A0>
    >
    constexpr auto do_invoke(Fp&& f, A0&& a0, Args&&... args)
        -> decltype((std::declval<A0>().get().*std::declval<Fp>())(std::declval<Args>()...))
    {
        return (a0.get().*f)(static_cast<Args&&>(args)...);
    }

    template<
        typename Fp,
        typename A0,
        typename... Args,
        typename = enable_if_bullet3<Fp, A0>
    >
    constexpr auto do_invoke(Fp&& f, A0&& a0, Args&&... args)
        -> decltype(((*std::declval<A0>()).*std::declval<Fp>())(std::declval<Args>()...))
    {
        return ((*static_cast<A0&&>(a0)).*f)(static_cast<Args&&>(args)...);
    }

    // bullets 4, 5 and 6

    template<
        typename Fp,
        typename A0,
        typename = enable_if_bullet4<Fp, A0>
    >
    constexpr auto do_invoke(Fp&& f, A0&& a0)
        -> decltype(std::declval<A0>().*std::declval<Fp>())
    {
        return static_cast<A0&&>(a0).*f;
    }

    template<
        typename Fp,
        typename A0,
        typename = enable_if_bullet5<Fp, A0>
    >
    constexpr auto do_invoke(Fp&& f, A0&& a0)
        -> decltype(std::declval<A0>().get().*std::declval<Fp>())
    {
        return a0.get().*f;
    }

    template<
        typename Fp,
        typename A0,
        typename = enable_if_bullet6<Fp, A0>
    >
    constexpr auto do_invoke(Fp&& f, A0&& a0)
        -> decltype((*std::declval<A0>()).*std::declval<Fp>())
    {
        return (*static_cast<A0&&>(a0)).*f;
    }

    // bullet 7

    template<typename Fp, typename... Args>
    constexpr auto do_invoke(Fp&& f, Args&&... args)
        -> decltype(std::declval<Fp>()(std::declval<Args>()...))
    {
        return static_cast<Fp&&>(f)(static_cast<Args&&>(args)...);
    }

    // invokable

    template<typename Ret, typename Fp, typename... Args>
    struct invokable_r
    {
        template<typename XFp, typename... XArgs>
        static auto cppsort_try_call(int)
            -> decltype(libcxx::do_invoke(std::declval<XFp>(), std::declval<XArgs>()...));

        template<typename XFp, typename... XArgs>
        static auto cppsort_try_call(...)
            -> std::__nat;

        // FIXME: Check that Ret, Fp, and Args... are all complete types, cv void,
        // or incomplete array types as required by the standard.
        using Result = decltype(cppsort_try_call<Fp, Args...>(0));

        using type = std::conditional_t<
            not std::is_same<Result, std::__nat>::value,
            std::conditional_t<
                std::is_void<Ret>::value,
                std::true_type,
                std::__is_core_convertible<Result, Ret>
            >,
            std::false_type
        >;
        static constexpr bool value = type::value;
    };

    template<typename Fp, typename... Args>
    using invokable = invokable_r<void, Fp, Args...>;

    template<
        bool IsInvokable,
        bool IsCVVoid,
        typename Ret,
        typename Fp,
        typename... Args
    >
    struct nothrow_invokable_r_imp
    {
        static constexpr bool value = false;
    };

    template<typename Ret, typename Fp, typename... Args>
    struct nothrow_invokable_r_imp<true, false, Ret, Fp, Args...>
    {
        using ThisT = nothrow_invokable_r_imp;

        template<typename _Tp>
        static auto test_noexcept(_Tp) noexcept
            -> void;

        static constexpr bool value =
            noexcept(ThisT::test_noexcept<Ret>(libcxx::do_invoke(std::declval<Fp>(), std::declval<Args>()...)));
    };

    template<typename Ret, typename Fp, typename... Args>
    struct nothrow_invokable_r_imp<true, true, Ret, Fp, Args...>
    {
        static constexpr bool value = noexcept(libcxx::do_invoke(std::declval<Fp>(), std::declval<Args>()...));
    };

    template<typename Ret, typename Fp, typename... Args>
    using nothrow_invokable_r =
        nothrow_invokable_r_imp<
            invokable_r<Ret, Fp, Args...>::value,
            std::is_void<Ret>::value,
            Ret, Fp, Args...
        >;

    template<typename Fp, typename... Args>
    using nothrow_invokable = nothrow_invokable_r_imp<
        invokable<Fp, Args...>::value,
        true, void, Fp, Args...
    >;

    template<typename Fp, typename... Args>
    struct do_invoke_of:
        std::enable_if<
            invokable<Fp, Args...>::value,
            typename invokable_r<void, Fp, Args...>::Result
        >
    {};

    // is_invocable

    template<typename Fn, typename... Args>
    struct is_invocable:
        std::integral_constant<bool, invokable<Fn, Args...>::value>
    {};

    template<typename Ret, typename Fn, typename... Args>
    struct is_invocable_r:
        std::integral_constant<bool, invokable_r<Ret, Fn, Args...>::value>
    {};

    template<typename Fn, typename... Args>
    constexpr bool is_invocable_v = is_invocable<Fn, Args...>::value;

    template<typename Ret, typename Fn, typename... Args>
    constexpr bool is_invocable_r_v = is_invocable_r<Ret, Fn, Args...>::value;

    // is_nothrow_invocable

    template<typename Fn, typename... Args>
    struct is_nothrow_invocable:
        std::integral_constant<bool, nothrow_invokable<Fn, Args...>::value>
    {};

    template<typename Ret, typename Fn, typename... Args>
    struct is_nothrow_invocable_r:
        std::integral_constant<bool, nothrow_invokable_r<Ret, Fn, Args...>::value>
    {};

    template<typename Fn, typename... Args>
    constexpr bool is_nothrow_invocable_v = is_nothrow_invocable<Fn, Args...>::value;

    template<typename Ret, typename Fn, typename... Args>
    constexpr bool is_nothrow_invocable_r_v = is_nothrow_invocable_r<Ret, Fn, Args...>::value;

    template<typename Fn, typename... Args>
    struct invoke_result:
        do_invoke_of<Fn, Args...>
    {};

    template<typename Fn, typename... Args>
    using invoke_result_t = typename invoke_result<Fn, Args...>::type;
}}}

#endif // CPPSORT_DETAIL_INVOKE_RESULT_H_
