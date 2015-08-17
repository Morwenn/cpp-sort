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
#ifndef CPPSORT_UTILITY_MAKE_INTEGER_RANGE_H_
#define CPPSORT_UTILITY_MAKE_INTEGER_RANGE_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <cstddef>
#include <utility>

namespace cppsort
{
namespace utility
{
    namespace detail
    {
        template<
            typename Integer,
            typename SequenceBase,
            Integer Begin,
            Integer Step,
            bool IsIncreasing
        >
        struct integer_range_impl;

        template<
            typename Integer,
            Integer... N,
            Integer Begin,
            Integer Step
        >
        struct integer_range_impl<Integer, std::integer_sequence<Integer, N...>, Begin, Step, true>
        {
            using type = std::integer_sequence<Integer, (N * Step + Begin)...>;
        };

        template<
            typename Integer,
            Integer... N,
            Integer Begin,
            Integer Step
        >
        struct integer_range_impl<Integer, std::integer_sequence<Integer, N...>, Begin, Step, false>
        {
            using type = std::integer_sequence<Integer, (Begin - N * Step)...>;
        };
    }

    template<
        typename Integer,
        Integer Begin,
        Integer End,
        Integer Step = 1
    >
    using make_integer_range = typename detail::integer_range_impl<
        Integer,
        std::make_integer_sequence<
            Integer,
            ((Begin < End ? End - Begin : Begin - End) - 1) / (Step ? Step : 1) + 1
        >,
        Begin,
        (Step ? Step : 1),
        (Begin < End)
    >::type;

    template<
        std::size_t Begin,
        std::size_t End,
        std::size_t Step = 1u
    >
    using make_index_range = make_integer_range<std::size_t, Begin, End, Step>;
}}

#endif // CPPSORT_UTILITY_MAKE_INTEGER_RANGE_H_
