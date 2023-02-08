/*
* Copyright (c) 2023 Morwenn
* SPDX-License-Identifier: MIT
*/
#ifndef CPPSORT_DETAIL_SORTING_NETWORK_SORT54_H_
#define CPPSORT_DETAIL_SORTING_NETWORK_SORT54_H_

namespace cppsort
{
namespace detail
{
    template<>
    struct sorting_network_sorter_impl<54>
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
            iter_swap_if(first, first + 13, compare, projection);
            iter_swap_if(first + 1, first + 12, compare, projection);
            iter_swap_if(first + 2, first + 15, compare, projection);
            iter_swap_if(first + 3, first + 14, compare, projection);
            iter_swap_if(first + 4, first + 8, compare, projection);
            iter_swap_if(first + 5, first + 6, compare, projection);
            iter_swap_if(first + 7, first + 11, compare, projection);
            iter_swap_if(first + 9, first + 10, compare, projection);
            iter_swap_if(first + 16, first + 34, compare, projection);
            iter_swap_if(first + 17, first + 33, compare, projection);
            iter_swap_if(first + 18, first + 32, compare, projection);
            iter_swap_if(first + 19, first + 37, compare, projection);
            iter_swap_if(first + 20, first + 36, compare, projection);
            iter_swap_if(first + 21, first + 35, compare, projection);
            iter_swap_if(first + 22, first + 30, compare, projection);
            iter_swap_if(first + 23, first + 31, compare, projection);
            iter_swap_if(first + 24, first + 29, compare, projection);
            iter_swap_if(first + 25, first + 27, compare, projection);
            iter_swap_if(first + 26, first + 28, compare, projection);
            iter_swap_if(first + 38, first + 51, compare, projection);
            iter_swap_if(first + 39, first + 50, compare, projection);
            iter_swap_if(first + 40, first + 53, compare, projection);
            iter_swap_if(first + 41, first + 52, compare, projection);
            iter_swap_if(first + 42, first + 46, compare, projection);
            iter_swap_if(first + 43, first + 44, compare, projection);
            iter_swap_if(first + 45, first + 49, compare, projection);
            iter_swap_if(first + 47, first + 48, compare, projection);
            iter_swap_if(first, first + 5, compare, projection);
            iter_swap_if(first + 1, first + 7, compare, projection);
            iter_swap_if(first + 2, first + 9, compare, projection);
            iter_swap_if(first + 3, first + 4, compare, projection);
            iter_swap_if(first + 6, first + 13, compare, projection);
            iter_swap_if(first + 8, first + 14, compare, projection);
            iter_swap_if(first + 10, first + 15, compare, projection);
            iter_swap_if(first + 11, first + 12, compare, projection);
            iter_swap_if(first + 16, first + 17, compare, projection);
            iter_swap_if(first + 18, first + 21, compare, projection);
            iter_swap_if(first + 19, first + 20, compare, projection);
            iter_swap_if(first + 22, first + 24, compare, projection);
            iter_swap_if(first + 23, first + 26, compare, projection);
            iter_swap_if(first + 27, first + 30, compare, projection);
            iter_swap_if(first + 29, first + 31, compare, projection);
            iter_swap_if(first + 32, first + 35, compare, projection);
            iter_swap_if(first + 33, first + 34, compare, projection);
            iter_swap_if(first + 36, first + 37, compare, projection);
            iter_swap_if(first + 38, first + 43, compare, projection);
            iter_swap_if(first + 39, first + 45, compare, projection);
            iter_swap_if(first + 40, first + 47, compare, projection);
            iter_swap_if(first + 41, first + 42, compare, projection);
            iter_swap_if(first + 44, first + 51, compare, projection);
            iter_swap_if(first + 46, first + 52, compare, projection);
            iter_swap_if(first + 48, first + 53, compare, projection);
            iter_swap_if(first + 49, first + 50, compare, projection);
            iter_swap_if(first, first + 1, compare, projection);
            iter_swap_if(first + 2, first + 3, compare, projection);
            iter_swap_if(first + 4, first + 5, compare, projection);
            iter_swap_if(first + 6, first + 8, compare, projection);
            iter_swap_if(first + 7, first + 9, compare, projection);
            iter_swap_if(first + 10, first + 11, compare, projection);
            iter_swap_if(first + 12, first + 13, compare, projection);
            iter_swap_if(first + 14, first + 15, compare, projection);
            iter_swap_if(first + 16, first + 18, compare, projection);
            iter_swap_if(first + 17, first + 32, compare, projection);
            iter_swap_if(first + 21, first + 36, compare, projection);
            iter_swap_if(first + 22, first + 25, compare, projection);
            iter_swap_if(first + 24, first + 26, compare, projection);
            iter_swap_if(first + 27, first + 29, compare, projection);
            iter_swap_if(first + 28, first + 31, compare, projection);
            iter_swap_if(first + 35, first + 37, compare, projection);
            iter_swap_if(first + 38, first + 39, compare, projection);
            iter_swap_if(first + 40, first + 41, compare, projection);
            iter_swap_if(first + 42, first + 43, compare, projection);
            iter_swap_if(first + 44, first + 46, compare, projection);
            iter_swap_if(first + 45, first + 47, compare, projection);
            iter_swap_if(first + 48, first + 49, compare, projection);
            iter_swap_if(first + 50, first + 51, compare, projection);
            iter_swap_if(first + 52, first + 53, compare, projection);
            iter_swap_if(first, first + 2, compare, projection);
            iter_swap_if(first + 1, first + 3, compare, projection);
            iter_swap_if(first + 4, first + 10, compare, projection);
            iter_swap_if(first + 5, first + 11, compare, projection);
            iter_swap_if(first + 6, first + 7, compare, projection);
            iter_swap_if(first + 8, first + 9, compare, projection);
            iter_swap_if(first + 12, first + 14, compare, projection);
            iter_swap_if(first + 13, first + 15, compare, projection);
            iter_swap_if(first + 16, first + 19, compare, projection);
            iter_swap_if(first + 17, first + 18, compare, projection);
            iter_swap_if(first + 20, first + 32, compare, projection);
            iter_swap_if(first + 21, first + 33, compare, projection);
            iter_swap_if(first + 22, first + 23, compare, projection);
            iter_swap_if(first + 25, first + 28, compare, projection);
            iter_swap_if(first + 30, first + 31, compare, projection);
            iter_swap_if(first + 34, first + 37, compare, projection);
            iter_swap_if(first + 35, first + 36, compare, projection);
            iter_swap_if(first + 38, first + 40, compare, projection);
            iter_swap_if(first + 39, first + 41, compare, projection);
            iter_swap_if(first + 42, first + 48, compare, projection);
            iter_swap_if(first + 43, first + 49, compare, projection);
            iter_swap_if(first + 44, first + 45, compare, projection);
            iter_swap_if(first + 46, first + 47, compare, projection);
            iter_swap_if(first + 50, first + 52, compare, projection);
            iter_swap_if(first + 51, first + 53, compare, projection);
            iter_swap_if(first, first + 16, compare, projection);
            iter_swap_if(first + 1, first + 2, compare, projection);
            iter_swap_if(first + 3, first + 12, compare, projection);
            iter_swap_if(first + 4, first + 6, compare, projection);
            iter_swap_if(first + 5, first + 7, compare, projection);
            iter_swap_if(first + 8, first + 10, compare, projection);
            iter_swap_if(first + 9, first + 11, compare, projection);
            iter_swap_if(first + 13, first + 14, compare, projection);
            iter_swap_if(first + 15, first + 31, compare, projection);
            iter_swap_if(first + 17, first + 20, compare, projection);
            iter_swap_if(first + 19, first + 21, compare, projection);
            iter_swap_if(first + 22, first + 38, compare, projection);
            iter_swap_if(first + 23, first + 27, compare, projection);
            iter_swap_if(first + 24, first + 25, compare, projection);
            iter_swap_if(first + 26, first + 30, compare, projection);
            iter_swap_if(first + 28, first + 29, compare, projection);
            iter_swap_if(first + 32, first + 34, compare, projection);
            iter_swap_if(first + 33, first + 36, compare, projection);
            iter_swap_if(first + 37, first + 53, compare, projection);
            iter_swap_if(first + 39, first + 40, compare, projection);
            iter_swap_if(first + 41, first + 50, compare, projection);
            iter_swap_if(first + 42, first + 44, compare, projection);
            iter_swap_if(first + 43, first + 45, compare, projection);
            iter_swap_if(first + 46, first + 48, compare, projection);
            iter_swap_if(first + 47, first + 49, compare, projection);
            iter_swap_if(first + 51, first + 52, compare, projection);
            iter_swap_if(first, first + 22, compare, projection);
            iter_swap_if(first + 1, first + 4, compare, projection);
            iter_swap_if(first + 2, first + 6, compare, projection);
            iter_swap_if(first + 5, first + 8, compare, projection);
            iter_swap_if(first + 7, first + 10, compare, projection);
            iter_swap_if(first + 9, first + 13, compare, projection);
            iter_swap_if(first + 11, first + 14, compare, projection);
            iter_swap_if(first + 15, first + 37, compare, projection);
            iter_swap_if(first + 16, first + 38, compare, projection);
            iter_swap_if(first + 17, first + 19, compare, projection);
            iter_swap_if(first + 18, first + 21, compare, projection);
            iter_swap_if(first + 23, first + 24, compare, projection);
            iter_swap_if(first + 25, first + 27, compare, projection);
            iter_swap_if(first + 26, first + 28, compare, projection);
            iter_swap_if(first + 29, first + 30, compare, projection);
            iter_swap_if(first + 31, first + 53, compare, projection);
            iter_swap_if(first + 32, first + 35, compare, projection);
            iter_swap_if(first + 34, first + 36, compare, projection);
            iter_swap_if(first + 39, first + 42, compare, projection);
            iter_swap_if(first + 40, first + 44, compare, projection);
            iter_swap_if(first + 43, first + 46, compare, projection);
            iter_swap_if(first + 45, first + 48, compare, projection);
            iter_swap_if(first + 47, first + 51, compare, projection);
            iter_swap_if(first + 49, first + 52, compare, projection);
            iter_swap_if(first + 2, first + 4, compare, projection);
            iter_swap_if(first + 3, first + 6, compare, projection);
            iter_swap_if(first + 9, first + 12, compare, projection);
            iter_swap_if(first + 11, first + 13, compare, projection);
            iter_swap_if(first + 14, first + 36, compare, projection);
            iter_swap_if(first + 16, first + 22, compare, projection);
            iter_swap_if(first + 17, first + 39, compare, projection);
            iter_swap_if(first + 18, first + 19, compare, projection);
            iter_swap_if(first + 20, first + 21, compare, projection);
            iter_swap_if(first + 24, first + 25, compare, projection);
            iter_swap_if(first + 26, first + 27, compare, projection);
            iter_swap_if(first + 28, first + 29, compare, projection);
            iter_swap_if(first + 31, first + 37, compare, projection);
            iter_swap_if(first + 32, first + 33, compare, projection);
            iter_swap_if(first + 34, first + 35, compare, projection);
            iter_swap_if(first + 40, first + 42, compare, projection);
            iter_swap_if(first + 41, first + 44, compare, projection);
            iter_swap_if(first + 47, first + 50, compare, projection);
            iter_swap_if(first + 49, first + 51, compare, projection);
            iter_swap_if(first + 1, first + 17, compare, projection);
            iter_swap_if(first + 2, first + 40, compare, projection);
            iter_swap_if(first + 3, first + 5, compare, projection);
            iter_swap_if(first + 6, first + 8, compare, projection);
            iter_swap_if(first + 7, first + 9, compare, projection);
            iter_swap_if(first + 10, first + 12, compare, projection);
            iter_swap_if(first + 13, first + 51, compare, projection);
            iter_swap_if(first + 18, first + 24, compare, projection);
            iter_swap_if(first + 20, first + 32, compare, projection);
            iter_swap_if(first + 21, first + 33, compare, projection);
            iter_swap_if(first + 25, first + 26, compare, projection);
            iter_swap_if(first + 27, first + 28, compare, projection);
            iter_swap_if(first + 29, first + 35, compare, projection);
            iter_swap_if(first + 36, first + 52, compare, projection);
            iter_swap_if(first + 41, first + 43, compare, projection);
            iter_swap_if(first + 44, first + 46, compare, projection);
            iter_swap_if(first + 45, first + 47, compare, projection);
            iter_swap_if(first + 48, first + 50, compare, projection);
            iter_swap_if(first + 2, first + 18, compare, projection);
            iter_swap_if(first + 3, first + 4, compare, projection);
            iter_swap_if(first + 5, first + 6, compare, projection);
            iter_swap_if(first + 7, first + 8, compare, projection);
            iter_swap_if(first + 9, first + 10, compare, projection);
            iter_swap_if(first + 11, first + 12, compare, projection);
            iter_swap_if(first + 14, first + 36, compare, projection);
            iter_swap_if(first + 17, first + 39, compare, projection);
            iter_swap_if(first + 19, first + 20, compare, projection);
            iter_swap_if(first + 21, first + 32, compare, projection);
            iter_swap_if(first + 33, first + 34, compare, projection);
            iter_swap_if(first + 35, first + 51, compare, projection);
            iter_swap_if(first + 41, first + 42, compare, projection);
            iter_swap_if(first + 43, first + 44, compare, projection);
            iter_swap_if(first + 45, first + 46, compare, projection);
            iter_swap_if(first + 47, first + 48, compare, projection);
            iter_swap_if(first + 49, first + 50, compare, projection);
            iter_swap_if(first + 2, first + 16, compare, projection);
            iter_swap_if(first + 5, first + 43, compare, projection);
            iter_swap_if(first + 6, first + 7, compare, projection);
            iter_swap_if(first + 8, first + 9, compare, projection);
            iter_swap_if(first + 10, first + 48, compare, projection);
            iter_swap_if(first + 17, first + 23, compare, projection);
            iter_swap_if(first + 20, first + 26, compare, projection);
            iter_swap_if(first + 21, first + 25, compare, projection);
            iter_swap_if(first + 27, first + 33, compare, projection);
            iter_swap_if(first + 28, first + 32, compare, projection);
            iter_swap_if(first + 30, first + 36, compare, projection);
            iter_swap_if(first + 37, first + 51, compare, projection);
            iter_swap_if(first + 44, first + 45, compare, projection);
            iter_swap_if(first + 46, first + 47, compare, projection);
            iter_swap_if(first + 3, first + 27, compare, projection);
            iter_swap_if(first + 6, first + 24, compare, projection);
            iter_swap_if(first + 8, first + 38, compare, projection);
            iter_swap_if(first + 9, first + 39, compare, projection);
            iter_swap_if(first + 10, first + 32, compare, projection);
            iter_swap_if(first + 11, first + 33, compare, projection);
            iter_swap_if(first + 13, first + 25, compare, projection);
            iter_swap_if(first + 14, first + 44, compare, projection);
            iter_swap_if(first + 15, first + 45, compare, projection);
            iter_swap_if(first + 17, first + 22, compare, projection);
            iter_swap_if(first + 19, first + 23, compare, projection);
            iter_swap_if(first + 20, first + 42, compare, projection);
            iter_swap_if(first + 21, first + 43, compare, projection);
            iter_swap_if(first + 26, first + 50, compare, projection);
            iter_swap_if(first + 28, first + 40, compare, projection);
            iter_swap_if(first + 29, first + 47, compare, projection);
            iter_swap_if(first + 30, first + 34, compare, projection);
            iter_swap_if(first + 31, first + 36, compare, projection);
            iter_swap_if(first + 1, first + 17, compare, projection);
            iter_swap_if(first + 3, first + 19, compare, projection);
            iter_swap_if(first + 4, first + 42, compare, projection);
            iter_swap_if(first + 5, first + 21, compare, projection);
            iter_swap_if(first + 6, first + 14, compare, projection);
            iter_swap_if(first + 7, first + 23, compare, projection);
            iter_swap_if(first + 9, first + 29, compare, projection);
            iter_swap_if(first + 11, first + 49, compare, projection);
            iter_swap_if(first + 12, first + 26, compare, projection);
            iter_swap_if(first + 18, first + 28, compare, projection);
            iter_swap_if(first + 24, first + 44, compare, projection);
            iter_swap_if(first + 25, first + 35, compare, projection);
            iter_swap_if(first + 27, first + 41, compare, projection);
            iter_swap_if(first + 30, first + 46, compare, projection);
            iter_swap_if(first + 32, first + 48, compare, projection);
            iter_swap_if(first + 34, first + 50, compare, projection);
            iter_swap_if(first + 36, first + 52, compare, projection);
            iter_swap_if(first + 39, first + 47, compare, projection);
            iter_swap_if(first + 1, first + 2, compare, projection);
            iter_swap_if(first + 4, first + 20, compare, projection);
            iter_swap_if(first + 5, first + 17, compare, projection);
            iter_swap_if(first + 6, first + 18, compare, projection);
            iter_swap_if(first + 7, first + 15, compare, projection);
            iter_swap_if(first + 8, first + 30, compare, projection);
            iter_swap_if(first + 10, first + 28, compare, projection);
            iter_swap_if(first + 19, first + 27, compare, projection);
            iter_swap_if(first + 21, first + 22, compare, projection);
            iter_swap_if(first + 23, first + 45, compare, projection);
            iter_swap_if(first + 25, first + 43, compare, projection);
            iter_swap_if(first + 26, first + 34, compare, projection);
            iter_swap_if(first + 31, first + 32, compare, projection);
            iter_swap_if(first + 33, first + 49, compare, projection);
            iter_swap_if(first + 35, first + 47, compare, projection);
            iter_swap_if(first + 36, first + 48, compare, projection);
            iter_swap_if(first + 38, first + 46, compare, projection);
            iter_swap_if(first + 51, first + 52, compare, projection);
            iter_swap_if(first + 3, first + 19, compare, projection);
            iter_swap_if(first + 8, first + 16, compare, projection);
            iter_swap_if(first + 9, first + 21, compare, projection);
            iter_swap_if(first + 10, first + 18, compare, projection);
            iter_swap_if(first + 11, first + 27, compare, projection);
            iter_swap_if(first + 12, first + 20, compare, projection);
            iter_swap_if(first + 13, first + 22, compare, projection);
            iter_swap_if(first + 14, first + 28, compare, projection);
            iter_swap_if(first + 15, first + 23, compare, projection);
            iter_swap_if(first + 25, first + 39, compare, projection);
            iter_swap_if(first + 26, first + 42, compare, projection);
            iter_swap_if(first + 30, first + 38, compare, projection);
            iter_swap_if(first + 31, first + 40, compare, projection);
            iter_swap_if(first + 32, first + 44, compare, projection);
            iter_swap_if(first + 33, first + 41, compare, projection);
            iter_swap_if(first + 34, first + 50, compare, projection);
            iter_swap_if(first + 35, first + 43, compare, projection);
            iter_swap_if(first + 37, first + 45, compare, projection);
            iter_swap_if(first + 4, first + 9, compare, projection);
            iter_swap_if(first + 7, first + 19, compare, projection);
            iter_swap_if(first + 12, first + 16, compare, projection);
            iter_swap_if(first + 13, first + 21, compare, projection);
            iter_swap_if(first + 20, first + 26, compare, projection);
            iter_swap_if(first + 22, first + 29, compare, projection);
            iter_swap_if(first + 24, first + 31, compare, projection);
            iter_swap_if(first + 27, first + 33, compare, projection);
            iter_swap_if(first + 32, first + 40, compare, projection);
            iter_swap_if(first + 34, first + 46, compare, projection);
            iter_swap_if(first + 37, first + 41, compare, projection);
            iter_swap_if(first + 44, first + 49, compare, projection);
            iter_swap_if(first + 4, first + 8, compare, projection);
            iter_swap_if(first + 9, first + 12, compare, projection);
            iter_swap_if(first + 11, first + 19, compare, projection);
            iter_swap_if(first + 13, first + 17, compare, projection);
            iter_swap_if(first + 14, first + 24, compare, projection);
            iter_swap_if(first + 15, first + 27, compare, projection);
            iter_swap_if(first + 20, first + 30, compare, projection);
            iter_swap_if(first + 22, first + 25, compare, projection);
            iter_swap_if(first + 23, first + 33, compare, projection);
            iter_swap_if(first + 26, first + 38, compare, projection);
            iter_swap_if(first + 28, first + 31, compare, projection);
            iter_swap_if(first + 29, first + 39, compare, projection);
            iter_swap_if(first + 34, first + 42, compare, projection);
            iter_swap_if(first + 36, first + 40, compare, projection);
            iter_swap_if(first + 41, first + 44, compare, projection);
            iter_swap_if(first + 45, first + 49, compare, projection);
            iter_swap_if(first + 3, first + 13, compare, projection);
            iter_swap_if(first + 6, first + 9, compare, projection);
            iter_swap_if(first + 10, first + 12, compare, projection);
            iter_swap_if(first + 11, first + 17, compare, projection);
            iter_swap_if(first + 14, first + 16, compare, projection);
            iter_swap_if(first + 15, first + 22, compare, projection);
            iter_swap_if(first + 18, first + 20, compare, projection);
            iter_swap_if(first + 19, first + 21, compare, projection);
            iter_swap_if(first + 23, first + 25, compare, projection);
            iter_swap_if(first + 24, first + 26, compare, projection);
            iter_swap_if(first + 27, first + 29, compare, projection);
            iter_swap_if(first + 28, first + 30, compare, projection);
            iter_swap_if(first + 31, first + 38, compare, projection);
            iter_swap_if(first + 32, first + 34, compare, projection);
            iter_swap_if(first + 33, first + 35, compare, projection);
            iter_swap_if(first + 36, first + 42, compare, projection);
            iter_swap_if(first + 37, first + 39, compare, projection);
            iter_swap_if(first + 40, first + 50, compare, projection);
            iter_swap_if(first + 41, first + 43, compare, projection);
            iter_swap_if(first + 44, first + 47, compare, projection);
            iter_swap_if(first + 3, first + 8, compare, projection);
            iter_swap_if(first + 7, first + 13, compare, projection);
            iter_swap_if(first + 9, first + 10, compare, projection);
            iter_swap_if(first + 14, first + 18, compare, projection);
            iter_swap_if(first + 15, first + 19, compare, projection);
            iter_swap_if(first + 16, first + 20, compare, projection);
            iter_swap_if(first + 21, first + 22, compare, projection);
            iter_swap_if(first + 23, first + 27, compare, projection);
            iter_swap_if(first + 24, first + 28, compare, projection);
            iter_swap_if(first + 25, first + 29, compare, projection);
            iter_swap_if(first + 26, first + 30, compare, projection);
            iter_swap_if(first + 31, first + 32, compare, projection);
            iter_swap_if(first + 33, first + 37, compare, projection);
            iter_swap_if(first + 34, first + 38, compare, projection);
            iter_swap_if(first + 35, first + 39, compare, projection);
            iter_swap_if(first + 40, first + 46, compare, projection);
            iter_swap_if(first + 43, first + 44, compare, projection);
            iter_swap_if(first + 45, first + 50, compare, projection);
            iter_swap_if(first + 3, first + 5, compare, projection);
            iter_swap_if(first + 6, first + 8, compare, projection);
            iter_swap_if(first + 7, first + 9, compare, projection);
            iter_swap_if(first + 11, first + 13, compare, projection);
            iter_swap_if(first + 12, first + 14, compare, projection);
            iter_swap_if(first + 15, first + 17, compare, projection);
            iter_swap_if(first + 16, first + 18, compare, projection);
            iter_swap_if(first + 19, first + 21, compare, projection);
            iter_swap_if(first + 20, first + 24, compare, projection);
            iter_swap_if(first + 22, first + 23, compare, projection);
            iter_swap_if(first + 25, first + 27, compare, projection);
            iter_swap_if(first + 26, first + 28, compare, projection);
            iter_swap_if(first + 29, first + 33, compare, projection);
            iter_swap_if(first + 30, first + 31, compare, projection);
            iter_swap_if(first + 32, first + 34, compare, projection);
            iter_swap_if(first + 35, first + 37, compare, projection);
            iter_swap_if(first + 36, first + 38, compare, projection);
            iter_swap_if(first + 39, first + 41, compare, projection);
            iter_swap_if(first + 40, first + 42, compare, projection);
            iter_swap_if(first + 44, first + 46, compare, projection);
            iter_swap_if(first + 45, first + 47, compare, projection);
            iter_swap_if(first + 48, first + 50, compare, projection);
            iter_swap_if(first + 3, first + 4, compare, projection);
            iter_swap_if(first + 5, first + 6, compare, projection);
            iter_swap_if(first + 7, first + 8, compare, projection);
            iter_swap_if(first + 10, first + 11, compare, projection);
            iter_swap_if(first + 12, first + 13, compare, projection);
            iter_swap_if(first + 14, first + 15, compare, projection);
            iter_swap_if(first + 16, first + 17, compare, projection);
            iter_swap_if(first + 18, first + 19, compare, projection);
            iter_swap_if(first + 20, first + 21, compare, projection);
            iter_swap_if(first + 22, first + 24, compare, projection);
            iter_swap_if(first + 23, first + 26, compare, projection);
            iter_swap_if(first + 25, first + 28, compare, projection);
            iter_swap_if(first + 27, first + 30, compare, projection);
            iter_swap_if(first + 29, first + 31, compare, projection);
            iter_swap_if(first + 32, first + 33, compare, projection);
            iter_swap_if(first + 34, first + 35, compare, projection);
            iter_swap_if(first + 36, first + 37, compare, projection);
            iter_swap_if(first + 38, first + 39, compare, projection);
            iter_swap_if(first + 40, first + 41, compare, projection);
            iter_swap_if(first + 42, first + 43, compare, projection);
            iter_swap_if(first + 45, first + 46, compare, projection);
            iter_swap_if(first + 47, first + 48, compare, projection);
            iter_swap_if(first + 49, first + 50, compare, projection);
            iter_swap_if(first + 9, first + 10, compare, projection);
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
            iter_swap_if(first + 41, first + 42, compare, projection);
            iter_swap_if(first + 43, first + 44, compare, projection);
        }

        template<typename DifferenceType=std::ptrdiff_t>
        CPPSORT_ATTRIBUTE_NODISCARD
        static constexpr auto index_pairs()
            -> std::array<utility::index_pair<DifferenceType>, 421>
        {
            return {{
                {0, 13}, {1, 12}, {2, 15}, {3, 14}, {4, 8}, {5, 6}, {7, 11}, {9, 10}, {16, 34}, {17, 33}, {18, 32}, {19, 37}, {20, 36}, {21, 35}, {22, 30}, {23, 31}, {24, 29}, {25, 27}, {26, 28}, {38, 51}, {39, 50}, {40, 53}, {41, 52}, {42, 46}, {43, 44}, {45, 49}, {47, 48},
                {0, 5}, {1, 7}, {2, 9}, {3, 4}, {6, 13}, {8, 14}, {10, 15}, {11, 12}, {16, 17}, {18, 21}, {19, 20}, {22, 24}, {23, 26}, {27, 30}, {29, 31}, {32, 35}, {33, 34}, {36, 37}, {38, 43}, {39, 45}, {40, 47}, {41, 42}, {44, 51}, {46, 52}, {48, 53}, {49, 50},
                {0, 1}, {2, 3}, {4, 5}, {6, 8}, {7, 9}, {10, 11}, {12, 13}, {14, 15}, {16, 18}, {17, 32}, {21, 36}, {22, 25}, {24, 26}, {27, 29}, {28, 31}, {35, 37}, {38, 39}, {40, 41}, {42, 43}, {44, 46}, {45, 47}, {48, 49}, {50, 51}, {52, 53},
                {0, 2}, {1, 3}, {4, 10}, {5, 11}, {6, 7}, {8, 9}, {12, 14}, {13, 15}, {16, 19}, {17, 18}, {20, 32}, {21, 33}, {22, 23}, {25, 28}, {30, 31}, {34, 37}, {35, 36}, {38, 40}, {39, 41}, {42, 48}, {43, 49}, {44, 45}, {46, 47}, {50, 52}, {51, 53},
                {0, 16}, {1, 2}, {3, 12}, {4, 6}, {5, 7}, {8, 10}, {9, 11}, {13, 14}, {15, 31}, {17, 20}, {19, 21}, {22, 38}, {23, 27}, {24, 25}, {26, 30}, {28, 29}, {32, 34}, {33, 36}, {37, 53}, {39, 40}, {41, 50}, {42, 44}, {43, 45}, {46, 48}, {47, 49}, {51, 52},
                {0, 22}, {1, 4}, {2, 6}, {5, 8}, {7, 10}, {9, 13}, {11, 14}, {15, 37}, {16, 38}, {17, 19}, {18, 21}, {23, 24}, {25, 27}, {26, 28}, {29, 30}, {31, 53}, {32, 35}, {34, 36}, {39, 42}, {40, 44}, {43, 46}, {45, 48}, {47, 51}, {49, 52},
                {2, 4}, {3, 6}, {9, 12}, {11, 13}, {14, 36}, {16, 22}, {17, 39}, {18, 19}, {20, 21}, {24, 25}, {26, 27}, {28, 29}, {31, 37}, {32, 33}, {34, 35}, {40, 42}, {41, 44}, {47, 50}, {49, 51},
                {1, 17}, {2, 40}, {3, 5}, {6, 8}, {7, 9}, {10, 12}, {13, 51}, {18, 24}, {20, 32}, {21, 33}, {25, 26}, {27, 28}, {29, 35}, {36, 52}, {41, 43}, {44, 46}, {45, 47}, {48, 50},
                {2, 18}, {3, 4}, {5, 6}, {7, 8}, {9, 10}, {11, 12}, {14, 36}, {17, 39}, {19, 20}, {21, 32}, {33, 34}, {35, 51}, {41, 42}, {43, 44}, {45, 46}, {47, 48}, {49, 50},
                {2, 16}, {5, 43}, {6, 7}, {8, 9}, {10, 48}, {17, 23}, {20, 26}, {21, 25}, {27, 33}, {28, 32}, {30, 36}, {37, 51}, {44, 45}, {46, 47},
                {3, 27}, {6, 24}, {8, 38}, {9, 39}, {10, 32}, {11, 33}, {13, 25}, {14, 44}, {15, 45}, {17, 22}, {19, 23}, {20, 42}, {21, 43}, {26, 50}, {28, 40}, {29, 47}, {30, 34}, {31, 36},
                {1, 17}, {3, 19}, {4, 42}, {5, 21}, {6, 14}, {7, 23}, {9, 29}, {11, 49}, {12, 26}, {18, 28}, {24, 44}, {25, 35}, {27, 41}, {30, 46}, {32, 48}, {34, 50}, {36, 52}, {39, 47},
                {1, 2}, {4, 20}, {5, 17}, {6, 18}, {7, 15}, {8, 30}, {10, 28}, {19, 27}, {21, 22}, {23, 45}, {25, 43}, {26, 34}, {31, 32}, {33, 49}, {35, 47}, {36, 48}, {38, 46}, {51, 52},
                {3, 19}, {8, 16}, {9, 21}, {10, 18}, {11, 27}, {12, 20}, {13, 22}, {14, 28}, {15, 23}, {25, 39}, {26, 42}, {30, 38}, {31, 40}, {32, 44}, {33, 41}, {34, 50}, {35, 43}, {37, 45},
                {4, 9}, {7, 19}, {12, 16}, {13, 21}, {20, 26}, {22, 29}, {24, 31}, {27, 33}, {32, 40}, {34, 46}, {37, 41}, {44, 49},
                {4, 8}, {9, 12}, {11, 19}, {13, 17}, {14, 24}, {15, 27}, {20, 30}, {22, 25}, {23, 33}, {26, 38}, {28, 31}, {29, 39}, {34, 42}, {36, 40}, {41, 44}, {45, 49},
                {3, 13}, {6, 9}, {10, 12}, {11, 17}, {14, 16}, {15, 22}, {18, 20}, {19, 21}, {23, 25}, {24, 26}, {27, 29}, {28, 30}, {31, 38}, {32, 34}, {33, 35}, {36, 42}, {37, 39}, {40, 50}, {41, 43}, {44, 47},
                {3, 8}, {7, 13}, {9, 10}, {14, 18}, {15, 19}, {16, 20}, {21, 22}, {23, 27}, {24, 28}, {25, 29}, {26, 30}, {31, 32}, {33, 37}, {34, 38}, {35, 39}, {40, 46}, {43, 44}, {45, 50},
                {3, 5}, {6, 8}, {7, 9}, {11, 13}, {12, 14}, {15, 17}, {16, 18}, {19, 21}, {20, 24}, {22, 23}, {25, 27}, {26, 28}, {29, 33}, {30, 31}, {32, 34}, {35, 37}, {36, 38}, {39, 41}, {40, 42}, {44, 46}, {45, 47}, {48, 50},
                {3, 4}, {5, 6}, {7, 8}, {10, 11}, {12, 13}, {14, 15}, {16, 17}, {18, 19}, {20, 21}, {22, 24}, {23, 26}, {25, 28}, {27, 30}, {29, 31}, {32, 33}, {34, 35}, {36, 37}, {38, 39}, {40, 41}, {42, 43}, {45, 46}, {47, 48}, {49, 50},
                {9, 10}, {11, 12}, {13, 14}, {15, 16}, {17, 18}, {19, 20}, {21, 22}, {23, 24}, {25, 26}, {27, 28}, {29, 30}, {31, 32}, {33, 34}, {35, 36}, {37, 38}, {39, 40}, {41, 42}, {43, 44},
            }};
        }
    };
}}

#endif // CPPSORT_DETAIL_SORTING_NETWORK_SORT54_H_
