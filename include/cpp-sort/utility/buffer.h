/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2015 Morwenn
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
                explicit buffer(std::size_t /* size */) {}

                auto size() const
                    -> std::size_t
                {
                    return N;
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
                    -> decltype(_memory.begin())
                {
                    return _memory.begin();
                }

                auto begin() const
                    -> decltype(_memory.begin())
                {
                    return _memory.begin();
                }

                auto cbegin() const
                    -> decltype(_memory.cbegin())
                {
                    return _memory.cbegin();
                }

                auto end()
                    -> decltype(_memory.end())
                {
                    return _memory.end();
                }

                auto end() const
                    -> decltype(_memory.end())
                {
                    return _memory.end();
                }

                auto cend() const
                    -> decltype(_memory.cend())
                {
                    return _memory.cend();
                }

                auto data()
                    -> decltype(_memory.data())
                {
                    return _memory.data();
                }

                auto data() const
                    -> decltype(_memory.data())
                {
                    return _memory.data();
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

                auto data()
                    -> decltype(_memory.get())
                {
                    return _memory.get();
                }

                auto data() const
                    -> decltype(_memory.get())
                {
                    return _memory.get();
                }
        };
    };
}}

#endif // CPPSORT_UTILITY_BUFFER_H_
