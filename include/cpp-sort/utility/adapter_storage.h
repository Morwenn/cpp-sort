/*
 * Copyright (c) 2018-2024 Morwenn
 * SPDX-License-Identifier: MIT
 */
#ifndef CPPSORT_UTILITY_ADAPTER_STORAGE_H_
#define CPPSORT_UTILITY_ADAPTER_STORAGE_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <type_traits>
#include <utility>
#include "../detail/type_traits.h"

namespace cppsort::utility
{
    // Storage for adapters that adapt only one sorter at a time:
    // it contains nothing if the sorter is empty and
    // default-constructible, and a copy of the sorter passed at
    // construction time otherwise
    //
    // It can be called like the sorter it wraps with operator(),
    // which should be hidden by the adapter's own operator()
    // despite inheritance

    template<
        typename Sorter,
        bool = std::is_empty_v<Sorter> && std::is_default_constructible_v<Sorter>
    >
    struct adapter_storage;

    template<typename Sorter>
    struct adapter_storage<Sorter, true>
    {
        adapter_storage() = default;
        explicit constexpr adapter_storage(const Sorter&) noexcept {}

        template<typename... Args>
        static constexpr auto operator()(Args&&... args)
            noexcept(std::is_nothrow_default_constructible_v<Sorter> &&
                     std::is_nothrow_invocable_v<Sorter, Args...>)
            -> decltype(Sorter{}(std::forward<Args>(args)...))
        {
            return Sorter{}(std::forward<Args>(args)...);
        }

        static constexpr auto get()
            noexcept(std::is_nothrow_default_constructible_v<Sorter>)
            -> Sorter
        {
            return Sorter{};
        }
    };

    template<typename Sorter>
    struct adapter_storage<Sorter, false>
    {
        [[no_unique_address]] Sorter sorter;

        adapter_storage() = default;

        explicit constexpr adapter_storage(const Sorter& sorter)
            noexcept(std::is_nothrow_copy_constructible_v<Sorter>):
            sorter(sorter)
        {}

        explicit constexpr adapter_storage(Sorter&& sorter)
            noexcept(std::is_nothrow_move_constructible_v<Sorter>):
            sorter(std::move(sorter))
        {}

        template<typename Self, typename... Args>
        constexpr auto operator()(this Self&& self, Args&&... args)
            noexcept(std::is_nothrow_invocable_v<Sorter, Args...>)
            -> decltype(std::forward<Self>(self).sorter(std::forward<Args>(args)...))
        {
            return std::forward<Self>(self).sorter(std::forward<Args>(args)...);
        }

        template<typename Self>
        constexpr auto get(this Self&& self) noexcept
            -> cppsort::detail::copy_cvref_t<Self, Sorter>
        {
            return static_cast<
                cppsort::detail::copy_cvref_t<Self, adapter_storage<Sorter, false>>
            >(self).sorter;
        }
    };
}

#endif // CPPSORT_UTILITY_ADAPTER_STORAGE_H_
