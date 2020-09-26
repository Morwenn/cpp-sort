/*
 * Copyright (c) 2015-2020 Morwenn
 * SPDX-License-Identifier: MIT
 */
#ifndef CPPSORT_UTILITY_MAKE_INTEGER_RANGE_H_
#define CPPSORT_UTILITY_MAKE_INTEGER_RANGE_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <cstddef>
#include <utility>
#include <cpp-sort/detail/config.h>

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
    using make_integer_range
        CPPSORT_DEPRECATED("make_integer_range is deprecated and will be removed in version 2.0.0")
        = typename detail::integer_range_impl<
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
    using make_index_range
        CPPSORT_DEPRECATED("make_index_range is deprecated and will be removed in version 2.0.0")
        = make_integer_range<std::size_t, Begin, End, Step>;
}}

#endif // CPPSORT_UTILITY_MAKE_INTEGER_RANGE_H_
