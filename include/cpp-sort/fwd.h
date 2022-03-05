/*
 * Copyright (c) 2016-2022 Morwenn
 * SPDX-License-Identifier: MIT
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

    struct adaptive_shivers_sorter;
    template<typename BufferProvider>
    struct block_sorter;
    struct cartesian_tree_sorter;
    struct counting_sorter;
    struct default_sorter;
    struct drop_merge_sorter;
    struct float_spread_sorter;
    template<typename BufferProvider>
    struct grail_sorter;
    struct heap_sorter;
    struct insertion_sorter;
    struct integer_spread_sorter;
    struct mel_sorter;
    struct merge_insertion_sorter;
    struct merge_sorter;
    struct pdq_sorter;
    struct poplar_sorter;
    struct quick_merge_sorter;
    struct quick_sorter;
    struct selection_sorter;
    struct ska_sorter;
    struct slab_sorter;
    struct smooth_sorter;
    struct spin_sorter;
    struct split_sorter;
    struct spread_sorter;
    struct std_sorter;
    struct string_spread_sorter;
    struct tim_sorter;
    struct verge_sorter;
    template<typename BufferProvider>
    struct wiki_sorter;

    ////////////////////////////////////////////////////////////
    // Fixed-size sorters

    template<std::size_t N>
    struct low_comparisons_sorter;
    template<std::size_t N>
    struct low_moves_sorter;
    template<std::size_t N>
    struct merge_exchange_network_sorter;
    template<std::size_t N>
    struct odd_even_merge_network_sorter;
    template<std::size_t N>
    struct sorting_network_sorter;

    ////////////////////////////////////////////////////////////
    // Sorter adapters

    template<typename Sorter>
    struct container_aware_adapter;
    template<typename Sorter, typename CountType=std::size_t>
    struct counting_adapter;
    template<typename... Sorters>
    struct hybrid_adapter;
    template<typename Sorter>
    struct indirect_adapter;
    template<typename Sorter>
    struct out_of_place_adapter;
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
