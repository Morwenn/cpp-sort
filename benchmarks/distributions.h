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
#include <cstddef>
#include <ctime>
#include <iterator>
#include <numeric>
#include <random>

////////////////////////////////////////////////////////////
// Distributions for benchmarks
//
// Distributions are functions object used to fill the
// collections to sort before benchmarking the sorting
// algorithms. These distributions allow to check how a
// sorting algorithm behaves with common patterns found in
// actual data sets.
//

struct shuffled
{
    template<typename RandomAccessIterator>
    auto operator()(RandomAccessIterator begin, RandomAccessIterator end) const
        -> void
    {
        // Pseudo-random numbers generator
        thread_local std::mt19937_64 engine(std::time(nullptr));

        std::iota(begin, end, 0);
        std::shuffle(begin, end, engine);
    }

    static constexpr const char* output = "shuffled.txt";
};

struct all_equal
{
    template<typename RandomAccessIterator>
    auto operator()(RandomAccessIterator begin, RandomAccessIterator end) const
        -> void
    {
        std::fill(begin, end, 0);
    }

    static constexpr const char* output = "all_equal.txt";
};

struct ascending
{
    template<typename RandomAccessIterator>
    auto operator()(RandomAccessIterator begin, RandomAccessIterator end) const
        -> void
    {
        std::iota(begin, end, 0);
    }

    static constexpr const char* output = "ascending.txt";
};

struct descending
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

struct pipe_organ
{
    template<typename RandomAccessIterator>
    auto operator()(RandomAccessIterator begin, RandomAccessIterator end) const
        -> void
    {
        std::size_t size = std::distance(begin, end);
        std::size_t count = 0u;
        for (std::size_t i = 0 ; i < size / 2u ; ++i)
        {
            begin[count++] = i;
        }
        for (std::size_t i = size / 2 ; i < size ; ++i)
        {
            begin[count++] = size - i;
        }
    }

    static constexpr const char* output = "pipe_organ.txt";
};

struct push_front
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

struct push_middle
{
    template<typename RandomAccessIterator>
    auto operator()(RandomAccessIterator begin, RandomAccessIterator end) const
        -> void
    {
        std::size_t size = std::distance(begin, end);
        if (size > 0)
        {
            std::size_t count = 0u;
            for (std::size_t i = 0u ; i < size ; ++i)
            {
                if (i != size / 2u)
                {
                    begin[count++] = i;
                }
            }
            begin[count] = size / 2;
        }
    }

    static constexpr const char* output = "push_middle.txt";
};
