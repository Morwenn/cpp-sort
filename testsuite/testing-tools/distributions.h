/*
 * Copyright (c) 2015-2021 Morwenn
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
        using fptr_t = void(*)(OutputIterator, long long int);

        template<typename OutputIterator>
        operator fptr_t<OutputIterator>() const
        {
            return [](OutputIterator out, long long int size) {
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
        auto operator()(OutputIterator out, long long int size) const
            -> void
        {
            // Pseudo-random number generator
            thread_local std::mt19937 engine(Catch::rngSeed());

            std::vector<int> vec;
            vec.reserve(size);

            for (long long int i = 0 ; i < size ; ++i) {
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
        auto operator()(OutputIterator out, long long int size) const
            -> void
        {
            for (long long int i = 0 ; i < size ; ++i) {
                *out++ = 0;
            }
        }
    };

    struct ascending:
        distribution<ascending>
    {
        template<typename OutputIterator>
        auto operator()(OutputIterator out, long long int size) const
            -> void
        {
            for (long long int i = 0 ; i < size ; ++i) {
                *out++ = i;
            }
        }
    };

    struct descending:
        distribution<descending>
    {
        template<typename OutputIterator>
        auto operator()(OutputIterator out, long long int size) const
            -> void
        {
            while (size--) {
                *out++ = size;
            }
        }
    };

    struct ascending_duplicates:
        distribution<ascending_duplicates>
    {
        // Ascending (sorted) distribution with series of 10
        // times the same integer value, used to test specific
        // algorithms against inputs with duplicate values

        template<typename OutputIterator>
        auto operator()(OutputIterator out, long long int size) const
            -> void
        {
            for (long long int i = 0 ; i < size ; ++i) {
                *out++ = i / 10;
            }
        }
    };

    struct pipe_organ:
        distribution<pipe_organ>
    {
        template<typename OutputIterator>
        auto operator()(OutputIterator out, long long int size) const
            -> void
        {
            for (long long int i = 0 ; i < size / 2 ; ++i) {
                *out++ = i;
            }
            for (long long int i = size / 2 ; i < size ; ++i) {
                *out++ = size - i;
            }
        }
    };

    struct push_front:
        distribution<push_front>
    {
        template<typename OutputIterator>
        auto operator()(OutputIterator out, long long int size) const
            -> void
        {
            if (size > 0) {
                for (long long int i = 0 ; i < size - 1 ; ++i) {
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
        auto operator()(OutputIterator out, long long int size) const
            -> void
        {
            if (size > 0) {
                for (long long int i = 0 ; i < size ; ++i) {
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
        auto operator()(OutputIterator out, long long int size) const
            -> void
        {
            long long int limit = size / cppsort::detail::log2(size) * 0.9;
            for (long long int i = 0 ; i < size ; ++i) {
                *out++ = i % limit;
            }
        }
    };

    struct descending_sawtooth:
        distribution<descending_sawtooth>
    {
        template<typename OutputIterator>
        auto operator()(OutputIterator out, long long int size) const
            -> void
        {
            long long int limit = size / cppsort::detail::log2(size) * 0.9;
            while (size--) {
                *out++ = size % limit;
            }
        }
    };

    struct alternating:
        distribution<alternating>
    {
        template<typename OutputIterator>
        auto operator()(OutputIterator out, long long int size) const
            -> void
        {
            for (long long int i = 0 ; i < size ; ++i) {
                *out++ = (i % 2) ? i : -i;
            }
        }
    };

    struct descending_plateau:
        distribution<descending_plateau>
    {
        template<typename OutputIterator>
        auto operator()(OutputIterator out, long long int size) const
            -> void
        {
            long long int i = size;
            while (i > 2 * size / 3) {
                *out++ = i;
                --i;
            }
            while (i > size / 3) {
                *out++ = size / 2;
                --i;
            }
            while (i > 0) {
                *out++ = i;
                --i;
            }
        }
    };

    struct median_of_3_killer:
        distribution<median_of_3_killer>
    {
        // This distribution comes from *A Killer Adversary for Quicksort*
        // by M. D. McIlroy, and is supposed to trick several quicksort
        // implementations with common pivot selection methods go quadratic

        template<typename OutputIterator>
        auto operator()(OutputIterator out, long long int size) const
            -> void
        {
            long long int j = size / 2;
            for (long long int i = 1 ; i < j + 1 ; ++i) {
                if (i % 2 != 0) {
                    *out++ = i;
                } else {
                    *out++ = j + i - 1;
                }
            }
            for (long long int i = 1 ; i < j + 1 ; ++i) {
                *out++ = 2 * i;
            }
        }
    };
}

#endif // CPPSORT_TESTSUITE_DISTRIBUTIONS_H_
