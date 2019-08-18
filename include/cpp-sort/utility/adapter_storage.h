/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2018-2019 Morwenn
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
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
        explicit constexpr adapter_storage(Sorter) noexcept {}

        template<typename... Args>
        constexpr auto operator()(Args&&... args) const
            noexcept(noexcept(Sorter{}(std::forward<Args>(args)...)))
            -> decltype(Sorter{}(std::forward<Args>(args)...))
        {
            return Sorter{}(std::forward<Args>(args)...);
        }

        constexpr auto get() const
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
            noexcept(std::is_nothrow_copy_constructible<Sorter>::value):
            sorter(std::move(sorter))
        {}

        template<typename... Args>
        constexpr auto operator()(Args&&... args) const
            noexcept(noexcept(sorter(std::forward<Args>(args)...)))
            -> decltype(sorter(std::forward<Args>(args)...))
        {
            return sorter(std::forward<Args>(args)...);
        }

        template<typename... Args>
        constexpr auto operator()(Args&&... args)
            noexcept(noexcept(sorter(std::forward<Args>(args)...)))
            -> decltype(sorter(std::forward<Args>(args)...))
        {
            return sorter(std::forward<Args>(args)...);
        }

        constexpr auto get() &
            -> Sorter&
        {
            return static_cast<Sorter&>(sorter);
        }

        constexpr auto get() const&
            -> const Sorter&
        {
            return static_cast<const Sorter&>(sorter);
        }

        constexpr auto get() &&
            -> Sorter&&
        {
            return static_cast<Sorter&&>(sorter);
        }

        constexpr auto get() const&&
            -> const Sorter&&
        {
            return static_cast<const Sorter&&>(sorter);
        }
    };
}}

#endif // CPPSORT_UTILITY_ADAPTER_STORAGE_H_
