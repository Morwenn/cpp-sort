/*
* Copyright (c) 2023 Morwenn
* SPDX-License-Identifier: MIT
*/
#ifndef CPPSORT_DETAIL_SORTING_NETWORK_SORT60_H_
#define CPPSORT_DETAIL_SORTING_NETWORK_SORT60_H_

namespace cppsort
{
namespace detail
{
    template<>
    struct sorting_network_sorter_impl<60>
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
            iter_swap_if(first + 1, first + 2, compare, projection);
            iter_swap_if(first + 3, first + 10, compare, projection);
            iter_swap_if(first + 4, first + 14, compare, projection);
            iter_swap_if(first + 5, first + 8, compare, projection);
            iter_swap_if(first + 6, first + 13, compare, projection);
            iter_swap_if(first + 7, first + 12, compare, projection);
            iter_swap_if(first + 9, first + 11, compare, projection);
            iter_swap_if(first + 16, first + 17, compare, projection);
            iter_swap_if(first + 18, first + 25, compare, projection);
            iter_swap_if(first + 19, first + 29, compare, projection);
            iter_swap_if(first + 20, first + 23, compare, projection);
            iter_swap_if(first + 21, first + 28, compare, projection);
            iter_swap_if(first + 22, first + 27, compare, projection);
            iter_swap_if(first + 24, first + 26, compare, projection);
            iter_swap_if(first + 31, first + 32, compare, projection);
            iter_swap_if(first + 33, first + 40, compare, projection);
            iter_swap_if(first + 34, first + 44, compare, projection);
            iter_swap_if(first + 35, first + 38, compare, projection);
            iter_swap_if(first + 36, first + 43, compare, projection);
            iter_swap_if(first + 37, first + 42, compare, projection);
            iter_swap_if(first + 39, first + 41, compare, projection);
            iter_swap_if(first + 46, first + 47, compare, projection);
            iter_swap_if(first + 48, first + 55, compare, projection);
            iter_swap_if(first + 49, first + 59, compare, projection);
            iter_swap_if(first + 50, first + 53, compare, projection);
            iter_swap_if(first + 51, first + 58, compare, projection);
            iter_swap_if(first + 52, first + 57, compare, projection);
            iter_swap_if(first + 54, first + 56, compare, projection);
            iter_swap_if(first, first + 14, compare, projection);
            iter_swap_if(first + 1, first + 5, compare, projection);
            iter_swap_if(first + 2, first + 8, compare, projection);
            iter_swap_if(first + 3, first + 7, compare, projection);
            iter_swap_if(first + 6, first + 9, compare, projection);
            iter_swap_if(first + 10, first + 12, compare, projection);
            iter_swap_if(first + 11, first + 13, compare, projection);
            iter_swap_if(first + 15, first + 29, compare, projection);
            iter_swap_if(first + 16, first + 20, compare, projection);
            iter_swap_if(first + 17, first + 23, compare, projection);
            iter_swap_if(first + 18, first + 22, compare, projection);
            iter_swap_if(first + 21, first + 24, compare, projection);
            iter_swap_if(first + 25, first + 27, compare, projection);
            iter_swap_if(first + 26, first + 28, compare, projection);
            iter_swap_if(first + 30, first + 44, compare, projection);
            iter_swap_if(first + 31, first + 35, compare, projection);
            iter_swap_if(first + 32, first + 38, compare, projection);
            iter_swap_if(first + 33, first + 37, compare, projection);
            iter_swap_if(first + 36, first + 39, compare, projection);
            iter_swap_if(first + 40, first + 42, compare, projection);
            iter_swap_if(first + 41, first + 43, compare, projection);
            iter_swap_if(first + 45, first + 59, compare, projection);
            iter_swap_if(first + 46, first + 50, compare, projection);
            iter_swap_if(first + 47, first + 53, compare, projection);
            iter_swap_if(first + 48, first + 52, compare, projection);
            iter_swap_if(first + 51, first + 54, compare, projection);
            iter_swap_if(first + 55, first + 57, compare, projection);
            iter_swap_if(first + 56, first + 58, compare, projection);
            iter_swap_if(first, first + 7, compare, projection);
            iter_swap_if(first + 1, first + 6, compare, projection);
            iter_swap_if(first + 2, first + 9, compare, projection);
            iter_swap_if(first + 4, first + 10, compare, projection);
            iter_swap_if(first + 5, first + 11, compare, projection);
            iter_swap_if(first + 8, first + 13, compare, projection);
            iter_swap_if(first + 12, first + 14, compare, projection);
            iter_swap_if(first + 15, first + 22, compare, projection);
            iter_swap_if(first + 16, first + 21, compare, projection);
            iter_swap_if(first + 17, first + 24, compare, projection);
            iter_swap_if(first + 19, first + 25, compare, projection);
            iter_swap_if(first + 20, first + 26, compare, projection);
            iter_swap_if(first + 23, first + 28, compare, projection);
            iter_swap_if(first + 27, first + 29, compare, projection);
            iter_swap_if(first + 30, first + 37, compare, projection);
            iter_swap_if(first + 31, first + 36, compare, projection);
            iter_swap_if(first + 32, first + 39, compare, projection);
            iter_swap_if(first + 34, first + 40, compare, projection);
            iter_swap_if(first + 35, first + 41, compare, projection);
            iter_swap_if(first + 38, first + 43, compare, projection);
            iter_swap_if(first + 42, first + 44, compare, projection);
            iter_swap_if(first + 45, first + 52, compare, projection);
            iter_swap_if(first + 46, first + 51, compare, projection);
            iter_swap_if(first + 47, first + 54, compare, projection);
            iter_swap_if(first + 49, first + 55, compare, projection);
            iter_swap_if(first + 50, first + 56, compare, projection);
            iter_swap_if(first + 53, first + 58, compare, projection);
            iter_swap_if(first + 57, first + 59, compare, projection);
            iter_swap_if(first, first + 6, compare, projection);
            iter_swap_if(first + 2, first + 4, compare, projection);
            iter_swap_if(first + 3, first + 5, compare, projection);
            iter_swap_if(first + 7, first + 11, compare, projection);
            iter_swap_if(first + 8, first + 10, compare, projection);
            iter_swap_if(first + 9, first + 12, compare, projection);
            iter_swap_if(first + 13, first + 14, compare, projection);
            iter_swap_if(first + 15, first + 21, compare, projection);
            iter_swap_if(first + 17, first + 19, compare, projection);
            iter_swap_if(first + 18, first + 20, compare, projection);
            iter_swap_if(first + 22, first + 26, compare, projection);
            iter_swap_if(first + 23, first + 25, compare, projection);
            iter_swap_if(first + 24, first + 27, compare, projection);
            iter_swap_if(first + 28, first + 29, compare, projection);
            iter_swap_if(first + 30, first + 36, compare, projection);
            iter_swap_if(first + 32, first + 34, compare, projection);
            iter_swap_if(first + 33, first + 35, compare, projection);
            iter_swap_if(first + 37, first + 41, compare, projection);
            iter_swap_if(first + 38, first + 40, compare, projection);
            iter_swap_if(first + 39, first + 42, compare, projection);
            iter_swap_if(first + 43, first + 44, compare, projection);
            iter_swap_if(first + 45, first + 51, compare, projection);
            iter_swap_if(first + 47, first + 49, compare, projection);
            iter_swap_if(first + 48, first + 50, compare, projection);
            iter_swap_if(first + 52, first + 56, compare, projection);
            iter_swap_if(first + 53, first + 55, compare, projection);
            iter_swap_if(first + 54, first + 57, compare, projection);
            iter_swap_if(first + 58, first + 59, compare, projection);
            iter_swap_if(first, first + 3, compare, projection);
            iter_swap_if(first + 1, first + 2, compare, projection);
            iter_swap_if(first + 4, first + 7, compare, projection);
            iter_swap_if(first + 5, first + 9, compare, projection);
            iter_swap_if(first + 6, first + 8, compare, projection);
            iter_swap_if(first + 10, first + 11, compare, projection);
            iter_swap_if(first + 12, first + 13, compare, projection);
            iter_swap_if(first + 14, first + 59, compare, projection);
            iter_swap_if(first + 15, first + 18, compare, projection);
            iter_swap_if(first + 16, first + 17, compare, projection);
            iter_swap_if(first + 19, first + 22, compare, projection);
            iter_swap_if(first + 20, first + 24, compare, projection);
            iter_swap_if(first + 21, first + 23, compare, projection);
            iter_swap_if(first + 25, first + 26, compare, projection);
            iter_swap_if(first + 27, first + 28, compare, projection);
            iter_swap_if(first + 29, first + 44, compare, projection);
            iter_swap_if(first + 30, first + 33, compare, projection);
            iter_swap_if(first + 31, first + 32, compare, projection);
            iter_swap_if(first + 34, first + 37, compare, projection);
            iter_swap_if(first + 35, first + 39, compare, projection);
            iter_swap_if(first + 36, first + 38, compare, projection);
            iter_swap_if(first + 40, first + 41, compare, projection);
            iter_swap_if(first + 42, first + 43, compare, projection);
            iter_swap_if(first + 45, first + 48, compare, projection);
            iter_swap_if(first + 46, first + 47, compare, projection);
            iter_swap_if(first + 49, first + 52, compare, projection);
            iter_swap_if(first + 50, first + 54, compare, projection);
            iter_swap_if(first + 51, first + 53, compare, projection);
            iter_swap_if(first + 55, first + 56, compare, projection);
            iter_swap_if(first + 57, first + 58, compare, projection);
            iter_swap_if(first, first + 1, compare, projection);
            iter_swap_if(first + 2, first + 3, compare, projection);
            iter_swap_if(first + 4, first + 6, compare, projection);
            iter_swap_if(first + 7, first + 9, compare, projection);
            iter_swap_if(first + 10, first + 12, compare, projection);
            iter_swap_if(first + 11, first + 13, compare, projection);
            iter_swap_if(first + 14, first + 29, compare, projection);
            iter_swap_if(first + 15, first + 16, compare, projection);
            iter_swap_if(first + 17, first + 18, compare, projection);
            iter_swap_if(first + 19, first + 21, compare, projection);
            iter_swap_if(first + 22, first + 24, compare, projection);
            iter_swap_if(first + 25, first + 27, compare, projection);
            iter_swap_if(first + 26, first + 28, compare, projection);
            iter_swap_if(first + 30, first + 31, compare, projection);
            iter_swap_if(first + 32, first + 33, compare, projection);
            iter_swap_if(first + 34, first + 36, compare, projection);
            iter_swap_if(first + 37, first + 39, compare, projection);
            iter_swap_if(first + 40, first + 42, compare, projection);
            iter_swap_if(first + 41, first + 43, compare, projection);
            iter_swap_if(first + 44, first + 59, compare, projection);
            iter_swap_if(first + 45, first + 46, compare, projection);
            iter_swap_if(first + 47, first + 48, compare, projection);
            iter_swap_if(first + 49, first + 51, compare, projection);
            iter_swap_if(first + 52, first + 54, compare, projection);
            iter_swap_if(first + 55, first + 57, compare, projection);
            iter_swap_if(first + 56, first + 58, compare, projection);
            iter_swap_if(first, first + 30, compare, projection);
            iter_swap_if(first + 1, first + 2, compare, projection);
            iter_swap_if(first + 3, first + 5, compare, projection);
            iter_swap_if(first + 8, first + 10, compare, projection);
            iter_swap_if(first + 11, first + 12, compare, projection);
            iter_swap_if(first + 13, first + 58, compare, projection);
            iter_swap_if(first + 15, first + 45, compare, projection);
            iter_swap_if(first + 16, first + 17, compare, projection);
            iter_swap_if(first + 18, first + 20, compare, projection);
            iter_swap_if(first + 23, first + 25, compare, projection);
            iter_swap_if(first + 26, first + 27, compare, projection);
            iter_swap_if(first + 28, first + 43, compare, projection);
            iter_swap_if(first + 29, first + 44, compare, projection);
            iter_swap_if(first + 31, first + 32, compare, projection);
            iter_swap_if(first + 33, first + 35, compare, projection);
            iter_swap_if(first + 38, first + 40, compare, projection);
            iter_swap_if(first + 41, first + 42, compare, projection);
            iter_swap_if(first + 46, first + 47, compare, projection);
            iter_swap_if(first + 48, first + 50, compare, projection);
            iter_swap_if(first + 53, first + 55, compare, projection);
            iter_swap_if(first + 56, first + 57, compare, projection);
            iter_swap_if(first, first + 15, compare, projection);
            iter_swap_if(first + 1, first + 31, compare, projection);
            iter_swap_if(first + 3, first + 4, compare, projection);
            iter_swap_if(first + 5, first + 6, compare, projection);
            iter_swap_if(first + 7, first + 8, compare, projection);
            iter_swap_if(first + 9, first + 10, compare, projection);
            iter_swap_if(first + 12, first + 57, compare, projection);
            iter_swap_if(first + 13, first + 28, compare, projection);
            iter_swap_if(first + 16, first + 46, compare, projection);
            iter_swap_if(first + 18, first + 19, compare, projection);
            iter_swap_if(first + 20, first + 21, compare, projection);
            iter_swap_if(first + 22, first + 23, compare, projection);
            iter_swap_if(first + 24, first + 25, compare, projection);
            iter_swap_if(first + 27, first + 42, compare, projection);
            iter_swap_if(first + 30, first + 45, compare, projection);
            iter_swap_if(first + 33, first + 34, compare, projection);
            iter_swap_if(first + 35, first + 36, compare, projection);
            iter_swap_if(first + 37, first + 38, compare, projection);
            iter_swap_if(first + 39, first + 40, compare, projection);
            iter_swap_if(first + 43, first + 58, compare, projection);
            iter_swap_if(first + 48, first + 49, compare, projection);
            iter_swap_if(first + 50, first + 51, compare, projection);
            iter_swap_if(first + 52, first + 53, compare, projection);
            iter_swap_if(first + 54, first + 55, compare, projection);
            iter_swap_if(first + 1, first + 16, compare, projection);
            iter_swap_if(first + 2, first + 3, compare, projection);
            iter_swap_if(first + 4, first + 5, compare, projection);
            iter_swap_if(first + 6, first + 7, compare, projection);
            iter_swap_if(first + 8, first + 9, compare, projection);
            iter_swap_if(first + 10, first + 11, compare, projection);
            iter_swap_if(first + 12, first + 27, compare, projection);
            iter_swap_if(first + 15, first + 30, compare, projection);
            iter_swap_if(first + 17, first + 18, compare, projection);
            iter_swap_if(first + 19, first + 20, compare, projection);
            iter_swap_if(first + 21, first + 22, compare, projection);
            iter_swap_if(first + 23, first + 24, compare, projection);
            iter_swap_if(first + 25, first + 26, compare, projection);
            iter_swap_if(first + 28, first + 43, compare, projection);
            iter_swap_if(first + 31, first + 46, compare, projection);
            iter_swap_if(first + 32, first + 33, compare, projection);
            iter_swap_if(first + 34, first + 35, compare, projection);
            iter_swap_if(first + 36, first + 37, compare, projection);
            iter_swap_if(first + 38, first + 39, compare, projection);
            iter_swap_if(first + 40, first + 41, compare, projection);
            iter_swap_if(first + 42, first + 57, compare, projection);
            iter_swap_if(first + 47, first + 48, compare, projection);
            iter_swap_if(first + 49, first + 50, compare, projection);
            iter_swap_if(first + 51, first + 52, compare, projection);
            iter_swap_if(first + 53, first + 54, compare, projection);
            iter_swap_if(first + 55, first + 56, compare, projection);
            iter_swap_if(first + 2, first + 32, compare, projection);
            iter_swap_if(first + 3, first + 18, compare, projection);
            iter_swap_if(first + 4, first + 49, compare, projection);
            iter_swap_if(first + 5, first + 6, compare, projection);
            iter_swap_if(first + 7, first + 8, compare, projection);
            iter_swap_if(first + 9, first + 39, compare, projection);
            iter_swap_if(first + 10, first + 40, compare, projection);
            iter_swap_if(first + 11, first + 26, compare, projection);
            iter_swap_if(first + 16, first + 31, compare, projection);
            iter_swap_if(first + 17, first + 47, compare, projection);
            iter_swap_if(first + 19, first + 34, compare, projection);
            iter_swap_if(first + 20, first + 21, compare, projection);
            iter_swap_if(first + 22, first + 23, compare, projection);
            iter_swap_if(first + 24, first + 54, compare, projection);
            iter_swap_if(first + 25, first + 55, compare, projection);
            iter_swap_if(first + 27, first + 42, compare, projection);
            iter_swap_if(first + 33, first + 48, compare, projection);
            iter_swap_if(first + 35, first + 36, compare, projection);
            iter_swap_if(first + 37, first + 38, compare, projection);
            iter_swap_if(first + 41, first + 56, compare, projection);
            iter_swap_if(first + 50, first + 51, compare, projection);
            iter_swap_if(first + 52, first + 53, compare, projection);
            iter_swap_if(first + 2, first + 17, compare, projection);
            iter_swap_if(first + 3, first + 33, compare, projection);
            iter_swap_if(first + 4, first + 19, compare, projection);
            iter_swap_if(first + 5, first + 50, compare, projection);
            iter_swap_if(first + 6, first + 51, compare, projection);
            iter_swap_if(first + 7, first + 37, compare, projection);
            iter_swap_if(first + 8, first + 38, compare, projection);
            iter_swap_if(first + 9, first + 24, compare, projection);
            iter_swap_if(first + 10, first + 25, compare, projection);
            iter_swap_if(first + 11, first + 41, compare, projection);
            iter_swap_if(first + 18, first + 48, compare, projection);
            iter_swap_if(first + 20, first + 35, compare, projection);
            iter_swap_if(first + 21, first + 36, compare, projection);
            iter_swap_if(first + 22, first + 52, compare, projection);
            iter_swap_if(first + 23, first + 53, compare, projection);
            iter_swap_if(first + 26, first + 56, compare, projection);
            iter_swap_if(first + 32, first + 47, compare, projection);
            iter_swap_if(first + 34, first + 49, compare, projection);
            iter_swap_if(first + 39, first + 54, compare, projection);
            iter_swap_if(first + 40, first + 55, compare, projection);
            iter_swap_if(first + 4, first + 15, compare, projection);
            iter_swap_if(first + 5, first + 20, compare, projection);
            iter_swap_if(first + 6, first + 21, compare, projection);
            iter_swap_if(first + 7, first + 22, compare, projection);
            iter_swap_if(first + 8, first + 23, compare, projection);
            iter_swap_if(first + 9, first + 31, compare, projection);
            iter_swap_if(first + 11, first + 48, compare, projection);
            iter_swap_if(first + 12, first + 19, compare, projection);
            iter_swap_if(first + 17, first + 32, compare, projection);
            iter_swap_if(first + 18, first + 33, compare, projection);
            iter_swap_if(first + 24, first + 39, compare, projection);
            iter_swap_if(first + 25, first + 40, compare, projection);
            iter_swap_if(first + 26, first + 41, compare, projection);
            iter_swap_if(first + 27, first + 49, compare, projection);
            iter_swap_if(first + 35, first + 50, compare, projection);
            iter_swap_if(first + 36, first + 51, compare, projection);
            iter_swap_if(first + 37, first + 52, compare, projection);
            iter_swap_if(first + 38, first + 53, compare, projection);
            iter_swap_if(first + 43, first + 54, compare, projection);
            iter_swap_if(first + 44, first + 55, compare, projection);
            iter_swap_if(first + 1, first + 4, compare, projection);
            iter_swap_if(first + 5, first + 16, compare, projection);
            iter_swap_if(first + 6, first + 17, compare, projection);
            iter_swap_if(first + 7, first + 18, compare, projection);
            iter_swap_if(first + 8, first + 30, compare, projection);
            iter_swap_if(first + 10, first + 32, compare, projection);
            iter_swap_if(first + 11, first + 33, compare, projection);
            iter_swap_if(first + 13, first + 20, compare, projection);
            iter_swap_if(first + 14, first + 21, compare, projection);
            iter_swap_if(first + 19, first + 34, compare, projection);
            iter_swap_if(first + 22, first + 37, compare, projection);
            iter_swap_if(first + 23, first + 38, compare, projection);
            iter_swap_if(first + 24, first + 46, compare, projection);
            iter_swap_if(first + 25, first + 47, compare, projection);
            iter_swap_if(first + 26, first + 48, compare, projection);
            iter_swap_if(first + 28, first + 50, compare, projection);
            iter_swap_if(first + 29, first + 51, compare, projection);
            iter_swap_if(first + 41, first + 52, compare, projection);
            iter_swap_if(first + 42, first + 53, compare, projection);
            iter_swap_if(first + 54, first + 56, compare, projection);
            iter_swap_if(first + 55, first + 58, compare, projection);
            iter_swap_if(first + 3, first + 5, compare, projection);
            iter_swap_if(first + 8, first + 15, compare, projection);
            iter_swap_if(first + 9, first + 16, compare, projection);
            iter_swap_if(first + 10, first + 17, compare, projection);
            iter_swap_if(first + 11, first + 18, compare, projection);
            iter_swap_if(first + 13, first + 24, compare, projection);
            iter_swap_if(first + 14, first + 25, compare, projection);
            iter_swap_if(first + 19, first + 30, compare, projection);
            iter_swap_if(first + 20, first + 35, compare, projection);
            iter_swap_if(first + 21, first + 36, compare, projection);
            iter_swap_if(first + 22, first + 33, compare, projection);
            iter_swap_if(first + 23, first + 45, compare, projection);
            iter_swap_if(first + 26, first + 37, compare, projection);
            iter_swap_if(first + 28, first + 46, compare, projection);
            iter_swap_if(first + 29, first + 47, compare, projection);
            iter_swap_if(first + 34, first + 38, compare, projection);
            iter_swap_if(first + 41, first + 48, compare, projection);
            iter_swap_if(first + 42, first + 49, compare, projection);
            iter_swap_if(first + 43, first + 50, compare, projection);
            iter_swap_if(first + 44, first + 51, compare, projection);
            iter_swap_if(first + 2, first + 8, compare, projection);
            iter_swap_if(first + 5, first + 9, compare, projection);
            iter_swap_if(first + 12, first + 23, compare, projection);
            iter_swap_if(first + 18, first + 22, compare, projection);
            iter_swap_if(first + 20, first + 31, compare, projection);
            iter_swap_if(first + 21, first + 32, compare, projection);
            iter_swap_if(first + 26, first + 33, compare, projection);
            iter_swap_if(first + 27, first + 45, compare, projection);
            iter_swap_if(first + 35, first + 39, compare, projection);
            iter_swap_if(first + 36, first + 40, compare, projection);
            iter_swap_if(first + 37, first + 41, compare, projection);
            iter_swap_if(first + 50, first + 54, compare, projection);
            iter_swap_if(first + 51, first + 57, compare, projection);
            iter_swap_if(first + 2, first + 4, compare, projection);
            iter_swap_if(first + 12, first + 19, compare, projection);
            iter_swap_if(first + 13, first + 20, compare, projection);
            iter_swap_if(first + 14, first + 21, compare, projection);
            iter_swap_if(first + 23, first + 30, compare, projection);
            iter_swap_if(first + 24, first + 31, compare, projection);
            iter_swap_if(first + 25, first + 32, compare, projection);
            iter_swap_if(first + 27, first + 34, compare, projection);
            iter_swap_if(first + 28, first + 35, compare, projection);
            iter_swap_if(first + 29, first + 36, compare, projection);
            iter_swap_if(first + 38, first + 45, compare, projection);
            iter_swap_if(first + 39, first + 46, compare, projection);
            iter_swap_if(first + 40, first + 47, compare, projection);
            iter_swap_if(first + 55, first + 57, compare, projection);
            iter_swap_if(first + 12, first + 15, compare, projection);
            iter_swap_if(first + 13, first + 16, compare, projection);
            iter_swap_if(first + 14, first + 17, compare, projection);
            iter_swap_if(first + 19, first + 23, compare, projection);
            iter_swap_if(first + 20, first + 24, compare, projection);
            iter_swap_if(first + 21, first + 25, compare, projection);
            iter_swap_if(first + 27, first + 30, compare, projection);
            iter_swap_if(first + 28, first + 31, compare, projection);
            iter_swap_if(first + 29, first + 32, compare, projection);
            iter_swap_if(first + 34, first + 38, compare, projection);
            iter_swap_if(first + 35, first + 39, compare, projection);
            iter_swap_if(first + 36, first + 40, compare, projection);
            iter_swap_if(first + 42, first + 45, compare, projection);
            iter_swap_if(first + 43, first + 46, compare, projection);
            iter_swap_if(first + 44, first + 47, compare, projection);
            iter_swap_if(first + 6, first + 12, compare, projection);
            iter_swap_if(first + 7, first + 13, compare, projection);
            iter_swap_if(first + 10, first + 15, compare, projection);
            iter_swap_if(first + 11, first + 16, compare, projection);
            iter_swap_if(first + 14, first + 19, compare, projection);
            iter_swap_if(first + 17, first + 23, compare, projection);
            iter_swap_if(first + 18, first + 20, compare, projection);
            iter_swap_if(first + 21, first + 27, compare, projection);
            iter_swap_if(first + 22, first + 24, compare, projection);
            iter_swap_if(first + 25, first + 30, compare, projection);
            iter_swap_if(first + 26, first + 28, compare, projection);
            iter_swap_if(first + 29, first + 34, compare, projection);
            iter_swap_if(first + 31, first + 33, compare, projection);
            iter_swap_if(first + 32, first + 38, compare, projection);
            iter_swap_if(first + 35, first + 37, compare, projection);
            iter_swap_if(first + 36, first + 42, compare, projection);
            iter_swap_if(first + 39, first + 41, compare, projection);
            iter_swap_if(first + 40, first + 45, compare, projection);
            iter_swap_if(first + 43, first + 48, compare, projection);
            iter_swap_if(first + 44, first + 49, compare, projection);
            iter_swap_if(first + 46, first + 52, compare, projection);
            iter_swap_if(first + 47, first + 53, compare, projection);
            iter_swap_if(first + 6, first + 8, compare, projection);
            iter_swap_if(first + 7, first + 9, compare, projection);
            iter_swap_if(first + 10, first + 12, compare, projection);
            iter_swap_if(first + 11, first + 15, compare, projection);
            iter_swap_if(first + 13, first + 14, compare, projection);
            iter_swap_if(first + 16, first + 18, compare, projection);
            iter_swap_if(first + 17, first + 19, compare, projection);
            iter_swap_if(first + 20, first + 22, compare, projection);
            iter_swap_if(first + 21, first + 23, compare, projection);
            iter_swap_if(first + 24, first + 27, compare, projection);
            iter_swap_if(first + 25, first + 26, compare, projection);
            iter_swap_if(first + 28, first + 30, compare, projection);
            iter_swap_if(first + 29, first + 31, compare, projection);
            iter_swap_if(first + 32, first + 34, compare, projection);
            iter_swap_if(first + 33, first + 35, compare, projection);
            iter_swap_if(first + 36, first + 37, compare, projection);
            iter_swap_if(first + 38, first + 39, compare, projection);
            iter_swap_if(first + 40, first + 42, compare, projection);
            iter_swap_if(first + 41, first + 43, compare, projection);
            iter_swap_if(first + 44, first + 48, compare, projection);
            iter_swap_if(first + 45, first + 46, compare, projection);
            iter_swap_if(first + 47, first + 49, compare, projection);
            iter_swap_if(first + 50, first + 52, compare, projection);
            iter_swap_if(first + 51, first + 53, compare, projection);
            iter_swap_if(first + 3, first + 6, compare, projection);
            iter_swap_if(first + 5, first + 8, compare, projection);
            iter_swap_if(first + 7, first + 10, compare, projection);
            iter_swap_if(first + 9, first + 12, compare, projection);
            iter_swap_if(first + 11, first + 13, compare, projection);
            iter_swap_if(first + 14, first + 15, compare, projection);
            iter_swap_if(first + 16, first + 17, compare, projection);
            iter_swap_if(first + 18, first + 19, compare, projection);
            iter_swap_if(first + 20, first + 21, compare, projection);
            iter_swap_if(first + 22, first + 23, compare, projection);
            iter_swap_if(first + 24, first + 25, compare, projection);
            iter_swap_if(first + 26, first + 27, compare, projection);
            iter_swap_if(first + 28, first + 29, compare, projection);
            iter_swap_if(first + 30, first + 31, compare, projection);
            iter_swap_if(first + 32, first + 33, compare, projection);
            iter_swap_if(first + 34, first + 35, compare, projection);
            iter_swap_if(first + 36, first + 38, compare, projection);
            iter_swap_if(first + 37, first + 39, compare, projection);
            iter_swap_if(first + 40, first + 41, compare, projection);
            iter_swap_if(first + 42, first + 43, compare, projection);
            iter_swap_if(first + 44, first + 45, compare, projection);
            iter_swap_if(first + 46, first + 48, compare, projection);
            iter_swap_if(first + 47, first + 50, compare, projection);
            iter_swap_if(first + 49, first + 52, compare, projection);
            iter_swap_if(first + 51, first + 54, compare, projection);
            iter_swap_if(first + 53, first + 56, compare, projection);
            iter_swap_if(first + 3, first + 4, compare, projection);
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
            iter_swap_if(first + 51, first + 52, compare, projection);
            iter_swap_if(first + 53, first + 54, compare, projection);
            iter_swap_if(first + 55, first + 56, compare, projection);
        }

        template<typename DifferenceType=std::ptrdiff_t>
        CPPSORT_ATTRIBUTE_NODISCARD
        static constexpr auto index_pairs()
            -> std::array<utility::index_pair<DifferenceType>, 483>
        {
            return {{
                {1, 2}, {3, 10}, {4, 14}, {5, 8}, {6, 13}, {7, 12}, {9, 11}, {16, 17}, {18, 25}, {19, 29}, {20, 23}, {21, 28}, {22, 27}, {24, 26}, {31, 32}, {33, 40}, {34, 44}, {35, 38}, {36, 43}, {37, 42}, {39, 41}, {46, 47}, {48, 55}, {49, 59}, {50, 53}, {51, 58}, {52, 57}, {54, 56},
                {0, 14}, {1, 5}, {2, 8}, {3, 7}, {6, 9}, {10, 12}, {11, 13}, {15, 29}, {16, 20}, {17, 23}, {18, 22}, {21, 24}, {25, 27}, {26, 28}, {30, 44}, {31, 35}, {32, 38}, {33, 37}, {36, 39}, {40, 42}, {41, 43}, {45, 59}, {46, 50}, {47, 53}, {48, 52}, {51, 54}, {55, 57}, {56, 58},
                {0, 7}, {1, 6}, {2, 9}, {4, 10}, {5, 11}, {8, 13}, {12, 14}, {15, 22}, {16, 21}, {17, 24}, {19, 25}, {20, 26}, {23, 28}, {27, 29}, {30, 37}, {31, 36}, {32, 39}, {34, 40}, {35, 41}, {38, 43}, {42, 44}, {45, 52}, {46, 51}, {47, 54}, {49, 55}, {50, 56}, {53, 58}, {57, 59},
                {0, 6}, {2, 4}, {3, 5}, {7, 11}, {8, 10}, {9, 12}, {13, 14}, {15, 21}, {17, 19}, {18, 20}, {22, 26}, {23, 25}, {24, 27}, {28, 29}, {30, 36}, {32, 34}, {33, 35}, {37, 41}, {38, 40}, {39, 42}, {43, 44}, {45, 51}, {47, 49}, {48, 50}, {52, 56}, {53, 55}, {54, 57}, {58, 59},
                {0, 3}, {1, 2}, {4, 7}, {5, 9}, {6, 8}, {10, 11}, {12, 13}, {14, 59}, {15, 18}, {16, 17}, {19, 22}, {20, 24}, {21, 23}, {25, 26}, {27, 28}, {29, 44}, {30, 33}, {31, 32}, {34, 37}, {35, 39}, {36, 38}, {40, 41}, {42, 43}, {45, 48}, {46, 47}, {49, 52}, {50, 54}, {51, 53}, {55, 56}, {57, 58},
                {0, 1}, {2, 3}, {4, 6}, {7, 9}, {10, 12}, {11, 13}, {14, 29}, {15, 16}, {17, 18}, {19, 21}, {22, 24}, {25, 27}, {26, 28}, {30, 31}, {32, 33}, {34, 36}, {37, 39}, {40, 42}, {41, 43}, {44, 59}, {45, 46}, {47, 48}, {49, 51}, {52, 54}, {55, 57}, {56, 58},
                {0, 30}, {1, 2}, {3, 5}, {8, 10}, {11, 12}, {13, 58}, {15, 45}, {16, 17}, {18, 20}, {23, 25}, {26, 27}, {28, 43}, {29, 44}, {31, 32}, {33, 35}, {38, 40}, {41, 42}, {46, 47}, {48, 50}, {53, 55}, {56, 57},
                {0, 15}, {1, 31}, {3, 4}, {5, 6}, {7, 8}, {9, 10}, {12, 57}, {13, 28}, {16, 46}, {18, 19}, {20, 21}, {22, 23}, {24, 25}, {27, 42}, {30, 45}, {33, 34}, {35, 36}, {37, 38}, {39, 40}, {43, 58}, {48, 49}, {50, 51}, {52, 53}, {54, 55},
                {1, 16}, {2, 3}, {4, 5}, {6, 7}, {8, 9}, {10, 11}, {12, 27}, {15, 30}, {17, 18}, {19, 20}, {21, 22}, {23, 24}, {25, 26}, {28, 43}, {31, 46}, {32, 33}, {34, 35}, {36, 37}, {38, 39}, {40, 41}, {42, 57}, {47, 48}, {49, 50}, {51, 52}, {53, 54}, {55, 56},
                {2, 32}, {3, 18}, {4, 49}, {5, 6}, {7, 8}, {9, 39}, {10, 40}, {11, 26}, {16, 31}, {17, 47}, {19, 34}, {20, 21}, {22, 23}, {24, 54}, {25, 55}, {27, 42}, {33, 48}, {35, 36}, {37, 38}, {41, 56}, {50, 51}, {52, 53},
                {2, 17}, {3, 33}, {4, 19}, {5, 50}, {6, 51}, {7, 37}, {8, 38}, {9, 24}, {10, 25}, {11, 41}, {18, 48}, {20, 35}, {21, 36}, {22, 52}, {23, 53}, {26, 56}, {32, 47}, {34, 49}, {39, 54}, {40, 55},
                {4, 15}, {5, 20}, {6, 21}, {7, 22}, {8, 23}, {9, 31}, {11, 48}, {12, 19}, {17, 32}, {18, 33}, {24, 39}, {25, 40}, {26, 41}, {27, 49}, {35, 50}, {36, 51}, {37, 52}, {38, 53}, {43, 54}, {44, 55},
                {1, 4}, {5, 16}, {6, 17}, {7, 18}, {8, 30}, {10, 32}, {11, 33}, {13, 20}, {14, 21}, {19, 34}, {22, 37}, {23, 38}, {24, 46}, {25, 47}, {26, 48}, {28, 50}, {29, 51}, {41, 52}, {42, 53}, {54, 56}, {55, 58},
                {3, 5}, {8, 15}, {9, 16}, {10, 17}, {11, 18}, {13, 24}, {14, 25}, {19, 30}, {20, 35}, {21, 36}, {22, 33}, {23, 45}, {26, 37}, {28, 46}, {29, 47}, {34, 38}, {41, 48}, {42, 49}, {43, 50}, {44, 51},
                {2, 8}, {5, 9}, {12, 23}, {18, 22}, {20, 31}, {21, 32}, {26, 33}, {27, 45}, {35, 39}, {36, 40}, {37, 41}, {50, 54}, {51, 57},
                {2, 4}, {12, 19}, {13, 20}, {14, 21}, {23, 30}, {24, 31}, {25, 32}, {27, 34}, {28, 35}, {29, 36}, {38, 45}, {39, 46}, {40, 47}, {55, 57},
                {12, 15}, {13, 16}, {14, 17}, {19, 23}, {20, 24}, {21, 25}, {27, 30}, {28, 31}, {29, 32}, {34, 38}, {35, 39}, {36, 40}, {42, 45}, {43, 46}, {44, 47},
                {6, 12}, {7, 13}, {10, 15}, {11, 16}, {14, 19}, {17, 23}, {18, 20}, {21, 27}, {22, 24}, {25, 30}, {26, 28}, {29, 34}, {31, 33}, {32, 38}, {35, 37}, {36, 42}, {39, 41}, {40, 45}, {43, 48}, {44, 49}, {46, 52}, {47, 53},
                {6, 8}, {7, 9}, {10, 12}, {11, 15}, {13, 14}, {16, 18}, {17, 19}, {20, 22}, {21, 23}, {24, 27}, {25, 26}, {28, 30}, {29, 31}, {32, 34}, {33, 35}, {36, 37}, {38, 39}, {40, 42}, {41, 43}, {44, 48}, {45, 46}, {47, 49}, {50, 52}, {51, 53},
                {3, 6}, {5, 8}, {7, 10}, {9, 12}, {11, 13}, {14, 15}, {16, 17}, {18, 19}, {20, 21}, {22, 23}, {24, 25}, {26, 27}, {28, 29}, {30, 31}, {32, 33}, {34, 35}, {36, 38}, {37, 39}, {40, 41}, {42, 43}, {44, 45}, {46, 48}, {47, 50}, {49, 52}, {51, 54}, {53, 56},
                {3, 4}, {5, 6}, {7, 8}, {9, 10}, {11, 12}, {13, 14}, {15, 16}, {17, 18}, {19, 20}, {21, 22}, {23, 24}, {25, 26}, {27, 28}, {29, 30}, {31, 32}, {33, 34}, {35, 36}, {37, 38}, {39, 40}, {41, 42}, {43, 44}, {45, 46}, {47, 48}, {49, 50}, {51, 52}, {53, 54}, {55, 56},
            }};
        }
    };
}}

#endif // CPPSORT_DETAIL_SORTING_NETWORK_SORT60_H_
