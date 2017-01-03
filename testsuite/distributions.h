/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2015-2017 Morwenn
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
#ifndef CPPSORT_TESTSUITE_DISTRIBUTIONS_H_
#define CPPSORT_TESTSUITE_DISTRIBUTIONS_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <algorithm>
#include <cstddef>
#include <iterator>
#include <numeric>
#include <random>
#include <vector>
#include <cpp-sort/utility/bitops.h>

#ifdef __MINGW32__
    // Poor seed for the pseudo-random number generation
#   include <ctime>
#endif

namespace dist
{
    template<typename Derived>
    struct distribution
    {
        template<typename OutputIterator>
        using fptr_t = void(*)(OutputIterator, std::size_t);

        template<typename OutputIterator>
        operator fptr_t<OutputIterator>() const
        {
            return [](OutputIterator out, std::size_t size) {
                return Derived{}(out, size);
            };
        }
    };

    struct shuffled:
        distribution<shuffled>
    {
        template<typename OutputIterator, typename T=long long int>
        auto operator()(OutputIterator out, long long int size, T start=T(0)) const
            -> void
        {
            // Pseudo-random number generator
#ifdef __MINGW32__
            thread_local std::mt19937 engine(std::time(nullptr));
#else
            thread_local std::mt19937 engine(std::random_device{}());
#endif

            std::vector<T> vec;
            vec.reserve(size);

            T end = start + size;
            for (auto i = start ; i < end ; ++i) {
                vec.emplace_back(i);
            }
            std::shuffle(std::begin(vec), std::end(vec), engine);
            std::move(std::begin(vec), std::end(vec), out);
        }
    };

    struct shuffled_16_values:
        distribution<shuffled_16_values>
    {
        template<typename OutputIterator>
        auto operator()(OutputIterator out, std::size_t size) const
            -> void
        {
            // Pseudo-random number generator
#ifdef __MINGW32__
            thread_local std::mt19937 engine(std::time(nullptr));
#else
            thread_local std::mt19937 engine(std::random_device{}());
#endif

            std::vector<int> vec;
            vec.reserve(size);

            for (std::size_t i = 0 ; i < size ; ++i) {
                vec.emplace_back(i % 16);
            }
            std::shuffle(std::begin(vec), std::end(vec), engine);
            std::move(std::begin(vec), std::end(vec), out);
        }
    };

    struct all_equal:
        distribution<all_equal>
    {
        template<typename OutputIterator>
        auto operator()(OutputIterator out, std::size_t size) const
            -> void
        {
            for (std::size_t i = 0 ; i < size ; ++i) {
                *out++ = 0;
            }
        }
    };

    struct ascending:
        distribution<ascending>
    {
        template<typename OutputIterator>
        auto operator()(OutputIterator out, std::size_t size) const
            -> void
        {
            for (std::size_t i = 0 ; i < size ; ++i) {
                *out++ = i;
            }
        }
    };

    struct descending:
        distribution<descending>
    {
        template<typename OutputIterator>
        auto operator()(OutputIterator out, std::size_t size) const
            -> void
        {
            while (size--) {
                *out++ = size;
            }
        }
    };

    struct pipe_organ:
        distribution<pipe_organ>
    {
        template<typename OutputIterator>
        auto operator()(OutputIterator out, std::size_t size) const
            -> void
        {
            for (std::size_t i = 0 ; i < size / 2 ; ++i) {
                *out++ = i;
            }
            for (std::size_t i = size / 2 ; i < size ; ++i) {
                *out++ = size - i;
            }
        }
    };

    struct push_front:
        distribution<push_front>
    {
        template<typename OutputIterator>
        auto operator()(OutputIterator out, std::size_t size) const
            -> void
        {
            if (size > 0) {
                for (std::size_t i = 0 ; i < size - 1 ; ++i) {
                    *out++ = i;
                }
                *out = 0;
            }
        }
    };

    struct push_middle:
        distribution<push_middle>
    {
        template<typename OutputIterator>
        auto operator()(OutputIterator out, std::size_t size) const
            -> void
        {
            if (size > 0) {
                for (std::size_t i = 0 ; i < size ; ++i) {
                    if (i != size / 2) {
                        *out++ = i;
                    }
                }
                *out = size / 2;
            }
        }
    };

    struct ascending_sawtooth:
        distribution<ascending_sawtooth>
    {
        template<typename OutputIterator>
        auto operator()(OutputIterator out, std::size_t size) const
            -> void
        {
            std::size_t limit = size / cppsort::utility::log2(size) * 1.1;
            for (std::size_t i = 0 ; i < size ; ++i) {
                *out++ = i % limit;
            }
        }
    };

    struct descending_sawtooth:
        distribution<descending_sawtooth>
    {
        template<typename OutputIterator>
        auto operator()(OutputIterator out, std::size_t size) const
            -> void
        {
            std::size_t limit = size / cppsort::utility::log2(size) * 1.1;
            while (size--) {
                *out++ = size % limit;
            }
        }
    };

    struct alternating:
        distribution<alternating>
    {
        template<typename OutputIterator>
        auto operator()(OutputIterator out, std::size_t size) const
            -> void
        {
            for (std::size_t i = 0 ; i < size ; ++i) {
                *out++ = (i % 2) ? i : -i;
            }
        }
    };

    struct alternating_16_values:
        distribution<alternating_16_values>
    {
        template<typename OutputIterator>
        auto operator()(OutputIterator out, std::size_t size) const
            -> void
        {
            for (std::size_t i = 0 ; i < size ; ++i) {
                *out++ = (i % 2) ? i % 16 : -(i % 16);
            }
        }
    };
}

#endif // CPPSORT_TESTSUITE_DISTRIBUTIONS_H_
