/*
 * Copyright (c) 2022 Morwenn
 * SPDX-License-Identifier: MIT
 */
#ifndef CPPSORT_MSTD_CONCEPTS_H_
#define CPPSORT_MSTD_CONCEPTS_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <cpp-sort/mstd/type_traits.h>

namespace cppsort::mstd
{
    ////////////////////////////////////////////////////////////
    // Arithmetic concepts

    template<typename T>
    concept integral = is_integral_v<T>;

    template <typename T>
    concept signed_integral = integral<T> && is_signed_v<T>;

    template <typename T>
    concept unsigned_integral = integral<T> && is_unsigned_v<T>;
}

#endif // CPPSORT_MSTD_CONCEPTS_H_
