/*
 * Copyright (c) 2015-2021 Morwenn
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

#if defined(_MSC_VER) || (defined(_LIBCPP_VERSION) && _LIBCPP_VERSION < 7000)
    template<>
    struct fixed_buffer<0>
    {
        // Not sure how to handle this one, but buffered sorters should
        // tolerate 0-sized buffers and do nothing with them; we take a
        // somewhat hazardous route and make some things rely on a 1-sized
        // std::array

        template<typename T>
        struct buffer
        {
            buffer() = default;
            explicit constexpr buffer(std::size_t /* size */) {}

            constexpr auto size() const
                -> typename std::array<T, 1>::size_type
            {
                return 0;
            }

            constexpr auto operator[](std::size_t pos)
                -> typename std::array<T, 1>::reference
            {
                // Should never be called
                return begin()[pos];
            }

            constexpr auto operator[](std::size_t pos) const
                -> typename std::array<T, 1>::const_reference
            {
                // Should never be called
                return begin()[pos];
            }

            constexpr auto begin()
                -> typename std::array<T, 1>::pointer
            {
                return nullptr;
            }

            constexpr auto begin() const
                -> typename std::array<T, 1>::const_pointer
            {
                return nullptr;
            }

            constexpr auto cbegin() const
                -> typename std::array<T, 1>::const_pointer
            {
                return nullptr;
            }

            constexpr auto end()
                -> typename std::array<T, 1>::pointer
            {
                return nullptr;
            }

            constexpr auto end() const
                -> typename std::array<T, 1>::const_pointer
            {
                return nullptr;
            }

            constexpr auto cend() const
                -> typename std::array<T, 1>::const_pointer
            {
                return nullptr;
            }
        };
    };
#endif

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
