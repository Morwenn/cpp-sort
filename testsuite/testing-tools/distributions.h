/*
 * Copyright (c) 2015-2019 Morwenn
 * SPDX-License-Identifier: MIT
 */
#ifndef CPPSORT_TESTSUITE_DISTRIBUTIONS_H_
#define CPPSORT_TESTSUITE_DISTRIBUTIONS_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <algorithm>
#include <cstddef>
#include <iterator>
#include <random>
#include <vector>
#include <catch2/catch.hpp>
#include <cpp-sort/detail/bitops.h>

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
            thread_local std::mt19937 engine(Catch::rngSeed());

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
            thread_local std::mt19937 engine(Catch::rngSeed());

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
            std::size_t limit = size / cppsort::detail::log2(size) * 0.9;
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
            std::size_t limit = size / cppsort::detail::log2(size) * 0.9;
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
