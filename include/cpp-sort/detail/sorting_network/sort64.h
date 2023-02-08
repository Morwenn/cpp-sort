/*
* Copyright (c) 2023 Morwenn
* SPDX-License-Identifier: MIT
*/
#ifndef CPPSORT_DETAIL_SORTING_NETWORK_SORT64_H_
#define CPPSORT_DETAIL_SORTING_NETWORK_SORT64_H_

namespace cppsort
{
namespace detail
{
    template<>
    struct sorting_network_sorter_impl<64>
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
            iter_swap_if(first, first + 2, compare, projection);
            iter_swap_if(first + 1, first + 3, compare, projection);
            iter_swap_if(first + 4, first + 6, compare, projection);
            iter_swap_if(first + 5, first + 7, compare, projection);
            iter_swap_if(first + 8, first + 10, compare, projection);
            iter_swap_if(first + 9, first + 11, compare, projection);
            iter_swap_if(first + 12, first + 14, compare, projection);
            iter_swap_if(first + 13, first + 15, compare, projection);
            iter_swap_if(first + 16, first + 18, compare, projection);
            iter_swap_if(first + 17, first + 19, compare, projection);
            iter_swap_if(first + 20, first + 22, compare, projection);
            iter_swap_if(first + 21, first + 23, compare, projection);
            iter_swap_if(first + 24, first + 26, compare, projection);
            iter_swap_if(first + 25, first + 27, compare, projection);
            iter_swap_if(first + 28, first + 30, compare, projection);
            iter_swap_if(first + 29, first + 31, compare, projection);
            iter_swap_if(first + 32, first + 34, compare, projection);
            iter_swap_if(first + 33, first + 35, compare, projection);
            iter_swap_if(first + 36, first + 38, compare, projection);
            iter_swap_if(first + 37, first + 39, compare, projection);
            iter_swap_if(first + 40, first + 42, compare, projection);
            iter_swap_if(first + 41, first + 43, compare, projection);
            iter_swap_if(first + 44, first + 46, compare, projection);
            iter_swap_if(first + 45, first + 47, compare, projection);
            iter_swap_if(first + 48, first + 50, compare, projection);
            iter_swap_if(first + 49, first + 51, compare, projection);
            iter_swap_if(first + 52, first + 54, compare, projection);
            iter_swap_if(first + 53, first + 55, compare, projection);
            iter_swap_if(first + 56, first + 58, compare, projection);
            iter_swap_if(first + 57, first + 59, compare, projection);
            iter_swap_if(first + 60, first + 62, compare, projection);
            iter_swap_if(first + 61, first + 63, compare, projection);
            iter_swap_if(first, first + 1, compare, projection);
            iter_swap_if(first + 2, first + 3, compare, projection);
            iter_swap_if(first + 4, first + 5, compare, projection);
            iter_swap_if(first + 6, first + 7, compare, projection);
            iter_swap_if(first + 8, first + 9, compare, projection);
            iter_swap_if(first + 10, first + 11, compare, projection);
            iter_swap_if(first + 12, first + 13, compare, projection);
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
            iter_swap_if(first + 36, first + 37, compare, projection);
            iter_swap_if(first + 38, first + 39, compare, projection);
            iter_swap_if(first + 40, first + 41, compare, projection);
            iter_swap_if(first + 42, first + 43, compare, projection);
            iter_swap_if(first + 44, first + 45, compare, projection);
            iter_swap_if(first + 46, first + 47, compare, projection);
            iter_swap_if(first + 48, first + 49, compare, projection);
            iter_swap_if(first + 50, first + 51, compare, projection);
            iter_swap_if(first + 52, first + 53, compare, projection);
            iter_swap_if(first + 54, first + 55, compare, projection);
            iter_swap_if(first + 56, first + 57, compare, projection);
            iter_swap_if(first + 58, first + 59, compare, projection);
            iter_swap_if(first + 60, first + 61, compare, projection);
            iter_swap_if(first + 62, first + 63, compare, projection);
            iter_swap_if(first, first + 52, compare, projection);
            iter_swap_if(first + 1, first + 2, compare, projection);
            iter_swap_if(first + 3, first + 55, compare, projection);
            iter_swap_if(first + 4, first + 48, compare, projection);
            iter_swap_if(first + 5, first + 6, compare, projection);
            iter_swap_if(first + 7, first + 51, compare, projection);
            iter_swap_if(first + 8, first + 60, compare, projection);
            iter_swap_if(first + 9, first + 10, compare, projection);
            iter_swap_if(first + 11, first + 63, compare, projection);
            iter_swap_if(first + 12, first + 56, compare, projection);
            iter_swap_if(first + 13, first + 14, compare, projection);
            iter_swap_if(first + 15, first + 59, compare, projection);
            iter_swap_if(first + 16, first + 32, compare, projection);
            iter_swap_if(first + 17, first + 18, compare, projection);
            iter_swap_if(first + 19, first + 35, compare, projection);
            iter_swap_if(first + 20, first + 24, compare, projection);
            iter_swap_if(first + 21, first + 22, compare, projection);
            iter_swap_if(first + 23, first + 27, compare, projection);
            iter_swap_if(first + 25, first + 26, compare, projection);
            iter_swap_if(first + 28, first + 44, compare, projection);
            iter_swap_if(first + 29, first + 30, compare, projection);
            iter_swap_if(first + 31, first + 47, compare, projection);
            iter_swap_if(first + 33, first + 34, compare, projection);
            iter_swap_if(first + 36, first + 40, compare, projection);
            iter_swap_if(first + 37, first + 38, compare, projection);
            iter_swap_if(first + 39, first + 43, compare, projection);
            iter_swap_if(first + 41, first + 42, compare, projection);
            iter_swap_if(first + 45, first + 46, compare, projection);
            iter_swap_if(first + 49, first + 50, compare, projection);
            iter_swap_if(first + 53, first + 54, compare, projection);
            iter_swap_if(first + 57, first + 58, compare, projection);
            iter_swap_if(first + 61, first + 62, compare, projection);
            iter_swap_if(first, first + 20, compare, projection);
            iter_swap_if(first + 1, first + 53, compare, projection);
            iter_swap_if(first + 2, first + 54, compare, projection);
            iter_swap_if(first + 3, first + 23, compare, projection);
            iter_swap_if(first + 4, first + 28, compare, projection);
            iter_swap_if(first + 5, first + 49, compare, projection);
            iter_swap_if(first + 6, first + 50, compare, projection);
            iter_swap_if(first + 7, first + 31, compare, projection);
            iter_swap_if(first + 8, first + 36, compare, projection);
            iter_swap_if(first + 9, first + 61, compare, projection);
            iter_swap_if(first + 10, first + 62, compare, projection);
            iter_swap_if(first + 11, first + 39, compare, projection);
            iter_swap_if(first + 12, first + 16, compare, projection);
            iter_swap_if(first + 13, first + 57, compare, projection);
            iter_swap_if(first + 14, first + 58, compare, projection);
            iter_swap_if(first + 15, first + 19, compare, projection);
            iter_swap_if(first + 17, first + 33, compare, projection);
            iter_swap_if(first + 18, first + 34, compare, projection);
            iter_swap_if(first + 21, first + 25, compare, projection);
            iter_swap_if(first + 22, first + 26, compare, projection);
            iter_swap_if(first + 24, first + 52, compare, projection);
            iter_swap_if(first + 27, first + 55, compare, projection);
            iter_swap_if(first + 29, first + 45, compare, projection);
            iter_swap_if(first + 30, first + 46, compare, projection);
            iter_swap_if(first + 32, first + 56, compare, projection);
            iter_swap_if(first + 35, first + 59, compare, projection);
            iter_swap_if(first + 37, first + 41, compare, projection);
            iter_swap_if(first + 38, first + 42, compare, projection);
            iter_swap_if(first + 40, first + 60, compare, projection);
            iter_swap_if(first + 43, first + 63, compare, projection);
            iter_swap_if(first + 44, first + 48, compare, projection);
            iter_swap_if(first + 47, first + 51, compare, projection);
            iter_swap_if(first, first + 4, compare, projection);
            iter_swap_if(first + 1, first + 21, compare, projection);
            iter_swap_if(first + 2, first + 22, compare, projection);
            iter_swap_if(first + 3, first + 7, compare, projection);
            iter_swap_if(first + 5, first + 29, compare, projection);
            iter_swap_if(first + 6, first + 30, compare, projection);
            iter_swap_if(first + 8, first + 12, compare, projection);
            iter_swap_if(first + 9, first + 37, compare, projection);
            iter_swap_if(first + 10, first + 38, compare, projection);
            iter_swap_if(first + 11, first + 15, compare, projection);
            iter_swap_if(first + 13, first + 17, compare, projection);
            iter_swap_if(first + 14, first + 18, compare, projection);
            iter_swap_if(first + 16, first + 20, compare, projection);
            iter_swap_if(first + 19, first + 23, compare, projection);
            iter_swap_if(first + 24, first + 32, compare, projection);
            iter_swap_if(first + 25, first + 53, compare, projection);
            iter_swap_if(first + 26, first + 54, compare, projection);
            iter_swap_if(first + 27, first + 35, compare, projection);
            iter_swap_if(first + 28, first + 36, compare, projection);
            iter_swap_if(first + 31, first + 39, compare, projection);
            iter_swap_if(first + 33, first + 57, compare, projection);
            iter_swap_if(first + 34, first + 58, compare, projection);
            iter_swap_if(first + 40, first + 44, compare, projection);
            iter_swap_if(first + 41, first + 61, compare, projection);
            iter_swap_if(first + 42, first + 62, compare, projection);
            iter_swap_if(first + 43, first + 47, compare, projection);
            iter_swap_if(first + 45, first + 49, compare, projection);
            iter_swap_if(first + 46, first + 50, compare, projection);
            iter_swap_if(first + 48, first + 52, compare, projection);
            iter_swap_if(first + 51, first + 55, compare, projection);
            iter_swap_if(first + 56, first + 60, compare, projection);
            iter_swap_if(first + 59, first + 63, compare, projection);
            iter_swap_if(first, first + 8, compare, projection);
            iter_swap_if(first + 1, first + 5, compare, projection);
            iter_swap_if(first + 2, first + 6, compare, projection);
            iter_swap_if(first + 3, first + 11, compare, projection);
            iter_swap_if(first + 4, first + 12, compare, projection);
            iter_swap_if(first + 7, first + 15, compare, projection);
            iter_swap_if(first + 9, first + 13, compare, projection);
            iter_swap_if(first + 10, first + 14, compare, projection);
            iter_swap_if(first + 16, first + 40, compare, projection);
            iter_swap_if(first + 17, first + 21, compare, projection);
            iter_swap_if(first + 18, first + 22, compare, projection);
            iter_swap_if(first + 19, first + 43, compare, projection);
            iter_swap_if(first + 20, first + 44, compare, projection);
            iter_swap_if(first + 23, first + 47, compare, projection);
            iter_swap_if(first + 24, first + 28, compare, projection);
            iter_swap_if(first + 25, first + 33, compare, projection);
            iter_swap_if(first + 26, first + 34, compare, projection);
            iter_swap_if(first + 27, first + 31, compare, projection);
            iter_swap_if(first + 29, first + 37, compare, projection);
            iter_swap_if(first + 30, first + 38, compare, projection);
            iter_swap_if(first + 32, first + 36, compare, projection);
            iter_swap_if(first + 35, first + 39, compare, projection);
            iter_swap_if(first + 41, first + 45, compare, projection);
            iter_swap_if(first + 42, first + 46, compare, projection);
            iter_swap_if(first + 48, first + 56, compare, projection);
            iter_swap_if(first + 49, first + 53, compare, projection);
            iter_swap_if(first + 50, first + 54, compare, projection);
            iter_swap_if(first + 51, first + 59, compare, projection);
            iter_swap_if(first + 52, first + 60, compare, projection);
            iter_swap_if(first + 55, first + 63, compare, projection);
            iter_swap_if(first + 57, first + 61, compare, projection);
            iter_swap_if(first + 58, first + 62, compare, projection);
            iter_swap_if(first + 1, first + 9, compare, projection);
            iter_swap_if(first + 2, first + 10, compare, projection);
            iter_swap_if(first + 4, first + 8, compare, projection);
            iter_swap_if(first + 5, first + 13, compare, projection);
            iter_swap_if(first + 6, first + 14, compare, projection);
            iter_swap_if(first + 7, first + 11, compare, projection);
            iter_swap_if(first + 12, first + 48, compare, projection);
            iter_swap_if(first + 15, first + 51, compare, projection);
            iter_swap_if(first + 16, first + 24, compare, projection);
            iter_swap_if(first + 17, first + 41, compare, projection);
            iter_swap_if(first + 18, first + 42, compare, projection);
            iter_swap_if(first + 19, first + 27, compare, projection);
            iter_swap_if(first + 20, first + 28, compare, projection);
            iter_swap_if(first + 21, first + 45, compare, projection);
            iter_swap_if(first + 22, first + 46, compare, projection);
            iter_swap_if(first + 23, first + 31, compare, projection);
            iter_swap_if(first + 25, first + 29, compare, projection);
            iter_swap_if(first + 26, first + 30, compare, projection);
            iter_swap_if(first + 32, first + 40, compare, projection);
            iter_swap_if(first + 33, first + 37, compare, projection);
            iter_swap_if(first + 34, first + 38, compare, projection);
            iter_swap_if(first + 35, first + 43, compare, projection);
            iter_swap_if(first + 36, first + 44, compare, projection);
            iter_swap_if(first + 39, first + 47, compare, projection);
            iter_swap_if(first + 49, first + 57, compare, projection);
            iter_swap_if(first + 50, first + 58, compare, projection);
            iter_swap_if(first + 52, first + 56, compare, projection);
            iter_swap_if(first + 53, first + 61, compare, projection);
            iter_swap_if(first + 54, first + 62, compare, projection);
            iter_swap_if(first + 55, first + 59, compare, projection);
            iter_swap_if(first + 4, first + 16, compare, projection);
            iter_swap_if(first + 5, first + 9, compare, projection);
            iter_swap_if(first + 6, first + 10, compare, projection);
            iter_swap_if(first + 7, first + 19, compare, projection);
            iter_swap_if(first + 8, first + 24, compare, projection);
            iter_swap_if(first + 11, first + 27, compare, projection);
            iter_swap_if(first + 13, first + 49, compare, projection);
            iter_swap_if(first + 14, first + 50, compare, projection);
            iter_swap_if(first + 17, first + 25, compare, projection);
            iter_swap_if(first + 18, first + 26, compare, projection);
            iter_swap_if(first + 20, first + 32, compare, projection);
            iter_swap_if(first + 21, first + 29, compare, projection);
            iter_swap_if(first + 22, first + 30, compare, projection);
            iter_swap_if(first + 23, first + 35, compare, projection);
            iter_swap_if(first + 28, first + 40, compare, projection);
            iter_swap_if(first + 31, first + 43, compare, projection);
            iter_swap_if(first + 33, first + 41, compare, projection);
            iter_swap_if(first + 34, first + 42, compare, projection);
            iter_swap_if(first + 36, first + 52, compare, projection);
            iter_swap_if(first + 37, first + 45, compare, projection);
            iter_swap_if(first + 38, first + 46, compare, projection);
            iter_swap_if(first + 39, first + 55, compare, projection);
            iter_swap_if(first + 44, first + 56, compare, projection);
            iter_swap_if(first + 47, first + 59, compare, projection);
            iter_swap_if(first + 53, first + 57, compare, projection);
            iter_swap_if(first + 54, first + 58, compare, projection);
            iter_swap_if(first + 1, first + 4, compare, projection);
            iter_swap_if(first + 5, first + 17, compare, projection);
            iter_swap_if(first + 6, first + 18, compare, projection);
            iter_swap_if(first + 8, first + 16, compare, projection);
            iter_swap_if(first + 9, first + 25, compare, projection);
            iter_swap_if(first + 10, first + 26, compare, projection);
            iter_swap_if(first + 11, first + 19, compare, projection);
            iter_swap_if(first + 12, first + 24, compare, projection);
            iter_swap_if(first + 15, first + 27, compare, projection);
            iter_swap_if(first + 21, first + 33, compare, projection);
            iter_swap_if(first + 22, first + 34, compare, projection);
            iter_swap_if(first + 29, first + 41, compare, projection);
            iter_swap_if(first + 30, first + 42, compare, projection);
            iter_swap_if(first + 36, first + 48, compare, projection);
            iter_swap_if(first + 37, first + 53, compare, projection);
            iter_swap_if(first + 38, first + 54, compare, projection);
            iter_swap_if(first + 39, first + 51, compare, projection);
            iter_swap_if(first + 44, first + 52, compare, projection);
            iter_swap_if(first + 45, first + 57, compare, projection);
            iter_swap_if(first + 46, first + 58, compare, projection);
            iter_swap_if(first + 47, first + 55, compare, projection);
            iter_swap_if(first + 59, first + 62, compare, projection);
            iter_swap_if(first + 2, first + 8, compare, projection);
            iter_swap_if(first + 9, first + 17, compare, projection);
            iter_swap_if(first + 10, first + 18, compare, projection);
            iter_swap_if(first + 12, first + 20, compare, projection);
            iter_swap_if(first + 13, first + 25, compare, projection);
            iter_swap_if(first + 14, first + 26, compare, projection);
            iter_swap_if(first + 15, first + 23, compare, projection);
            iter_swap_if(first + 24, first + 32, compare, projection);
            iter_swap_if(first + 27, first + 35, compare, projection);
            iter_swap_if(first + 28, first + 36, compare, projection);
            iter_swap_if(first + 31, first + 39, compare, projection);
            iter_swap_if(first + 37, first + 49, compare, projection);
            iter_swap_if(first + 38, first + 50, compare, projection);
            iter_swap_if(first + 40, first + 48, compare, projection);
            iter_swap_if(first + 43, first + 51, compare, projection);
            iter_swap_if(first + 45, first + 53, compare, projection);
            iter_swap_if(first + 46, first + 54, compare, projection);
            iter_swap_if(first + 55, first + 61, compare, projection);
            iter_swap_if(first + 2, first + 4, compare, projection);
            iter_swap_if(first + 12, first + 16, compare, projection);
            iter_swap_if(first + 13, first + 21, compare, projection);
            iter_swap_if(first + 14, first + 22, compare, projection);
            iter_swap_if(first + 15, first + 19, compare, projection);
            iter_swap_if(first + 20, first + 24, compare, projection);
            iter_swap_if(first + 23, first + 27, compare, projection);
            iter_swap_if(first + 25, first + 33, compare, projection);
            iter_swap_if(first + 26, first + 34, compare, projection);
            iter_swap_if(first + 28, first + 32, compare, projection);
            iter_swap_if(first + 29, first + 37, compare, projection);
            iter_swap_if(first + 30, first + 38, compare, projection);
            iter_swap_if(first + 31, first + 35, compare, projection);
            iter_swap_if(first + 36, first + 40, compare, projection);
            iter_swap_if(first + 39, first + 43, compare, projection);
            iter_swap_if(first + 41, first + 49, compare, projection);
            iter_swap_if(first + 42, first + 50, compare, projection);
            iter_swap_if(first + 44, first + 48, compare, projection);
            iter_swap_if(first + 47, first + 51, compare, projection);
            iter_swap_if(first + 59, first + 61, compare, projection);
            iter_swap_if(first + 4, first + 16, compare, projection);
            iter_swap_if(first + 5, first + 20, compare, projection);
            iter_swap_if(first + 10, first + 40, compare, projection);
            iter_swap_if(first + 13, first + 17, compare, projection);
            iter_swap_if(first + 14, first + 18, compare, projection);
            iter_swap_if(first + 21, first + 25, compare, projection);
            iter_swap_if(first + 22, first + 26, compare, projection);
            iter_swap_if(first + 23, first + 53, compare, projection);
            iter_swap_if(first + 24, first + 28, compare, projection);
            iter_swap_if(first + 27, first + 31, compare, projection);
            iter_swap_if(first + 29, first + 33, compare, projection);
            iter_swap_if(first + 30, first + 34, compare, projection);
            iter_swap_if(first + 32, first + 36, compare, projection);
            iter_swap_if(first + 35, first + 39, compare, projection);
            iter_swap_if(first + 37, first + 41, compare, projection);
            iter_swap_if(first + 38, first + 42, compare, projection);
            iter_swap_if(first + 43, first + 58, compare, projection);
            iter_swap_if(first + 45, first + 49, compare, projection);
            iter_swap_if(first + 46, first + 50, compare, projection);
            iter_swap_if(first + 47, first + 59, compare, projection);
            iter_swap_if(first + 3, first + 17, compare, projection);
            iter_swap_if(first + 6, first + 36, compare, projection);
            iter_swap_if(first + 7, first + 21, compare, projection);
            iter_swap_if(first + 8, first + 32, compare, projection);
            iter_swap_if(first + 9, first + 24, compare, projection);
            iter_swap_if(first + 11, first + 41, compare, projection);
            iter_swap_if(first + 13, first + 28, compare, projection);
            iter_swap_if(first + 14, first + 44, compare, projection);
            iter_swap_if(first + 15, first + 45, compare, projection);
            iter_swap_if(first + 18, first + 48, compare, projection);
            iter_swap_if(first + 19, first + 49, compare, projection);
            iter_swap_if(first + 22, first + 52, compare, projection);
            iter_swap_if(first + 25, first + 29, compare, projection);
            iter_swap_if(first + 26, first + 30, compare, projection);
            iter_swap_if(first + 27, first + 57, compare, projection);
            iter_swap_if(first + 31, first + 55, compare, projection);
            iter_swap_if(first + 33, first + 37, compare, projection);
            iter_swap_if(first + 34, first + 38, compare, projection);
            iter_swap_if(first + 35, first + 50, compare, projection);
            iter_swap_if(first + 39, first + 54, compare, projection);
            iter_swap_if(first + 42, first + 56, compare, projection);
            iter_swap_if(first + 46, first + 60, compare, projection);
            iter_swap_if(first + 6, first + 20, compare, projection);
            iter_swap_if(first + 8, first + 16, compare, projection);
            iter_swap_if(first + 10, first + 24, compare, projection);
            iter_swap_if(first + 11, first + 25, compare, projection);
            iter_swap_if(first + 14, first + 28, compare, projection);
            iter_swap_if(first + 15, first + 29, compare, projection);
            iter_swap_if(first + 17, first + 33, compare, projection);
            iter_swap_if(first + 18, first + 32, compare, projection);
            iter_swap_if(first + 21, first + 37, compare, projection);
            iter_swap_if(first + 22, first + 36, compare, projection);
            iter_swap_if(first + 26, first + 42, compare, projection);
            iter_swap_if(first + 27, first + 41, compare, projection);
            iter_swap_if(first + 30, first + 46, compare, projection);
            iter_swap_if(first + 31, first + 45, compare, projection);
            iter_swap_if(first + 34, first + 48, compare, projection);
            iter_swap_if(first + 35, first + 49, compare, projection);
            iter_swap_if(first + 38, first + 52, compare, projection);
            iter_swap_if(first + 39, first + 53, compare, projection);
            iter_swap_if(first + 43, first + 57, compare, projection);
            iter_swap_if(first + 47, first + 55, compare, projection);
            iter_swap_if(first + 3, first + 18, compare, projection);
            iter_swap_if(first + 5, first + 8, compare, projection);
            iter_swap_if(first + 6, first + 12, compare, projection);
            iter_swap_if(first + 7, first + 22, compare, projection);
            iter_swap_if(first + 15, first + 21, compare, projection);
            iter_swap_if(first + 17, first + 32, compare, projection);
            iter_swap_if(first + 19, first + 33, compare, projection);
            iter_swap_if(first + 23, first + 37, compare, projection);
            iter_swap_if(first + 26, first + 40, compare, projection);
            iter_swap_if(first + 30, first + 44, compare, projection);
            iter_swap_if(first + 31, first + 46, compare, projection);
            iter_swap_if(first + 41, first + 56, compare, projection);
            iter_swap_if(first + 42, first + 48, compare, projection);
            iter_swap_if(first + 45, first + 60, compare, projection);
            iter_swap_if(first + 51, first + 57, compare, projection);
            iter_swap_if(first + 55, first + 58, compare, projection);
            iter_swap_if(first + 3, first + 16, compare, projection);
            iter_swap_if(first + 7, first + 20, compare, projection);
            iter_swap_if(first + 11, first + 26, compare, projection);
            iter_swap_if(first + 18, first + 24, compare, projection);
            iter_swap_if(first + 19, first + 25, compare, projection);
            iter_swap_if(first + 22, first + 28, compare, projection);
            iter_swap_if(first + 23, first + 29, compare, projection);
            iter_swap_if(first + 27, first + 33, compare, projection);
            iter_swap_if(first + 30, first + 36, compare, projection);
            iter_swap_if(first + 34, first + 40, compare, projection);
            iter_swap_if(first + 35, first + 41, compare, projection);
            iter_swap_if(first + 37, first + 52, compare, projection);
            iter_swap_if(first + 38, first + 44, compare, projection);
            iter_swap_if(first + 39, first + 45, compare, projection);
            iter_swap_if(first + 43, first + 56, compare, projection);
            iter_swap_if(first + 47, first + 60, compare, projection);
            iter_swap_if(first + 3, first + 9, compare, projection);
            iter_swap_if(first + 7, first + 13, compare, projection);
            iter_swap_if(first + 10, first + 16, compare, projection);
            iter_swap_if(first + 11, first + 17, compare, projection);
            iter_swap_if(first + 14, first + 20, compare, projection);
            iter_swap_if(first + 15, first + 30, compare, projection);
            iter_swap_if(first + 19, first + 34, compare, projection);
            iter_swap_if(first + 21, first + 36, compare, projection);
            iter_swap_if(first + 23, first + 38, compare, projection);
            iter_swap_if(first + 25, first + 40, compare, projection);
            iter_swap_if(first + 26, first + 32, compare, projection);
            iter_swap_if(first + 27, first + 42, compare, projection);
            iter_swap_if(first + 29, first + 44, compare, projection);
            iter_swap_if(first + 31, first + 37, compare, projection);
            iter_swap_if(first + 33, first + 48, compare, projection);
            iter_swap_if(first + 43, first + 49, compare, projection);
            iter_swap_if(first + 46, first + 52, compare, projection);
            iter_swap_if(first + 47, first + 53, compare, projection);
            iter_swap_if(first + 50, first + 56, compare, projection);
            iter_swap_if(first + 54, first + 60, compare, projection);
            iter_swap_if(first + 3, first + 8, compare, projection);
            iter_swap_if(first + 7, first + 10, compare, projection);
            iter_swap_if(first + 9, first + 12, compare, projection);
            iter_swap_if(first + 11, first + 18, compare, projection);
            iter_swap_if(first + 13, first + 14, compare, projection);
            iter_swap_if(first + 15, first + 24, compare, projection);
            iter_swap_if(first + 17, first + 22, compare, projection);
            iter_swap_if(first + 19, first + 28, compare, projection);
            iter_swap_if(first + 21, first + 26, compare, projection);
            iter_swap_if(first + 23, first + 25, compare, projection);
            iter_swap_if(first + 27, first + 34, compare, projection);
            iter_swap_if(first + 29, first + 36, compare, projection);
            iter_swap_if(first + 30, first + 32, compare, projection);
            iter_swap_if(first + 31, first + 33, compare, projection);
            iter_swap_if(first + 35, first + 44, compare, projection);
            iter_swap_if(first + 37, first + 42, compare, projection);
            iter_swap_if(first + 38, first + 40, compare, projection);
            iter_swap_if(first + 39, first + 48, compare, projection);
            iter_swap_if(first + 41, first + 46, compare, projection);
            iter_swap_if(first + 45, first + 52, compare, projection);
            iter_swap_if(first + 49, first + 50, compare, projection);
            iter_swap_if(first + 51, first + 54, compare, projection);
            iter_swap_if(first + 53, first + 56, compare, projection);
            iter_swap_if(first + 55, first + 60, compare, projection);
            iter_swap_if(first + 3, first + 6, compare, projection);
            iter_swap_if(first + 7, first + 12, compare, projection);
            iter_swap_if(first + 11, first + 16, compare, projection);
            iter_swap_if(first + 15, first + 17, compare, projection);
            iter_swap_if(first + 18, first + 20, compare, projection);
            iter_swap_if(first + 19, first + 24, compare, projection);
            iter_swap_if(first + 21, first + 22, compare, projection);
            iter_swap_if(first + 23, first + 30, compare, projection);
            iter_swap_if(first + 25, first + 32, compare, projection);
            iter_swap_if(first + 26, first + 28, compare, projection);
            iter_swap_if(first + 27, first + 29, compare, projection);
            iter_swap_if(first + 31, first + 38, compare, projection);
            iter_swap_if(first + 33, first + 40, compare, projection);
            iter_swap_if(first + 34, first + 36, compare, projection);
            iter_swap_if(first + 35, first + 37, compare, projection);
            iter_swap_if(first + 39, first + 44, compare, projection);
            iter_swap_if(first + 41, first + 42, compare, projection);
            iter_swap_if(first + 43, first + 45, compare, projection);
            iter_swap_if(first + 46, first + 48, compare, projection);
            iter_swap_if(first + 47, first + 52, compare, projection);
            iter_swap_if(first + 51, first + 56, compare, projection);
            iter_swap_if(first + 57, first + 60, compare, projection);
            iter_swap_if(first + 3, first + 5, compare, projection);
            iter_swap_if(first + 6, first + 8, compare, projection);
            iter_swap_if(first + 7, first + 9, compare, projection);
            iter_swap_if(first + 10, first + 12, compare, projection);
            iter_swap_if(first + 11, first + 13, compare, projection);
            iter_swap_if(first + 14, first + 16, compare, projection);
            iter_swap_if(first + 15, first + 18, compare, projection);
            iter_swap_if(first + 17, first + 20, compare, projection);
            iter_swap_if(first + 19, first + 21, compare, projection);
            iter_swap_if(first + 22, first + 24, compare, projection);
            iter_swap_if(first + 23, first + 26, compare, projection);
            iter_swap_if(first + 25, first + 28, compare, projection);
            iter_swap_if(first + 27, first + 30, compare, projection);
            iter_swap_if(first + 29, first + 32, compare, projection);
            iter_swap_if(first + 31, first + 34, compare, projection);
            iter_swap_if(first + 33, first + 36, compare, projection);
            iter_swap_if(first + 35, first + 38, compare, projection);
            iter_swap_if(first + 37, first + 40, compare, projection);
            iter_swap_if(first + 39, first + 41, compare, projection);
            iter_swap_if(first + 42, first + 44, compare, projection);
            iter_swap_if(first + 43, first + 46, compare, projection);
            iter_swap_if(first + 45, first + 48, compare, projection);
            iter_swap_if(first + 47, first + 49, compare, projection);
            iter_swap_if(first + 50, first + 52, compare, projection);
            iter_swap_if(first + 51, first + 53, compare, projection);
            iter_swap_if(first + 54, first + 56, compare, projection);
            iter_swap_if(first + 55, first + 57, compare, projection);
            iter_swap_if(first + 58, first + 60, compare, projection);
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
            iter_swap_if(first + 41, first + 42, compare, projection);
            iter_swap_if(first + 43, first + 44, compare, projection);
            iter_swap_if(first + 45, first + 46, compare, projection);
            iter_swap_if(first + 47, first + 48, compare, projection);
            iter_swap_if(first + 49, first + 50, compare, projection);
            iter_swap_if(first + 51, first + 52, compare, projection);
            iter_swap_if(first + 55, first + 56, compare, projection);
            iter_swap_if(first + 59, first + 60, compare, projection);
        }

        template<typename DifferenceType=std::ptrdiff_t>
        CPPSORT_ATTRIBUTE_NODISCARD
        static constexpr auto index_pairs()
            -> std::array<utility::index_pair<DifferenceType>, 521>
        {
            return {{
                {0, 2}, {1, 3}, {4, 6}, {5, 7}, {8, 10}, {9, 11}, {12, 14}, {13, 15}, {16, 18}, {17, 19}, {20, 22}, {21, 23}, {24, 26}, {25, 27}, {28, 30}, {29, 31}, {32, 34}, {33, 35}, {36, 38}, {37, 39}, {40, 42}, {41, 43}, {44, 46}, {45, 47}, {48, 50}, {49, 51}, {52, 54}, {53, 55}, {56, 58}, {57, 59}, {60, 62}, {61, 63},
                {0, 1}, {2, 3}, {4, 5}, {6, 7}, {8, 9}, {10, 11}, {12, 13}, {14, 15}, {16, 17}, {18, 19}, {20, 21}, {22, 23}, {24, 25}, {26, 27}, {28, 29}, {30, 31}, {32, 33}, {34, 35}, {36, 37}, {38, 39}, {40, 41}, {42, 43}, {44, 45}, {46, 47}, {48, 49}, {50, 51}, {52, 53}, {54, 55}, {56, 57}, {58, 59}, {60, 61}, {62, 63},
                {0, 52}, {1, 2}, {3, 55}, {4, 48}, {5, 6}, {7, 51}, {8, 60}, {9, 10}, {11, 63}, {12, 56}, {13, 14}, {15, 59}, {16, 32}, {17, 18}, {19, 35}, {20, 24}, {21, 22}, {23, 27}, {25, 26}, {28, 44}, {29, 30}, {31, 47}, {33, 34}, {36, 40}, {37, 38}, {39, 43}, {41, 42}, {45, 46}, {49, 50}, {53, 54}, {57, 58}, {61, 62},
                {0, 20}, {1, 53}, {2, 54}, {3, 23}, {4, 28}, {5, 49}, {6, 50}, {7, 31}, {8, 36}, {9, 61}, {10, 62}, {11, 39}, {12, 16}, {13, 57}, {14, 58}, {15, 19}, {17, 33}, {18, 34}, {21, 25}, {22, 26}, {24, 52}, {27, 55}, {29, 45}, {30, 46}, {32, 56}, {35, 59}, {37, 41}, {38, 42}, {40, 60}, {43, 63}, {44, 48}, {47, 51},
                {0, 4}, {1, 21}, {2, 22}, {3, 7}, {5, 29}, {6, 30}, {8, 12}, {9, 37}, {10, 38}, {11, 15}, {13, 17}, {14, 18}, {16, 20}, {19, 23}, {24, 32}, {25, 53}, {26, 54}, {27, 35}, {28, 36}, {31, 39}, {33, 57}, {34, 58}, {40, 44}, {41, 61}, {42, 62}, {43, 47}, {45, 49}, {46, 50}, {48, 52}, {51, 55}, {56, 60}, {59, 63},
                {0, 8}, {1, 5}, {2, 6}, {3, 11}, {4, 12}, {7, 15}, {9, 13}, {10, 14}, {16, 40}, {17, 21}, {18, 22}, {19, 43}, {20, 44}, {23, 47}, {24, 28}, {25, 33}, {26, 34}, {27, 31}, {29, 37}, {30, 38}, {32, 36}, {35, 39}, {41, 45}, {42, 46}, {48, 56}, {49, 53}, {50, 54}, {51, 59}, {52, 60}, {55, 63}, {57, 61}, {58, 62},
                {1, 9}, {2, 10}, {4, 8}, {5, 13}, {6, 14}, {7, 11}, {12, 48}, {15, 51}, {16, 24}, {17, 41}, {18, 42}, {19, 27}, {20, 28}, {21, 45}, {22, 46}, {23, 31}, {25, 29}, {26, 30}, {32, 40}, {33, 37}, {34, 38}, {35, 43}, {36, 44}, {39, 47}, {49, 57}, {50, 58}, {52, 56}, {53, 61}, {54, 62}, {55, 59},
                {4, 16}, {5, 9}, {6, 10}, {7, 19}, {8, 24}, {11, 27}, {13, 49}, {14, 50}, {17, 25}, {18, 26}, {20, 32}, {21, 29}, {22, 30}, {23, 35}, {28, 40}, {31, 43}, {33, 41}, {34, 42}, {36, 52}, {37, 45}, {38, 46}, {39, 55}, {44, 56}, {47, 59}, {53, 57}, {54, 58},
                {1, 4}, {5, 17}, {6, 18}, {8, 16}, {9, 25}, {10, 26}, {11, 19}, {12, 24}, {15, 27}, {21, 33}, {22, 34}, {29, 41}, {30, 42}, {36, 48}, {37, 53}, {38, 54}, {39, 51}, {44, 52}, {45, 57}, {46, 58}, {47, 55}, {59, 62},
                {2, 8}, {9, 17}, {10, 18}, {12, 20}, {13, 25}, {14, 26}, {15, 23}, {24, 32}, {27, 35}, {28, 36}, {31, 39}, {37, 49}, {38, 50}, {40, 48}, {43, 51}, {45, 53}, {46, 54}, {55, 61},
                {2, 4}, {12, 16}, {13, 21}, {14, 22}, {15, 19}, {20, 24}, {23, 27}, {25, 33}, {26, 34}, {28, 32}, {29, 37}, {30, 38}, {31, 35}, {36, 40}, {39, 43}, {41, 49}, {42, 50}, {44, 48}, {47, 51}, {59, 61},
                {4, 16}, {5, 20}, {10, 40}, {13, 17}, {14, 18}, {21, 25}, {22, 26}, {23, 53}, {24, 28}, {27, 31}, {29, 33}, {30, 34}, {32, 36}, {35, 39}, {37, 41}, {38, 42}, {43, 58}, {45, 49}, {46, 50}, {47, 59},
                {3, 17}, {6, 36}, {7, 21}, {8, 32}, {9, 24}, {11, 41}, {13, 28}, {14, 44}, {15, 45}, {18, 48}, {19, 49}, {22, 52}, {25, 29}, {26, 30}, {27, 57}, {31, 55}, {33, 37}, {34, 38}, {35, 50}, {39, 54}, {42, 56}, {46, 60},
                {6, 20}, {8, 16}, {10, 24}, {11, 25}, {14, 28}, {15, 29}, {17, 33}, {18, 32}, {21, 37}, {22, 36}, {26, 42}, {27, 41}, {30, 46}, {31, 45}, {34, 48}, {35, 49}, {38, 52}, {39, 53}, {43, 57}, {47, 55},
                {3, 18}, {5, 8}, {6, 12}, {7, 22}, {15, 21}, {17, 32}, {19, 33}, {23, 37}, {26, 40}, {30, 44}, {31, 46}, {41, 56}, {42, 48}, {45, 60}, {51, 57}, {55, 58},
                {3, 16}, {7, 20}, {11, 26}, {18, 24}, {19, 25}, {22, 28}, {23, 29}, {27, 33}, {30, 36}, {34, 40}, {35, 41}, {37, 52}, {38, 44}, {39, 45}, {43, 56}, {47, 60},
                {3, 9}, {7, 13}, {10, 16}, {11, 17}, {14, 20}, {15, 30}, {19, 34}, {21, 36}, {23, 38}, {25, 40}, {26, 32}, {27, 42}, {29, 44}, {31, 37}, {33, 48}, {43, 49}, {46, 52}, {47, 53}, {50, 56}, {54, 60},
                {3, 8}, {7, 10}, {9, 12}, {11, 18}, {13, 14}, {15, 24}, {17, 22}, {19, 28}, {21, 26}, {23, 25}, {27, 34}, {29, 36}, {30, 32}, {31, 33}, {35, 44}, {37, 42}, {38, 40}, {39, 48}, {41, 46}, {45, 52}, {49, 50}, {51, 54}, {53, 56}, {55, 60},
                {3, 6}, {7, 12}, {11, 16}, {15, 17}, {18, 20}, {19, 24}, {21, 22}, {23, 30}, {25, 32}, {26, 28}, {27, 29}, {31, 38}, {33, 40}, {34, 36}, {35, 37}, {39, 44}, {41, 42}, {43, 45}, {46, 48}, {47, 52}, {51, 56}, {57, 60},
                {3, 5}, {6, 8}, {7, 9}, {10, 12}, {11, 13}, {14, 16}, {15, 18}, {17, 20}, {19, 21}, {22, 24}, {23, 26}, {25, 28}, {27, 30}, {29, 32}, {31, 34}, {33, 36}, {35, 38}, {37, 40}, {39, 41}, {42, 44}, {43, 46}, {45, 48}, {47, 49}, {50, 52}, {51, 53}, {54, 56}, {55, 57}, {58, 60},
                {3, 4}, {7, 8}, {11, 12}, {13, 14}, {15, 16}, {17, 18}, {19, 20}, {21, 22}, {23, 24}, {25, 26}, {27, 28}, {29, 30}, {31, 32}, {33, 34}, {35, 36}, {37, 38}, {39, 40}, {41, 42}, {43, 44}, {45, 46}, {47, 48}, {49, 50}, {51, 52}, {55, 56}, {59, 60},
            }};
        }
    };
}}

#endif // CPPSORT_DETAIL_SORTING_NETWORK_SORT64_H_
