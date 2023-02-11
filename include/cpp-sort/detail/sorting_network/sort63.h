/*
* Copyright (c) 2023 Morwenn
* SPDX-License-Identifier: MIT
*/
#ifndef CPPSORT_DETAIL_SORTING_NETWORK_SORT63_H_
#define CPPSORT_DETAIL_SORTING_NETWORK_SORT63_H_

namespace cppsort
{
namespace detail
{
    template<>
    struct sorting_network_sorter_impl<63>
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
            iter_swap_if(first + 15, first + 28, compare, projection);
            iter_swap_if(first + 16, first + 27, compare, projection);
            iter_swap_if(first + 17, first + 30, compare, projection);
            iter_swap_if(first + 18, first + 29, compare, projection);
            iter_swap_if(first + 19, first + 23, compare, projection);
            iter_swap_if(first + 20, first + 21, compare, projection);
            iter_swap_if(first + 22, first + 26, compare, projection);
            iter_swap_if(first + 24, first + 25, compare, projection);
            iter_swap_if(first + 31, first + 44, compare, projection);
            iter_swap_if(first + 32, first + 43, compare, projection);
            iter_swap_if(first + 33, first + 46, compare, projection);
            iter_swap_if(first + 34, first + 45, compare, projection);
            iter_swap_if(first + 35, first + 39, compare, projection);
            iter_swap_if(first + 36, first + 37, compare, projection);
            iter_swap_if(first + 38, first + 42, compare, projection);
            iter_swap_if(first + 40, first + 41, compare, projection);
            iter_swap_if(first + 47, first + 60, compare, projection);
            iter_swap_if(first + 48, first + 59, compare, projection);
            iter_swap_if(first + 49, first + 62, compare, projection);
            iter_swap_if(first + 50, first + 61, compare, projection);
            iter_swap_if(first + 51, first + 55, compare, projection);
            iter_swap_if(first + 52, first + 53, compare, projection);
            iter_swap_if(first + 54, first + 58, compare, projection);
            iter_swap_if(first + 56, first + 57, compare, projection);
            iter_swap_if(first, first + 14, compare, projection);
            iter_swap_if(first + 1, first + 5, compare, projection);
            iter_swap_if(first + 2, first + 8, compare, projection);
            iter_swap_if(first + 3, first + 7, compare, projection);
            iter_swap_if(first + 6, first + 9, compare, projection);
            iter_swap_if(first + 10, first + 12, compare, projection);
            iter_swap_if(first + 11, first + 13, compare, projection);
            iter_swap_if(first + 15, first + 20, compare, projection);
            iter_swap_if(first + 16, first + 22, compare, projection);
            iter_swap_if(first + 17, first + 24, compare, projection);
            iter_swap_if(first + 18, first + 19, compare, projection);
            iter_swap_if(first + 21, first + 28, compare, projection);
            iter_swap_if(first + 23, first + 29, compare, projection);
            iter_swap_if(first + 25, first + 30, compare, projection);
            iter_swap_if(first + 26, first + 27, compare, projection);
            iter_swap_if(first + 31, first + 36, compare, projection);
            iter_swap_if(first + 32, first + 38, compare, projection);
            iter_swap_if(first + 33, first + 40, compare, projection);
            iter_swap_if(first + 34, first + 35, compare, projection);
            iter_swap_if(first + 37, first + 44, compare, projection);
            iter_swap_if(first + 39, first + 45, compare, projection);
            iter_swap_if(first + 41, first + 46, compare, projection);
            iter_swap_if(first + 42, first + 43, compare, projection);
            iter_swap_if(first + 47, first + 52, compare, projection);
            iter_swap_if(first + 48, first + 54, compare, projection);
            iter_swap_if(first + 49, first + 56, compare, projection);
            iter_swap_if(first + 50, first + 51, compare, projection);
            iter_swap_if(first + 53, first + 60, compare, projection);
            iter_swap_if(first + 55, first + 61, compare, projection);
            iter_swap_if(first + 57, first + 62, compare, projection);
            iter_swap_if(first + 58, first + 59, compare, projection);
            iter_swap_if(first, first + 7, compare, projection);
            iter_swap_if(first + 1, first + 6, compare, projection);
            iter_swap_if(first + 2, first + 9, compare, projection);
            iter_swap_if(first + 4, first + 10, compare, projection);
            iter_swap_if(first + 5, first + 11, compare, projection);
            iter_swap_if(first + 8, first + 13, compare, projection);
            iter_swap_if(first + 12, first + 14, compare, projection);
            iter_swap_if(first + 15, first + 16, compare, projection);
            iter_swap_if(first + 17, first + 18, compare, projection);
            iter_swap_if(first + 19, first + 20, compare, projection);
            iter_swap_if(first + 21, first + 23, compare, projection);
            iter_swap_if(first + 22, first + 24, compare, projection);
            iter_swap_if(first + 25, first + 26, compare, projection);
            iter_swap_if(first + 27, first + 28, compare, projection);
            iter_swap_if(first + 29, first + 30, compare, projection);
            iter_swap_if(first + 31, first + 32, compare, projection);
            iter_swap_if(first + 33, first + 34, compare, projection);
            iter_swap_if(first + 35, first + 36, compare, projection);
            iter_swap_if(first + 37, first + 39, compare, projection);
            iter_swap_if(first + 38, first + 40, compare, projection);
            iter_swap_if(first + 41, first + 42, compare, projection);
            iter_swap_if(first + 43, first + 44, compare, projection);
            iter_swap_if(first + 45, first + 46, compare, projection);
            iter_swap_if(first + 47, first + 48, compare, projection);
            iter_swap_if(first + 49, first + 50, compare, projection);
            iter_swap_if(first + 51, first + 52, compare, projection);
            iter_swap_if(first + 53, first + 55, compare, projection);
            iter_swap_if(first + 54, first + 56, compare, projection);
            iter_swap_if(first + 57, first + 58, compare, projection);
            iter_swap_if(first + 59, first + 60, compare, projection);
            iter_swap_if(first + 61, first + 62, compare, projection);
            iter_swap_if(first, first + 6, compare, projection);
            iter_swap_if(first + 2, first + 4, compare, projection);
            iter_swap_if(first + 3, first + 5, compare, projection);
            iter_swap_if(first + 7, first + 11, compare, projection);
            iter_swap_if(first + 8, first + 10, compare, projection);
            iter_swap_if(first + 9, first + 12, compare, projection);
            iter_swap_if(first + 13, first + 14, compare, projection);
            iter_swap_if(first + 15, first + 17, compare, projection);
            iter_swap_if(first + 16, first + 18, compare, projection);
            iter_swap_if(first + 19, first + 25, compare, projection);
            iter_swap_if(first + 20, first + 26, compare, projection);
            iter_swap_if(first + 21, first + 22, compare, projection);
            iter_swap_if(first + 23, first + 24, compare, projection);
            iter_swap_if(first + 27, first + 29, compare, projection);
            iter_swap_if(first + 28, first + 30, compare, projection);
            iter_swap_if(first + 31, first + 33, compare, projection);
            iter_swap_if(first + 32, first + 34, compare, projection);
            iter_swap_if(first + 35, first + 41, compare, projection);
            iter_swap_if(first + 36, first + 42, compare, projection);
            iter_swap_if(first + 37, first + 38, compare, projection);
            iter_swap_if(first + 39, first + 40, compare, projection);
            iter_swap_if(first + 43, first + 45, compare, projection);
            iter_swap_if(first + 44, first + 46, compare, projection);
            iter_swap_if(first + 47, first + 49, compare, projection);
            iter_swap_if(first + 48, first + 50, compare, projection);
            iter_swap_if(first + 51, first + 57, compare, projection);
            iter_swap_if(first + 52, first + 58, compare, projection);
            iter_swap_if(first + 53, first + 54, compare, projection);
            iter_swap_if(first + 55, first + 56, compare, projection);
            iter_swap_if(first + 59, first + 61, compare, projection);
            iter_swap_if(first + 60, first + 62, compare, projection);
            iter_swap_if(first, first + 3, compare, projection);
            iter_swap_if(first + 1, first + 2, compare, projection);
            iter_swap_if(first + 4, first + 7, compare, projection);
            iter_swap_if(first + 5, first + 9, compare, projection);
            iter_swap_if(first + 6, first + 8, compare, projection);
            iter_swap_if(first + 10, first + 11, compare, projection);
            iter_swap_if(first + 12, first + 13, compare, projection);
            iter_swap_if(first + 15, first + 47, compare, projection);
            iter_swap_if(first + 16, first + 17, compare, projection);
            iter_swap_if(first + 18, first + 27, compare, projection);
            iter_swap_if(first + 19, first + 21, compare, projection);
            iter_swap_if(first + 20, first + 22, compare, projection);
            iter_swap_if(first + 23, first + 25, compare, projection);
            iter_swap_if(first + 24, first + 26, compare, projection);
            iter_swap_if(first + 28, first + 29, compare, projection);
            iter_swap_if(first + 32, first + 33, compare, projection);
            iter_swap_if(first + 34, first + 43, compare, projection);
            iter_swap_if(first + 35, first + 37, compare, projection);
            iter_swap_if(first + 36, first + 38, compare, projection);
            iter_swap_if(first + 39, first + 41, compare, projection);
            iter_swap_if(first + 40, first + 42, compare, projection);
            iter_swap_if(first + 44, first + 45, compare, projection);
            iter_swap_if(first + 46, first + 62, compare, projection);
            iter_swap_if(first + 48, first + 49, compare, projection);
            iter_swap_if(first + 50, first + 59, compare, projection);
            iter_swap_if(first + 51, first + 53, compare, projection);
            iter_swap_if(first + 52, first + 54, compare, projection);
            iter_swap_if(first + 55, first + 57, compare, projection);
            iter_swap_if(first + 56, first + 58, compare, projection);
            iter_swap_if(first + 60, first + 61, compare, projection);
            iter_swap_if(first, first + 1, compare, projection);
            iter_swap_if(first + 2, first + 3, compare, projection);
            iter_swap_if(first + 4, first + 6, compare, projection);
            iter_swap_if(first + 7, first + 9, compare, projection);
            iter_swap_if(first + 10, first + 12, compare, projection);
            iter_swap_if(first + 11, first + 13, compare, projection);
            iter_swap_if(first + 16, first + 19, compare, projection);
            iter_swap_if(first + 17, first + 21, compare, projection);
            iter_swap_if(first + 20, first + 23, compare, projection);
            iter_swap_if(first + 22, first + 25, compare, projection);
            iter_swap_if(first + 24, first + 28, compare, projection);
            iter_swap_if(first + 26, first + 29, compare, projection);
            iter_swap_if(first + 32, first + 35, compare, projection);
            iter_swap_if(first + 33, first + 37, compare, projection);
            iter_swap_if(first + 36, first + 39, compare, projection);
            iter_swap_if(first + 38, first + 41, compare, projection);
            iter_swap_if(first + 40, first + 44, compare, projection);
            iter_swap_if(first + 42, first + 45, compare, projection);
            iter_swap_if(first + 48, first + 51, compare, projection);
            iter_swap_if(first + 49, first + 53, compare, projection);
            iter_swap_if(first + 52, first + 55, compare, projection);
            iter_swap_if(first + 54, first + 57, compare, projection);
            iter_swap_if(first + 56, first + 60, compare, projection);
            iter_swap_if(first + 58, first + 61, compare, projection);
            iter_swap_if(first, first + 31, compare, projection);
            iter_swap_if(first + 1, first + 2, compare, projection);
            iter_swap_if(first + 3, first + 5, compare, projection);
            iter_swap_if(first + 8, first + 10, compare, projection);
            iter_swap_if(first + 11, first + 12, compare, projection);
            iter_swap_if(first + 14, first + 61, compare, projection);
            iter_swap_if(first + 16, first + 48, compare, projection);
            iter_swap_if(first + 17, first + 19, compare, projection);
            iter_swap_if(first + 18, first + 21, compare, projection);
            iter_swap_if(first + 24, first + 27, compare, projection);
            iter_swap_if(first + 26, first + 28, compare, projection);
            iter_swap_if(first + 29, first + 45, compare, projection);
            iter_swap_if(first + 33, first + 35, compare, projection);
            iter_swap_if(first + 34, first + 37, compare, projection);
            iter_swap_if(first + 40, first + 43, compare, projection);
            iter_swap_if(first + 42, first + 44, compare, projection);
            iter_swap_if(first + 49, first + 51, compare, projection);
            iter_swap_if(first + 50, first + 53, compare, projection);
            iter_swap_if(first + 56, first + 59, compare, projection);
            iter_swap_if(first + 58, first + 60, compare, projection);
            iter_swap_if(first, first + 15, compare, projection);
            iter_swap_if(first + 1, first + 32, compare, projection);
            iter_swap_if(first + 3, first + 4, compare, projection);
            iter_swap_if(first + 5, first + 6, compare, projection);
            iter_swap_if(first + 7, first + 8, compare, projection);
            iter_swap_if(first + 9, first + 10, compare, projection);
            iter_swap_if(first + 13, first + 60, compare, projection);
            iter_swap_if(first + 14, first + 29, compare, projection);
            iter_swap_if(first + 17, first + 49, compare, projection);
            iter_swap_if(first + 18, first + 20, compare, projection);
            iter_swap_if(first + 21, first + 23, compare, projection);
            iter_swap_if(first + 22, first + 24, compare, projection);
            iter_swap_if(first + 25, first + 27, compare, projection);
            iter_swap_if(first + 28, first + 44, compare, projection);
            iter_swap_if(first + 31, first + 47, compare, projection);
            iter_swap_if(first + 34, first + 36, compare, projection);
            iter_swap_if(first + 37, first + 39, compare, projection);
            iter_swap_if(first + 38, first + 40, compare, projection);
            iter_swap_if(first + 41, first + 43, compare, projection);
            iter_swap_if(first + 45, first + 61, compare, projection);
            iter_swap_if(first + 50, first + 52, compare, projection);
            iter_swap_if(first + 53, first + 55, compare, projection);
            iter_swap_if(first + 54, first + 56, compare, projection);
            iter_swap_if(first + 57, first + 59, compare, projection);
            iter_swap_if(first + 1, first + 16, compare, projection);
            iter_swap_if(first + 2, first + 3, compare, projection);
            iter_swap_if(first + 4, first + 5, compare, projection);
            iter_swap_if(first + 6, first + 7, compare, projection);
            iter_swap_if(first + 8, first + 9, compare, projection);
            iter_swap_if(first + 10, first + 11, compare, projection);
            iter_swap_if(first + 13, first + 28, compare, projection);
            iter_swap_if(first + 15, first + 31, compare, projection);
            iter_swap_if(first + 18, first + 19, compare, projection);
            iter_swap_if(first + 20, first + 21, compare, projection);
            iter_swap_if(first + 22, first + 23, compare, projection);
            iter_swap_if(first + 24, first + 25, compare, projection);
            iter_swap_if(first + 26, first + 27, compare, projection);
            iter_swap_if(first + 29, first + 45, compare, projection);
            iter_swap_if(first + 32, first + 48, compare, projection);
            iter_swap_if(first + 34, first + 35, compare, projection);
            iter_swap_if(first + 36, first + 37, compare, projection);
            iter_swap_if(first + 38, first + 39, compare, projection);
            iter_swap_if(first + 40, first + 41, compare, projection);
            iter_swap_if(first + 42, first + 43, compare, projection);
            iter_swap_if(first + 44, first + 60, compare, projection);
            iter_swap_if(first + 50, first + 51, compare, projection);
            iter_swap_if(first + 52, first + 53, compare, projection);
            iter_swap_if(first + 54, first + 55, compare, projection);
            iter_swap_if(first + 56, first + 57, compare, projection);
            iter_swap_if(first + 58, first + 59, compare, projection);
            iter_swap_if(first + 2, first + 33, compare, projection);
            iter_swap_if(first + 4, first + 51, compare, projection);
            iter_swap_if(first + 5, first + 6, compare, projection);
            iter_swap_if(first + 7, first + 8, compare, projection);
            iter_swap_if(first + 10, first + 41, compare, projection);
            iter_swap_if(first + 11, first + 30, compare, projection);
            iter_swap_if(first + 12, first + 59, compare, projection);
            iter_swap_if(first + 16, first + 32, compare, projection);
            iter_swap_if(first + 18, first + 50, compare, projection);
            iter_swap_if(first + 19, first + 35, compare, projection);
            iter_swap_if(first + 20, first + 36, compare, projection);
            iter_swap_if(first + 21, first + 22, compare, projection);
            iter_swap_if(first + 23, first + 24, compare, projection);
            iter_swap_if(first + 25, first + 57, compare, projection);
            iter_swap_if(first + 26, first + 58, compare, projection);
            iter_swap_if(first + 27, first + 43, compare, projection);
            iter_swap_if(first + 28, first + 44, compare, projection);
            iter_swap_if(first + 37, first + 38, compare, projection);
            iter_swap_if(first + 39, first + 40, compare, projection);
            iter_swap_if(first + 53, first + 54, compare, projection);
            iter_swap_if(first + 55, first + 56, compare, projection);
            iter_swap_if(first + 2, first + 17, compare, projection);
            iter_swap_if(first + 3, first + 38, compare, projection);
            iter_swap_if(first + 4, first + 19, compare, projection);
            iter_swap_if(first + 5, first + 52, compare, projection);
            iter_swap_if(first + 6, first + 53, compare, projection);
            iter_swap_if(first + 7, first + 42, compare, projection);
            iter_swap_if(first + 8, first + 39, compare, projection);
            iter_swap_if(first + 9, first + 40, compare, projection);
            iter_swap_if(first + 10, first + 25, compare, projection);
            iter_swap_if(first + 11, first + 46, compare, projection);
            iter_swap_if(first + 12, first + 27, compare, projection);
            iter_swap_if(first + 18, first + 34, compare, projection);
            iter_swap_if(first + 21, first + 37, compare, projection);
            iter_swap_if(first + 22, first + 54, compare, projection);
            iter_swap_if(first + 23, first + 55, compare, projection);
            iter_swap_if(first + 24, first + 56, compare, projection);
            iter_swap_if(first + 30, first + 62, compare, projection);
            iter_swap_if(first + 33, first + 49, compare, projection);
            iter_swap_if(first + 35, first + 51, compare, projection);
            iter_swap_if(first + 41, first + 57, compare, projection);
            iter_swap_if(first + 43, first + 59, compare, projection);
            iter_swap_if(first + 3, first + 22, compare, projection);
            iter_swap_if(first + 4, first + 15, compare, projection);
            iter_swap_if(first + 5, first + 20, compare, projection);
            iter_swap_if(first + 6, first + 21, compare, projection);
            iter_swap_if(first + 7, first + 50, compare, projection);
            iter_swap_if(first + 8, first + 23, compare, projection);
            iter_swap_if(first + 9, first + 24, compare, projection);
            iter_swap_if(first + 12, first + 19, compare, projection);
            iter_swap_if(first + 17, first + 33, compare, projection);
            iter_swap_if(first + 25, first + 41, compare, projection);
            iter_swap_if(first + 26, first + 34, compare, projection);
            iter_swap_if(first + 27, first + 43, compare, projection);
            iter_swap_if(first + 30, first + 46, compare, projection);
            iter_swap_if(first + 36, first + 52, compare, projection);
            iter_swap_if(first + 37, first + 53, compare, projection);
            iter_swap_if(first + 38, first + 54, compare, projection);
            iter_swap_if(first + 39, first + 55, compare, projection);
            iter_swap_if(first + 40, first + 56, compare, projection);
            iter_swap_if(first + 42, first + 58, compare, projection);
            iter_swap_if(first + 45, first + 57, compare, projection);
            iter_swap_if(first + 61, first + 62, compare, projection);
            iter_swap_if(first + 1, first + 4, compare, projection);
            iter_swap_if(first + 3, first + 18, compare, projection);
            iter_swap_if(first + 5, first + 16, compare, projection);
            iter_swap_if(first + 6, first + 17, compare, projection);
            iter_swap_if(first + 7, first + 26, compare, projection);
            iter_swap_if(first + 8, first + 31, compare, projection);
            iter_swap_if(first + 9, first + 32, compare, projection);
            iter_swap_if(first + 10, first + 33, compare, projection);
            iter_swap_if(first + 11, first + 38, compare, projection);
            iter_swap_if(first + 13, first + 20, compare, projection);
            iter_swap_if(first + 14, first + 21, compare, projection);
            iter_swap_if(first + 19, first + 35, compare, projection);
            iter_swap_if(first + 23, first + 39, compare, projection);
            iter_swap_if(first + 24, first + 40, compare, projection);
            iter_swap_if(first + 25, first + 49, compare, projection);
            iter_swap_if(first + 27, first + 51, compare, projection);
            iter_swap_if(first + 28, first + 52, compare, projection);
            iter_swap_if(first + 29, first + 53, compare, projection);
            iter_swap_if(first + 30, first + 54, compare, projection);
            iter_swap_if(first + 42, first + 50, compare, projection);
            iter_swap_if(first + 43, first + 55, compare, projection);
            iter_swap_if(first + 44, first + 56, compare, projection);
            iter_swap_if(first + 46, first + 58, compare, projection);
            iter_swap_if(first + 7, first + 18, compare, projection);
            iter_swap_if(first + 8, first + 15, compare, projection);
            iter_swap_if(first + 9, first + 16, compare, projection);
            iter_swap_if(first + 10, first + 17, compare, projection);
            iter_swap_if(first + 14, first + 25, compare, projection);
            iter_swap_if(first + 19, first + 31, compare, projection);
            iter_swap_if(first + 20, first + 36, compare, projection);
            iter_swap_if(first + 21, first + 37, compare, projection);
            iter_swap_if(first + 22, first + 38, compare, projection);
            iter_swap_if(first + 23, first + 47, compare, projection);
            iter_swap_if(first + 24, first + 48, compare, projection);
            iter_swap_if(first + 29, first + 49, compare, projection);
            iter_swap_if(first + 34, first + 42, compare, projection);
            iter_swap_if(first + 35, first + 39, compare, projection);
            iter_swap_if(first + 43, first + 51, compare, projection);
            iter_swap_if(first + 44, first + 52, compare, projection);
            iter_swap_if(first + 45, first + 53, compare, projection);
            iter_swap_if(first + 46, first + 54, compare, projection);
            iter_swap_if(first + 58, first + 60, compare, projection);
            iter_swap_if(first + 2, first + 8, compare, projection);
            iter_swap_if(first + 3, first + 9, compare, projection);
            iter_swap_if(first + 11, first + 22, compare, projection);
            iter_swap_if(first + 12, first + 23, compare, projection);
            iter_swap_if(first + 13, first + 24, compare, projection);
            iter_swap_if(first + 20, first + 32, compare, projection);
            iter_swap_if(first + 21, first + 33, compare, projection);
            iter_swap_if(first + 26, first + 34, compare, projection);
            iter_swap_if(first + 27, first + 47, compare, projection);
            iter_swap_if(first + 28, first + 48, compare, projection);
            iter_swap_if(first + 30, first + 38, compare, projection);
            iter_swap_if(first + 36, first + 40, compare, projection);
            iter_swap_if(first + 37, first + 41, compare, projection);
            iter_swap_if(first + 42, first + 50, compare, projection);
            iter_swap_if(first + 53, first + 59, compare, projection);
            iter_swap_if(first + 54, first + 56, compare, projection);
            iter_swap_if(first + 60, first + 61, compare, projection);
            iter_swap_if(first + 2, first + 4, compare, projection);
            iter_swap_if(first + 3, first + 8, compare, projection);
            iter_swap_if(first + 11, first + 26, compare, projection);
            iter_swap_if(first + 12, first + 19, compare, projection);
            iter_swap_if(first + 13, first + 20, compare, projection);
            iter_swap_if(first + 14, first + 21, compare, projection);
            iter_swap_if(first + 22, first + 34, compare, projection);
            iter_swap_if(first + 23, first + 31, compare, projection);
            iter_swap_if(first + 24, first + 32, compare, projection);
            iter_swap_if(first + 25, first + 33, compare, projection);
            iter_swap_if(first + 27, first + 35, compare, projection);
            iter_swap_if(first + 28, first + 36, compare, projection);
            iter_swap_if(first + 29, first + 37, compare, projection);
            iter_swap_if(first + 30, first + 42, compare, projection);
            iter_swap_if(first + 38, first + 50, compare, projection);
            iter_swap_if(first + 39, first + 47, compare, projection);
            iter_swap_if(first + 40, first + 48, compare, projection);
            iter_swap_if(first + 41, first + 49, compare, projection);
            iter_swap_if(first + 56, first + 57, compare, projection);
            iter_swap_if(first + 58, first + 59, compare, projection);
            iter_swap_if(first + 11, first + 18, compare, projection);
            iter_swap_if(first + 12, first + 15, compare, projection);
            iter_swap_if(first + 13, first + 16, compare, projection);
            iter_swap_if(first + 14, first + 17, compare, projection);
            iter_swap_if(first + 19, first + 23, compare, projection);
            iter_swap_if(first + 20, first + 24, compare, projection);
            iter_swap_if(first + 21, first + 25, compare, projection);
            iter_swap_if(first + 22, first + 26, compare, projection);
            iter_swap_if(first + 27, first + 31, compare, projection);
            iter_swap_if(first + 28, first + 32, compare, projection);
            iter_swap_if(first + 29, first + 33, compare, projection);
            iter_swap_if(first + 30, first + 34, compare, projection);
            iter_swap_if(first + 35, first + 39, compare, projection);
            iter_swap_if(first + 36, first + 40, compare, projection);
            iter_swap_if(first + 37, first + 41, compare, projection);
            iter_swap_if(first + 38, first + 42, compare, projection);
            iter_swap_if(first + 43, first + 47, compare, projection);
            iter_swap_if(first + 44, first + 48, compare, projection);
            iter_swap_if(first + 45, first + 49, compare, projection);
            iter_swap_if(first + 46, first + 50, compare, projection);
            iter_swap_if(first + 56, first + 58, compare, projection);
            iter_swap_if(first + 57, first + 59, compare, projection);
            iter_swap_if(first + 6, first + 12, compare, projection);
            iter_swap_if(first + 7, first + 13, compare, projection);
            iter_swap_if(first + 10, first + 15, compare, projection);
            iter_swap_if(first + 11, first + 16, compare, projection);
            iter_swap_if(first + 14, first + 19, compare, projection);
            iter_swap_if(first + 17, first + 23, compare, projection);
            iter_swap_if(first + 18, first + 20, compare, projection);
            iter_swap_if(first + 21, first + 27, compare, projection);
            iter_swap_if(first + 22, first + 24, compare, projection);
            iter_swap_if(first + 25, first + 31, compare, projection);
            iter_swap_if(first + 26, first + 28, compare, projection);
            iter_swap_if(first + 29, first + 35, compare, projection);
            iter_swap_if(first + 30, first + 32, compare, projection);
            iter_swap_if(first + 33, first + 39, compare, projection);
            iter_swap_if(first + 34, first + 36, compare, projection);
            iter_swap_if(first + 37, first + 43, compare, projection);
            iter_swap_if(first + 38, first + 40, compare, projection);
            iter_swap_if(first + 41, first + 47, compare, projection);
            iter_swap_if(first + 42, first + 44, compare, projection);
            iter_swap_if(first + 45, first + 51, compare, projection);
            iter_swap_if(first + 46, first + 48, compare, projection);
            iter_swap_if(first + 49, first + 55, compare, projection);
            iter_swap_if(first + 50, first + 52, compare, projection);
            iter_swap_if(first + 57, first + 58, compare, projection);
            iter_swap_if(first + 59, first + 60, compare, projection);
            iter_swap_if(first + 5, first + 6, compare, projection);
            iter_swap_if(first + 7, first + 10, compare, projection);
            iter_swap_if(first + 9, first + 12, compare, projection);
            iter_swap_if(first + 11, first + 15, compare, projection);
            iter_swap_if(first + 13, first + 14, compare, projection);
            iter_swap_if(first + 16, first + 19, compare, projection);
            iter_swap_if(first + 17, first + 18, compare, projection);
            iter_swap_if(first + 20, first + 23, compare, projection);
            iter_swap_if(first + 21, first + 22, compare, projection);
            iter_swap_if(first + 24, first + 27, compare, projection);
            iter_swap_if(first + 25, first + 26, compare, projection);
            iter_swap_if(first + 28, first + 31, compare, projection);
            iter_swap_if(first + 29, first + 30, compare, projection);
            iter_swap_if(first + 32, first + 35, compare, projection);
            iter_swap_if(first + 33, first + 34, compare, projection);
            iter_swap_if(first + 36, first + 37, compare, projection);
            iter_swap_if(first + 38, first + 39, compare, projection);
            iter_swap_if(first + 40, first + 41, compare, projection);
            iter_swap_if(first + 42, first + 43, compare, projection);
            iter_swap_if(first + 44, first + 47, compare, projection);
            iter_swap_if(first + 45, first + 46, compare, projection);
            iter_swap_if(first + 48, first + 50, compare, projection);
            iter_swap_if(first + 49, first + 51, compare, projection);
            iter_swap_if(first + 52, first + 53, compare, projection);
            iter_swap_if(first + 54, first + 55, compare, projection);
            iter_swap_if(first + 3, first + 5, compare, projection);
            iter_swap_if(first + 6, first + 8, compare, projection);
            iter_swap_if(first + 7, first + 9, compare, projection);
            iter_swap_if(first + 10, first + 12, compare, projection);
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
            iter_swap_if(first + 40, first + 42, compare, projection);
            iter_swap_if(first + 41, first + 43, compare, projection);
            iter_swap_if(first + 44, first + 45, compare, projection);
            iter_swap_if(first + 46, first + 47, compare, projection);
            iter_swap_if(first + 48, first + 49, compare, projection);
            iter_swap_if(first + 50, first + 51, compare, projection);
            iter_swap_if(first + 52, first + 54, compare, projection);
            iter_swap_if(first + 53, first + 55, compare, projection);
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
        static constexpr auto index_pairs() noexcept
            -> std::array<utility::index_pair<DifferenceType>, 515>
        {
            return {{
                {1, 2}, {3, 10}, {4, 14}, {5, 8}, {6, 13}, {7, 12}, {9, 11}, {15, 28}, {16, 27}, {17, 30}, {18, 29}, {19, 23}, {20, 21}, {22, 26}, {24, 25}, {31, 44}, {32, 43}, {33, 46}, {34, 45}, {35, 39}, {36, 37}, {38, 42}, {40, 41}, {47, 60}, {48, 59}, {49, 62}, {50, 61}, {51, 55}, {52, 53}, {54, 58}, {56, 57},
                {0, 14}, {1, 5}, {2, 8}, {3, 7}, {6, 9}, {10, 12}, {11, 13}, {15, 20}, {16, 22}, {17, 24}, {18, 19}, {21, 28}, {23, 29}, {25, 30}, {26, 27}, {31, 36}, {32, 38}, {33, 40}, {34, 35}, {37, 44}, {39, 45}, {41, 46}, {42, 43}, {47, 52}, {48, 54}, {49, 56}, {50, 51}, {53, 60}, {55, 61}, {57, 62}, {58, 59},
                {0, 7}, {1, 6}, {2, 9}, {4, 10}, {5, 11}, {8, 13}, {12, 14}, {15, 16}, {17, 18}, {19, 20}, {21, 23}, {22, 24}, {25, 26}, {27, 28}, {29, 30}, {31, 32}, {33, 34}, {35, 36}, {37, 39}, {38, 40}, {41, 42}, {43, 44}, {45, 46}, {47, 48}, {49, 50}, {51, 52}, {53, 55}, {54, 56}, {57, 58}, {59, 60}, {61, 62},
                {0, 6}, {2, 4}, {3, 5}, {7, 11}, {8, 10}, {9, 12}, {13, 14}, {15, 17}, {16, 18}, {19, 25}, {20, 26}, {21, 22}, {23, 24}, {27, 29}, {28, 30}, {31, 33}, {32, 34}, {35, 41}, {36, 42}, {37, 38}, {39, 40}, {43, 45}, {44, 46}, {47, 49}, {48, 50}, {51, 57}, {52, 58}, {53, 54}, {55, 56}, {59, 61}, {60, 62},
                {0, 3}, {1, 2}, {4, 7}, {5, 9}, {6, 8}, {10, 11}, {12, 13}, {15, 47}, {16, 17}, {18, 27}, {19, 21}, {20, 22}, {23, 25}, {24, 26}, {28, 29}, {32, 33}, {34, 43}, {35, 37}, {36, 38}, {39, 41}, {40, 42}, {44, 45}, {46, 62}, {48, 49}, {50, 59}, {51, 53}, {52, 54}, {55, 57}, {56, 58}, {60, 61},
                {0, 1}, {2, 3}, {4, 6}, {7, 9}, {10, 12}, {11, 13}, {16, 19}, {17, 21}, {20, 23}, {22, 25}, {24, 28}, {26, 29}, {32, 35}, {33, 37}, {36, 39}, {38, 41}, {40, 44}, {42, 45}, {48, 51}, {49, 53}, {52, 55}, {54, 57}, {56, 60}, {58, 61},
                {0, 31}, {1, 2}, {3, 5}, {8, 10}, {11, 12}, {14, 61}, {16, 48}, {17, 19}, {18, 21}, {24, 27}, {26, 28}, {29, 45}, {33, 35}, {34, 37}, {40, 43}, {42, 44}, {49, 51}, {50, 53}, {56, 59}, {58, 60},
                {0, 15}, {1, 32}, {3, 4}, {5, 6}, {7, 8}, {9, 10}, {13, 60}, {14, 29}, {17, 49}, {18, 20}, {21, 23}, {22, 24}, {25, 27}, {28, 44}, {31, 47}, {34, 36}, {37, 39}, {38, 40}, {41, 43}, {45, 61}, {50, 52}, {53, 55}, {54, 56}, {57, 59},
                {1, 16}, {2, 3}, {4, 5}, {6, 7}, {8, 9}, {10, 11}, {13, 28}, {15, 31}, {18, 19}, {20, 21}, {22, 23}, {24, 25}, {26, 27}, {29, 45}, {32, 48}, {34, 35}, {36, 37}, {38, 39}, {40, 41}, {42, 43}, {44, 60}, {50, 51}, {52, 53}, {54, 55}, {56, 57}, {58, 59},
                {2, 33}, {4, 51}, {5, 6}, {7, 8}, {10, 41}, {11, 30}, {12, 59}, {16, 32}, {18, 50}, {19, 35}, {20, 36}, {21, 22}, {23, 24}, {25, 57}, {26, 58}, {27, 43}, {28, 44}, {37, 38}, {39, 40}, {53, 54}, {55, 56},
                {2, 17}, {3, 38}, {4, 19}, {5, 52}, {6, 53}, {7, 42}, {8, 39}, {9, 40}, {10, 25}, {11, 46}, {12, 27}, {18, 34}, {21, 37}, {22, 54}, {23, 55}, {24, 56}, {30, 62}, {33, 49}, {35, 51}, {41, 57}, {43, 59},
                {3, 22}, {4, 15}, {5, 20}, {6, 21}, {7, 50}, {8, 23}, {9, 24}, {12, 19}, {17, 33}, {25, 41}, {26, 34}, {27, 43}, {30, 46}, {36, 52}, {37, 53}, {38, 54}, {39, 55}, {40, 56}, {42, 58}, {45, 57}, {61, 62},
                {1, 4}, {3, 18}, {5, 16}, {6, 17}, {7, 26}, {8, 31}, {9, 32}, {10, 33}, {11, 38}, {13, 20}, {14, 21}, {19, 35}, {23, 39}, {24, 40}, {25, 49}, {27, 51}, {28, 52}, {29, 53}, {30, 54}, {42, 50}, {43, 55}, {44, 56}, {46, 58},
                {7, 18}, {8, 15}, {9, 16}, {10, 17}, {14, 25}, {19, 31}, {20, 36}, {21, 37}, {22, 38}, {23, 47}, {24, 48}, {29, 49}, {34, 42}, {35, 39}, {43, 51}, {44, 52}, {45, 53}, {46, 54}, {58, 60},
                {2, 8}, {3, 9}, {11, 22}, {12, 23}, {13, 24}, {20, 32}, {21, 33}, {26, 34}, {27, 47}, {28, 48}, {30, 38}, {36, 40}, {37, 41}, {42, 50}, {53, 59}, {54, 56}, {60, 61},
                {2, 4}, {3, 8}, {11, 26}, {12, 19}, {13, 20}, {14, 21}, {22, 34}, {23, 31}, {24, 32}, {25, 33}, {27, 35}, {28, 36}, {29, 37}, {30, 42}, {38, 50}, {39, 47}, {40, 48}, {41, 49}, {56, 57}, {58, 59},
                {11, 18}, {12, 15}, {13, 16}, {14, 17}, {19, 23}, {20, 24}, {21, 25}, {22, 26}, {27, 31}, {28, 32}, {29, 33}, {30, 34}, {35, 39}, {36, 40}, {37, 41}, {38, 42}, {43, 47}, {44, 48}, {45, 49}, {46, 50}, {56, 58}, {57, 59},
                {6, 12}, {7, 13}, {10, 15}, {11, 16}, {14, 19}, {17, 23}, {18, 20}, {21, 27}, {22, 24}, {25, 31}, {26, 28}, {29, 35}, {30, 32}, {33, 39}, {34, 36}, {37, 43}, {38, 40}, {41, 47}, {42, 44}, {45, 51}, {46, 48}, {49, 55}, {50, 52}, {57, 58}, {59, 60},
                {5, 6}, {7, 10}, {9, 12}, {11, 15}, {13, 14}, {16, 19}, {17, 18}, {20, 23}, {21, 22}, {24, 27}, {25, 26}, {28, 31}, {29, 30}, {32, 35}, {33, 34}, {36, 37}, {38, 39}, {40, 41}, {42, 43}, {44, 47}, {45, 46}, {48, 50}, {49, 51}, {52, 53}, {54, 55},
                {3, 5}, {6, 8}, {7, 9}, {10, 12}, {11, 13}, {14, 15}, {16, 17}, {18, 19}, {20, 21}, {22, 23}, {24, 25}, {26, 27}, {28, 29}, {30, 31}, {32, 33}, {34, 35}, {36, 38}, {37, 39}, {40, 42}, {41, 43}, {44, 45}, {46, 47}, {48, 49}, {50, 51}, {52, 54}, {53, 55},
                {3, 4}, {5, 6}, {7, 8}, {9, 10}, {11, 12}, {13, 14}, {15, 16}, {17, 18}, {19, 20}, {21, 22}, {23, 24}, {25, 26}, {27, 28}, {29, 30}, {31, 32}, {33, 34}, {35, 36}, {37, 38}, {39, 40}, {41, 42}, {43, 44}, {45, 46}, {47, 48}, {49, 50}, {51, 52}, {53, 54}, {55, 56},
            }};
        }
    };
}}

#endif // CPPSORT_DETAIL_SORTING_NETWORK_SORT63_H_
