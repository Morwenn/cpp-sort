/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2015-2020 Morwenn
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
#include <utility>
#include <vector>
#include <cpp-sort/detail/bitops.h>
#include <cpp-sort/detail/type_traits.h>
#include <cpp-sort/utility/as_function.h>
#include <cpp-sort/utility/functional.h>

// Pseudo-random number generator, used by some distributions
thread_local std::mt19937_64 distributions_prng(std::time(nullptr));

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
struct base_distribution
{
    template<typename OutputIterator>
    using fptr_t = void(*)(OutputIterator, std::size_t);

    template<typename OutputIterator, typename Projection>
    using fptr_proj_t = void(*)(OutputIterator, std::size_t, Projection);

    template<typename OutputIterator>
    operator fptr_t<OutputIterator>() const
    {
        return [](OutputIterator out, std::size_t size)
        {
            return Derived{}(out, size);
        };
    }

    template<typename OutputIterator, typename Projection>
    operator fptr_proj_t<OutputIterator, Projection>() const
    {
        return [](OutputIterator out, std::size_t size, Projection projection)
        {
            return Derived{}(out, size, std::move(projection));
        };
    }
};

struct shuffled:
    base_distribution<shuffled>
{
    template<typename OutputIterator, typename Projection=cppsort::utility::identity>
    auto operator()(OutputIterator out, std::size_t size, Projection projection={}) const
        -> void
    {
        std::vector<int> vec;
        for (std::size_t i = 0 ; i < size ; ++i) {
            vec.emplace_back(i);
        }
        std::shuffle(std::begin(vec), std::end(vec), distributions_prng);

        auto&& proj = cppsort::utility::as_function(projection);
        std::transform(std::begin(vec), std::end(vec), out, proj);
    }

    static constexpr const char* output = "shuffled.txt";
};

struct shuffled_16_values:
    base_distribution<shuffled_16_values>
{
    template<typename OutputIterator, typename Projection=cppsort::utility::identity>
    auto operator()(OutputIterator out, std::size_t size, Projection projection={}) const
        -> void
    {
        std::vector<int> vec;
        for (std::size_t i = 0 ; i < size ; ++i) {
            vec.emplace_back(i % 16);
        }
        std::shuffle(std::begin(vec), std::end(vec), distributions_prng);

        auto&& proj = cppsort::utility::as_function(projection);
        std::transform(std::begin(vec), std::end(vec), out, proj);
    }

    static constexpr const char* output = "shuffled.txt";
};

struct all_equal:
    base_distribution<all_equal>
{
    template<typename OutputIterator, typename Projection=cppsort::utility::identity>
    auto operator()(OutputIterator out, std::size_t size, Projection projection={}) const
        -> void
    {
        auto&& proj = cppsort::utility::as_function(projection);
        for (std::size_t i = 0 ; i < size ; ++i) {
            *out++ = proj(0);
        }
    }

    static constexpr const char* output = "all_equal.txt";
};

struct ascending:
    base_distribution<ascending>
{
    template<typename OutputIterator, typename Projection=cppsort::utility::identity>
    auto operator()(OutputIterator out, std::size_t size, Projection projection={}) const
        -> void
    {
        auto&& proj = cppsort::utility::as_function(projection);
        for (std::size_t i = 0 ; i < size ; ++i) {
            *out++ = proj(i);
        }
    }

    static constexpr const char* output = "ascending.txt";
};

struct descending:
    base_distribution<descending>
{
    template<typename OutputIterator, typename Projection=cppsort::utility::identity>
    auto operator()(OutputIterator out, std::size_t size, Projection projection={}) const
        -> void
    {
        auto&& proj = cppsort::utility::as_function(projection);
        while (size--) {
            *out++ = proj(size);
        }
    }

    static constexpr const char* output = "descending.txt";
};

struct pipe_organ:
    base_distribution<pipe_organ>
{
    template<typename OutputIterator, typename Projection=cppsort::utility::identity>
    auto operator()(OutputIterator out, std::size_t size, Projection projection={}) const
        -> void
    {
        auto&& proj = cppsort::utility::as_function(projection);
        for (std::size_t i = 0 ; i < size / 2 ; ++i) {
            *out++ = proj(i);
        }
        for (std::size_t i = size / 2 ; i < size ; ++i) {
            *out++ = proj(size - i);
        }
    }

    static constexpr const char* output = "pipe_organ.txt";
};

struct push_front:
    base_distribution<push_front>
{
    template<typename OutputIterator, typename Projection=cppsort::utility::identity>
    auto operator()(OutputIterator out, std::size_t size, Projection projection={}) const
        -> void
    {
        auto&& proj = cppsort::utility::as_function(projection);
        if (size > 0) {
            for (std::size_t i = 0 ; i < size - 1 ; ++i) {
                *out++ = proj(i);
            }
            *out = proj(0);
        }
    }

    static constexpr const char* output = "push_front.txt";
};

struct push_middle:
    base_distribution<push_middle>
{
    template<typename OutputIterator, typename Projection=cppsort::utility::identity>
    auto operator()(OutputIterator out, std::size_t size, Projection projection={}) const
        -> void
    {
        auto&& proj = cppsort::utility::as_function(projection);
        if (size > 0) {
            for (std::size_t i = 0 ; i < size ; ++i) {
                if (i != size / 2) {
                    *out++ = proj(i);
                }
            }
            *out = proj(size / 2);
        }
    }

    static constexpr const char* output = "push_middle.txt";
};

struct ascending_sawtooth:
    base_distribution<ascending_sawtooth>
{
    template<typename OutputIterator, typename Projection=cppsort::utility::identity>
    auto operator()(OutputIterator out, std::size_t size, Projection projection={}) const
        -> void
    {
        auto&& proj = cppsort::utility::as_function(projection);
        std::size_t limit = size / cppsort::detail::log2(size) + 50;
        for (std::size_t i = 0 ; i < size ; ++i) {
            *out++ = proj(i % limit);
        }
    }

    static constexpr const char* output = "ascending_sawtooth.txt";
};

struct ascending_sawtooth_bad:
    base_distribution<ascending_sawtooth_bad>
{
    template<typename OutputIterator, typename Projection=cppsort::utility::identity>
    auto operator()(OutputIterator out, std::size_t size, Projection projection={}) const
        -> void
    {
        auto&& proj = cppsort::utility::as_function(projection);
        std::size_t limit = size / cppsort::detail::log2(size) - 50;
        for (std::size_t i = 0 ; i < size ; ++i) {
            *out++ = proj(i % limit);
        }
    }

    static constexpr const char* output = "ascending_sawtooth.txt";
};

struct descending_sawtooth:
    base_distribution<descending_sawtooth>
{
    template<typename OutputIterator, typename Projection=cppsort::utility::identity>
    auto operator()(OutputIterator out, std::size_t size, Projection projection={}) const
        -> void
    {
        auto&& proj = cppsort::utility::as_function(projection);
        std::size_t limit = size / cppsort::detail::log2(size) + 50;
        while (size--) {
            *out++ = proj(size % limit);
        }
    }

    static constexpr const char* output = "descending_sawtooth.txt";
};

struct descending_sawtooth_bad:
    base_distribution<descending_sawtooth_bad>
{
    template<typename OutputIterator, typename Projection=cppsort::utility::identity>
    auto operator()(OutputIterator out, std::size_t size, Projection projection={}) const
        -> void
    {
        auto&& proj = cppsort::utility::as_function(projection);
        std::size_t limit = size / cppsort::detail::log2(size) - 50;
        while (size--) {
            *out++ = proj(size % limit);
        }
    }

    static constexpr const char* output = "descending_sawtooth.txt";
};

struct alternating:
    base_distribution<alternating>
{
    template<typename OutputIterator, typename Projection=cppsort::utility::identity>
    auto operator()(OutputIterator out, std::size_t size, Projection projection={}) const
        -> void
    {
        auto&& proj = cppsort::utility::as_function(projection);
        for (std::size_t i = 0 ; i < size ; ++i) {
            *out++ = proj((i % 2) ? i : -i);
        }
    }

    static constexpr const char* output = "alternating.txt";
};

struct alternating_16_values:
    base_distribution<alternating_16_values>
{
    template<typename OutputIterator, typename Projection=cppsort::utility::identity>
    auto operator()(OutputIterator out, std::size_t size, Projection projection={}) const
        -> void
    {
        auto&& proj = cppsort::utility::as_function(projection);
        for (std::size_t i = 0 ; i < size ; ++i) {
            *out++ = proj((i % 2) ? i % 16 : -(i % 16));
        }
    }

    static constexpr const char* output = "alternating_16_values.txt";
};

struct sparse_inversions:
    base_distribution<sparse_inversions>
{
    template<typename OutputIterator, typename Projection=cppsort::utility::identity>
    auto operator()(OutputIterator out, std::size_t size, Projection projection={}) const
        -> void
    {
        auto&& proj = cppsort::utility::as_function(projection);
        const auto size_run = size / cppsort::detail::log2(size) + 30;
        for (std::size_t i = 0 ; i < size ; ++i) {
            *out++ = proj((i % size_run) == 0 ? 0 : i);
        }
    }

    static constexpr const char* output = "alternating_16_values.txt";
};

struct vergesort_killer:
    base_distribution<vergesort_killer>
{
    template<typename OutputIterator, typename Projection=cppsort::utility::identity>
    auto operator()(OutputIterator out, std::size_t size, Projection projection={}) const
        -> void
    {
        // WARNING: not for small collections, mostly because I'm lazy...

        const auto size_run = size / cppsort::detail::log2(size);
        auto desc = descending{};
        auto killer = pipe_organ{};

        auto size_output_left = size;
        while (true) {
            killer(out, size_run - 50, projection);
            size_output_left -= size_run - 50;
            if (size_output_left < size_run + 50) break;
            desc(out, size_run + 50, projection);
            size_output_left -= size_run + 50;
            if (size_output_left < size_run - 50) break;
        };

        // Just in case
        if (size_output_left) {
            shuffled{}(out, size_output_left, projection);
        }
    }

    static constexpr const char* output = "vergesort_killer.txt";
};
