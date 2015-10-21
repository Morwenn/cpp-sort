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
#ifndef CPPSORT_FIXED_LOW_COMPARISONS_SORTER_H_
#define CPPSORT_FIXED_LOW_COMPARISONS_SORTER_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <cstddef>
#include <iterator>

namespace cppsort
{
    ////////////////////////////////////////////////////////////
    // Adapter

    template<std::size_t N>
    struct low_comparisons_sorter
    {
        static_assert(
            N && false,
            "low_comparisons_sorter has no specialization for this size of N"
        );
    };

    ////////////////////////////////////////////////////////////
    // Sorter traits

    template<std::size_t N>
    struct sorter_traits<low_comparisons_sorter<N>>
    {
        using iterator_category = std::random_access_iterator_tag;

        // Some of the algorithms are stable, some other are not,
        // the stability *could* be documented depending on which
        // fixed-size algorithms are used, but it would be lots of
        // work...
        static constexpr bool is_stable = false;
    };
}

// Specializations of low_comparisons_sorter for some values of N
#include "../detail/low_comparisons/sort0.h"
#include "../detail/low_comparisons/sort1.h"
#include "../detail/low_comparisons/sort2.h"
#include "../detail/low_comparisons/sort3.h"
#include "../detail/low_comparisons/sort4.h"
#include "../detail/low_comparisons/sort5.h"
#include "../detail/low_comparisons/sort6.h"
#include "../detail/low_comparisons/sort7.h"
#include "../detail/low_comparisons/sort8.h"
#include "../detail/low_comparisons/sort9.h"
#include "../detail/low_comparisons/sort10.h"
#include "../detail/low_comparisons/sort11.h"
#include "../detail/low_comparisons/sort12.h"
#include "../detail/low_comparisons/sort13.h"

#endif // CPPSORT_FIXED_LOW_COMPARISONS_SORTER_H_
