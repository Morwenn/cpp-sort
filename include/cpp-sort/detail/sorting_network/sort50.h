/*
* Copyright (c) 2023 Morwenn
* SPDX-License-Identifier: MIT
*/
#ifndef CPPSORT_DETAIL_SORTING_NETWORK_SORT50_H_
#define CPPSORT_DETAIL_SORTING_NETWORK_SORT50_H_

namespace cppsort
{
namespace detail
{
    template<>
    struct sorting_network_sorter_impl<50>
    {
        template<
            typename RandomAccessIterator,
            typename Compare = std::less<>,
            typename Projection = utility::identity,
            typename = detail::enable_if_t<is_projection_iterator_v<
                Projection, RandomAccessIterator, Compare
            >>
        >
        auto operator()(RandomAccessIterator first, RandomAccessIterator,
                        Compare compare={}, Projection projection={}) const
            -> void
        {
            iter_swap_if(first, first + 8, compare, projection);
            iter_swap_if(first + 1, first + 7, compare, projection);
            iter_swap_if(first + 2, first + 6, compare, projection);
            iter_swap_if(first + 3, first + 11, compare, projection);
            iter_swap_if(first + 4, first + 10, compare, projection);
            iter_swap_if(first + 5, first + 9, compare, projection);
            iter_swap_if(first + 12, first + 24, compare, projection);
            iter_swap_if(first + 13, first + 22, compare, projection);
            iter_swap_if(first + 14, first + 21, compare, projection);
            iter_swap_if(first + 15, first + 19, compare, projection);
            iter_swap_if(first + 17, first + 23, compare, projection);
            iter_swap_if(first + 18, first + 20, compare, projection);
            iter_swap_if(first + 25, first + 33, compare, projection);
            iter_swap_if(first + 26, first + 32, compare, projection);
            iter_swap_if(first + 27, first + 31, compare, projection);
            iter_swap_if(first + 28, first + 36, compare, projection);
            iter_swap_if(first + 29, first + 35, compare, projection);
            iter_swap_if(first + 30, first + 34, compare, projection);
            iter_swap_if(first + 37, first + 49, compare, projection);
            iter_swap_if(first + 38, first + 47, compare, projection);
            iter_swap_if(first + 39, first + 46, compare, projection);
            iter_swap_if(first + 40, first + 44, compare, projection);
            iter_swap_if(first + 42, first + 48, compare, projection);
            iter_swap_if(first + 43, first + 45, compare, projection);
            iter_swap_if(first, first + 1, compare, projection);
            iter_swap_if(first + 2, first + 5, compare, projection);
            iter_swap_if(first + 3, first + 4, compare, projection);
            iter_swap_if(first + 6, first + 9, compare, projection);
            iter_swap_if(first + 7, first + 8, compare, projection);
            iter_swap_if(first + 10, first + 11, compare, projection);
            iter_swap_if(first + 13, first + 18, compare, projection);
            iter_swap_if(first + 14, first + 15, compare, projection);
            iter_swap_if(first + 16, first + 23, compare, projection);
            iter_swap_if(first + 19, first + 21, compare, projection);
            iter_swap_if(first + 20, first + 22, compare, projection);
            iter_swap_if(first + 25, first + 26, compare, projection);
            iter_swap_if(first + 27, first + 30, compare, projection);
            iter_swap_if(first + 28, first + 29, compare, projection);
            iter_swap_if(first + 31, first + 34, compare, projection);
            iter_swap_if(first + 32, first + 33, compare, projection);
            iter_swap_if(first + 35, first + 36, compare, projection);
            iter_swap_if(first + 38, first + 43, compare, projection);
            iter_swap_if(first + 39, first + 40, compare, projection);
            iter_swap_if(first + 41, first + 48, compare, projection);
            iter_swap_if(first + 44, first + 46, compare, projection);
            iter_swap_if(first + 45, first + 47, compare, projection);
            iter_swap_if(first, first + 2, compare, projection);
            iter_swap_if(first + 1, first + 6, compare, projection);
            iter_swap_if(first + 5, first + 10, compare, projection);
            iter_swap_if(first + 9, first + 11, compare, projection);
            iter_swap_if(first + 12, first + 16, compare, projection);
            iter_swap_if(first + 13, first + 14, compare, projection);
            iter_swap_if(first + 15, first + 18, compare, projection);
            iter_swap_if(first + 19, first + 20, compare, projection);
            iter_swap_if(first + 21, first + 22, compare, projection);
            iter_swap_if(first + 23, first + 24, compare, projection);
            iter_swap_if(first + 25, first + 27, compare, projection);
            iter_swap_if(first + 26, first + 31, compare, projection);
            iter_swap_if(first + 30, first + 35, compare, projection);
            iter_swap_if(first + 34, first + 36, compare, projection);
            iter_swap_if(first + 37, first + 41, compare, projection);
            iter_swap_if(first + 38, first + 39, compare, projection);
            iter_swap_if(first + 40, first + 43, compare, projection);
            iter_swap_if(first + 44, first + 45, compare, projection);
            iter_swap_if(first + 46, first + 47, compare, projection);
            iter_swap_if(first + 48, first + 49, compare, projection);
            iter_swap_if(first, first + 3, compare, projection);
            iter_swap_if(first + 1, first + 2, compare, projection);
            iter_swap_if(first + 4, first + 6, compare, projection);
            iter_swap_if(first + 5, first + 7, compare, projection);
            iter_swap_if(first + 8, first + 11, compare, projection);
            iter_swap_if(first + 9, first + 10, compare, projection);
            iter_swap_if(first + 16, first + 18, compare, projection);
            iter_swap_if(first + 17, first + 21, compare, projection);
            iter_swap_if(first + 20, first + 23, compare, projection);
            iter_swap_if(first + 22, first + 24, compare, projection);
            iter_swap_if(first + 25, first + 28, compare, projection);
            iter_swap_if(first + 26, first + 27, compare, projection);
            iter_swap_if(first + 29, first + 31, compare, projection);
            iter_swap_if(first + 30, first + 32, compare, projection);
            iter_swap_if(first + 33, first + 36, compare, projection);
            iter_swap_if(first + 34, first + 35, compare, projection);
            iter_swap_if(first + 41, first + 43, compare, projection);
            iter_swap_if(first + 42, first + 46, compare, projection);
            iter_swap_if(first + 45, first + 48, compare, projection);
            iter_swap_if(first + 47, first + 49, compare, projection);
            iter_swap_if(first, first + 25, compare, projection);
            iter_swap_if(first + 1, first + 4, compare, projection);
            iter_swap_if(first + 3, first + 5, compare, projection);
            iter_swap_if(first + 6, first + 8, compare, projection);
            iter_swap_if(first + 7, first + 10, compare, projection);
            iter_swap_if(first + 12, first + 17, compare, projection);
            iter_swap_if(first + 15, first + 20, compare, projection);
            iter_swap_if(first + 16, first + 19, compare, projection);
            iter_swap_if(first + 18, first + 23, compare, projection);
            iter_swap_if(first + 21, first + 22, compare, projection);
            iter_swap_if(first + 26, first + 29, compare, projection);
            iter_swap_if(first + 28, first + 30, compare, projection);
            iter_swap_if(first + 31, first + 33, compare, projection);
            iter_swap_if(first + 32, first + 35, compare, projection);
            iter_swap_if(first + 37, first + 42, compare, projection);
            iter_swap_if(first + 40, first + 45, compare, projection);
            iter_swap_if(first + 41, first + 44, compare, projection);
            iter_swap_if(first + 43, first + 48, compare, projection);
            iter_swap_if(first + 46, first + 47, compare, projection);
            iter_swap_if(first + 1, first + 3, compare, projection);
            iter_swap_if(first + 2, first + 5, compare, projection);
            iter_swap_if(first + 6, first + 9, compare, projection);
            iter_swap_if(first + 8, first + 10, compare, projection);
            iter_swap_if(first + 12, first + 13, compare, projection);
            iter_swap_if(first + 14, first + 17, compare, projection);
            iter_swap_if(first + 18, first + 21, compare, projection);
            iter_swap_if(first + 19, first + 20, compare, projection);
            iter_swap_if(first + 22, first + 23, compare, projection);
            iter_swap_if(first + 26, first + 28, compare, projection);
            iter_swap_if(first + 27, first + 30, compare, projection);
            iter_swap_if(first + 31, first + 34, compare, projection);
            iter_swap_if(first + 33, first + 35, compare, projection);
            iter_swap_if(first + 37, first + 38, compare, projection);
            iter_swap_if(first + 39, first + 42, compare, projection);
            iter_swap_if(first + 43, first + 46, compare, projection);
            iter_swap_if(first + 44, first + 45, compare, projection);
            iter_swap_if(first + 47, first + 48, compare, projection);
            iter_swap_if(first + 2, first + 3, compare, projection);
            iter_swap_if(first + 4, first + 5, compare, projection);
            iter_swap_if(first + 6, first + 7, compare, projection);
            iter_swap_if(first + 8, first + 9, compare, projection);
            iter_swap_if(first + 11, first + 23, compare, projection);
            iter_swap_if(first + 12, first + 37, compare, projection);
            iter_swap_if(first + 13, first + 15, compare, projection);
            iter_swap_if(first + 14, first + 16, compare, projection);
            iter_swap_if(first + 17, first + 18, compare, projection);
            iter_swap_if(first + 21, first + 22, compare, projection);
            iter_swap_if(first + 27, first + 28, compare, projection);
            iter_swap_if(first + 29, first + 30, compare, projection);
            iter_swap_if(first + 31, first + 32, compare, projection);
            iter_swap_if(first + 33, first + 34, compare, projection);
            iter_swap_if(first + 36, first + 48, compare, projection);
            iter_swap_if(first + 38, first + 40, compare, projection);
            iter_swap_if(first + 39, first + 41, compare, projection);
            iter_swap_if(first + 42, first + 43, compare, projection);
            iter_swap_if(first + 46, first + 47, compare, projection);
            iter_swap_if(first, first + 12, compare, projection);
            iter_swap_if(first + 4, first + 6, compare, projection);
            iter_swap_if(first + 5, first + 7, compare, projection);
            iter_swap_if(first + 10, first + 22, compare, projection);
            iter_swap_if(first + 11, first + 36, compare, projection);
            iter_swap_if(first + 13, first + 14, compare, projection);
            iter_swap_if(first + 15, first + 16, compare, projection);
            iter_swap_if(first + 17, first + 19, compare, projection);
            iter_swap_if(first + 18, first + 20, compare, projection);
            iter_swap_if(first + 23, first + 48, compare, projection);
            iter_swap_if(first + 29, first + 31, compare, projection);
            iter_swap_if(first + 30, first + 32, compare, projection);
            iter_swap_if(first + 35, first + 47, compare, projection);
            iter_swap_if(first + 38, first + 39, compare, projection);
            iter_swap_if(first + 40, first + 41, compare, projection);
            iter_swap_if(first + 42, first + 44, compare, projection);
            iter_swap_if(first + 43, first + 45, compare, projection);
            iter_swap_if(first + 1, first + 13, compare, projection);
            iter_swap_if(first + 3, first + 4, compare, projection);
            iter_swap_if(first + 5, first + 6, compare, projection);
            iter_swap_if(first + 7, first + 8, compare, projection);
            iter_swap_if(first + 10, first + 35, compare, projection);
            iter_swap_if(first + 14, first + 15, compare, projection);
            iter_swap_if(first + 16, first + 17, compare, projection);
            iter_swap_if(first + 18, first + 19, compare, projection);
            iter_swap_if(first + 20, first + 21, compare, projection);
            iter_swap_if(first + 22, first + 47, compare, projection);
            iter_swap_if(first + 23, first + 36, compare, projection);
            iter_swap_if(first + 26, first + 38, compare, projection);
            iter_swap_if(first + 28, first + 29, compare, projection);
            iter_swap_if(first + 30, first + 31, compare, projection);
            iter_swap_if(first + 32, first + 33, compare, projection);
            iter_swap_if(first + 39, first + 40, compare, projection);
            iter_swap_if(first + 41, first + 42, compare, projection);
            iter_swap_if(first + 43, first + 44, compare, projection);
            iter_swap_if(first + 45, first + 46, compare, projection);
            iter_swap_if(first + 1, first + 26, compare, projection);
            iter_swap_if(first + 2, first + 14, compare, projection);
            iter_swap_if(first + 4, first + 29, compare, projection);
            iter_swap_if(first + 5, first + 30, compare, projection);
            iter_swap_if(first + 6, first + 31, compare, projection);
            iter_swap_if(first + 7, first + 32, compare, projection);
            iter_swap_if(first + 8, first + 49, compare, projection);
            iter_swap_if(first + 9, first + 21, compare, projection);
            iter_swap_if(first + 13, first + 38, compare, projection);
            iter_swap_if(first + 15, first + 16, compare, projection);
            iter_swap_if(first + 17, first + 18, compare, projection);
            iter_swap_if(first + 19, first + 44, compare, projection);
            iter_swap_if(first + 20, first + 45, compare, projection);
            iter_swap_if(first + 22, first + 35, compare, projection);
            iter_swap_if(first + 24, first + 33, compare, projection);
            iter_swap_if(first + 27, first + 39, compare, projection);
            iter_swap_if(first + 34, first + 46, compare, projection);
            iter_swap_if(first + 40, first + 41, compare, projection);
            iter_swap_if(first + 42, first + 43, compare, projection);
            iter_swap_if(first + 2, first + 27, compare, projection);
            iter_swap_if(first + 3, first + 15, compare, projection);
            iter_swap_if(first + 4, first + 37, compare, projection);
            iter_swap_if(first + 7, first + 19, compare, projection);
            iter_swap_if(first + 8, first + 24, compare, projection);
            iter_swap_if(first + 9, first + 34, compare, projection);
            iter_swap_if(first + 13, first + 26, compare, projection);
            iter_swap_if(first + 14, first + 39, compare, projection);
            iter_swap_if(first + 16, first + 41, compare, projection);
            iter_swap_if(first + 17, first + 42, compare, projection);
            iter_swap_if(first + 18, first + 43, compare, projection);
            iter_swap_if(first + 21, first + 46, compare, projection);
            iter_swap_if(first + 28, first + 40, compare, projection);
            iter_swap_if(first + 29, first + 45, compare, projection);
            iter_swap_if(first + 32, first + 44, compare, projection);
            iter_swap_if(first + 33, first + 49, compare, projection);
            iter_swap_if(first + 3, first + 28, compare, projection);
            iter_swap_if(first + 4, first + 12, compare, projection);
            iter_swap_if(first + 5, first + 17, compare, projection);
            iter_swap_if(first + 6, first + 18, compare, projection);
            iter_swap_if(first + 9, first + 38, compare, projection);
            iter_swap_if(first + 10, first + 39, compare, projection);
            iter_swap_if(first + 14, first + 27, compare, projection);
            iter_swap_if(first + 15, first + 40, compare, projection);
            iter_swap_if(first + 16, first + 25, compare, projection);
            iter_swap_if(first + 19, first + 32, compare, projection);
            iter_swap_if(first + 20, first + 29, compare, projection);
            iter_swap_if(first + 21, first + 34, compare, projection);
            iter_swap_if(first + 24, first + 33, compare, projection);
            iter_swap_if(first + 30, first + 42, compare, projection);
            iter_swap_if(first + 31, first + 43, compare, projection);
            iter_swap_if(first + 36, first + 44, compare, projection);
            iter_swap_if(first + 48, first + 49, compare, projection);
            iter_swap_if(first + 5, first + 13, compare, projection);
            iter_swap_if(first + 6, first + 14, compare, projection);
            iter_swap_if(first + 8, first + 16, compare, projection);
            iter_swap_if(first + 9, first + 26, compare, projection);
            iter_swap_if(first + 10, first + 27, compare, projection);
            iter_swap_if(first + 11, first + 40, compare, projection);
            iter_swap_if(first + 12, first + 20, compare, projection);
            iter_swap_if(first + 15, first + 28, compare, projection);
            iter_swap_if(first + 17, first + 30, compare, projection);
            iter_swap_if(first + 18, first + 31, compare, projection);
            iter_swap_if(first + 21, first + 38, compare, projection);
            iter_swap_if(first + 22, first + 39, compare, projection);
            iter_swap_if(first + 25, first + 41, compare, projection);
            iter_swap_if(first + 29, first + 37, compare, projection);
            iter_swap_if(first + 33, first + 45, compare, projection);
            iter_swap_if(first + 34, first + 42, compare, projection);
            iter_swap_if(first + 35, first + 43, compare, projection);
            iter_swap_if(first + 47, first + 48, compare, projection);
            iter_swap_if(first + 4, first + 8, compare, projection);
            iter_swap_if(first + 7, first + 15, compare, projection);
            iter_swap_if(first + 9, first + 13, compare, projection);
            iter_swap_if(first + 10, first + 14, compare, projection);
            iter_swap_if(first + 11, first + 28, compare, projection);
            iter_swap_if(first + 16, first + 25, compare, projection);
            iter_swap_if(first + 17, first + 26, compare, projection);
            iter_swap_if(first + 18, first + 27, compare, projection);
            iter_swap_if(first + 20, first + 29, compare, projection);
            iter_swap_if(first + 21, first + 30, compare, projection);
            iter_swap_if(first + 22, first + 31, compare, projection);
            iter_swap_if(first + 23, first + 40, compare, projection);
            iter_swap_if(first + 24, first + 41, compare, projection);
            iter_swap_if(first + 34, first + 38, compare, projection);
            iter_swap_if(first + 35, first + 39, compare, projection);
            iter_swap_if(first + 43, first + 45, compare, projection);
            iter_swap_if(first + 1, first + 4, compare, projection);
            iter_swap_if(first + 3, first + 9, compare, projection);
            iter_swap_if(first + 8, first + 12, compare, projection);
            iter_swap_if(first + 11, first + 15, compare, projection);
            iter_swap_if(first + 13, first + 17, compare, projection);
            iter_swap_if(first + 14, first + 18, compare, projection);
            iter_swap_if(first + 16, first + 20, compare, projection);
            iter_swap_if(first + 19, first + 28, compare, projection);
            iter_swap_if(first + 21, first + 26, compare, projection);
            iter_swap_if(first + 22, first + 27, compare, projection);
            iter_swap_if(first + 23, first + 32, compare, projection);
            iter_swap_if(first + 24, first + 25, compare, projection);
            iter_swap_if(first + 30, first + 34, compare, projection);
            iter_swap_if(first + 31, first + 35, compare, projection);
            iter_swap_if(first + 33, first + 41, compare, projection);
            iter_swap_if(first + 36, first + 40, compare, projection);
            iter_swap_if(first + 2, first + 8, compare, projection);
            iter_swap_if(first + 7, first + 13, compare, projection);
            iter_swap_if(first + 11, first + 17, compare, projection);
            iter_swap_if(first + 12, first + 16, compare, projection);
            iter_swap_if(first + 15, first + 19, compare, projection);
            iter_swap_if(first + 23, first + 28, compare, projection);
            iter_swap_if(first + 24, first + 29, compare, projection);
            iter_swap_if(first + 25, first + 37, compare, projection);
            iter_swap_if(first + 32, first + 36, compare, projection);
            iter_swap_if(first + 39, first + 41, compare, projection);
            iter_swap_if(first + 40, first + 46, compare, projection);
            iter_swap_if(first + 2, first + 4, compare, projection);
            iter_swap_if(first + 5, first + 8, compare, projection);
            iter_swap_if(first + 6, first + 12, compare, projection);
            iter_swap_if(first + 10, first + 16, compare, projection);
            iter_swap_if(first + 11, first + 13, compare, projection);
            iter_swap_if(first + 15, first + 21, compare, projection);
            iter_swap_if(first + 19, first + 26, compare, projection);
            iter_swap_if(first + 20, first + 24, compare, projection);
            iter_swap_if(first + 23, first + 30, compare, projection);
            iter_swap_if(first + 25, first + 29, compare, projection);
            iter_swap_if(first + 28, first + 34, compare, projection);
            iter_swap_if(first + 32, first + 38, compare, projection);
            iter_swap_if(first + 33, first + 37, compare, projection);
            iter_swap_if(first + 36, first + 42, compare, projection);
            iter_swap_if(first + 40, first + 43, compare, projection);
            iter_swap_if(first + 45, first + 46, compare, projection);
            iter_swap_if(first + 6, first + 8, compare, projection);
            iter_swap_if(first + 7, first + 10, compare, projection);
            iter_swap_if(first + 9, first + 12, compare, projection);
            iter_swap_if(first + 14, first + 20, compare, projection);
            iter_swap_if(first + 18, first + 24, compare, projection);
            iter_swap_if(first + 22, first + 25, compare, projection);
            iter_swap_if(first + 27, first + 29, compare, projection);
            iter_swap_if(first + 31, first + 33, compare, projection);
            iter_swap_if(first + 35, first + 37, compare, projection);
            iter_swap_if(first + 36, first + 39, compare, projection);
            iter_swap_if(first + 41, first + 42, compare, projection);
            iter_swap_if(first + 44, first + 46, compare, projection);
            iter_swap_if(first + 3, first + 6, compare, projection);
            iter_swap_if(first + 7, first + 12, compare, projection);
            iter_swap_if(first + 14, first + 16, compare, projection);
            iter_swap_if(first + 15, first + 18, compare, projection);
            iter_swap_if(first + 17, first + 20, compare, projection);
            iter_swap_if(first + 19, first + 24, compare, projection);
            iter_swap_if(first + 21, first + 22, compare, projection);
            iter_swap_if(first + 23, first + 25, compare, projection);
            iter_swap_if(first + 26, first + 27, compare, projection);
            iter_swap_if(first + 28, first + 29, compare, projection);
            iter_swap_if(first + 30, first + 31, compare, projection);
            iter_swap_if(first + 32, first + 35, compare, projection);
            iter_swap_if(first + 33, first + 34, compare, projection);
            iter_swap_if(first + 37, first + 38, compare, projection);
            iter_swap_if(first + 40, first + 41, compare, projection);
            iter_swap_if(first + 42, first + 43, compare, projection);
            iter_swap_if(first + 44, first + 47, compare, projection);
            iter_swap_if(first + 3, first + 5, compare, projection);
            iter_swap_if(first + 6, first + 8, compare, projection);
            iter_swap_if(first + 7, first + 9, compare, projection);
            iter_swap_if(first + 10, first + 12, compare, projection);
            iter_swap_if(first + 11, first + 14, compare, projection);
            iter_swap_if(first + 13, first + 16, compare, projection);
            iter_swap_if(first + 15, first + 17, compare, projection);
            iter_swap_if(first + 18, first + 20, compare, projection);
            iter_swap_if(first + 19, first + 21, compare, projection);
            iter_swap_if(first + 22, first + 24, compare, projection);
            iter_swap_if(first + 23, first + 26, compare, projection);
            iter_swap_if(first + 25, first + 27, compare, projection);
            iter_swap_if(first + 28, first + 30, compare, projection);
            iter_swap_if(first + 29, first + 31, compare, projection);
            iter_swap_if(first + 32, first + 33, compare, projection);
            iter_swap_if(first + 34, first + 35, compare, projection);
            iter_swap_if(first + 36, first + 37, compare, projection);
            iter_swap_if(first + 38, first + 39, compare, projection);
            iter_swap_if(first + 41, first + 42, compare, projection);
            iter_swap_if(first + 44, first + 45, compare, projection);
            iter_swap_if(first + 46, first + 47, compare, projection);
            iter_swap_if(first + 3, first + 4, compare, projection);
            iter_swap_if(first + 7, first + 8, compare, projection);
            iter_swap_if(first + 11, first + 12, compare, projection);
            iter_swap_if(first + 13, first + 14, compare, projection);
            iter_swap_if(first + 15, first + 16, compare, projection);
            iter_swap_if(first + 17, first + 18, compare, projection);
            iter_swap_if(first + 19, first + 20, compare, projection);
            iter_swap_if(first + 21, first + 22, compare, projection);
            iter_swap_if(first + 23, first + 24, compare, projection);
            iter_swap_if(first + 25, first + 26, compare, projection);
            iter_swap_if(first + 27, first + 28, compare, projection);
            iter_swap_if(first + 29, first + 30, compare, projection);
            iter_swap_if(first + 31, first + 32, compare, projection);
            iter_swap_if(first + 33, first + 34, compare, projection);
            iter_swap_if(first + 35, first + 36, compare, projection);
            iter_swap_if(first + 37, first + 38, compare, projection);
            iter_swap_if(first + 39, first + 40, compare, projection);
            iter_swap_if(first + 43, first + 44, compare, projection);
            iter_swap_if(first + 47, first + 48, compare, projection);
        }

        template<typename DifferenceType=std::ptrdiff_t>
        CPPSORT_ATTRIBUTE_NODISCARD
        static constexpr auto index_pairs()
            -> std::array<utility::index_pair<DifferenceType>, 376>
        {
            return {{
                {0, 8}, {1, 7}, {2, 6}, {3, 11}, {4, 10}, {5, 9}, {12, 24}, {13, 22}, {14, 21}, {15, 19}, {17, 23}, {18, 20}, {25, 33}, {26, 32}, {27, 31}, {28, 36}, {29, 35}, {30, 34}, {37, 49}, {38, 47}, {39, 46}, {40, 44}, {42, 48}, {43, 45},
                {0, 1}, {2, 5}, {3, 4}, {6, 9}, {7, 8}, {10, 11}, {13, 18}, {14, 15}, {16, 23}, {19, 21}, {20, 22}, {25, 26}, {27, 30}, {28, 29}, {31, 34}, {32, 33}, {35, 36}, {38, 43}, {39, 40}, {41, 48}, {44, 46}, {45, 47},
                {0, 2}, {1, 6}, {5, 10}, {9, 11}, {12, 16}, {13, 14}, {15, 18}, {19, 20}, {21, 22}, {23, 24}, {25, 27}, {26, 31}, {30, 35}, {34, 36}, {37, 41}, {38, 39}, {40, 43}, {44, 45}, {46, 47}, {48, 49},
                {0, 3}, {1, 2}, {4, 6}, {5, 7}, {8, 11}, {9, 10}, {16, 18}, {17, 21}, {20, 23}, {22, 24}, {25, 28}, {26, 27}, {29, 31}, {30, 32}, {33, 36}, {34, 35}, {41, 43}, {42, 46}, {45, 48}, {47, 49},
                {0, 25}, {1, 4}, {3, 5}, {6, 8}, {7, 10}, {12, 17}, {15, 20}, {16, 19}, {18, 23}, {21, 22}, {26, 29}, {28, 30}, {31, 33}, {32, 35}, {37, 42}, {40, 45}, {41, 44}, {43, 48}, {46, 47},
                {1, 3}, {2, 5}, {6, 9}, {8, 10}, {12, 13}, {14, 17}, {18, 21}, {19, 20}, {22, 23}, {26, 28}, {27, 30}, {31, 34}, {33, 35}, {37, 38}, {39, 42}, {43, 46}, {44, 45}, {47, 48},
                {2, 3}, {4, 5}, {6, 7}, {8, 9}, {11, 23}, {12, 37}, {13, 15}, {14, 16}, {17, 18}, {21, 22}, {27, 28}, {29, 30}, {31, 32}, {33, 34}, {36, 48}, {38, 40}, {39, 41}, {42, 43}, {46, 47},
                {0, 12}, {4, 6}, {5, 7}, {10, 22}, {11, 36}, {13, 14}, {15, 16}, {17, 19}, {18, 20}, {23, 48}, {29, 31}, {30, 32}, {35, 47}, {38, 39}, {40, 41}, {42, 44}, {43, 45},
                {1, 13}, {3, 4}, {5, 6}, {7, 8}, {10, 35}, {14, 15}, {16, 17}, {18, 19}, {20, 21}, {22, 47}, {23, 36}, {26, 38}, {28, 29}, {30, 31}, {32, 33}, {39, 40}, {41, 42}, {43, 44}, {45, 46},
                {1, 26}, {2, 14}, {4, 29}, {5, 30}, {6, 31}, {7, 32}, {8, 49}, {9, 21}, {13, 38}, {15, 16}, {17, 18}, {19, 44}, {20, 45}, {22, 35}, {24, 33}, {27, 39}, {34, 46}, {40, 41}, {42, 43},
                {2, 27}, {3, 15}, {4, 37}, {7, 19}, {8, 24}, {9, 34}, {13, 26}, {14, 39}, {16, 41}, {17, 42}, {18, 43}, {21, 46}, {28, 40}, {29, 45}, {32, 44}, {33, 49},
                {3, 28}, {4, 12}, {5, 17}, {6, 18}, {9, 38}, {10, 39}, {14, 27}, {15, 40}, {16, 25}, {19, 32}, {20, 29}, {21, 34}, {24, 33}, {30, 42}, {31, 43}, {36, 44}, {48, 49},
                {5, 13}, {6, 14}, {8, 16}, {9, 26}, {10, 27}, {11, 40}, {12, 20}, {15, 28}, {17, 30}, {18, 31}, {21, 38}, {22, 39}, {25, 41}, {29, 37}, {33, 45}, {34, 42}, {35, 43}, {47, 48},
                {4, 8}, {7, 15}, {9, 13}, {10, 14}, {11, 28}, {16, 25}, {17, 26}, {18, 27}, {20, 29}, {21, 30}, {22, 31}, {23, 40}, {24, 41}, {34, 38}, {35, 39}, {43, 45},
                {1, 4}, {3, 9}, {8, 12}, {11, 15}, {13, 17}, {14, 18}, {16, 20}, {19, 28}, {21, 26}, {22, 27}, {23, 32}, {24, 25}, {30, 34}, {31, 35}, {33, 41}, {36, 40},
                {2, 8}, {7, 13}, {11, 17}, {12, 16}, {15, 19}, {23, 28}, {24, 29}, {25, 37}, {32, 36}, {39, 41}, {40, 46},
                {2, 4}, {5, 8}, {6, 12}, {10, 16}, {11, 13}, {15, 21}, {19, 26}, {20, 24}, {23, 30}, {25, 29}, {28, 34}, {32, 38}, {33, 37}, {36, 42}, {40, 43}, {45, 46},
                {6, 8}, {7, 10}, {9, 12}, {14, 20}, {18, 24}, {22, 25}, {27, 29}, {31, 33}, {35, 37}, {36, 39}, {41, 42}, {44, 46},
                {3, 6}, {7, 12}, {14, 16}, {15, 18}, {17, 20}, {19, 24}, {21, 22}, {23, 25}, {26, 27}, {28, 29}, {30, 31}, {32, 35}, {33, 34}, {37, 38}, {40, 41}, {42, 43}, {44, 47},
                {3, 5}, {6, 8}, {7, 9}, {10, 12}, {11, 14}, {13, 16}, {15, 17}, {18, 20}, {19, 21}, {22, 24}, {23, 26}, {25, 27}, {28, 30}, {29, 31}, {32, 33}, {34, 35}, {36, 37}, {38, 39}, {41, 42}, {44, 45}, {46, 47},
                {3, 4}, {7, 8}, {11, 12}, {13, 14}, {15, 16}, {17, 18}, {19, 20}, {21, 22}, {23, 24}, {25, 26}, {27, 28}, {29, 30}, {31, 32}, {33, 34}, {35, 36}, {37, 38}, {39, 40}, {43, 44}, {47, 48},
            }};
        }
    };
}}

#endif // CPPSORT_DETAIL_SORTING_NETWORK_SORT50_H_
