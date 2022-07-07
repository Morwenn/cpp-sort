/*
 * Copyright (c) 2021-2022 Morwenn
 * SPDX-License-Identifier: MIT
 */
#ifndef CPPSORT_TESTSUITE_RANDOM_H_
#define CPPSORT_TESTSUITE_RANDOM_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <cassert>
#include <cstdint>
#include <functional>
#include <utility>
#include <cpp-sort/detail/bitops.h>
#include <cpp-sort/utility/as_function.h>

namespace hasard // Blame POSIX for picking the good name
{
    ////////////////////////////////////////////////////////////
    // xoshiro256**
    // https://prng.di.unimi.it/xoshiro256starstar.c

    struct xoshiro256ss
    {
        public:

            using result_type = std::uint64_t;

            ////////////////////////////////////////////////////////////
            // Construction

            constexpr xoshiro256ss():
                xoshiro256ss(0)
            {}

            constexpr explicit xoshiro256ss(result_type seed):
                s{splitmix64(seed),
                  splitmix64(seed),
                  splitmix64(seed),
                  splitmix64(seed)}
            {
                // From the original link: "If you have a 64-bit seed, we
                // suggest to seed a splitmix64 generator and use its
                // output to fill s."
            }

            ////////////////////////////////////////////////////////////
            // Generation

            constexpr auto operator()()
                -> result_type
            {
                const result_type result = cppsort::detail::rotl(s[1] * 5, 7) * 9;
                const result_type t = s[1] << 17;

                s[2] ^= s[0];
                s[3] ^= s[1];
                s[1] ^= s[2];
                s[0] ^= s[3];
                s[2] ^= t;
                s[3] = cppsort::detail::rotl(s[3], 45);

                return result;
            }

            ////////////////////////////////////////////////////////////
            // Characteristics

            static constexpr std::size_t word_size = std::numeric_limits<result_type>::digits;

            static constexpr auto min()
                -> result_type
            {
                return 0;
            }

            static constexpr auto max()
                -> result_type
            {
                return static_cast<result_type>(-1);
            }

        private:

            // State of the PRNG
            result_type s[4];

            static constexpr auto splitmix64(std::uint64_t& x)
                -> std::uint64_t
            {
                std::uint64_t z = (x += 0x9e3779b97f4a7c15ull);
                z = (z ^ (z >> 30)) * 0xbf58476d1ce4e5b9ull;
                z = (z ^ (z >> 27)) * 0x94d049bb133111ebull;
                return z ^ (z >> 31);
            }
    };

    ////////////////////////////////////////////////////////////
    // rand_bit_generator
    //
    // Takes a standard URBG and allows to fetch random bits one
    // by one for algorithms that use such random bits

    template<typename URBG>
    struct rand_bit_generator
    {
        public:
            using result_type = typename URBG::result_type;

            explicit rand_bit_generator(URBG&& engine):
                engine(std::move(engine)),
                word(0),
                pos(0)
            {}

            auto next_bit()
                -> result_type
            {
                if (pos == 0) {
                    word = engine();
                    pos = URBG::word_size;
                }
                --pos;
                return (word & (result_type(1) << pos)) >> pos;
            }

        private:
            URBG engine;
            result_type word;
            int pos;
    };

    ////////////////////////////////////////////////////////////
    // randint
    //
    // Returns a random integer in the range [low, high].
    // See *Optimal Discrete Uniform Generation from Coin Flips,
    // and Applications* by Jérémie Lumbroso

    template<typename Integer, typename URBG>
    auto randint(Integer low, Integer high, URBG& generator)
        -> Integer
    {
        using urng_int_type = typename URBG::result_type;

        if (low == high) {
            return low;
        }
        urng_int_type n = high - low;

        urng_int_type v = 1;
        urng_int_type c = 0;
        while (true) {
            v <<= 1;
            c = (c << 1) + generator.next_bit();
            if (v >= n) {
                if (c < n) {
                    return static_cast<Integer>(low + c);
                } else {
                    v -= n;
                    c -= n;
                }
            }
        }
    }

    ////////////////////////////////////////////////////////////
    // fill_with_shuffle
    //
    // Takes an output iterator and fills the corresponding
    // range with a shuffle of all the integers in the range
    // [start, start + size]

    template<
        typename T=long long int,
        typename OutputIterator,
        typename URBG,
        typename Projection = std::identity
    >
    auto fill_with_shuffle(OutputIterator out, long long int size, long long int start,
                           URBG& engine, Projection projection={})
        -> void
    {
        assert(size >= 4);
        auto&& proj = cppsort::utility::as_function(projection);

        // Generate a shuffle of all the integers in the range [start, start + size)
        // with a linear congruential generator
        // https://stackoverflow.com/a/44821946/1364752

        long long int m = cppsort::detail::hyperceil<unsigned long long int>(size);
        auto a = randint(1ll, (m >> 2) - 1, engine) * 4 + 1;
        auto c = randint(3ll, m, engine) | 1;

        auto x = 1ll;
        for (auto i = 0ll; i < size; ++i) {
            do {
                x = (x * a + c) % m;
            } while (x >= size);
            *out = static_cast<T>(proj(x + start));
            ++out;
        }
    }

    ////////////////////////////////////////////////////////////
    // Thread-local "Meyers singletons"

    // Pseudo-random number generator engine
    auto engine()
        -> xoshiro256ss&;

    // Utility allowing to fetch random bits from a URBG one by one
    auto bit_gen()
        -> rand_bit_generator<xoshiro256ss>&;
}

#endif // CPPSORT_TESTSUITE_RANDOM_H_
