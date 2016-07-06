/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2015-2016 Morwenn
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
#ifndef CPPSORT_FIXED_SORTING_NETWORK_SORTER_H_
#define CPPSORT_FIXED_SORTING_NETWORK_SORTER_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <cstddef>
#include <iterator>
#include <cpp-sort/sorter_facade.h>
#include <cpp-sort/sorter_traits.h>

namespace cppsort
{
    ////////////////////////////////////////////////////////////
    // Adapter

    namespace detail
    {
        template<std::size_t N>
        struct sorting_network_sorter_impl
        {
            static_assert(
                N && false,
                "sorting_network_sorter has no specialization for this size of N"
            );
        };
    }

    template<std::size_t N>
    struct sorting_network_sorter:
        sorter_facade<detail::sorting_network_sorter_impl<N>>
    {};

    ////////////////////////////////////////////////////////////
    // Sorter traits

    template<std::size_t N>
    struct sorter_traits<sorting_network_sorter<N>>
    {
        using iterator_category = std::random_access_iterator_tag;

        // Some of the algorithms are stable, some other are not,
        // the stability *could* be documented depending on which
        // fixed-size algorithms are used, but it would be lots of
        // work...
        using is_always_stable = std::false_type;
    };

    template<>
    struct fixed_sorter_traits<sorting_network_sorter>
    {
        using domain = std::make_index_sequence<33>;
        using iterator_category = std::random_access_iterator_tag;
        using is_always_stable = std::false_type;
    };
}

// Specializations of sorting_network_sorter for some values of N
#include "../detail/sorting_network/sort0.h"
#include "../detail/sorting_network/sort1.h"
#include "../detail/sorting_network/sort2.h"
#include "../detail/sorting_network/sort3.h"
#include "../detail/sorting_network/sort4.h"
#include "../detail/sorting_network/sort5.h"
#include "../detail/sorting_network/sort6.h"
#include "../detail/sorting_network/sort7.h"
#include "../detail/sorting_network/sort8.h"
#include "../detail/sorting_network/sort9.h"
#include "../detail/sorting_network/sort10.h"
#include "../detail/sorting_network/sort11.h"
#include "../detail/sorting_network/sort12.h"
#include "../detail/sorting_network/sort13.h"
#include "../detail/sorting_network/sort14.h"
#include "../detail/sorting_network/sort15.h"
#include "../detail/sorting_network/sort16.h"
#include "../detail/sorting_network/sort17.h"
#include "../detail/sorting_network/sort18.h"
#include "../detail/sorting_network/sort19.h"
#include "../detail/sorting_network/sort20.h"
#include "../detail/sorting_network/sort21.h"
#include "../detail/sorting_network/sort22.h"
#include "../detail/sorting_network/sort23.h"
#include "../detail/sorting_network/sort24.h"
#include "../detail/sorting_network/sort25.h"
#include "../detail/sorting_network/sort26.h"
#include "../detail/sorting_network/sort27.h"
#include "../detail/sorting_network/sort28.h"
#include "../detail/sorting_network/sort29.h"
#include "../detail/sorting_network/sort30.h"
#include "../detail/sorting_network/sort31.h"
#include "../detail/sorting_network/sort32.h"

#endif // CPPSORT_FIXED_SORTING_NETWORK_SORTER_H_
