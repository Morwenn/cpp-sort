/*
 * Copyright (c) 2018-2021 Morwenn
 * SPDX-License-Identifier: MIT
 */
#ifndef CPPSORT_UTILITY_ADAPTER_STORAGE_H_
#define CPPSORT_UTILITY_ADAPTER_STORAGE_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <type_traits>
#include <utility>

namespace cppsort
{
namespace utility
{
    //
    // Storage for adapters that adapt only one sorter at a
    // time: it will contain nothing if the sorter is empty
    // and default-constructible, and a copy of the sorter
    // passed at construction time otherwise
    //
    // It can be called like the sorter it wraps with operator(),
    // which should be hidden by the adapter's own operator()
    // despite inheritance
    //

    template<
        typename Sorter,
        bool = std::is_empty<Sorter>::value && std::is_default_constructible<Sorter>::value
    >
    struct adapter_storage;

    template<typename Sorter>
    struct adapter_storage<Sorter, true>
    {
        adapter_storage() = default;
        explicit constexpr adapter_storage(const Sorter&) noexcept {}

        template<typename... Args>
        constexpr auto operator()(Args&&... args) const
#ifdef __cpp_lib_is_invocable
            noexcept(std::is_nothrow_default_constructible_v<Sorter> &&
                     std::is_nothrow_invocable_v<Sorter, Args...>)
#else
            noexcept(noexcept(Sorter{}(std::forward<Args>(args)...)))
#endif
            -> decltype(Sorter{}(std::forward<Args>(args)...))
        {
            return Sorter{}(std::forward<Args>(args)...);
        }

        constexpr auto get() const
            noexcept(std::is_nothrow_default_constructible<Sorter>::value)
            -> Sorter
        {
            return Sorter{};
        }
    };

    template<typename Sorter>
    struct adapter_storage<Sorter, false>
    {
        Sorter sorter;

        adapter_storage() = default;

        explicit constexpr adapter_storage(const Sorter& sorter)
            noexcept(std::is_nothrow_copy_constructible<Sorter>::value):
            sorter(sorter)
        {}

        explicit constexpr adapter_storage(Sorter&& sorter)
            noexcept(std::is_nothrow_move_constructible<Sorter>::value):
            sorter(std::move(sorter))
        {}

        template<typename... Args>
        constexpr auto operator()(Args&&... args) const
#ifdef __cpp_lib_is_invocable
            noexcept(std::is_nothrow_invocable_v<Sorter, Args...>)
#else
            noexcept(noexcept(sorter(std::forward<Args>(args)...)))
#endif
            -> decltype(sorter(std::forward<Args>(args)...))
        {
            return sorter(std::forward<Args>(args)...);
        }

        template<typename... Args>
        constexpr auto operator()(Args&&... args)
#ifdef __cpp_lib_is_invocable
            noexcept(std::is_nothrow_invocable_v<Sorter, Args...>)
#else
            noexcept(noexcept(sorter(std::forward<Args>(args)...)))
#endif
            -> decltype(sorter(std::forward<Args>(args)...))
        {
            return sorter(std::forward<Args>(args)...);
        }

        constexpr auto get() & noexcept
            -> Sorter&
        {
            return static_cast<Sorter&>(sorter);
        }

        constexpr auto get() const& noexcept
            -> const Sorter&
        {
            return static_cast<const Sorter&>(sorter);
        }

        constexpr auto get() && noexcept
            -> Sorter&&
        {
            return static_cast<Sorter&&>(sorter);
        }

        constexpr auto get() const&& noexcept
            -> const Sorter&&
        {
            return static_cast<const Sorter&&>(sorter);
        }
    };
}}

#endif // CPPSORT_UTILITY_ADAPTER_STORAGE_H_
