/*
 * Copyright (c) 2021 Morwenn
 * SPDX-License-Identifier: MIT
 */
#ifndef CPPSORT_DETAIL_RANDOM_H_
#define CPPSORT_DETAIL_RANDOM_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <cassert>
#include <random>
#include <utility>
#include <cpp-sort/utility/as_function.h>
#include <cpp-sort/utility/functional.h>
#include "bitops.h"

namespace cppsort
{
namespace detail
{
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
        typename Projection = utility::identity
    >
    auto fill_with_shuffle(OutputIterator out, long long int size, long long int start,
                           URBG& engine, Projection projection={})
        -> void
    {
        assert(size >= 4);
        auto&& proj = utility::as_function(projection);

        // Generate a shuffle of all the integers in the range [start, start + size)
        // with a linear congruential generator
        // https://stackoverflow.com/a/44821946/1364752

        long long int m = hyperceil<unsigned long long int>(size);
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
}}

#endif // CPPSORT_DETAIL_RANDOM_H_
