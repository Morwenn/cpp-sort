/*
* Copyright (c) 2023 Morwenn
* SPDX-License-Identifier: MIT
*/
#ifndef CPPSORT_DETAIL_SORTING_NETWORK_SORT56_H_
#define CPPSORT_DETAIL_SORTING_NETWORK_SORT56_H_

namespace cppsort
{
namespace detail
{
    template<>
    struct sorting_network_sorter_impl<56>
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
            iter_swap_if(first + 16, first + 37, compare, projection);
            iter_swap_if(first + 17, first + 36, compare, projection);
            iter_swap_if(first + 18, first + 39, compare, projection);
            iter_swap_if(first + 19, first + 38, compare, projection);
            iter_swap_if(first + 20, first + 32, compare, projection);
            iter_swap_if(first + 21, first + 22, compare, projection);
            iter_swap_if(first + 23, first + 35, compare, projection);
            iter_swap_if(first + 24, first + 26, compare, projection);
            iter_swap_if(first + 25, first + 27, compare, projection);
            iter_swap_if(first + 28, first + 30, compare, projection);
            iter_swap_if(first + 29, first + 31, compare, projection);
            iter_swap_if(first + 33, first + 34, compare, projection);
            iter_swap_if(first + 40, first + 53, compare, projection);
            iter_swap_if(first + 41, first + 52, compare, projection);
            iter_swap_if(first + 42, first + 55, compare, projection);
            iter_swap_if(first + 43, first + 54, compare, projection);
            iter_swap_if(first + 44, first + 48, compare, projection);
            iter_swap_if(first + 45, first + 46, compare, projection);
            iter_swap_if(first + 47, first + 51, compare, projection);
            iter_swap_if(first + 49, first + 50, compare, projection);
            iter_swap_if(first, first + 5, compare, projection);
            iter_swap_if(first + 1, first + 7, compare, projection);
            iter_swap_if(first + 2, first + 9, compare, projection);
            iter_swap_if(first + 3, first + 4, compare, projection);
            iter_swap_if(first + 6, first + 13, compare, projection);
            iter_swap_if(first + 8, first + 14, compare, projection);
            iter_swap_if(first + 10, first + 15, compare, projection);
            iter_swap_if(first + 11, first + 12, compare, projection);
            iter_swap_if(first + 16, first + 21, compare, projection);
            iter_swap_if(first + 17, first + 23, compare, projection);
            iter_swap_if(first + 18, first + 33, compare, projection);
            iter_swap_if(first + 19, first + 20, compare, projection);
            iter_swap_if(first + 22, first + 37, compare, projection);
            iter_swap_if(first + 24, first + 28, compare, projection);
            iter_swap_if(first + 25, first + 29, compare, projection);
            iter_swap_if(first + 26, first + 30, compare, projection);
            iter_swap_if(first + 27, first + 31, compare, projection);
            iter_swap_if(first + 32, first + 38, compare, projection);
            iter_swap_if(first + 34, first + 39, compare, projection);
            iter_swap_if(first + 35, first + 36, compare, projection);
            iter_swap_if(first + 40, first + 45, compare, projection);
            iter_swap_if(first + 41, first + 47, compare, projection);
            iter_swap_if(first + 42, first + 49, compare, projection);
            iter_swap_if(first + 43, first + 44, compare, projection);
            iter_swap_if(first + 46, first + 53, compare, projection);
            iter_swap_if(first + 48, first + 54, compare, projection);
            iter_swap_if(first + 50, first + 55, compare, projection);
            iter_swap_if(first + 51, first + 52, compare, projection);
            iter_swap_if(first, first + 1, compare, projection);
            iter_swap_if(first + 2, first + 3, compare, projection);
            iter_swap_if(first + 4, first + 5, compare, projection);
            iter_swap_if(first + 6, first + 8, compare, projection);
            iter_swap_if(first + 7, first + 9, compare, projection);
            iter_swap_if(first + 10, first + 11, compare, projection);
            iter_swap_if(first + 12, first + 13, compare, projection);
            iter_swap_if(first + 14, first + 15, compare, projection);
            iter_swap_if(first + 16, first + 17, compare, projection);
            iter_swap_if(first + 18, first + 19, compare, projection);
            iter_swap_if(first + 20, first + 21, compare, projection);
            iter_swap_if(first + 22, first + 32, compare, projection);
            iter_swap_if(first + 23, first + 33, compare, projection);
            iter_swap_if(first + 24, first + 25, compare, projection);
            iter_swap_if(first + 26, first + 27, compare, projection);
            iter_swap_if(first + 28, first + 29, compare, projection);
            iter_swap_if(first + 30, first + 31, compare, projection);
            iter_swap_if(first + 34, first + 35, compare, projection);
            iter_swap_if(first + 36, first + 37, compare, projection);
            iter_swap_if(first + 38, first + 39, compare, projection);
            iter_swap_if(first + 40, first + 41, compare, projection);
            iter_swap_if(first + 42, first + 43, compare, projection);
            iter_swap_if(first + 44, first + 45, compare, projection);
            iter_swap_if(first + 46, first + 48, compare, projection);
            iter_swap_if(first + 47, first + 49, compare, projection);
            iter_swap_if(first + 50, first + 51, compare, projection);
            iter_swap_if(first + 52, first + 53, compare, projection);
            iter_swap_if(first + 54, first + 55, compare, projection);
            iter_swap_if(first, first + 2, compare, projection);
            iter_swap_if(first + 1, first + 3, compare, projection);
            iter_swap_if(first + 4, first + 10, compare, projection);
            iter_swap_if(first + 5, first + 11, compare, projection);
            iter_swap_if(first + 6, first + 7, compare, projection);
            iter_swap_if(first + 8, first + 9, compare, projection);
            iter_swap_if(first + 12, first + 14, compare, projection);
            iter_swap_if(first + 13, first + 15, compare, projection);
            iter_swap_if(first + 16, first + 18, compare, projection);
            iter_swap_if(first + 17, first + 19, compare, projection);
            iter_swap_if(first + 20, first + 34, compare, projection);
            iter_swap_if(first + 21, first + 35, compare, projection);
            iter_swap_if(first + 22, first + 23, compare, projection);
            iter_swap_if(first + 26, first + 28, compare, projection);
            iter_swap_if(first + 27, first + 29, compare, projection);
            iter_swap_if(first + 32, first + 33, compare, projection);
            iter_swap_if(first + 36, first + 38, compare, projection);
            iter_swap_if(first + 37, first + 39, compare, projection);
            iter_swap_if(first + 40, first + 42, compare, projection);
            iter_swap_if(first + 41, first + 43, compare, projection);
            iter_swap_if(first + 44, first + 50, compare, projection);
            iter_swap_if(first + 45, first + 51, compare, projection);
            iter_swap_if(first + 46, first + 47, compare, projection);
            iter_swap_if(first + 48, first + 49, compare, projection);
            iter_swap_if(first + 52, first + 54, compare, projection);
            iter_swap_if(first + 53, first + 55, compare, projection);
            iter_swap_if(first, first + 24, compare, projection);
            iter_swap_if(first + 1, first + 2, compare, projection);
            iter_swap_if(first + 3, first + 12, compare, projection);
            iter_swap_if(first + 4, first + 6, compare, projection);
            iter_swap_if(first + 5, first + 7, compare, projection);
            iter_swap_if(first + 8, first + 10, compare, projection);
            iter_swap_if(first + 9, first + 11, compare, projection);
            iter_swap_if(first + 13, first + 14, compare, projection);
            iter_swap_if(first + 15, first + 39, compare, projection);
            iter_swap_if(first + 16, first + 40, compare, projection);
            iter_swap_if(first + 17, first + 18, compare, projection);
            iter_swap_if(first + 19, first + 36, compare, projection);
            iter_swap_if(first + 20, first + 22, compare, projection);
            iter_swap_if(first + 21, first + 23, compare, projection);
            iter_swap_if(first + 25, first + 28, compare, projection);
            iter_swap_if(first + 27, first + 30, compare, projection);
            iter_swap_if(first + 31, first + 55, compare, projection);
            iter_swap_if(first + 32, first + 34, compare, projection);
            iter_swap_if(first + 33, first + 35, compare, projection);
            iter_swap_if(first + 37, first + 38, compare, projection);
            iter_swap_if(first + 41, first + 42, compare, projection);
            iter_swap_if(first + 43, first + 52, compare, projection);
            iter_swap_if(first + 44, first + 46, compare, projection);
            iter_swap_if(first + 45, first + 47, compare, projection);
            iter_swap_if(first + 48, first + 50, compare, projection);
            iter_swap_if(first + 49, first + 51, compare, projection);
            iter_swap_if(first + 53, first + 54, compare, projection);
            iter_swap_if(first, first + 16, compare, projection);
            iter_swap_if(first + 1, first + 4, compare, projection);
            iter_swap_if(first + 2, first + 6, compare, projection);
            iter_swap_if(first + 5, first + 8, compare, projection);
            iter_swap_if(first + 7, first + 10, compare, projection);
            iter_swap_if(first + 9, first + 13, compare, projection);
            iter_swap_if(first + 11, first + 14, compare, projection);
            iter_swap_if(first + 15, first + 31, compare, projection);
            iter_swap_if(first + 17, first + 20, compare, projection);
            iter_swap_if(first + 18, first + 22, compare, projection);
            iter_swap_if(first + 21, first + 32, compare, projection);
            iter_swap_if(first + 23, first + 34, compare, projection);
            iter_swap_if(first + 24, first + 40, compare, projection);
            iter_swap_if(first + 25, first + 26, compare, projection);
            iter_swap_if(first + 27, first + 28, compare, projection);
            iter_swap_if(first + 29, first + 30, compare, projection);
            iter_swap_if(first + 33, first + 37, compare, projection);
            iter_swap_if(first + 35, first + 38, compare, projection);
            iter_swap_if(first + 39, first + 55, compare, projection);
            iter_swap_if(first + 41, first + 44, compare, projection);
            iter_swap_if(first + 42, first + 46, compare, projection);
            iter_swap_if(first + 45, first + 48, compare, projection);
            iter_swap_if(first + 47, first + 50, compare, projection);
            iter_swap_if(first + 49, first + 53, compare, projection);
            iter_swap_if(first + 51, first + 54, compare, projection);
            iter_swap_if(first + 1, first + 17, compare, projection);
            iter_swap_if(first + 2, first + 4, compare, projection);
            iter_swap_if(first + 3, first + 6, compare, projection);
            iter_swap_if(first + 9, first + 12, compare, projection);
            iter_swap_if(first + 11, first + 13, compare, projection);
            iter_swap_if(first + 14, first + 30, compare, projection);
            iter_swap_if(first + 16, first + 24, compare, projection);
            iter_swap_if(first + 18, first + 20, compare, projection);
            iter_swap_if(first + 19, first + 22, compare, projection);
            iter_swap_if(first + 25, first + 41, compare, projection);
            iter_swap_if(first + 31, first + 39, compare, projection);
            iter_swap_if(first + 33, first + 36, compare, projection);
            iter_swap_if(first + 35, first + 37, compare, projection);
            iter_swap_if(first + 38, first + 54, compare, projection);
            iter_swap_if(first + 42, first + 44, compare, projection);
            iter_swap_if(first + 43, first + 46, compare, projection);
            iter_swap_if(first + 49, first + 52, compare, projection);
            iter_swap_if(first + 51, first + 53, compare, projection);
            iter_swap_if(first + 1, first + 25, compare, projection);
            iter_swap_if(first + 2, first + 26, compare, projection);
            iter_swap_if(first + 3, first + 5, compare, projection);
            iter_swap_if(first + 6, first + 8, compare, projection);
            iter_swap_if(first + 7, first + 9, compare, projection);
            iter_swap_if(first + 10, first + 12, compare, projection);
            iter_swap_if(first + 13, first + 37, compare, projection);
            iter_swap_if(first + 14, first + 38, compare, projection);
            iter_swap_if(first + 17, first + 41, compare, projection);
            iter_swap_if(first + 18, first + 42, compare, projection);
            iter_swap_if(first + 19, first + 21, compare, projection);
            iter_swap_if(first + 22, first + 32, compare, projection);
            iter_swap_if(first + 23, first + 33, compare, projection);
            iter_swap_if(first + 29, first + 53, compare, projection);
            iter_swap_if(first + 30, first + 54, compare, projection);
            iter_swap_if(first + 34, first + 36, compare, projection);
            iter_swap_if(first + 43, first + 45, compare, projection);
            iter_swap_if(first + 46, first + 48, compare, projection);
            iter_swap_if(first + 47, first + 49, compare, projection);
            iter_swap_if(first + 50, first + 52, compare, projection);
            iter_swap_if(first + 2, first + 18, compare, projection);
            iter_swap_if(first + 3, first + 4, compare, projection);
            iter_swap_if(first + 5, first + 6, compare, projection);
            iter_swap_if(first + 7, first + 8, compare, projection);
            iter_swap_if(first + 9, first + 10, compare, projection);
            iter_swap_if(first + 11, first + 12, compare, projection);
            iter_swap_if(first + 13, first + 29, compare, projection);
            iter_swap_if(first + 17, first + 25, compare, projection);
            iter_swap_if(first + 19, first + 20, compare, projection);
            iter_swap_if(first + 21, first + 22, compare, projection);
            iter_swap_if(first + 23, first + 32, compare, projection);
            iter_swap_if(first + 26, first + 42, compare, projection);
            iter_swap_if(first + 30, first + 38, compare, projection);
            iter_swap_if(first + 33, first + 34, compare, projection);
            iter_swap_if(first + 35, first + 36, compare, projection);
            iter_swap_if(first + 37, first + 53, compare, projection);
            iter_swap_if(first + 43, first + 44, compare, projection);
            iter_swap_if(first + 45, first + 46, compare, projection);
            iter_swap_if(first + 47, first + 48, compare, projection);
            iter_swap_if(first + 49, first + 50, compare, projection);
            iter_swap_if(first + 51, first + 52, compare, projection);
            iter_swap_if(first + 2, first + 16, compare, projection);
            iter_swap_if(first + 3, first + 27, compare, projection);
            iter_swap_if(first + 4, first + 44, compare, projection);
            iter_swap_if(first + 5, first + 45, compare, projection);
            iter_swap_if(first + 6, first + 7, compare, projection);
            iter_swap_if(first + 8, first + 9, compare, projection);
            iter_swap_if(first + 10, first + 50, compare, projection);
            iter_swap_if(first + 11, first + 51, compare, projection);
            iter_swap_if(first + 12, first + 36, compare, projection);
            iter_swap_if(first + 13, first + 25, compare, projection);
            iter_swap_if(first + 18, first + 26, compare, projection);
            iter_swap_if(first + 19, first + 43, compare, projection);
            iter_swap_if(first + 22, first + 23, compare, projection);
            iter_swap_if(first + 28, first + 52, compare, projection);
            iter_swap_if(first + 29, first + 37, compare, projection);
            iter_swap_if(first + 30, first + 42, compare, projection);
            iter_swap_if(first + 32, first + 33, compare, projection);
            iter_swap_if(first + 39, first + 53, compare, projection);
            iter_swap_if(first + 46, first + 47, compare, projection);
            iter_swap_if(first + 48, first + 49, compare, projection);
            iter_swap_if(first + 3, first + 19, compare, projection);
            iter_swap_if(first + 4, first + 20, compare, projection);
            iter_swap_if(first + 5, first + 21, compare, projection);
            iter_swap_if(first + 6, first + 22, compare, projection);
            iter_swap_if(first + 7, first + 15, compare, projection);
            iter_swap_if(first + 8, first + 32, compare, projection);
            iter_swap_if(first + 9, first + 41, compare, projection);
            iter_swap_if(first + 12, first + 28, compare, projection);
            iter_swap_if(first + 14, first + 46, compare, projection);
            iter_swap_if(first + 23, first + 47, compare, projection);
            iter_swap_if(first + 27, first + 43, compare, projection);
            iter_swap_if(first + 33, first + 49, compare, projection);
            iter_swap_if(first + 34, first + 50, compare, projection);
            iter_swap_if(first + 35, first + 51, compare, projection);
            iter_swap_if(first + 36, first + 52, compare, projection);
            iter_swap_if(first + 40, first + 48, compare, projection);
            iter_swap_if(first + 5, first + 13, compare, projection);
            iter_swap_if(first + 6, first + 14, compare, projection);
            iter_swap_if(first + 7, first + 23, compare, projection);
            iter_swap_if(first + 8, first + 40, compare, projection);
            iter_swap_if(first + 9, first + 33, compare, projection);
            iter_swap_if(first + 10, first + 34, compare, projection);
            iter_swap_if(first + 11, first + 35, compare, projection);
            iter_swap_if(first + 12, first + 24, compare, projection);
            iter_swap_if(first + 15, first + 47, compare, projection);
            iter_swap_if(first + 19, first + 27, compare, projection);
            iter_swap_if(first + 20, first + 44, compare, projection);
            iter_swap_if(first + 21, first + 45, compare, projection);
            iter_swap_if(first + 22, first + 46, compare, projection);
            iter_swap_if(first + 28, first + 36, compare, projection);
            iter_swap_if(first + 31, first + 43, compare, projection);
            iter_swap_if(first + 32, first + 48, compare, projection);
            iter_swap_if(first + 41, first + 49, compare, projection);
            iter_swap_if(first + 42, first + 50, compare, projection);
            iter_swap_if(first + 4, first + 12, compare, projection);
            iter_swap_if(first + 6, first + 18, compare, projection);
            iter_swap_if(first + 7, first + 19, compare, projection);
            iter_swap_if(first + 8, first + 16, compare, projection);
            iter_swap_if(first + 9, first + 17, compare, projection);
            iter_swap_if(first + 10, first + 26, compare, projection);
            iter_swap_if(first + 11, first + 27, compare, projection);
            iter_swap_if(first + 15, first + 23, compare, projection);
            iter_swap_if(first + 20, first + 24, compare, projection);
            iter_swap_if(first + 21, first + 25, compare, projection);
            iter_swap_if(first + 28, first + 44, compare, projection);
            iter_swap_if(first + 29, first + 45, compare, projection);
            iter_swap_if(first + 30, first + 34, compare, projection);
            iter_swap_if(first + 31, first + 35, compare, projection);
            iter_swap_if(first + 32, first + 40, compare, projection);
            iter_swap_if(first + 36, first + 48, compare, projection);
            iter_swap_if(first + 37, first + 49, compare, projection);
            iter_swap_if(first + 38, first + 46, compare, projection);
            iter_swap_if(first + 39, first + 47, compare, projection);
            iter_swap_if(first + 43, first + 51, compare, projection);
            iter_swap_if(first + 1, first + 4, compare, projection);
            iter_swap_if(first + 3, first + 9, compare, projection);
            iter_swap_if(first + 10, first + 18, compare, projection);
            iter_swap_if(first + 11, first + 19, compare, projection);
            iter_swap_if(first + 12, first + 20, compare, projection);
            iter_swap_if(first + 14, first + 30, compare, projection);
            iter_swap_if(first + 17, first + 21, compare, projection);
            iter_swap_if(first + 22, first + 26, compare, projection);
            iter_swap_if(first + 24, first + 28, compare, projection);
            iter_swap_if(first + 25, first + 41, compare, projection);
            iter_swap_if(first + 27, first + 31, compare, projection);
            iter_swap_if(first + 29, first + 33, compare, projection);
            iter_swap_if(first + 34, first + 38, compare, projection);
            iter_swap_if(first + 35, first + 43, compare, projection);
            iter_swap_if(first + 36, first + 44, compare, projection);
            iter_swap_if(first + 37, first + 45, compare, projection);
            iter_swap_if(first + 46, first + 52, compare, projection);
            iter_swap_if(first + 51, first + 54, compare, projection);
            iter_swap_if(first + 1, first + 2, compare, projection);
            iter_swap_if(first + 4, first + 8, compare, projection);
            iter_swap_if(first + 5, first + 9, compare, projection);
            iter_swap_if(first + 13, first + 21, compare, projection);
            iter_swap_if(first + 14, first + 22, compare, projection);
            iter_swap_if(first + 15, first + 27, compare, projection);
            iter_swap_if(first + 16, first + 20, compare, projection);
            iter_swap_if(first + 23, first + 31, compare, projection);
            iter_swap_if(first + 24, first + 32, compare, projection);
            iter_swap_if(first + 25, first + 29, compare, projection);
            iter_swap_if(first + 26, first + 30, compare, projection);
            iter_swap_if(first + 28, first + 40, compare, projection);
            iter_swap_if(first + 33, first + 41, compare, projection);
            iter_swap_if(first + 34, first + 42, compare, projection);
            iter_swap_if(first + 35, first + 39, compare, projection);
            iter_swap_if(first + 46, first + 50, compare, projection);
            iter_swap_if(first + 47, first + 51, compare, projection);
            iter_swap_if(first + 53, first + 54, compare, projection);
            iter_swap_if(first + 3, first + 4, compare, projection);
            iter_swap_if(first + 12, first + 16, compare, projection);
            iter_swap_if(first + 13, first + 17, compare, projection);
            iter_swap_if(first + 14, first + 24, compare, projection);
            iter_swap_if(first + 15, first + 21, compare, projection);
            iter_swap_if(first + 18, first + 20, compare, projection);
            iter_swap_if(first + 19, first + 25, compare, projection);
            iter_swap_if(first + 22, first + 26, compare, projection);
            iter_swap_if(first + 23, first + 27, compare, projection);
            iter_swap_if(first + 28, first + 32, compare, projection);
            iter_swap_if(first + 29, first + 33, compare, projection);
            iter_swap_if(first + 30, first + 36, compare, projection);
            iter_swap_if(first + 31, first + 41, compare, projection);
            iter_swap_if(first + 34, first + 40, compare, projection);
            iter_swap_if(first + 35, first + 37, compare, projection);
            iter_swap_if(first + 38, first + 42, compare, projection);
            iter_swap_if(first + 39, first + 43, compare, projection);
            iter_swap_if(first + 51, first + 52, compare, projection);
            iter_swap_if(first + 4, first + 8, compare, projection);
            iter_swap_if(first + 6, first + 12, compare, projection);
            iter_swap_if(first + 7, first + 13, compare, projection);
            iter_swap_if(first + 10, first + 16, compare, projection);
            iter_swap_if(first + 11, first + 17, compare, projection);
            iter_swap_if(first + 14, first + 18, compare, projection);
            iter_swap_if(first + 15, first + 19, compare, projection);
            iter_swap_if(first + 20, first + 24, compare, projection);
            iter_swap_if(first + 21, first + 25, compare, projection);
            iter_swap_if(first + 22, first + 28, compare, projection);
            iter_swap_if(first + 23, first + 29, compare, projection);
            iter_swap_if(first + 26, first + 32, compare, projection);
            iter_swap_if(first + 27, first + 33, compare, projection);
            iter_swap_if(first + 30, first + 34, compare, projection);
            iter_swap_if(first + 31, first + 35, compare, projection);
            iter_swap_if(first + 36, first + 40, compare, projection);
            iter_swap_if(first + 37, first + 41, compare, projection);
            iter_swap_if(first + 38, first + 44, compare, projection);
            iter_swap_if(first + 39, first + 45, compare, projection);
            iter_swap_if(first + 42, first + 48, compare, projection);
            iter_swap_if(first + 43, first + 49, compare, projection);
            iter_swap_if(first + 47, first + 51, compare, projection);
            iter_swap_if(first + 5, first + 8, compare, projection);
            iter_swap_if(first + 7, first + 10, compare, projection);
            iter_swap_if(first + 9, first + 12, compare, projection);
            iter_swap_if(first + 11, first + 14, compare, projection);
            iter_swap_if(first + 13, first + 16, compare, projection);
            iter_swap_if(first + 15, first + 20, compare, projection);
            iter_swap_if(first + 17, first + 18, compare, projection);
            iter_swap_if(first + 19, first + 22, compare, projection);
            iter_swap_if(first + 21, first + 24, compare, projection);
            iter_swap_if(first + 23, first + 26, compare, projection);
            iter_swap_if(first + 25, first + 28, compare, projection);
            iter_swap_if(first + 27, first + 30, compare, projection);
            iter_swap_if(first + 29, first + 32, compare, projection);
            iter_swap_if(first + 31, first + 34, compare, projection);
            iter_swap_if(first + 33, first + 36, compare, projection);
            iter_swap_if(first + 35, first + 40, compare, projection);
            iter_swap_if(first + 37, first + 38, compare, projection);
            iter_swap_if(first + 39, first + 42, compare, projection);
            iter_swap_if(first + 41, first + 44, compare, projection);
            iter_swap_if(first + 43, first + 46, compare, projection);
            iter_swap_if(first + 45, first + 48, compare, projection);
            iter_swap_if(first + 47, first + 50, compare, projection);
            iter_swap_if(first + 6, first + 8, compare, projection);
            iter_swap_if(first + 7, first + 9, compare, projection);
            iter_swap_if(first + 10, first + 12, compare, projection);
            iter_swap_if(first + 11, first + 13, compare, projection);
            iter_swap_if(first + 14, first + 16, compare, projection);
            iter_swap_if(first + 15, first + 17, compare, projection);
            iter_swap_if(first + 18, first + 20, compare, projection);
            iter_swap_if(first + 19, first + 21, compare, projection);
            iter_swap_if(first + 22, first + 24, compare, projection);
            iter_swap_if(first + 23, first + 25, compare, projection);
            iter_swap_if(first + 26, first + 28, compare, projection);
            iter_swap_if(first + 27, first + 29, compare, projection);
            iter_swap_if(first + 30, first + 32, compare, projection);
            iter_swap_if(first + 31, first + 33, compare, projection);
            iter_swap_if(first + 34, first + 36, compare, projection);
            iter_swap_if(first + 35, first + 37, compare, projection);
            iter_swap_if(first + 38, first + 40, compare, projection);
            iter_swap_if(first + 39, first + 41, compare, projection);
            iter_swap_if(first + 42, first + 44, compare, projection);
            iter_swap_if(first + 43, first + 45, compare, projection);
            iter_swap_if(first + 46, first + 48, compare, projection);
            iter_swap_if(first + 47, first + 49, compare, projection);
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
        static constexpr auto index_pairs() noexcept
            -> std::array<utility::index_pair<DifferenceType>, 438>
        {
            return {{
                {0, 13}, {1, 12}, {2, 15}, {3, 14}, {4, 8}, {5, 6}, {7, 11}, {9, 10}, {16, 37}, {17, 36}, {18, 39}, {19, 38}, {20, 32}, {21, 22}, {23, 35}, {24, 26}, {25, 27}, {28, 30}, {29, 31}, {33, 34}, {40, 53}, {41, 52}, {42, 55}, {43, 54}, {44, 48}, {45, 46}, {47, 51}, {49, 50},
                {0, 5}, {1, 7}, {2, 9}, {3, 4}, {6, 13}, {8, 14}, {10, 15}, {11, 12}, {16, 21}, {17, 23}, {18, 33}, {19, 20}, {22, 37}, {24, 28}, {25, 29}, {26, 30}, {27, 31}, {32, 38}, {34, 39}, {35, 36}, {40, 45}, {41, 47}, {42, 49}, {43, 44}, {46, 53}, {48, 54}, {50, 55}, {51, 52},
                {0, 1}, {2, 3}, {4, 5}, {6, 8}, {7, 9}, {10, 11}, {12, 13}, {14, 15}, {16, 17}, {18, 19}, {20, 21}, {22, 32}, {23, 33}, {24, 25}, {26, 27}, {28, 29}, {30, 31}, {34, 35}, {36, 37}, {38, 39}, {40, 41}, {42, 43}, {44, 45}, {46, 48}, {47, 49}, {50, 51}, {52, 53}, {54, 55},
                {0, 2}, {1, 3}, {4, 10}, {5, 11}, {6, 7}, {8, 9}, {12, 14}, {13, 15}, {16, 18}, {17, 19}, {20, 34}, {21, 35}, {22, 23}, {26, 28}, {27, 29}, {32, 33}, {36, 38}, {37, 39}, {40, 42}, {41, 43}, {44, 50}, {45, 51}, {46, 47}, {48, 49}, {52, 54}, {53, 55},
                {0, 24}, {1, 2}, {3, 12}, {4, 6}, {5, 7}, {8, 10}, {9, 11}, {13, 14}, {15, 39}, {16, 40}, {17, 18}, {19, 36}, {20, 22}, {21, 23}, {25, 28}, {27, 30}, {31, 55}, {32, 34}, {33, 35}, {37, 38}, {41, 42}, {43, 52}, {44, 46}, {45, 47}, {48, 50}, {49, 51}, {53, 54},
                {0, 16}, {1, 4}, {2, 6}, {5, 8}, {7, 10}, {9, 13}, {11, 14}, {15, 31}, {17, 20}, {18, 22}, {21, 32}, {23, 34}, {24, 40}, {25, 26}, {27, 28}, {29, 30}, {33, 37}, {35, 38}, {39, 55}, {41, 44}, {42, 46}, {45, 48}, {47, 50}, {49, 53}, {51, 54},
                {1, 17}, {2, 4}, {3, 6}, {9, 12}, {11, 13}, {14, 30}, {16, 24}, {18, 20}, {19, 22}, {25, 41}, {31, 39}, {33, 36}, {35, 37}, {38, 54}, {42, 44}, {43, 46}, {49, 52}, {51, 53},
                {1, 25}, {2, 26}, {3, 5}, {6, 8}, {7, 9}, {10, 12}, {13, 37}, {14, 38}, {17, 41}, {18, 42}, {19, 21}, {22, 32}, {23, 33}, {29, 53}, {30, 54}, {34, 36}, {43, 45}, {46, 48}, {47, 49}, {50, 52},
                {2, 18}, {3, 4}, {5, 6}, {7, 8}, {9, 10}, {11, 12}, {13, 29}, {17, 25}, {19, 20}, {21, 22}, {23, 32}, {26, 42}, {30, 38}, {33, 34}, {35, 36}, {37, 53}, {43, 44}, {45, 46}, {47, 48}, {49, 50}, {51, 52},
                {2, 16}, {3, 27}, {4, 44}, {5, 45}, {6, 7}, {8, 9}, {10, 50}, {11, 51}, {12, 36}, {13, 25}, {18, 26}, {19, 43}, {22, 23}, {28, 52}, {29, 37}, {30, 42}, {32, 33}, {39, 53}, {46, 47}, {48, 49},
                {3, 19}, {4, 20}, {5, 21}, {6, 22}, {7, 15}, {8, 32}, {9, 41}, {12, 28}, {14, 46}, {23, 47}, {27, 43}, {33, 49}, {34, 50}, {35, 51}, {36, 52}, {40, 48},
                {5, 13}, {6, 14}, {7, 23}, {8, 40}, {9, 33}, {10, 34}, {11, 35}, {12, 24}, {15, 47}, {19, 27}, {20, 44}, {21, 45}, {22, 46}, {28, 36}, {31, 43}, {32, 48}, {41, 49}, {42, 50},
                {4, 12}, {6, 18}, {7, 19}, {8, 16}, {9, 17}, {10, 26}, {11, 27}, {15, 23}, {20, 24}, {21, 25}, {28, 44}, {29, 45}, {30, 34}, {31, 35}, {32, 40}, {36, 48}, {37, 49}, {38, 46}, {39, 47}, {43, 51},
                {1, 4}, {3, 9}, {10, 18}, {11, 19}, {12, 20}, {14, 30}, {17, 21}, {22, 26}, {24, 28}, {25, 41}, {27, 31}, {29, 33}, {34, 38}, {35, 43}, {36, 44}, {37, 45}, {46, 52}, {51, 54},
                {1, 2}, {4, 8}, {5, 9}, {13, 21}, {14, 22}, {15, 27}, {16, 20}, {23, 31}, {24, 32}, {25, 29}, {26, 30}, {28, 40}, {33, 41}, {34, 42}, {35, 39}, {46, 50}, {47, 51}, {53, 54},
                {3, 4}, {12, 16}, {13, 17}, {14, 24}, {15, 21}, {18, 20}, {19, 25}, {22, 26}, {23, 27}, {28, 32}, {29, 33}, {30, 36}, {31, 41}, {34, 40}, {35, 37}, {38, 42}, {39, 43}, {51, 52},
                {4, 8}, {6, 12}, {7, 13}, {10, 16}, {11, 17}, {14, 18}, {15, 19}, {20, 24}, {21, 25}, {22, 28}, {23, 29}, {26, 32}, {27, 33}, {30, 34}, {31, 35}, {36, 40}, {37, 41}, {38, 44}, {39, 45}, {42, 48}, {43, 49}, {47, 51},
                {5, 8}, {7, 10}, {9, 12}, {11, 14}, {13, 16}, {15, 20}, {17, 18}, {19, 22}, {21, 24}, {23, 26}, {25, 28}, {27, 30}, {29, 32}, {31, 34}, {33, 36}, {35, 40}, {37, 38}, {39, 42}, {41, 44}, {43, 46}, {45, 48}, {47, 50},
                {6, 8}, {7, 9}, {10, 12}, {11, 13}, {14, 16}, {15, 17}, {18, 20}, {19, 21}, {22, 24}, {23, 25}, {26, 28}, {27, 29}, {30, 32}, {31, 33}, {34, 36}, {35, 37}, {38, 40}, {39, 41}, {42, 44}, {43, 45}, {46, 48}, {47, 49},
                {5, 6}, {7, 8}, {9, 10}, {11, 12}, {13, 14}, {15, 16}, {17, 18}, {19, 20}, {21, 22}, {23, 24}, {25, 26}, {27, 28}, {29, 30}, {31, 32}, {33, 34}, {35, 36}, {37, 38}, {39, 40}, {41, 42}, {43, 44}, {45, 46}, {47, 48}, {49, 50},
            }};
        }
    };
}}

#endif // CPPSORT_DETAIL_SORTING_NETWORK_SORT56_H_
