/*
 * Copyright (c) 2015-2021 Morwenn
 * SPDX-License-Identifier: MIT
 */
#ifndef CPPSORT_TESTSUITE_DISTRIBUTIONS_H_
#define CPPSORT_TESTSUITE_DISTRIBUTIONS_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <random>
#include <catch2/catch.hpp>
#include <cpp-sort/detail/bitops.h>
#include <cpp-sort/detail/random.h>

namespace dist
{
    inline auto gen()
        -> cppsort::detail::rand_bit_generator<std::mt19937_64>&
    {
        // Utility allowing to fetch random bits from a URBG one by one
        thread_local cppsort::detail::rand_bit_generator<std::mt19937_64> res{
            std::mt19937_64(Catch::rngSeed())
        };
        return res;
    }

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

        // Make it easier to specify explicit template parameters
        template<typename T=long long int, typename OutputIterator>
        auto call(OutputIterator out, long long int size) const
            -> decltype(auto)
        {
            return static_cast<const Derived&>(*this).template operator()<T>(out, size);
        }

        template<typename T=long long int, typename OutputIterator>
        auto call(OutputIterator out, long long int size, long long int start) const
            -> decltype(auto)
        {
            return static_cast<const Derived&>(*this).template operator()<T>(out, size, start);
        }
    };

    struct shuffled:
        distribution<shuffled>
    {
        template<typename T=long long int, typename OutputIterator>
        auto operator()(OutputIterator out, long long int size, long long int start=0ll) const
            -> void
        {
            cppsort::detail::fill_with_shuffle<T>(out, size, start, gen());
        }
    };

    struct shuffled_16_values:
        distribution<shuffled_16_values>
    {
        static constexpr auto mod_16(long long int value)
            -> long long int
        {
            return value % 16;
        }

        template<typename T=long long int, typename OutputIterator>
        auto operator()(OutputIterator out, long long int size) const
            -> void
        {
            cppsort::detail::fill_with_shuffle<T>(out, size, 0, gen(), &mod_16);
        }
    };

    struct all_equal:
        distribution<all_equal>
    {
        template<typename T=long long int, typename OutputIterator>
        auto operator()(OutputIterator out, long long int size) const
            -> void
        {
            for (long long int i = 0 ; i < size ; ++i) {
                *out++ = static_cast<T>(0);
            }
        }
    };

    struct ascending:
        distribution<ascending>
    {
        template<typename T=long long int, typename OutputIterator>
        auto operator()(OutputIterator out, long long int size) const
            -> void
        {
            for (long long int i = 0 ; i < size ; ++i) {
                *out++ = static_cast<T>(i);
            }
        }
    };

    struct descending:
        distribution<descending>
    {
        template<typename T=long long int, typename OutputIterator>
        auto operator()(OutputIterator out, long long int size) const
            -> void
        {
            while (size--) {
                *out++ = static_cast<T>(size);
            }
        }
    };

    struct ascending_duplicates:
        distribution<ascending_duplicates>
    {
        // Ascending (sorted) distribution with series of 10
        // times the same integer value, used to test specific
        // algorithms against inputs with duplicate values

        template<typename T=long long int, typename OutputIterator>
        auto operator()(OutputIterator out, long long int size) const
            -> void
        {
            for (long long int i = 0 ; i < size ; ++i) {
                *out++ = static_cast<T>(i / 10);
            }
        }
    };

    struct pipe_organ:
        distribution<pipe_organ>
    {
        template<typename T=long long int, typename OutputIterator>
        auto operator()(OutputIterator out, long long int size) const
            -> void
        {
            for (long long int i = 0 ; i < size / 2 ; ++i) {
                *out++ = static_cast<T>(i);
            }
            for (long long int i = size / 2 ; i < size ; ++i) {
                *out++ = static_cast<T>(size - i);
            }
        }
    };

    struct push_front:
        distribution<push_front>
    {
        template<typename T=long long int, typename OutputIterator>
        auto operator()(OutputIterator out, long long int size) const
            -> void
        {
            if (size > 0) {
                for (long long int i = 0 ; i < size - 1 ; ++i) {
                    *out++ = static_cast<T>(i);
                }
                *out = static_cast<T>(0);
            }
        }
    };

    struct push_middle:
        distribution<push_middle>
    {
        template<typename T=long long int, typename OutputIterator>
        auto operator()(OutputIterator out, long long int size) const
            -> void
        {
            if (size > 0) {
                for (long long int i = 0 ; i < size ; ++i) {
                    if (i != size / 2) {
                        *out++ = static_cast<T>(i);
                    }
                }
                *out = static_cast<T>(size / 2);
            }
        }
    };

    struct ascending_sawtooth:
        distribution<ascending_sawtooth>
    {
        template<typename T=long long int, typename OutputIterator>
        auto operator()(OutputIterator out, long long int size) const
            -> void
        {
            auto limit = static_cast<long long int>(size / cppsort::detail::log2(size) * 0.9);
            for (long long int i = 0 ; i < size ; ++i) {
                *out++ = static_cast<T>(i % limit);
            }
        }
    };

    struct descending_sawtooth:
        distribution<descending_sawtooth>
    {
        template<typename T=long long int, typename OutputIterator>
        auto operator()(OutputIterator out, long long int size) const
            -> void
        {
            auto limit = static_cast<long long int>(size / cppsort::detail::log2(size) * 0.9);
            while (size--) {
                *out++ = static_cast<T>(size % limit);
            }
        }
    };

    struct alternating:
        distribution<alternating>
    {
        template<typename T=long long int, typename OutputIterator>
        auto operator()(OutputIterator out, long long int size) const
            -> void
        {
            for (long long int i = 0 ; i < size ; ++i) {
                *out++ = static_cast<T>((i % 2) ? i : -i);
            }
        }
    };

    struct descending_plateau:
        distribution<descending_plateau>
    {
        template<typename T=long long int, typename OutputIterator>
        auto operator()(OutputIterator out, long long int size) const
            -> void
        {
            long long int i = size;
            while (i > 2 * size / 3) {
                *out++ = static_cast<T>(i);
                --i;
            }
            while (i > size / 3) {
                *out++ = static_cast<T>(size / 2);
                --i;
            }
            while (i > 0) {
                *out++ = static_cast<T>(i);
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

        template<typename T=long long int, typename OutputIterator>
        auto operator()(OutputIterator out, long long int size) const
            -> void
        {
            long long int j = size / 2;
            for (long long int i = 1 ; i < j + 1 ; ++i) {
                if (i % 2 != 0) {
                    *out++ = static_cast<T>(i);
                } else {
                    *out++ = static_cast<T>(j + i - 1);
                }
            }
            for (long long int i = 1 ; i < j + 1 ; ++i) {
                *out++ = static_cast<T>(2 * i);
            }
        }
    };
}

#endif // CPPSORT_TESTSUITE_DISTRIBUTIONS_H_
