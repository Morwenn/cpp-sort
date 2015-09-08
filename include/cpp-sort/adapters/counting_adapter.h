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
#ifndef CPPSORT_ADAPTERS_COUNTING_ADAPTER_H_
#define CPPSORT_ADAPTERS_COUNTING_ADAPTER_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <cstddef>
#include <functional>
#include <cpp-sort/sorter_traits.h>
#include "../detail/comparison_counter.h"

namespace cppsort
{
    ////////////////////////////////////////////////////////////
    // Adapter

    template<
        typename Sorter,
        typename CountType = std::size_t
    >
    struct counting_adapter
    {
        template<
            typename Iterable,
            typename Compare = std::less<>
        >
        auto operator()(Iterable& iterable, Compare compare={}) const
            -> CountType
        {
            detail::comparison_counter<Compare, CountType> cmp(compare);
            Sorter{}(iterable, cmp);
            return cmp.count;
        }
    };

    ////////////////////////////////////////////////////////////
    // Sorter traits

    template<typename Sorter, typename CountType>
    struct sorter_traits<counting_adapter<Sorter, CountType>>
    {
        using iterator_category = iterator_category<Sorter>;
        static constexpr bool is_stable = is_stable<Sorter>;
    };
}

#endif // CPPSORT_ADAPTERS_COUNTING_ADAPTER_H_
