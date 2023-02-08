/*
* Copyright (c) 2023 Morwenn
* SPDX-License-Identifier: MIT
*/
#ifndef CPPSORT_DETAIL_SORTING_NETWORK_SORT55_H_
#define CPPSORT_DETAIL_SORTING_NETWORK_SORT55_H_

namespace cppsort
{
namespace detail
{
    template<>
    struct sorting_network_sorter_impl<55>
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
            iter_swap_if(first + 1, first + 12, compare, projection);
            iter_swap_if(first + 2, first + 13, compare, projection);
            iter_swap_if(first + 3, first + 14, compare, projection);
            iter_swap_if(first + 4, first + 8, compare, projection);
            iter_swap_if(first + 5, first + 6, compare, projection);
            iter_swap_if(first + 7, first + 11, compare, projection);
            iter_swap_if(first + 9, first + 10, compare, projection);
            iter_swap_if(first + 15, first + 18, compare, projection);
            iter_swap_if(first + 16, first + 37, compare, projection);
            iter_swap_if(first + 17, first + 36, compare, projection);
            iter_swap_if(first + 19, first + 38, compare, projection);
            iter_swap_if(first + 20, first + 32, compare, projection);
            iter_swap_if(first + 21, first + 22, compare, projection);
            iter_swap_if(first + 23, first + 35, compare, projection);
            iter_swap_if(first + 24, first + 26, compare, projection);
            iter_swap_if(first + 25, first + 27, compare, projection);
            iter_swap_if(first + 28, first + 30, compare, projection);
            iter_swap_if(first + 29, first + 31, compare, projection);
            iter_swap_if(first + 33, first + 34, compare, projection);
            iter_swap_if(first + 39, first + 42, compare, projection);
            iter_swap_if(first + 40, first + 53, compare, projection);
            iter_swap_if(first + 41, first + 52, compare, projection);
            iter_swap_if(first + 43, first + 54, compare, projection);
            iter_swap_if(first + 44, first + 48, compare, projection);
            iter_swap_if(first + 45, first + 46, compare, projection);
            iter_swap_if(first + 47, first + 51, compare, projection);
            iter_swap_if(first + 49, first + 50, compare, projection);
            iter_swap_if(first, first + 5, compare, projection);
            iter_swap_if(first + 1, first + 7, compare, projection);
            iter_swap_if(first + 2, first + 9, compare, projection);
            iter_swap_if(first + 3, first + 4, compare, projection);
            iter_swap_if(first + 8, first + 14, compare, projection);
            iter_swap_if(first + 10, first + 13, compare, projection);
            iter_swap_if(first + 11, first + 12, compare, projection);
            iter_swap_if(first + 15, first + 33, compare, projection);
            iter_swap_if(first + 16, first + 21, compare, projection);
            iter_swap_if(first + 17, first + 23, compare, projection);
            iter_swap_if(first + 18, first + 34, compare, projection);
            iter_swap_if(first + 19, first + 20, compare, projection);
            iter_swap_if(first + 22, first + 37, compare, projection);
            iter_swap_if(first + 24, first + 28, compare, projection);
            iter_swap_if(first + 25, first + 29, compare, projection);
            iter_swap_if(first + 26, first + 30, compare, projection);
            iter_swap_if(first + 27, first + 31, compare, projection);
            iter_swap_if(first + 32, first + 38, compare, projection);
            iter_swap_if(first + 35, first + 36, compare, projection);
            iter_swap_if(first + 39, first + 49, compare, projection);
            iter_swap_if(first + 40, first + 45, compare, projection);
            iter_swap_if(first + 41, first + 47, compare, projection);
            iter_swap_if(first + 42, first + 50, compare, projection);
            iter_swap_if(first + 43, first + 44, compare, projection);
            iter_swap_if(first + 46, first + 53, compare, projection);
            iter_swap_if(first + 48, first + 54, compare, projection);
            iter_swap_if(first + 51, first + 52, compare, projection);
            iter_swap_if(first, first + 1, compare, projection);
            iter_swap_if(first + 2, first + 3, compare, projection);
            iter_swap_if(first + 4, first + 5, compare, projection);
            iter_swap_if(first + 6, first + 8, compare, projection);
            iter_swap_if(first + 7, first + 9, compare, projection);
            iter_swap_if(first + 10, first + 11, compare, projection);
            iter_swap_if(first + 13, first + 14, compare, projection);
            iter_swap_if(first + 15, first + 19, compare, projection);
            iter_swap_if(first + 16, first + 17, compare, projection);
            iter_swap_if(first + 18, first + 35, compare, projection);
            iter_swap_if(first + 20, first + 21, compare, projection);
            iter_swap_if(first + 22, first + 32, compare, projection);
            iter_swap_if(first + 23, first + 33, compare, projection);
            iter_swap_if(first + 24, first + 25, compare, projection);
            iter_swap_if(first + 26, first + 27, compare, projection);
            iter_swap_if(first + 28, first + 29, compare, projection);
            iter_swap_if(first + 30, first + 31, compare, projection);
            iter_swap_if(first + 34, first + 38, compare, projection);
            iter_swap_if(first + 36, first + 37, compare, projection);
            iter_swap_if(first + 39, first + 43, compare, projection);
            iter_swap_if(first + 40, first + 41, compare, projection);
            iter_swap_if(first + 42, first + 51, compare, projection);
            iter_swap_if(first + 44, first + 45, compare, projection);
            iter_swap_if(first + 46, first + 48, compare, projection);
            iter_swap_if(first + 47, first + 49, compare, projection);
            iter_swap_if(first + 50, first + 54, compare, projection);
            iter_swap_if(first + 52, first + 53, compare, projection);
            iter_swap_if(first, first + 2, compare, projection);
            iter_swap_if(first + 1, first + 3, compare, projection);
            iter_swap_if(first + 4, first + 10, compare, projection);
            iter_swap_if(first + 5, first + 11, compare, projection);
            iter_swap_if(first + 6, first + 7, compare, projection);
            iter_swap_if(first + 8, first + 9, compare, projection);
            iter_swap_if(first + 12, first + 13, compare, projection);
            iter_swap_if(first + 15, first + 16, compare, projection);
            iter_swap_if(first + 17, first + 19, compare, projection);
            iter_swap_if(first + 18, first + 20, compare, projection);
            iter_swap_if(first + 21, first + 35, compare, projection);
            iter_swap_if(first + 22, first + 23, compare, projection);
            iter_swap_if(first + 26, first + 28, compare, projection);
            iter_swap_if(first + 27, first + 29, compare, projection);
            iter_swap_if(first + 32, first + 33, compare, projection);
            iter_swap_if(first + 34, first + 36, compare, projection);
            iter_swap_if(first + 37, first + 38, compare, projection);
            iter_swap_if(first + 39, first + 40, compare, projection);
            iter_swap_if(first + 41, first + 43, compare, projection);
            iter_swap_if(first + 42, first + 44, compare, projection);
            iter_swap_if(first + 45, first + 51, compare, projection);
            iter_swap_if(first + 46, first + 47, compare, projection);
            iter_swap_if(first + 48, first + 49, compare, projection);
            iter_swap_if(first + 50, first + 52, compare, projection);
            iter_swap_if(first + 53, first + 54, compare, projection);
            iter_swap_if(first, first + 24, compare, projection);
            iter_swap_if(first + 1, first + 2, compare, projection);
            iter_swap_if(first + 3, first + 12, compare, projection);
            iter_swap_if(first + 4, first + 6, compare, projection);
            iter_swap_if(first + 5, first + 7, compare, projection);
            iter_swap_if(first + 8, first + 10, compare, projection);
            iter_swap_if(first + 9, first + 11, compare, projection);
            iter_swap_if(first + 13, first + 14, compare, projection);
            iter_swap_if(first + 15, first + 39, compare, projection);
            iter_swap_if(first + 16, first + 17, compare, projection);
            iter_swap_if(first + 18, first + 22, compare, projection);
            iter_swap_if(first + 19, first + 34, compare, projection);
            iter_swap_if(first + 20, first + 32, compare, projection);
            iter_swap_if(first + 21, first + 23, compare, projection);
            iter_swap_if(first + 25, first + 28, compare, projection);
            iter_swap_if(first + 27, first + 30, compare, projection);
            iter_swap_if(first + 31, first + 54, compare, projection);
            iter_swap_if(first + 33, first + 35, compare, projection);
            iter_swap_if(first + 36, first + 37, compare, projection);
            iter_swap_if(first + 40, first + 41, compare, projection);
            iter_swap_if(first + 42, first + 46, compare, projection);
            iter_swap_if(first + 43, first + 50, compare, projection);
            iter_swap_if(first + 44, first + 48, compare, projection);
            iter_swap_if(first + 45, first + 47, compare, projection);
            iter_swap_if(first + 49, first + 51, compare, projection);
            iter_swap_if(first + 52, first + 53, compare, projection);
            iter_swap_if(first, first + 15, compare, projection);
            iter_swap_if(first + 1, first + 4, compare, projection);
            iter_swap_if(first + 2, first + 6, compare, projection);
            iter_swap_if(first + 5, first + 8, compare, projection);
            iter_swap_if(first + 7, first + 10, compare, projection);
            iter_swap_if(first + 9, first + 13, compare, projection);
            iter_swap_if(first + 11, first + 14, compare, projection);
            iter_swap_if(first + 16, first + 18, compare, projection);
            iter_swap_if(first + 17, first + 22, compare, projection);
            iter_swap_if(first + 20, first + 21, compare, projection);
            iter_swap_if(first + 23, first + 32, compare, projection);
            iter_swap_if(first + 24, first + 39, compare, projection);
            iter_swap_if(first + 25, first + 26, compare, projection);
            iter_swap_if(first + 27, first + 28, compare, projection);
            iter_swap_if(first + 29, first + 30, compare, projection);
            iter_swap_if(first + 31, first + 38, compare, projection);
            iter_swap_if(first + 33, first + 36, compare, projection);
            iter_swap_if(first + 35, first + 37, compare, projection);
            iter_swap_if(first + 40, first + 42, compare, projection);
            iter_swap_if(first + 41, first + 46, compare, projection);
            iter_swap_if(first + 44, first + 45, compare, projection);
            iter_swap_if(first + 47, first + 48, compare, projection);
            iter_swap_if(first + 49, first + 52, compare, projection);
            iter_swap_if(first + 51, first + 53, compare, projection);
            iter_swap_if(first + 1, first + 16, compare, projection);
            iter_swap_if(first + 2, first + 4, compare, projection);
            iter_swap_if(first + 3, first + 6, compare, projection);
            iter_swap_if(first + 9, first + 12, compare, projection);
            iter_swap_if(first + 11, first + 13, compare, projection);
            iter_swap_if(first + 14, first + 30, compare, projection);
            iter_swap_if(first + 15, first + 24, compare, projection);
            iter_swap_if(first + 17, first + 18, compare, projection);
            iter_swap_if(first + 19, first + 22, compare, projection);
            iter_swap_if(first + 25, first + 40, compare, projection);
            iter_swap_if(first + 33, first + 34, compare, projection);
            iter_swap_if(first + 35, first + 36, compare, projection);
            iter_swap_if(first + 37, first + 53, compare, projection);
            iter_swap_if(first + 38, first + 54, compare, projection);
            iter_swap_if(first + 41, first + 42, compare, projection);
            iter_swap_if(first + 43, first + 46, compare, projection);
            iter_swap_if(first + 49, first + 50, compare, projection);
            iter_swap_if(first + 51, first + 52, compare, projection);
            iter_swap_if(first + 1, first + 25, compare, projection);
            iter_swap_if(first + 2, first + 26, compare, projection);
            iter_swap_if(first + 3, first + 5, compare, projection);
            iter_swap_if(first + 6, first + 8, compare, projection);
            iter_swap_if(first + 7, first + 9, compare, projection);
            iter_swap_if(first + 10, first + 12, compare, projection);
            iter_swap_if(first + 13, first + 36, compare, projection);
            iter_swap_if(first + 14, first + 37, compare, projection);
            iter_swap_if(first + 16, first + 40, compare, projection);
            iter_swap_if(first + 17, first + 41, compare, projection);
            iter_swap_if(first + 19, first + 20, compare, projection);
            iter_swap_if(first + 21, first + 22, compare, projection);
            iter_swap_if(first + 23, first + 33, compare, projection);
            iter_swap_if(first + 29, first + 52, compare, projection);
            iter_swap_if(first + 30, first + 53, compare, projection);
            iter_swap_if(first + 32, first + 34, compare, projection);
            iter_swap_if(first + 43, first + 44, compare, projection);
            iter_swap_if(first + 45, first + 46, compare, projection);
            iter_swap_if(first + 47, first + 49, compare, projection);
            iter_swap_if(first + 48, first + 50, compare, projection);
            iter_swap_if(first + 2, first + 17, compare, projection);
            iter_swap_if(first + 3, first + 4, compare, projection);
            iter_swap_if(first + 5, first + 6, compare, projection);
            iter_swap_if(first + 7, first + 8, compare, projection);
            iter_swap_if(first + 9, first + 10, compare, projection);
            iter_swap_if(first + 11, first + 12, compare, projection);
            iter_swap_if(first + 13, first + 29, compare, projection);
            iter_swap_if(first + 16, first + 25, compare, projection);
            iter_swap_if(first + 18, first + 19, compare, projection);
            iter_swap_if(first + 20, first + 21, compare, projection);
            iter_swap_if(first + 22, first + 23, compare, projection);
            iter_swap_if(first + 26, first + 41, compare, projection);
            iter_swap_if(first + 30, first + 37, compare, projection);
            iter_swap_if(first + 32, first + 33, compare, projection);
            iter_swap_if(first + 34, first + 35, compare, projection);
            iter_swap_if(first + 36, first + 52, compare, projection);
            iter_swap_if(first + 42, first + 43, compare, projection);
            iter_swap_if(first + 44, first + 45, compare, projection);
            iter_swap_if(first + 46, first + 47, compare, projection);
            iter_swap_if(first + 48, first + 49, compare, projection);
            iter_swap_if(first + 50, first + 51, compare, projection);
            iter_swap_if(first + 2, first + 15, compare, projection);
            iter_swap_if(first + 3, first + 27, compare, projection);
            iter_swap_if(first + 4, first + 43, compare, projection);
            iter_swap_if(first + 5, first + 44, compare, projection);
            iter_swap_if(first + 6, first + 7, compare, projection);
            iter_swap_if(first + 8, first + 9, compare, projection);
            iter_swap_if(first + 10, first + 49, compare, projection);
            iter_swap_if(first + 11, first + 50, compare, projection);
            iter_swap_if(first + 12, first + 35, compare, projection);
            iter_swap_if(first + 13, first + 25, compare, projection);
            iter_swap_if(first + 17, first + 26, compare, projection);
            iter_swap_if(first + 18, first + 42, compare, projection);
            iter_swap_if(first + 21, first + 22, compare, projection);
            iter_swap_if(first + 23, first + 32, compare, projection);
            iter_swap_if(first + 28, first + 51, compare, projection);
            iter_swap_if(first + 29, first + 36, compare, projection);
            iter_swap_if(first + 30, first + 41, compare, projection);
            iter_swap_if(first + 45, first + 46, compare, projection);
            iter_swap_if(first + 47, first + 48, compare, projection);
            iter_swap_if(first + 52, first + 54, compare, projection);
            iter_swap_if(first + 3, first + 18, compare, projection);
            iter_swap_if(first + 4, first + 19, compare, projection);
            iter_swap_if(first + 5, first + 20, compare, projection);
            iter_swap_if(first + 6, first + 21, compare, projection);
            iter_swap_if(first + 7, first + 31, compare, projection);
            iter_swap_if(first + 8, first + 23, compare, projection);
            iter_swap_if(first + 9, first + 40, compare, projection);
            iter_swap_if(first + 12, first + 28, compare, projection);
            iter_swap_if(first + 14, first + 45, compare, projection);
            iter_swap_if(first + 22, first + 46, compare, projection);
            iter_swap_if(first + 27, first + 42, compare, projection);
            iter_swap_if(first + 32, first + 48, compare, projection);
            iter_swap_if(first + 33, first + 49, compare, projection);
            iter_swap_if(first + 34, first + 50, compare, projection);
            iter_swap_if(first + 35, first + 51, compare, projection);
            iter_swap_if(first + 39, first + 47, compare, projection);
            iter_swap_if(first + 5, first + 13, compare, projection);
            iter_swap_if(first + 6, first + 14, compare, projection);
            iter_swap_if(first + 7, first + 22, compare, projection);
            iter_swap_if(first + 8, first + 39, compare, projection);
            iter_swap_if(first + 9, first + 32, compare, projection);
            iter_swap_if(first + 10, first + 33, compare, projection);
            iter_swap_if(first + 11, first + 34, compare, projection);
            iter_swap_if(first + 12, first + 24, compare, projection);
            iter_swap_if(first + 18, first + 27, compare, projection);
            iter_swap_if(first + 19, first + 43, compare, projection);
            iter_swap_if(first + 20, first + 44, compare, projection);
            iter_swap_if(first + 21, first + 45, compare, projection);
            iter_swap_if(first + 23, first + 47, compare, projection);
            iter_swap_if(first + 28, first + 35, compare, projection);
            iter_swap_if(first + 31, first + 46, compare, projection);
            iter_swap_if(first + 38, first + 42, compare, projection);
            iter_swap_if(first + 40, first + 48, compare, projection);
            iter_swap_if(first + 41, first + 49, compare, projection);
            iter_swap_if(first + 4, first + 12, compare, projection);
            iter_swap_if(first + 6, first + 17, compare, projection);
            iter_swap_if(first + 7, first + 18, compare, projection);
            iter_swap_if(first + 8, first + 15, compare, projection);
            iter_swap_if(first + 9, first + 16, compare, projection);
            iter_swap_if(first + 10, first + 26, compare, projection);
            iter_swap_if(first + 11, first + 27, compare, projection);
            iter_swap_if(first + 19, first + 24, compare, projection);
            iter_swap_if(first + 20, first + 25, compare, projection);
            iter_swap_if(first + 22, first + 31, compare, projection);
            iter_swap_if(first + 23, first + 39, compare, projection);
            iter_swap_if(first + 28, first + 43, compare, projection);
            iter_swap_if(first + 29, first + 44, compare, projection);
            iter_swap_if(first + 30, first + 33, compare, projection);
            iter_swap_if(first + 34, first + 38, compare, projection);
            iter_swap_if(first + 35, first + 47, compare, projection);
            iter_swap_if(first + 36, first + 48, compare, projection);
            iter_swap_if(first + 37, first + 45, compare, projection);
            iter_swap_if(first + 42, first + 50, compare, projection);
            iter_swap_if(first + 46, first + 52, compare, projection);
            iter_swap_if(first + 1, first + 4, compare, projection);
            iter_swap_if(first + 3, first + 9, compare, projection);
            iter_swap_if(first + 10, first + 17, compare, projection);
            iter_swap_if(first + 11, first + 18, compare, projection);
            iter_swap_if(first + 12, first + 19, compare, projection);
            iter_swap_if(first + 14, first + 30, compare, projection);
            iter_swap_if(first + 16, first + 20, compare, projection);
            iter_swap_if(first + 21, first + 26, compare, projection);
            iter_swap_if(first + 24, first + 28, compare, projection);
            iter_swap_if(first + 25, first + 40, compare, projection);
            iter_swap_if(first + 27, first + 34, compare, projection);
            iter_swap_if(first + 29, first + 32, compare, projection);
            iter_swap_if(first + 33, first + 37, compare, projection);
            iter_swap_if(first + 35, first + 43, compare, projection);
            iter_swap_if(first + 36, first + 44, compare, projection);
            iter_swap_if(first + 38, first + 42, compare, projection);
            iter_swap_if(first + 45, first + 51, compare, projection);
            iter_swap_if(first + 50, first + 53, compare, projection);
            iter_swap_if(first + 1, first + 2, compare, projection);
            iter_swap_if(first + 4, first + 8, compare, projection);
            iter_swap_if(first + 5, first + 9, compare, projection);
            iter_swap_if(first + 13, first + 20, compare, projection);
            iter_swap_if(first + 14, first + 21, compare, projection);
            iter_swap_if(first + 15, first + 19, compare, projection);
            iter_swap_if(first + 22, first + 27, compare, projection);
            iter_swap_if(first + 23, first + 24, compare, projection);
            iter_swap_if(first + 25, first + 29, compare, projection);
            iter_swap_if(first + 26, first + 30, compare, projection);
            iter_swap_if(first + 28, first + 39, compare, projection);
            iter_swap_if(first + 31, first + 34, compare, projection);
            iter_swap_if(first + 32, first + 40, compare, projection);
            iter_swap_if(first + 33, first + 41, compare, projection);
            iter_swap_if(first + 38, first + 46, compare, projection);
            iter_swap_if(first + 45, first + 49, compare, projection);
            iter_swap_if(first + 50, first + 52, compare, projection);
            iter_swap_if(first + 53, first + 54, compare, projection);
            iter_swap_if(first + 3, first + 4, compare, projection);
            iter_swap_if(first + 12, first + 15, compare, projection);
            iter_swap_if(first + 13, first + 16, compare, projection);
            iter_swap_if(first + 14, first + 23, compare, projection);
            iter_swap_if(first + 17, first + 19, compare, projection);
            iter_swap_if(first + 18, first + 25, compare, projection);
            iter_swap_if(first + 20, first + 22, compare, projection);
            iter_swap_if(first + 21, first + 26, compare, projection);
            iter_swap_if(first + 24, first + 28, compare, projection);
            iter_swap_if(first + 27, first + 31, compare, projection);
            iter_swap_if(first + 29, first + 32, compare, projection);
            iter_swap_if(first + 30, first + 35, compare, projection);
            iter_swap_if(first + 33, first + 39, compare, projection);
            iter_swap_if(first + 34, first + 40, compare, projection);
            iter_swap_if(first + 36, first + 38, compare, projection);
            iter_swap_if(first + 37, first + 41, compare, projection);
            iter_swap_if(first + 42, first + 46, compare, projection);
            iter_swap_if(first + 51, first + 52, compare, projection);
            iter_swap_if(first + 4, first + 8, compare, projection);
            iter_swap_if(first + 6, first + 12, compare, projection);
            iter_swap_if(first + 7, first + 13, compare, projection);
            iter_swap_if(first + 10, first + 15, compare, projection);
            iter_swap_if(first + 11, first + 16, compare, projection);
            iter_swap_if(first + 14, first + 17, compare, projection);
            iter_swap_if(first + 18, first + 20, compare, projection);
            iter_swap_if(first + 19, first + 23, compare, projection);
            iter_swap_if(first + 21, first + 24, compare, projection);
            iter_swap_if(first + 22, first + 25, compare, projection);
            iter_swap_if(first + 26, first + 28, compare, projection);
            iter_swap_if(first + 27, first + 29, compare, projection);
            iter_swap_if(first + 30, first + 33, compare, projection);
            iter_swap_if(first + 31, first + 32, compare, projection);
            iter_swap_if(first + 34, first + 36, compare, projection);
            iter_swap_if(first + 35, first + 39, compare, projection);
            iter_swap_if(first + 37, first + 43, compare, projection);
            iter_swap_if(first + 38, first + 40, compare, projection);
            iter_swap_if(first + 41, first + 47, compare, projection);
            iter_swap_if(first + 42, first + 44, compare, projection);
            iter_swap_if(first + 46, first + 48, compare, projection);
            iter_swap_if(first + 50, first + 51, compare, projection);
            iter_swap_if(first + 5, first + 8, compare, projection);
            iter_swap_if(first + 7, first + 10, compare, projection);
            iter_swap_if(first + 9, first + 12, compare, projection);
            iter_swap_if(first + 11, first + 14, compare, projection);
            iter_swap_if(first + 13, first + 15, compare, projection);
            iter_swap_if(first + 16, first + 17, compare, projection);
            iter_swap_if(first + 18, first + 19, compare, projection);
            iter_swap_if(first + 20, first + 21, compare, projection);
            iter_swap_if(first + 22, first + 23, compare, projection);
            iter_swap_if(first + 24, first + 25, compare, projection);
            iter_swap_if(first + 26, first + 27, compare, projection);
            iter_swap_if(first + 28, first + 29, compare, projection);
            iter_swap_if(first + 30, first + 31, compare, projection);
            iter_swap_if(first + 32, first + 35, compare, projection);
            iter_swap_if(first + 33, first + 34, compare, projection);
            iter_swap_if(first + 36, first + 39, compare, projection);
            iter_swap_if(first + 37, first + 38, compare, projection);
            iter_swap_if(first + 40, first + 43, compare, projection);
            iter_swap_if(first + 41, first + 42, compare, projection);
            iter_swap_if(first + 44, first + 47, compare, projection);
            iter_swap_if(first + 45, first + 46, compare, projection);
            iter_swap_if(first + 49, first + 50, compare, projection);
            iter_swap_if(first + 6, first + 8, compare, projection);
            iter_swap_if(first + 7, first + 9, compare, projection);
            iter_swap_if(first + 10, first + 12, compare, projection);
            iter_swap_if(first + 11, first + 13, compare, projection);
            iter_swap_if(first + 14, first + 15, compare, projection);
            iter_swap_if(first + 16, first + 18, compare, projection);
            iter_swap_if(first + 17, first + 19, compare, projection);
            iter_swap_if(first + 20, first + 22, compare, projection);
            iter_swap_if(first + 21, first + 23, compare, projection);
            iter_swap_if(first + 24, first + 26, compare, projection);
            iter_swap_if(first + 25, first + 27, compare, projection);
            iter_swap_if(first + 28, first + 30, compare, projection);
            iter_swap_if(first + 29, first + 31, compare, projection);
            iter_swap_if(first + 32, first + 33, compare, projection);
            iter_swap_if(first + 34, first + 35, compare, projection);
            iter_swap_if(first + 36, first + 37, compare, projection);
            iter_swap_if(first + 38, first + 39, compare, projection);
            iter_swap_if(first + 40, first + 41, compare, projection);
            iter_swap_if(first + 42, first + 43, compare, projection);
            iter_swap_if(first + 44, first + 45, compare, projection);
            iter_swap_if(first + 46, first + 47, compare, projection);
            iter_swap_if(first + 48, first + 49, compare, projection);
            iter_swap_if(first + 5, first + 6, compare, projection);
            iter_swap_if(first + 7, first + 8, compare, projection);
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
            iter_swap_if(first + 45, first + 46, compare, projection);
            iter_swap_if(first + 47, first + 48, compare, projection);
            iter_swap_if(first + 49, first + 50, compare, projection);
        }

        template<typename DifferenceType=std::ptrdiff_t>
        CPPSORT_ATTRIBUTE_NODISCARD
        static constexpr auto index_pairs()
            -> std::array<utility::index_pair<DifferenceType>, 432>
        {
            return {{
                {1, 12}, {2, 13}, {3, 14}, {4, 8}, {5, 6}, {7, 11}, {9, 10}, {15, 18}, {16, 37}, {17, 36}, {19, 38}, {20, 32}, {21, 22}, {23, 35}, {24, 26}, {25, 27}, {28, 30}, {29, 31}, {33, 34}, {39, 42}, {40, 53}, {41, 52}, {43, 54}, {44, 48}, {45, 46}, {47, 51}, {49, 50},
                {0, 5}, {1, 7}, {2, 9}, {3, 4}, {8, 14}, {10, 13}, {11, 12}, {15, 33}, {16, 21}, {17, 23}, {18, 34}, {19, 20}, {22, 37}, {24, 28}, {25, 29}, {26, 30}, {27, 31}, {32, 38}, {35, 36}, {39, 49}, {40, 45}, {41, 47}, {42, 50}, {43, 44}, {46, 53}, {48, 54}, {51, 52},
                {0, 1}, {2, 3}, {4, 5}, {6, 8}, {7, 9}, {10, 11}, {13, 14}, {15, 19}, {16, 17}, {18, 35}, {20, 21}, {22, 32}, {23, 33}, {24, 25}, {26, 27}, {28, 29}, {30, 31}, {34, 38}, {36, 37}, {39, 43}, {40, 41}, {42, 51}, {44, 45}, {46, 48}, {47, 49}, {50, 54}, {52, 53},
                {0, 2}, {1, 3}, {4, 10}, {5, 11}, {6, 7}, {8, 9}, {12, 13}, {15, 16}, {17, 19}, {18, 20}, {21, 35}, {22, 23}, {26, 28}, {27, 29}, {32, 33}, {34, 36}, {37, 38}, {39, 40}, {41, 43}, {42, 44}, {45, 51}, {46, 47}, {48, 49}, {50, 52}, {53, 54},
                {0, 24}, {1, 2}, {3, 12}, {4, 6}, {5, 7}, {8, 10}, {9, 11}, {13, 14}, {15, 39}, {16, 17}, {18, 22}, {19, 34}, {20, 32}, {21, 23}, {25, 28}, {27, 30}, {31, 54}, {33, 35}, {36, 37}, {40, 41}, {42, 46}, {43, 50}, {44, 48}, {45, 47}, {49, 51}, {52, 53},
                {0, 15}, {1, 4}, {2, 6}, {5, 8}, {7, 10}, {9, 13}, {11, 14}, {16, 18}, {17, 22}, {20, 21}, {23, 32}, {24, 39}, {25, 26}, {27, 28}, {29, 30}, {31, 38}, {33, 36}, {35, 37}, {40, 42}, {41, 46}, {44, 45}, {47, 48}, {49, 52}, {51, 53},
                {1, 16}, {2, 4}, {3, 6}, {9, 12}, {11, 13}, {14, 30}, {15, 24}, {17, 18}, {19, 22}, {25, 40}, {33, 34}, {35, 36}, {37, 53}, {38, 54}, {41, 42}, {43, 46}, {49, 50}, {51, 52},
                {1, 25}, {2, 26}, {3, 5}, {6, 8}, {7, 9}, {10, 12}, {13, 36}, {14, 37}, {16, 40}, {17, 41}, {19, 20}, {21, 22}, {23, 33}, {29, 52}, {30, 53}, {32, 34}, {43, 44}, {45, 46}, {47, 49}, {48, 50},
                {2, 17}, {3, 4}, {5, 6}, {7, 8}, {9, 10}, {11, 12}, {13, 29}, {16, 25}, {18, 19}, {20, 21}, {22, 23}, {26, 41}, {30, 37}, {32, 33}, {34, 35}, {36, 52}, {42, 43}, {44, 45}, {46, 47}, {48, 49}, {50, 51},
                {2, 15}, {3, 27}, {4, 43}, {5, 44}, {6, 7}, {8, 9}, {10, 49}, {11, 50}, {12, 35}, {13, 25}, {17, 26}, {18, 42}, {21, 22}, {23, 32}, {28, 51}, {29, 36}, {30, 41}, {45, 46}, {47, 48}, {52, 54},
                {3, 18}, {4, 19}, {5, 20}, {6, 21}, {7, 31}, {8, 23}, {9, 40}, {12, 28}, {14, 45}, {22, 46}, {27, 42}, {32, 48}, {33, 49}, {34, 50}, {35, 51}, {39, 47},
                {5, 13}, {6, 14}, {7, 22}, {8, 39}, {9, 32}, {10, 33}, {11, 34}, {12, 24}, {18, 27}, {19, 43}, {20, 44}, {21, 45}, {23, 47}, {28, 35}, {31, 46}, {38, 42}, {40, 48}, {41, 49},
                {4, 12}, {6, 17}, {7, 18}, {8, 15}, {9, 16}, {10, 26}, {11, 27}, {19, 24}, {20, 25}, {22, 31}, {23, 39}, {28, 43}, {29, 44}, {30, 33}, {34, 38}, {35, 47}, {36, 48}, {37, 45}, {42, 50}, {46, 52},
                {1, 4}, {3, 9}, {10, 17}, {11, 18}, {12, 19}, {14, 30}, {16, 20}, {21, 26}, {24, 28}, {25, 40}, {27, 34}, {29, 32}, {33, 37}, {35, 43}, {36, 44}, {38, 42}, {45, 51}, {50, 53},
                {1, 2}, {4, 8}, {5, 9}, {13, 20}, {14, 21}, {15, 19}, {22, 27}, {23, 24}, {25, 29}, {26, 30}, {28, 39}, {31, 34}, {32, 40}, {33, 41}, {38, 46}, {45, 49}, {50, 52}, {53, 54},
                {3, 4}, {12, 15}, {13, 16}, {14, 23}, {17, 19}, {18, 25}, {20, 22}, {21, 26}, {24, 28}, {27, 31}, {29, 32}, {30, 35}, {33, 39}, {34, 40}, {36, 38}, {37, 41}, {42, 46}, {51, 52},
                {4, 8}, {6, 12}, {7, 13}, {10, 15}, {11, 16}, {14, 17}, {18, 20}, {19, 23}, {21, 24}, {22, 25}, {26, 28}, {27, 29}, {30, 33}, {31, 32}, {34, 36}, {35, 39}, {37, 43}, {38, 40}, {41, 47}, {42, 44}, {46, 48}, {50, 51},
                {5, 8}, {7, 10}, {9, 12}, {11, 14}, {13, 15}, {16, 17}, {18, 19}, {20, 21}, {22, 23}, {24, 25}, {26, 27}, {28, 29}, {30, 31}, {32, 35}, {33, 34}, {36, 39}, {37, 38}, {40, 43}, {41, 42}, {44, 47}, {45, 46}, {49, 50},
                {6, 8}, {7, 9}, {10, 12}, {11, 13}, {14, 15}, {16, 18}, {17, 19}, {20, 22}, {21, 23}, {24, 26}, {25, 27}, {28, 30}, {29, 31}, {32, 33}, {34, 35}, {36, 37}, {38, 39}, {40, 41}, {42, 43}, {44, 45}, {46, 47}, {48, 49},
                {5, 6}, {7, 8}, {9, 10}, {11, 12}, {13, 14}, {15, 16}, {17, 18}, {19, 20}, {21, 22}, {23, 24}, {25, 26}, {27, 28}, {29, 30}, {31, 32}, {33, 34}, {35, 36}, {37, 38}, {39, 40}, {41, 42}, {43, 44}, {45, 46}, {47, 48}, {49, 50},
            }};
        }
    };
}}

#endif // CPPSORT_DETAIL_SORTING_NETWORK_SORT55_H_
