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
#ifndef CPPSORT_DETAIL_SORTING_NETWORK_SORT15_H_
#define CPPSORT_DETAIL_SORTING_NETWORK_SORT15_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <functional>
#include <type_traits>
#include <cpp-sort/sorter_traits.h>
#include <cpp-sort/utility/functional.h>
#include "../swap_if.h"

namespace cppsort
{
namespace detail
{
    template<>
    struct sorting_network_sorter_impl<15u>
    {
        template<
            typename RandomAccessIterator,
            typename Compare = std::less<>,
            typename Projection = utility::identity,
            typename = std::enable_if_t<is_projection_iterator<
                Projection, RandomAccessIterator, Compare
            >>
        >
        auto operator()(RandomAccessIterator first, RandomAccessIterator,
                        Compare compare={}, Projection projection={}) const
            -> void
        {
            swap_if(first[0u], first[1u], compare, projection);
            swap_if(first[2u], first[3u], compare, projection);
            swap_if(first[4u], first[5u], compare, projection);
            swap_if(first[6u], first[7u], compare, projection);
            swap_if(first[8u], first[9u], compare, projection);
            swap_if(first[10u], first[11u], compare, projection);
            swap_if(first[12u], first[13u], compare, projection);
            swap_if(first[0u], first[2u], compare, projection);
            swap_if(first[4u], first[6u], compare, projection);
            swap_if(first[8u], first[10u], compare, projection);
            swap_if(first[12u], first[14u], compare, projection);
            swap_if(first[1u], first[3u], compare, projection);
            swap_if(first[5u], first[7u], compare, projection);
            swap_if(first[9u], first[11u], compare, projection);
            swap_if(first[0u], first[4u], compare, projection);
            swap_if(first[8u], first[12u], compare, projection);
            swap_if(first[1u], first[5u], compare, projection);
            swap_if(first[9u], first[13u], compare, projection);
            swap_if(first[2u], first[6u], compare, projection);
            swap_if(first[10u], first[14u], compare, projection);
            swap_if(first[3u], first[7u], compare, projection);
            swap_if(first[0u], first[8u], compare, projection);
            swap_if(first[1u], first[9u], compare, projection);
            swap_if(first[2u], first[10u], compare, projection);
            swap_if(first[3u], first[11u], compare, projection);
            swap_if(first[4u], first[12u], compare, projection);
            swap_if(first[5u], first[13u], compare, projection);
            swap_if(first[6u], first[14u], compare, projection);
            swap_if(first[5u], first[10u], compare, projection);
            swap_if(first[6u], first[9u], compare, projection);
            swap_if(first[3u], first[12u], compare, projection);
            swap_if(first[13u], first[14u], compare, projection);
            swap_if(first[7u], first[11u], compare, projection);
            swap_if(first[1u], first[2u], compare, projection);
            swap_if(first[4u], first[8u], compare, projection);
            swap_if(first[1u], first[4u], compare, projection);
            swap_if(first[7u], first[13u], compare, projection);
            swap_if(first[2u], first[8u], compare, projection);
            swap_if(first[11u], first[14u], compare, projection);
            swap_if(first[2u], first[4u], compare, projection);
            swap_if(first[5u], first[6u], compare, projection);
            swap_if(first[9u], first[10u], compare, projection);
            swap_if(first[11u], first[13u], compare, projection);
            swap_if(first[3u], first[8u], compare, projection);
            swap_if(first[7u], first[12u], compare, projection);
            swap_if(first[6u], first[8u], compare, projection);
            swap_if(first[10u], first[12u], compare, projection);
            swap_if(first[3u], first[5u], compare, projection);
            swap_if(first[7u], first[9u], compare, projection);
            swap_if(first[3u], first[4u], compare, projection);
            swap_if(first[5u], first[6u], compare, projection);
            swap_if(first[7u], first[8u], compare, projection);
            swap_if(first[9u], first[10u], compare, projection);
            swap_if(first[11u], first[12u], compare, projection);
            swap_if(first[6u], first[7u], compare, projection);
            swap_if(first[8u], first[9u], compare, projection);
        }
    };
}}

#endif // CPPSORT_DETAIL_SORTING_NETWORK_SORT15_H_
