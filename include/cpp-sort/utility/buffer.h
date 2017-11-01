/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2015-2016 Morwenn
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
                    -> decltype(_memory.begin())
                {
                    return _memory.begin();
                }

                constexpr auto begin() const
                    -> decltype(_memory.begin())
                {
                    return _memory.begin();
                }

                constexpr auto cbegin() const
                    -> decltype(_memory.cbegin())
                {
                    return _memory.cbegin();
                }

                constexpr auto end()
                    -> decltype(_memory.end())
                {
                    return _memory.end();
                }

                constexpr auto end() const
                    -> decltype(_memory.end())
                {
                    return _memory.end();
                }

                constexpr auto cend() const
                    -> decltype(_memory.cend())
                {
                    return _memory.cend();
                }
        };
    };

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

    ////////////////////////////////////////////////////////////
    // Dynamic buffer accepting a size policy

    template<typename SizePolicy>
    struct dynamic_buffer
    {
        template<typename T>
        class buffer
        {
            private:

                std::size_t _size;
                std::unique_ptr<T[]> _memory;

            public:

                explicit buffer(std::size_t size):
                    _size(SizePolicy{}(size)),
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
    };
}}

#endif // CPPSORT_UTILITY_BUFFER_H_
