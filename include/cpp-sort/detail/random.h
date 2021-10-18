/*
 * Copyright (c) 2021 Morwenn
 * SPDX-License-Identifier: MIT
 */
#ifndef CPPSORT_DETAIL_RANDOM_H_
#define CPPSORT_DETAIL_RANDOM_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <random>

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

            explicit rand_bit_generator(URBG& engine):
                engine_ptr(&engine),
                word(0),
                pos(0)
            {}

            auto next_bit()
                -> result_type
            {
                if (pos == 0) {
                    word = (*engine_ptr)();
                    pos = URBG::word_size;
                }
                --pos;
                return (word & (result_type(1) << pos)) >> pos;
            }

        private:
            URBG* engine_ptr;
            typename URBG::result_type word;
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
}}

#endif // CPPSORT_DETAIL_RANDOM_H_
