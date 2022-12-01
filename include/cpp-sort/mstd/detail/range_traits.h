/*
 * Copyright (c) 2022 Morwenn
 * SPDX-License-Identifier: MIT
 */

//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef CPPSORT_MSTD_DETAIL_RANGE_TRAITS_H_
#define CPPSORT_MSTD_DETAIL_RANGE_TRAITS_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <ranges>
#include <utility>
#include <cpp-sort/mstd/iterator.h>
#include "end.h"
#include "range.h"

namespace cppsort::mstd
{
    ////////////////////////////////////////////////////////////
    // sentinel_t

    template<range R>
    using sentinel_t = decltype(mstd::end(std::declval<R&>()));

    ////////////////////////////////////////////////////////////
    // range_difference_t

    template<range R>
    using range_difference_t = iter_difference_t<iterator_t<R>>;

    ////////////////////////////////////////////////////////////
    // range_reference_t

    template<range R>
    using range_reference_t = std::iter_reference_t<iterator_t<R>>;
}

#endif // CPPSORT_MSTD_DETAIL_RANGE_TRAITS_H_
