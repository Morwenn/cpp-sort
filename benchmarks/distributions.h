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
#include <algorithm>
#include <ctime>
#include <iterator>
#include <numeric>
#include <random>
#include <cpp-sort/utility/bitops.h>

////////////////////////////////////////////////////////////
// Distributions for benchmarks
//
// Distributions are function objects used to fill the
// collections to be sorter before benchmarking the sorting
// algorithms. These distributions allow to check how a
// sorting algorithm behaves with common patterns found in
// actual data sets.
//

template<typename Derived>
struct distribution
{
    template<typename Iterator>
    using fptr_t = void(*)(Iterator, Iterator);

    template<typename Iterator>
    operator fptr_t<Iterator>() const
    {
        return [](Iterator first, Iterator last)
        {
            return Derived{}(first, last);
        };
    }
};

struct shuffled:
    distribution<shuffled>
{
    template<typename RandomAccessIterator>
    auto operator()(RandomAccessIterator begin, RandomAccessIterator end) const
        -> void
    {
        // Pseudo-random number generator
        thread_local std::mt19937_64 engine(std::time(nullptr));

        std::iota(begin, end, 0);
        std::shuffle(begin, end, engine);
    }

    static constexpr const char* output = "shuffled.txt";
};

struct shuffled_16_values:
    distribution<shuffled_16_values>
{
    template<typename RandomAccessIterator>
    auto operator()(RandomAccessIterator begin, RandomAccessIterator end) const
        -> void
    {
        // Pseudo-random number generator
        thread_local std::mt19937_64 engine(std::time(nullptr));

        std::size_t count = 0;
        for (RandomAccessIterator it = begin ; it != end ; ++it, ++count)
        {
            *it = count % 16;
        }
        std::shuffle(begin, end, engine);
    }

    static constexpr const char* output = "shuffled.txt";
};

struct all_equal:
    distribution<all_equal>
{
    template<typename RandomAccessIterator>
    auto operator()(RandomAccessIterator begin, RandomAccessIterator end) const
        -> void
    {
        std::fill(begin, end, 0);
    }

    static constexpr const char* output = "all_equal.txt";
};

struct ascending:
    distribution<ascending>
{
    template<typename RandomAccessIterator>
    auto operator()(RandomAccessIterator begin, RandomAccessIterator end) const
        -> void
    {
        std::iota(begin, end, 0);
    }

    static constexpr const char* output = "ascending.txt";
};

struct descending:
    distribution<descending>
{
    template<typename RandomAccessIterator>
    auto operator()(RandomAccessIterator begin, RandomAccessIterator end) const
        -> void
    {
        std::iota(begin, end, 0);
        std::reverse(begin, end);
    }

    static constexpr const char* output = "descending.txt";
};

struct pipe_organ:
    distribution<pipe_organ>
{
    template<typename RandomAccessIterator>
    auto operator()(RandomAccessIterator begin, RandomAccessIterator end) const
        -> void
    {
        int size = std::distance(begin, end);
        int count = 0;
        for (int i = 0 ; i < size / 2 ; ++i)
        {
            begin[count++] = i;
        }
        for (int i = size / 2 ; i < size ; ++i)
        {
            begin[count++] = size - i;
        }
    }

    static constexpr const char* output = "pipe_organ.txt";
};

struct push_front:
    distribution<push_front>
{
    template<typename RandomAccessIterator>
    auto operator()(RandomAccessIterator begin, RandomAccessIterator end) const
        -> void
    {
        if (std::distance(begin, end) > 0)
        {
            std::iota(begin, end, 1);
            *std::prev(end) = 0;
        }
    }

    static constexpr const char* output = "push_front.txt";
};

struct push_middle:
    distribution<push_middle>
{
    template<typename RandomAccessIterator>
    auto operator()(RandomAccessIterator begin, RandomAccessIterator end) const
        -> void
    {
        int size = std::distance(begin, end);
        if (size > 0)
        {
            int count = 0;
            for (int i = 0 ; i < size ; ++i)
            {
                if (i != size / 2)
                {
                    begin[count++] = i;
                }
            }
            begin[count] = size / 2;
        }
    }

    static constexpr const char* output = "push_middle.txt";
};

struct ascending_sawtooth:
    distribution<ascending_sawtooth>
{
    template<typename RandomAccessIterator>
    auto operator()(RandomAccessIterator begin, RandomAccessIterator end) const
        -> void
    {
        int size = std::distance(begin, end);
        int limit = size / cppsort::utility::log2(size) * 1.1;

        int count = 0;
        for (auto it = begin ; it != end ; ++it, ++count)
        {
            *it = count % limit;
        }
    }

    static constexpr const char* output = "ascending_sawtooth.txt";
};

struct descending_sawtooth:
    distribution<descending_sawtooth>
{
    template<typename RandomAccessIterator>
    auto operator()(RandomAccessIterator begin, RandomAccessIterator end) const
        -> void
    {
        int size = std::distance(begin, end);
        int limit = size / cppsort::utility::log2(size) * 1.1;

        int count = 0;
        for (auto it = end ; it != begin ; --it, ++count)
        {
            *std::prev(it) = count % limit;
        }
    }

    static constexpr const char* output = "descending_sawtooth.txt";
};

struct alternating:
    distribution<alternating>
{
    template<typename RandomAccessIterator>
    auto operator()(RandomAccessIterator begin, RandomAccessIterator end) const
        -> void
    {
        int count = 0;
        for (auto it = begin ; it != end ; ++it, ++count)
        {
            *it = (count % 2) ? count : -count;
        }
    }

    static constexpr const char* output = "alternating.txt";
};

struct alternating_16_values:
    distribution<alternating_16_values>
{
    template<typename RandomAccessIterator>
    auto operator()(RandomAccessIterator begin, RandomAccessIterator end) const
        -> void
    {
        int count = 0;
        for (auto it = begin ; it != end ; ++it, ++count)
        {
            *it = (count % 2) ? count % 16 : -(count % 16);
        }
    }

    static constexpr const char* output = "alternating_16_values.txt";
};
