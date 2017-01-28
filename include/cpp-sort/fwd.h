/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2016-2017 Morwenn
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
#ifndef CPPSORT_FWD_H_
#define CPPSORT_FWD_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <cstddef>

namespace cppsort
{
    //
    // This header contains forward declarations for
    // every sorter and sorter adapter in the library,
    // which helps to specialize some of the adapters
    // or to provide information about some sorters
    // without actually having to include the whole
    // thing
    //

    ////////////////////////////////////////////////////////////
    // Sorters

    template<typename BufferProvider>
    struct block_sorter;
    struct counting_sorter;
    struct default_sorter;
    struct drop_merge_sorter;
    struct float_spread_sorter;
    template<typename BufferProvider>
    struct grail_sorter;
    struct heap_sorter;
    struct insertion_sorter;
    struct integer_spread_sorter;
    struct merge_insertion_sorter;
    struct merge_sorter;
    struct pdq_sorter;
    struct poplar_sorter;
    struct quick_sorter;
    struct selection_sorter;
    struct ska_sorter;
    struct smooth_sorter;
    struct spread_sorter;
    struct std_sorter;
    struct string_spread_sorter;
    struct tim_sorter;
    struct verge_sorter;

    ////////////////////////////////////////////////////////////
    // Fixed-size sorters

    template<std::size_t N>
    struct low_comparisons_sorter;
    template<std::size_t N>
    struct low_moves_sorter;
    template<std::size_t N>
    struct sorting_network_sorter;

    ////////////////////////////////////////////////////////////
    // Sorter adapters

    template<typename Sorter>
    struct container_aware_adapter;
    template<typename Sorter, typename CountType>
    struct counting_adapter;
    template<typename... Sorters>
    struct hybrid_adapter;
    template<typename Sorter>
    struct indirect_adapter;
    template<typename Sorter>
    struct schwartz_adapter;
    template<typename Sorter>
    struct self_sort_adapter;
    template<template<std::size_t> class FixedSizeSorter, typename Indices>
    struct small_array_adapter;
    template<typename Sorter>
    struct stable_adapter;
    template<typename Sorter>
    struct verge_adapter;
}

#endif // CPPSORT_FWD_H_
