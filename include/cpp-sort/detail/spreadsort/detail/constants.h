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

//constant definitions for the Boost Sort library

//          Copyright Steven J. Ross 2001 - 2014
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/libs/sort for library home page.

#ifndef CPPSORT_DETAIL_SPREADSORT_DETAIL_CONSTANTS_H_
#define CPPSORT_DETAIL_SPREADSORT_DETAIL_CONSTANTS_H_

namespace cppsort
{
namespace detail
{
namespace spreadsort
{
namespace detail
{
    //Tuning constants
    //This should be tuned to your processor cache;
    //if you go too large you get cache misses on bins
    //The smaller this number, the less worst-case memory usage.
    //If too small, too many recursions slow down spreadsort
    enum { max_splits = 11,
    //It's better to have a few cache misses and finish sorting
    //than to run another iteration
    max_finishing_splits = max_splits + 1,
    //Sets the minimum number of items per bin.
    int_log_mean_bin_size = 2,
    //Used to force a comparison-based sorting for small bins, if it's faster.
    //Minimum value 1
    int_log_min_split_count = 9,
    //This is the minimum split count to use spreadsort when it will finish in one
    //iteration.  Make this larger the faster std::sort is relative to integer_sort.
    int_log_finishing_count = 31,
    //Sets the minimum number of items per bin for floating point.
    float_log_mean_bin_size = 2,
    //Used to force a comparison-based sorting for small bins, if it's faster.
    //Minimum value 1
    float_log_min_split_count = 8,
    //This is the minimum split count to use spreadsort when it will finish in one
    //iteration.  Make this larger the faster std::sort is relative to float_sort.
    float_log_finishing_count = 4,
    //There is a minimum size below which it is not worth using spreadsort
    min_sort_size = 1000 };
}}}}

#endif // CPPSORT_DETAIL_SPREADSORT_DETAIL_CONSTANTS_H_
