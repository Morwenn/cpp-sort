/*
 * Copyright (c) 2015-2022 Morwenn
 * SPDX-License-Identifier: MIT
 */
#ifndef CPPSORT_UTILITY_BUFFER_H_
#define CPPSORT_UTILITY_BUFFER_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <array>
#include <cstddef>
#include <memory>

namespace cppsort
{
namespace utility
{
    ////////////////////////////////////////////////////////////
    // Fixed-size buffer

    template<std::size_t N>
    struct fixed_buffer
    {
        template<typename T>
        class buffer
        {
            private:

                std::array<T, N> _memory;

            public:

                buffer() = default;
                explicit constexpr buffer(std::size_t /* size */) {}

                constexpr auto size() const
                    -> typename std::array<T, N>::size_type
                {
                    return N;
                }

                constexpr auto operator[](std::size_t pos)
                    -> decltype(_memory[pos])
                {
                    return _memory[pos];
                }

                constexpr auto operator[](std::size_t pos) const
                    -> decltype(_memory[pos])
                {
                    return _memory[pos];
                }

                constexpr auto begin()
                    -> decltype(_memory.data())
                {
                    return _memory.data();
                }

                constexpr auto begin() const
                    -> decltype(_memory.data())
                {
                    return _memory.data();
                }

                constexpr auto cbegin() const
                    -> decltype(_memory.data())
                {
                    return _memory.data();
                }

                constexpr auto end()
                    -> decltype(_memory.data() + _memory.size())
                {
                    return _memory.data() + _memory.size();
                }

                constexpr auto end() const
                    -> decltype(_memory.data() + _memory.size())
                {
                    return _memory.data() + _memory.size();
                }

                constexpr auto cend() const
                    -> decltype(_memory.data() + _memory.size())
                {
                    return _memory.data() + _memory.size();
                }
        };
    };

    ////////////////////////////////////////////////////////////
    // Dynamic buffer accepting a size policy

    namespace detail
    {
        // This class is used as a base class by dynamic_buffer::buffer
        // to reduce template bloat, notably by making sure that it isn't
        // instantiated for every different size policy

        template<typename T>
        class dynamic_buffer_impl
        {
            private:

                std::size_t _size;
                std::unique_ptr<T[]> _memory;

            public:

                explicit dynamic_buffer_impl(std::size_t size):
                    _size(size),
                    _memory(std::make_unique<T[]>(_size))
                {}

                auto size() const
                    -> std::size_t
                {
                    return _size;
                }

                auto operator[](std::size_t pos)
                    -> decltype(_memory[pos])
                {
                    return _memory[pos];
                }

                auto operator[](std::size_t pos) const
                    -> decltype(_memory[pos])
                {
                    return _memory[pos];
                }

                auto begin()
                    -> decltype(_memory.get())
                {
                    return _memory.get();
                }

                auto begin() const
                    -> decltype(_memory.get())
                {
                    return _memory.get();
                }

                auto cbegin() const
                    -> decltype(_memory.get())
                {
                    return _memory.get();
                }

                auto end()
                    -> decltype(_memory.get() + size())
                {
                    return _memory.get() + size();
                }

                auto end() const
                    -> decltype(_memory.get() + size())
                {
                    return _memory.get() + size();
                }

                auto cend() const
                    -> decltype(_memory.get() + size())
                {
                    return _memory.get() + size();
                }
        };
    }

    template<typename SizePolicy>
    struct dynamic_buffer
    {
        template<typename T>
        struct buffer:
            detail::dynamic_buffer_impl<T>
        {
            explicit buffer(std::size_t size):
                detail::dynamic_buffer_impl<T>(
                    static_cast<std::size_t>(SizePolicy{}(size))
                )
            {}
        };
    };
}}

#endif // CPPSORT_UTILITY_BUFFER_H_
