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
#include <algorithm>
#include <ctime>
#include <iterator>
#include <numeric>
#include <random>
#include <vector>
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
    template<typename OutputIterator>
    using fptr_t = void(*)(OutputIterator, std::size_t);

    template<typename OutputIterator>
    operator fptr_t<OutputIterator>() const
    {
        return [](OutputIterator out, std::size_t size)
        {
            return Derived{}(out, size);
        };
    }
};

struct shuffled:
    distribution<shuffled>
{
    template<typename OutputIterator>
    auto operator()(OutputIterator out, std::size_t size) const
        -> void
    {
        // Pseudo-random number generator
        thread_local std::mt19937_64 engine(std::time(nullptr));

        std::vector<int> vec;
        for (std::size_t i = 0 ; i < size ; ++i)
        {
            vec.emplace_back(i);
        }
        std::shuffle(std::begin(vec), std::end(vec), engine);
        std::move(std::begin(vec), std::end(vec), out);
    }

    static constexpr const char* output = "shuffled.txt";
};

struct shuffled_16_values:
    distribution<shuffled_16_values>
{
    template<typename OutputIterator>
    auto operator()(OutputIterator out, std::size_t size) const
        -> void
    {
        // Pseudo-random number generator
        thread_local std::mt19937_64 engine(std::time(nullptr));

        std::vector<int> vec;
        for (std::size_t i = 0 ; i < size ; ++i)
        {
            vec.emplace_back(i % 16);
        }
        std::shuffle(std::begin(vec), std::end(vec), engine);
        std::move(std::begin(vec), std::end(vec), out);
    }

    static constexpr const char* output = "shuffled.txt";
};

struct all_equal:
    distribution<all_equal>
{
    template<typename OutputIterator>
    auto operator()(OutputIterator out, std::size_t size) const
        -> void
    {
        for (std::size_t i = 0 ; i < size ; ++i)
        {
            *out++ = 0;
        }
    }

    static constexpr const char* output = "all_equal.txt";
};

struct ascending:
    distribution<ascending>
{
    template<typename OutputIterator>
    auto operator()(OutputIterator out, std::size_t size) const
        -> void
    {
        for (std::size_t i = 0 ; i < size ; ++i)
        {
            *out++ = i;
        }
    }

    static constexpr const char* output = "ascending.txt";
};

struct descending:
    distribution<descending>
{
    template<typename OutputIterator>
    auto operator()(OutputIterator out, std::size_t size) const
        -> void
    {
        while (size--)
        {
            *out++ = size;
        }
    }

    static constexpr const char* output = "descending.txt";
};

struct pipe_organ:
    distribution<pipe_organ>
{
    template<typename OutputIterator>
    auto operator()(OutputIterator out, std::size_t size) const
        -> void
    {
        for (std::size_t i = 0 ; i < size / 2 ; ++i)
        {
            *out++ = i;
        }
        for (std::size_t i = size / 2 ; i < size ; ++i)
        {
            *out++ = size - i;
        }
    }

    static constexpr const char* output = "pipe_organ.txt";
};

struct push_front:
    distribution<push_front>
{
    template<typename OutputIterator>
    auto operator()(OutputIterator out, std::size_t size) const
        -> void
    {
        if (size > 0)
        {
            for (std::size_t i = 0 ; i < size - 1 ; ++i)
            {
                *out++ = i;
            }
            *out = 0;
        }
    }

    static constexpr const char* output = "push_front.txt";
};

struct push_middle:
    distribution<push_middle>
{
    template<typename OutputIterator>
    auto operator()(OutputIterator out, std::size_t size) const
        -> void
    {
        if (size > 0)
        {
            for (std::size_t i = 0 ; i < size ; ++i)
            {
                if (i != size / 2)
                {
                    *out++ = i;
                }
            }
            *out = size / 2;
        }
    }

    static constexpr const char* output = "push_middle.txt";
};

struct ascending_sawtooth:
    distribution<ascending_sawtooth>
{
    template<typename OutputIterator>
    auto operator()(OutputIterator out, std::size_t size) const
        -> void
    {
        std::size_t limit = size / cppsort::utility::log2(size) + 50;
        for (std::size_t i = 0 ; i < size ; ++i)
        {
            *out++ = i % limit;
        }
    }

    static constexpr const char* output = "ascending_sawtooth.txt";
};

struct ascending_sawtooth_bad:
    distribution<ascending_sawtooth_bad>
{
    template<typename OutputIterator>
    auto operator()(OutputIterator out, std::size_t size) const
        -> void
    {
        std::size_t limit = size / cppsort::utility::log2(size) - 50;
        for (std::size_t i = 0 ; i < size ; ++i) {
            *out++ = i % limit;
        }
    }

    static constexpr const char* output = "ascending_sawtooth.txt";
};

struct descending_sawtooth:
    distribution<descending_sawtooth>
{
    template<typename OutputIterator>
    auto operator()(OutputIterator out, std::size_t size) const
        -> void
    {
        std::size_t limit = size / cppsort::utility::log2(size) + 50;
        while (size--)
        {
            *out++ = size % limit;
        }
    }

    static constexpr const char* output = "descending_sawtooth.txt";
};

struct descending_sawtooth_bad:
    distribution<descending_sawtooth_bad>
{
    template<typename OutputIterator>
    auto operator()(OutputIterator out, std::size_t size) const
        -> void
    {
        std::size_t limit = size / cppsort::utility::log2(size) - 50;
        while (size--) {
            *out++ = size % limit;
        }
    }

    static constexpr const char* output = "descending_sawtooth.txt";
};

struct alternating:
    distribution<alternating>
{
    template<typename OutputIterator>
    auto operator()(OutputIterator out, std::size_t size) const
        -> void
    {
        for (std::size_t i = 0 ; i < size ; ++i)
        {
            *out++ = (i % 2) ? i : -i;
        }
    }

    static constexpr const char* output = "alternating.txt";
};

struct alternating_16_values:
    distribution<alternating_16_values>
{
    template<typename OutputIterator>
    auto operator()(OutputIterator out, std::size_t size) const
        -> void
    {
        for (std::size_t i = 0 ; i < size ; ++i)
        {
            *out++ = (i % 2) ? i % 16 : -(i % 16);
        }
    }

    static constexpr const char* output = "alternating_16_values.txt";
};

struct sparse_inversions:
    distribution<sparse_inversions>
{
    template<typename OutputIterator>
    auto operator()(OutputIterator out, std::size_t size) const
        -> void
    {
        const auto size_run = size / cppsort::utility::log2(size) + 30;

        for (std::size_t i = 0 ; i < size ; ++i) {
            *out++ = (i % size_run) == 0 ? 0 : i;
        }
    }

    static constexpr const char* output = "alternating_16_values.txt";
};

struct vergesort_killer:
    distribution<vergesort_killer>
{
    template<typename OutputIterator>
    auto operator()(OutputIterator out, std::size_t size) const
        -> void
    {
        // WARNING: not for small collections, mostly because I'm lazy...

        const auto size_run = size / cppsort::utility::log2(size);
        auto desc = descending{};
        auto killer = pipe_organ{};

        auto size_output_left = size;
        while (true) {
            killer(out, size_run - 50);
            size_output_left -= size_run - 50;
            if (size_output_left < size_run + 50) break;
            desc(out, size_run + 50);
            size_output_left -= size_run + 50;
            if (size_output_left < size_run - 50) break;
        };

        // Just in case
        if (size_output_left) {
            shuffled{}(out, size_output_left);
        }
    }

    static constexpr const char* output = "vergesort_killer.txt";
};
