/*
 * Copyright (c) 2023 Morwenn
 * SPDX-License-Identifier: MIT
 */
#ifndef CPPSORT_UTILITY_METRICS_TOOLS_H_
#define CPPSORT_UTILITY_METRICS_TOOLS_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <ostream>
#include <tuple>
#include <type_traits>
#include <utility>
#include "../detail/type_traits.h"

namespace cppsort
{
namespace utility
{
    ////////////////////////////////////////////////////////////
    // metric<T, Tag>
    //
    // metric is a simple class holding a value of a given type
    // and associating it to a tag.

    template<typename T, typename Tag>
    class metric
    {
        private:

            T _value;

        public:

            ////////////////////////////////////////////////////////////
            // Construction

            metric() = default;
            metric(const metric&) = default;
            metric(metric&&) = default;

            constexpr explicit metric(const T& value)
                noexcept(std::is_nothrow_copy_constructible<T>::value):
                _value(value)
            {}

            constexpr explicit metric(T&& value)
                noexcept(std::is_nothrow_move_constructible<T>::value):
                _value(std::move(value))
            {}

            ////////////////////////////////////////////////////////////
            // Accessors

            constexpr explicit operator T() const
            {
                return _value;
            }

            constexpr auto value() const
                -> T
            {
                return _value;
            }

            ////////////////////////////////////////////////////////////
            // Assignment

            metric& operator=(const metric&) = default;
            metric& operator=(metric&&) = default;

            constexpr auto operator=(const T& other)
                noexcept(std::is_nothrow_copy_assignable<T>::value)
                -> metric&
            {
                _value = other;
                return *this;
            }

            constexpr auto operator=(T&& other)
                noexcept(std::is_nothrow_move_assignable<T>::value)
                -> metric&
            {
                _value = std::move(other);
                return *this;
            }

            template<typename U>
            constexpr auto operator=(const metric<U, Tag>& other)
                noexcept(std::is_nothrow_assignable<T&, const U&>::value)
                -> metric&
            {
                _value = other._value;
                return *this;
            }

            template<typename U>
            constexpr auto operator=(metric<U, Tag>&& other)
                noexcept(std::is_nothrow_assignable<T&, U>::value)
                -> metric&
            {
                _value = std::move(other._value);
                return *this;
            }

            ////////////////////////////////////////////////////////////
            // Comparison operators

            template<typename U>
            friend constexpr auto operator==(const metric& lhs, const metric<U, Tag>& rhs)
                -> bool
            {
                return lhs._value == rhs.value();
            }

            friend constexpr auto operator==(const metric& lhs, const T& rhs)
                -> bool
            {
                return lhs._value == rhs;
            }

            friend constexpr auto operator==(const T& lhs, const metric& rhs)
                -> bool
            {
                return lhs == rhs._value;
            }

            template<typename U>
            friend constexpr auto operator!=(const metric& lhs, const metric<U, Tag>& rhs)
                -> bool
            {
                return lhs._value != rhs.value();
            }

            friend constexpr auto operator!=(const metric& lhs, const T& rhs)
                -> bool
            {
                return lhs._value != rhs;
            }

            friend constexpr auto operator!=(const T& lhs, const metric& rhs)
                -> bool
            {
                return lhs != rhs._value;
            }

            ////////////////////////////////////////////////////////////
            // Relational operators

            template<typename U>
            friend constexpr auto operator<(const metric& lhs, const metric<U, Tag>& rhs)
                -> bool
            {
                return lhs._value < rhs.value();
            }

            friend constexpr auto operator<(const metric& lhs, const T& rhs)
                -> bool
            {
                return lhs._value < rhs;
            }

            friend constexpr auto operator<(const T& lhs, const metric& rhs)
                -> bool
            {
                return lhs < rhs._value;
            }

            template<typename U>
            friend constexpr auto operator<=(const metric& lhs, const metric<U, Tag>& rhs)
                -> bool
            {
                return lhs._value <= rhs.value();
            }

            friend constexpr auto operator<=(const metric& lhs, const T& rhs)
                -> bool
            {
                return lhs._value <= rhs;
            }

            friend constexpr auto operator<=(const T& lhs, const metric& rhs)
                -> bool
            {
                return lhs <= rhs._value;
            }

            template<typename U>
            friend constexpr auto operator>(const metric& lhs, const metric<U, Tag>& rhs)
                -> bool
            {
                return lhs._value > rhs.value();
            }

            friend constexpr auto operator>(const metric& lhs, const T& rhs)
                -> bool
            {
                return lhs._value > rhs;
            }

            friend constexpr auto operator>(const T& lhs, const metric& rhs)
                -> bool
            {
                return lhs > rhs._value;
            }

            template<typename U>
            friend constexpr auto operator>=(const metric& lhs, const metric<U, Tag>& rhs)
                -> bool
            {
                return lhs._value >= rhs.value();
            }

            friend constexpr auto operator>=(const metric& lhs, const T& rhs)
                -> bool
            {
                return lhs._value >= rhs;
            }

            friend constexpr auto operator>=(const T& lhs, const metric& rhs)
                -> bool
            {
                return lhs >= rhs._value;
            }

            ////////////////////////////////////////////////////////////
            // Stream operators

            template<typename U=T>
            friend auto operator<<(std::ostream& stream, const metric& met)
                -> decltype(stream << std::declval<U&>())
            {
                stream << met.value();
                return stream;
            }
    };

    ////////////////////////////////////////////////////////////
    // metrics<metric<T1, Tag1>, metric<T2, Tag2>...>

    template<typename... Args>
    class metrics;

    template<typename... TT, typename... Tags>
    class metrics<metric<TT, Tags>...>
    {
        private:

            std::tuple<metric<TT, Tags>...> metrics_;

        public:

            ////////////////////////////////////////////////////////////
            // Construction

            metrics() = default;
            metrics(const metrics&) = default;
            metrics(metrics&&) = default;

            template<typename... Args>
            constexpr explicit metrics(Args&&... args):
                metrics_(std::forward<Args>(args)...)
            {}

            ////////////////////////////////////////////////////////////
            // operator=

            metrics& operator=(const metrics&) = default;
            metrics& operator=(metrics&&) = default;

            ////////////////////////////////////////////////////////////
            // Index-based get()

            template<std::size_t Idx>
            friend constexpr auto get(metrics& mm)
                -> std::tuple_element_t<Idx, std::tuple<metric<TT, Tags>...>>&
            {
                return std::get<Idx>(mm.metrics_);
            }

            template<std::size_t Idx>
            friend constexpr auto get(const metrics& mm)
                -> const std::tuple_element_t<Idx, std::tuple<metric<TT, Tags>...>>&
            {
                return std::get<Idx>(mm.metrics_);
            }

            template<std::size_t Idx>
            friend constexpr auto get(metrics&& mm)
                -> std::tuple_element_t<Idx, std::tuple<metric<TT, Tags>...>>&&
            {
                return std::get<Idx>(std::move(mm).metrics_);
            }

            template<std::size_t Idx>
            friend constexpr auto get(const metrics&& mm)
                -> const std::tuple_element_t<Idx, std::tuple<metric<TT, Tags>...>>&&
            {
                return std::get<Idx>(std::move(mm).metrics_);
            }

            ////////////////////////////////////////////////////////////
            // Tag-based get()

            template<typename Tag, std::size_t Idx=cppsort::detail::index_of<Tag, Tags...>>
            friend constexpr auto get(metrics& mm)
                -> std::tuple_element_t<Idx, std::tuple<metric<TT, Tags>...>>&
            {
                return std::get<Idx>(mm.metrics_);
            }

            template<typename Tag, std::size_t Idx=cppsort::detail::index_of<Tag, Tags...>>
            friend constexpr auto get(const metrics& mm)
                -> const std::tuple_element_t<Idx, std::tuple<metric<TT, Tags>...>>&
            {
                return std::get<Idx>(mm.metrics_);
            }

            template<typename Tag, std::size_t Idx=cppsort::detail::index_of<Tag, Tags...>>
            friend constexpr auto get(metrics&& mm)
                -> std::tuple_element_t<Idx, std::tuple<metric<TT, Tags>...>>&&
            {
                return std::get<Idx>(std::move(mm).metrics_);
            }

            template<typename Tag, std::size_t Idx=cppsort::detail::index_of<Tag, Tags...>>
            friend constexpr auto get(const metrics&& mm)
                -> const std::tuple_element_t<Idx, std::tuple<metric<TT, Tags>...>>&&
            {
                return std::get<Idx>(std::move(mm).metrics_);
            }
    };
}}

#endif // CPPSORT_UTILITY_METRICS_TOOLS_H_
