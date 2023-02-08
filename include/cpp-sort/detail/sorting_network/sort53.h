/*
* Copyright (c) 2023 Morwenn
* SPDX-License-Identifier: MIT
*/
#ifndef CPPSORT_DETAIL_SORTING_NETWORK_SORT53_H_
#define CPPSORT_DETAIL_SORTING_NETWORK_SORT53_H_

namespace cppsort
{
namespace detail
{
    template<>
    struct sorting_network_sorter_impl<53>
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
            iter_swap_if(first, first + 12, compare, projection);
            iter_swap_if(first + 1, first + 10, compare, projection);
            iter_swap_if(first + 2, first + 9, compare, projection);
            iter_swap_if(first + 3, first + 7, compare, projection);
            iter_swap_if(first + 5, first + 11, compare, projection);
            iter_swap_if(first + 6, first + 8, compare, projection);
            iter_swap_if(first + 13, first + 25, compare, projection);
            iter_swap_if(first + 14, first + 23, compare, projection);
            iter_swap_if(first + 15, first + 22, compare, projection);
            iter_swap_if(first + 16, first + 20, compare, projection);
            iter_swap_if(first + 18, first + 24, compare, projection);
            iter_swap_if(first + 19, first + 21, compare, projection);
            iter_swap_if(first + 26, first + 38, compare, projection);
            iter_swap_if(first + 27, first + 36, compare, projection);
            iter_swap_if(first + 28, first + 35, compare, projection);
            iter_swap_if(first + 29, first + 33, compare, projection);
            iter_swap_if(first + 31, first + 37, compare, projection);
            iter_swap_if(first + 32, first + 34, compare, projection);
            iter_swap_if(first + 39, first + 45, compare, projection);
            iter_swap_if(first + 40, first + 50, compare, projection);
            iter_swap_if(first + 41, first + 51, compare, projection);
            iter_swap_if(first + 42, first + 49, compare, projection);
            iter_swap_if(first + 43, first + 44, compare, projection);
            iter_swap_if(first + 46, first + 52, compare, projection);
            iter_swap_if(first + 47, first + 48, compare, projection);
            iter_swap_if(first + 1, first + 6, compare, projection);
            iter_swap_if(first + 2, first + 3, compare, projection);
            iter_swap_if(first + 4, first + 11, compare, projection);
            iter_swap_if(first + 7, first + 9, compare, projection);
            iter_swap_if(first + 8, first + 10, compare, projection);
            iter_swap_if(first + 14, first + 19, compare, projection);
            iter_swap_if(first + 15, first + 16, compare, projection);
            iter_swap_if(first + 17, first + 24, compare, projection);
            iter_swap_if(first + 20, first + 22, compare, projection);
            iter_swap_if(first + 21, first + 23, compare, projection);
            iter_swap_if(first + 27, first + 32, compare, projection);
            iter_swap_if(first + 28, first + 29, compare, projection);
            iter_swap_if(first + 30, first + 37, compare, projection);
            iter_swap_if(first + 33, first + 35, compare, projection);
            iter_swap_if(first + 34, first + 36, compare, projection);
            iter_swap_if(first + 40, first + 41, compare, projection);
            iter_swap_if(first + 42, first + 46, compare, projection);
            iter_swap_if(first + 43, first + 47, compare, projection);
            iter_swap_if(first + 44, first + 48, compare, projection);
            iter_swap_if(first + 45, first + 49, compare, projection);
            iter_swap_if(first + 50, first + 51, compare, projection);
            iter_swap_if(first, first + 4, compare, projection);
            iter_swap_if(first + 1, first + 2, compare, projection);
            iter_swap_if(first + 3, first + 6, compare, projection);
            iter_swap_if(first + 7, first + 8, compare, projection);
            iter_swap_if(first + 9, first + 10, compare, projection);
            iter_swap_if(first + 11, first + 12, compare, projection);
            iter_swap_if(first + 13, first + 17, compare, projection);
            iter_swap_if(first + 14, first + 15, compare, projection);
            iter_swap_if(first + 16, first + 19, compare, projection);
            iter_swap_if(first + 20, first + 21, compare, projection);
            iter_swap_if(first + 22, first + 23, compare, projection);
            iter_swap_if(first + 24, first + 25, compare, projection);
            iter_swap_if(first + 26, first + 30, compare, projection);
            iter_swap_if(first + 27, first + 28, compare, projection);
            iter_swap_if(first + 29, first + 32, compare, projection);
            iter_swap_if(first + 33, first + 34, compare, projection);
            iter_swap_if(first + 35, first + 36, compare, projection);
            iter_swap_if(first + 37, first + 38, compare, projection);
            iter_swap_if(first + 39, first + 43, compare, projection);
            iter_swap_if(first + 40, first + 42, compare, projection);
            iter_swap_if(first + 44, first + 45, compare, projection);
            iter_swap_if(first + 46, first + 47, compare, projection);
            iter_swap_if(first + 48, first + 52, compare, projection);
            iter_swap_if(first + 49, first + 51, compare, projection);
            iter_swap_if(first + 4, first + 6, compare, projection);
            iter_swap_if(first + 5, first + 9, compare, projection);
            iter_swap_if(first + 8, first + 11, compare, projection);
            iter_swap_if(first + 10, first + 12, compare, projection);
            iter_swap_if(first + 17, first + 19, compare, projection);
            iter_swap_if(first + 18, first + 22, compare, projection);
            iter_swap_if(first + 21, first + 24, compare, projection);
            iter_swap_if(first + 23, first + 25, compare, projection);
            iter_swap_if(first + 30, first + 32, compare, projection);
            iter_swap_if(first + 31, first + 35, compare, projection);
            iter_swap_if(first + 34, first + 37, compare, projection);
            iter_swap_if(first + 36, first + 38, compare, projection);
            iter_swap_if(first + 39, first + 40, compare, projection);
            iter_swap_if(first + 41, first + 48, compare, projection);
            iter_swap_if(first + 42, first + 46, compare, projection);
            iter_swap_if(first + 43, first + 50, compare, projection);
            iter_swap_if(first + 45, first + 49, compare, projection);
            iter_swap_if(first + 51, first + 52, compare, projection);
            iter_swap_if(first, first + 5, compare, projection);
            iter_swap_if(first + 3, first + 8, compare, projection);
            iter_swap_if(first + 4, first + 7, compare, projection);
            iter_swap_if(first + 6, first + 11, compare, projection);
            iter_swap_if(first + 9, first + 10, compare, projection);
            iter_swap_if(first + 13, first + 18, compare, projection);
            iter_swap_if(first + 16, first + 21, compare, projection);
            iter_swap_if(first + 17, first + 20, compare, projection);
            iter_swap_if(first + 19, first + 24, compare, projection);
            iter_swap_if(first + 22, first + 23, compare, projection);
            iter_swap_if(first + 25, first + 38, compare, projection);
            iter_swap_if(first + 26, first + 31, compare, projection);
            iter_swap_if(first + 29, first + 34, compare, projection);
            iter_swap_if(first + 30, first + 33, compare, projection);
            iter_swap_if(first + 32, first + 37, compare, projection);
            iter_swap_if(first + 35, first + 36, compare, projection);
            iter_swap_if(first + 41, first + 44, compare, projection);
            iter_swap_if(first + 43, first + 46, compare, projection);
            iter_swap_if(first + 45, first + 48, compare, projection);
            iter_swap_if(first + 47, first + 50, compare, projection);
            iter_swap_if(first, first + 1, compare, projection);
            iter_swap_if(first + 2, first + 5, compare, projection);
            iter_swap_if(first + 6, first + 9, compare, projection);
            iter_swap_if(first + 7, first + 8, compare, projection);
            iter_swap_if(first + 10, first + 11, compare, projection);
            iter_swap_if(first + 13, first + 14, compare, projection);
            iter_swap_if(first + 15, first + 18, compare, projection);
            iter_swap_if(first + 19, first + 22, compare, projection);
            iter_swap_if(first + 20, first + 21, compare, projection);
            iter_swap_if(first + 23, first + 24, compare, projection);
            iter_swap_if(first + 26, first + 27, compare, projection);
            iter_swap_if(first + 28, first + 31, compare, projection);
            iter_swap_if(first + 32, first + 35, compare, projection);
            iter_swap_if(first + 33, first + 34, compare, projection);
            iter_swap_if(first + 36, first + 37, compare, projection);
            iter_swap_if(first + 40, first + 41, compare, projection);
            iter_swap_if(first + 42, first + 43, compare, projection);
            iter_swap_if(first + 45, first + 46, compare, projection);
            iter_swap_if(first + 48, first + 49, compare, projection);
            iter_swap_if(first + 50, first + 51, compare, projection);
            iter_swap_if(first, first + 26, compare, projection);
            iter_swap_if(first + 1, first + 3, compare, projection);
            iter_swap_if(first + 2, first + 4, compare, projection);
            iter_swap_if(first + 5, first + 6, compare, projection);
            iter_swap_if(first + 9, first + 10, compare, projection);
            iter_swap_if(first + 11, first + 24, compare, projection);
            iter_swap_if(first + 13, first + 39, compare, projection);
            iter_swap_if(first + 14, first + 16, compare, projection);
            iter_swap_if(first + 15, first + 17, compare, projection);
            iter_swap_if(first + 18, first + 19, compare, projection);
            iter_swap_if(first + 22, first + 23, compare, projection);
            iter_swap_if(first + 27, first + 29, compare, projection);
            iter_swap_if(first + 28, first + 30, compare, projection);
            iter_swap_if(first + 31, first + 32, compare, projection);
            iter_swap_if(first + 35, first + 36, compare, projection);
            iter_swap_if(first + 40, first + 42, compare, projection);
            iter_swap_if(first + 41, first + 43, compare, projection);
            iter_swap_if(first + 44, first + 45, compare, projection);
            iter_swap_if(first + 46, first + 47, compare, projection);
            iter_swap_if(first + 48, first + 50, compare, projection);
            iter_swap_if(first + 49, first + 51, compare, projection);
            iter_swap_if(first, first + 13, compare, projection);
            iter_swap_if(first + 1, first + 2, compare, projection);
            iter_swap_if(first + 3, first + 4, compare, projection);
            iter_swap_if(first + 5, first + 7, compare, projection);
            iter_swap_if(first + 6, first + 8, compare, projection);
            iter_swap_if(first + 10, first + 23, compare, projection);
            iter_swap_if(first + 12, first + 51, compare, projection);
            iter_swap_if(first + 14, first + 15, compare, projection);
            iter_swap_if(first + 16, first + 17, compare, projection);
            iter_swap_if(first + 18, first + 20, compare, projection);
            iter_swap_if(first + 19, first + 21, compare, projection);
            iter_swap_if(first + 26, first + 39, compare, projection);
            iter_swap_if(first + 27, first + 28, compare, projection);
            iter_swap_if(first + 29, first + 30, compare, projection);
            iter_swap_if(first + 31, first + 33, compare, projection);
            iter_swap_if(first + 32, first + 34, compare, projection);
            iter_swap_if(first + 41, first + 42, compare, projection);
            iter_swap_if(first + 43, first + 46, compare, projection);
            iter_swap_if(first + 45, first + 48, compare, projection);
            iter_swap_if(first + 49, first + 50, compare, projection);
            iter_swap_if(first + 1, first + 27, compare, projection);
            iter_swap_if(first + 2, first + 3, compare, projection);
            iter_swap_if(first + 4, first + 5, compare, projection);
            iter_swap_if(first + 6, first + 7, compare, projection);
            iter_swap_if(first + 8, first + 9, compare, projection);
            iter_swap_if(first + 12, first + 25, compare, projection);
            iter_swap_if(first + 13, first + 26, compare, projection);
            iter_swap_if(first + 14, first + 40, compare, projection);
            iter_swap_if(first + 15, first + 16, compare, projection);
            iter_swap_if(first + 17, first + 18, compare, projection);
            iter_swap_if(first + 19, first + 20, compare, projection);
            iter_swap_if(first + 21, first + 22, compare, projection);
            iter_swap_if(first + 28, first + 29, compare, projection);
            iter_swap_if(first + 30, first + 31, compare, projection);
            iter_swap_if(first + 32, first + 33, compare, projection);
            iter_swap_if(first + 34, first + 35, compare, projection);
            iter_swap_if(first + 37, first + 50, compare, projection);
            iter_swap_if(first + 38, first + 51, compare, projection);
            iter_swap_if(first + 43, first + 44, compare, projection);
            iter_swap_if(first + 45, first + 46, compare, projection);
            iter_swap_if(first + 47, first + 48, compare, projection);
            iter_swap_if(first + 1, first + 14, compare, projection);
            iter_swap_if(first + 2, first + 15, compare, projection);
            iter_swap_if(first + 3, first + 4, compare, projection);
            iter_swap_if(first + 5, first + 6, compare, projection);
            iter_swap_if(first + 7, first + 33, compare, projection);
            iter_swap_if(first + 8, first + 34, compare, projection);
            iter_swap_if(first + 9, first + 35, compare, projection);
            iter_swap_if(first + 11, first + 37, compare, projection);
            iter_swap_if(first + 16, first + 17, compare, projection);
            iter_swap_if(first + 18, first + 19, compare, projection);
            iter_swap_if(first + 22, first + 52, compare, projection);
            iter_swap_if(first + 24, first + 50, compare, projection);
            iter_swap_if(first + 25, first + 38, compare, projection);
            iter_swap_if(first + 27, first + 40, compare, projection);
            iter_swap_if(first + 28, first + 41, compare, projection);
            iter_swap_if(first + 29, first + 30, compare, projection);
            iter_swap_if(first + 31, first + 32, compare, projection);
            iter_swap_if(first + 42, first + 43, compare, projection);
            iter_swap_if(first + 44, first + 45, compare, projection);
            iter_swap_if(first + 46, first + 47, compare, projection);
            iter_swap_if(first + 48, first + 49, compare, projection);
            iter_swap_if(first + 2, first + 28, compare, projection);
            iter_swap_if(first + 3, first + 16, compare, projection);
            iter_swap_if(first + 4, first + 43, compare, projection);
            iter_swap_if(first + 5, first + 31, compare, projection);
            iter_swap_if(first + 6, first + 32, compare, projection);
            iter_swap_if(first + 9, first + 22, compare, projection);
            iter_swap_if(first + 14, first + 27, compare, projection);
            iter_swap_if(first + 15, first + 41, compare, projection);
            iter_swap_if(first + 17, first + 30, compare, projection);
            iter_swap_if(first + 19, first + 45, compare, projection);
            iter_swap_if(first + 20, first + 46, compare, projection);
            iter_swap_if(first + 21, first + 47, compare, projection);
            iter_swap_if(first + 24, first + 37, compare, projection);
            iter_swap_if(first + 29, first + 42, compare, projection);
            iter_swap_if(first + 35, first + 52, compare, projection);
            iter_swap_if(first + 36, first + 49, compare, projection);
            iter_swap_if(first + 3, first + 29, compare, projection);
            iter_swap_if(first + 4, first + 17, compare, projection);
            iter_swap_if(first + 5, first + 44, compare, projection);
            iter_swap_if(first + 6, first + 19, compare, projection);
            iter_swap_if(first + 7, first + 20, compare, projection);
            iter_swap_if(first + 8, first + 21, compare, projection);
            iter_swap_if(first + 10, first + 36, compare, projection);
            iter_swap_if(first + 15, first + 28, compare, projection);
            iter_swap_if(first + 16, first + 42, compare, projection);
            iter_swap_if(first + 18, first + 31, compare, projection);
            iter_swap_if(first + 22, first + 35, compare, projection);
            iter_swap_if(first + 23, first + 49, compare, projection);
            iter_swap_if(first + 30, first + 43, compare, projection);
            iter_swap_if(first + 32, first + 45, compare, projection);
            iter_swap_if(first + 33, first + 46, compare, projection);
            iter_swap_if(first + 34, first + 47, compare, projection);
            iter_swap_if(first + 4, first + 13, compare, projection);
            iter_swap_if(first + 6, first + 15, compare, projection);
            iter_swap_if(first + 8, first + 26, compare, projection);
            iter_swap_if(first + 10, first + 41, compare, projection);
            iter_swap_if(first + 11, first + 42, compare, projection);
            iter_swap_if(first + 12, first + 17, compare, projection);
            iter_swap_if(first + 16, first + 29, compare, projection);
            iter_swap_if(first + 19, first + 32, compare, projection);
            iter_swap_if(first + 20, first + 33, compare, projection);
            iter_swap_if(first + 21, first + 34, compare, projection);
            iter_swap_if(first + 23, first + 36, compare, projection);
            iter_swap_if(first + 25, first + 43, compare, projection);
            iter_swap_if(first + 31, first + 48, compare, projection);
            iter_swap_if(first + 37, first + 46, compare, projection);
            iter_swap_if(first + 38, first + 47, compare, projection);
            iter_swap_if(first + 40, first + 44, compare, projection);
            iter_swap_if(first + 1, first + 4, compare, projection);
            iter_swap_if(first + 7, first + 16, compare, projection);
            iter_swap_if(first + 8, first + 13, compare, projection);
            iter_swap_if(first + 9, first + 31, compare, projection);
            iter_swap_if(first + 10, first + 28, compare, projection);
            iter_swap_if(first + 11, first + 29, compare, projection);
            iter_swap_if(first + 17, first + 30, compare, projection);
            iter_swap_if(first + 18, first + 40, compare, projection);
            iter_swap_if(first + 21, first + 39, compare, projection);
            iter_swap_if(first + 22, first + 44, compare, projection);
            iter_swap_if(first + 23, first + 41, compare, projection);
            iter_swap_if(first + 24, first + 42, compare, projection);
            iter_swap_if(first + 35, first + 48, compare, projection);
            iter_swap_if(first + 36, first + 45, compare, projection);
            iter_swap_if(first + 38, first + 43, compare, projection);
            iter_swap_if(first + 47, first + 52, compare, projection);
            iter_swap_if(first + 2, first + 8, compare, projection);
            iter_swap_if(first + 5, first + 18, compare, projection);
            iter_swap_if(first + 9, first + 27, compare, projection);
            iter_swap_if(first + 10, first + 15, compare, projection);
            iter_swap_if(first + 11, first + 16, compare, projection);
            iter_swap_if(first + 12, first + 21, compare, projection);
            iter_swap_if(first + 17, first + 26, compare, projection);
            iter_swap_if(first + 19, first + 28, compare, projection);
            iter_swap_if(first + 20, first + 29, compare, projection);
            iter_swap_if(first + 23, first + 32, compare, projection);
            iter_swap_if(first + 24, first + 33, compare, projection);
            iter_swap_if(first + 25, first + 39, compare, projection);
            iter_swap_if(first + 30, first + 34, compare, projection);
            iter_swap_if(first + 31, first + 40, compare, projection);
            iter_swap_if(first + 35, first + 44, compare, projection);
            iter_swap_if(first + 36, first + 41, compare, projection);
            iter_swap_if(first + 37, first + 42, compare, projection);
            iter_swap_if(first + 43, first + 49, compare, projection);
            iter_swap_if(first + 46, first + 47, compare, projection);
            iter_swap_if(first + 2, first + 4, compare, projection);
            iter_swap_if(first + 5, first + 14, compare, projection);
            iter_swap_if(first + 12, first + 17, compare, projection);
            iter_swap_if(first + 15, first + 19, compare, projection);
            iter_swap_if(first + 16, first + 20, compare, projection);
            iter_swap_if(first + 21, first + 26, compare, projection);
            iter_swap_if(first + 23, first + 28, compare, projection);
            iter_swap_if(first + 24, first + 29, compare, projection);
            iter_swap_if(first + 25, first + 30, compare, projection);
            iter_swap_if(first + 27, first + 31, compare, projection);
            iter_swap_if(first + 32, first + 36, compare, projection);
            iter_swap_if(first + 33, first + 37, compare, projection);
            iter_swap_if(first + 34, first + 39, compare, projection);
            iter_swap_if(first + 35, first + 40, compare, projection);
            iter_swap_if(first + 42, first + 48, compare, projection);
            iter_swap_if(first + 49, first + 50, compare, projection);
            iter_swap_if(first + 5, first + 8, compare, projection);
            iter_swap_if(first + 9, first + 14, compare, projection);
            iter_swap_if(first + 12, first + 13, compare, projection);
            iter_swap_if(first + 17, first + 21, compare, projection);
            iter_swap_if(first + 18, first + 27, compare, projection);
            iter_swap_if(first + 22, first + 31, compare, projection);
            iter_swap_if(first + 25, first + 26, compare, projection);
            iter_swap_if(first + 30, first + 34, compare, projection);
            iter_swap_if(first + 33, first + 40, compare, projection);
            iter_swap_if(first + 37, first + 44, compare, projection);
            iter_swap_if(first + 38, first + 39, compare, projection);
            iter_swap_if(first + 43, first + 48, compare, projection);
            iter_swap_if(first + 47, first + 49, compare, projection);
            iter_swap_if(first + 50, first + 52, compare, projection);
            iter_swap_if(first + 3, first + 9, compare, projection);
            iter_swap_if(first + 6, first + 12, compare, projection);
            iter_swap_if(first + 10, first + 13, compare, projection);
            iter_swap_if(first + 14, first + 18, compare, projection);
            iter_swap_if(first + 15, first + 17, compare, projection);
            iter_swap_if(first + 19, first + 21, compare, projection);
            iter_swap_if(first + 22, first + 27, compare, projection);
            iter_swap_if(first + 23, first + 25, compare, projection);
            iter_swap_if(first + 26, first + 28, compare, projection);
            iter_swap_if(first + 30, first + 32, compare, projection);
            iter_swap_if(first + 31, first + 35, compare, projection);
            iter_swap_if(first + 34, first + 36, compare, projection);
            iter_swap_if(first + 38, first + 41, compare, projection);
            iter_swap_if(first + 39, first + 45, compare, projection);
            iter_swap_if(first + 49, first + 50, compare, projection);
            iter_swap_if(first + 51, first + 52, compare, projection);
            iter_swap_if(first + 3, first + 6, compare, projection);
            iter_swap_if(first + 7, first + 14, compare, projection);
            iter_swap_if(first + 10, first + 12, compare, projection);
            iter_swap_if(first + 11, first + 18, compare, projection);
            iter_swap_if(first + 16, first + 22, compare, projection);
            iter_swap_if(first + 20, first + 27, compare, projection);
            iter_swap_if(first + 24, first + 31, compare, projection);
            iter_swap_if(first + 25, first + 26, compare, projection);
            iter_swap_if(first + 29, first + 35, compare, projection);
            iter_swap_if(first + 32, first + 33, compare, projection);
            iter_swap_if(first + 36, first + 37, compare, projection);
            iter_swap_if(first + 38, first + 40, compare, projection);
            iter_swap_if(first + 39, first + 42, compare, projection);
            iter_swap_if(first + 41, first + 44, compare, projection);
            iter_swap_if(first + 45, first + 46, compare, projection);
            iter_swap_if(first + 3, first + 8, compare, projection);
            iter_swap_if(first + 7, first + 9, compare, projection);
            iter_swap_if(first + 11, first + 15, compare, projection);
            iter_swap_if(first + 13, first + 14, compare, projection);
            iter_swap_if(first + 16, first + 19, compare, projection);
            iter_swap_if(first + 17, first + 18, compare, projection);
            iter_swap_if(first + 20, first + 23, compare, projection);
            iter_swap_if(first + 21, first + 22, compare, projection);
            iter_swap_if(first + 24, first + 27, compare, projection);
            iter_swap_if(first + 28, first + 31, compare, projection);
            iter_swap_if(first + 29, first + 30, compare, projection);
            iter_swap_if(first + 34, first + 35, compare, projection);
            iter_swap_if(first + 36, first + 38, compare, projection);
            iter_swap_if(first + 37, first + 40, compare, projection);
            iter_swap_if(first + 39, first + 44, compare, projection);
            iter_swap_if(first + 43, first + 45, compare, projection);
            iter_swap_if(first + 46, first + 48, compare, projection);
            iter_swap_if(first + 3, first + 5, compare, projection);
            iter_swap_if(first + 6, first + 8, compare, projection);
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
            iter_swap_if(first + 32, first + 34, compare, projection);
            iter_swap_if(first + 33, first + 35, compare, projection);
            iter_swap_if(first + 37, first + 38, compare, projection);
            iter_swap_if(first + 39, first + 41, compare, projection);
            iter_swap_if(first + 42, first + 44, compare, projection);
            iter_swap_if(first + 45, first + 46, compare, projection);
            iter_swap_if(first + 47, first + 48, compare, projection);
            iter_swap_if(first + 3, first + 4, compare, projection);
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
            iter_swap_if(first + 39, first + 40, compare, projection);
            iter_swap_if(first + 43, first + 44, compare, projection);
        }

        template<typename DifferenceType=std::ptrdiff_t>
        CPPSORT_ATTRIBUTE_NODISCARD
        static constexpr auto index_pairs()
            -> std::array<utility::index_pair<DifferenceType>, 411>
        {
            return {{
                {0, 12}, {1, 10}, {2, 9}, {3, 7}, {5, 11}, {6, 8}, {13, 25}, {14, 23}, {15, 22}, {16, 20}, {18, 24}, {19, 21}, {26, 38}, {27, 36}, {28, 35}, {29, 33}, {31, 37}, {32, 34}, {39, 45}, {40, 50}, {41, 51}, {42, 49}, {43, 44}, {46, 52}, {47, 48},
                {1, 6}, {2, 3}, {4, 11}, {7, 9}, {8, 10}, {14, 19}, {15, 16}, {17, 24}, {20, 22}, {21, 23}, {27, 32}, {28, 29}, {30, 37}, {33, 35}, {34, 36}, {40, 41}, {42, 46}, {43, 47}, {44, 48}, {45, 49}, {50, 51},
                {0, 4}, {1, 2}, {3, 6}, {7, 8}, {9, 10}, {11, 12}, {13, 17}, {14, 15}, {16, 19}, {20, 21}, {22, 23}, {24, 25}, {26, 30}, {27, 28}, {29, 32}, {33, 34}, {35, 36}, {37, 38}, {39, 43}, {40, 42}, {44, 45}, {46, 47}, {48, 52}, {49, 51},
                {4, 6}, {5, 9}, {8, 11}, {10, 12}, {17, 19}, {18, 22}, {21, 24}, {23, 25}, {30, 32}, {31, 35}, {34, 37}, {36, 38}, {39, 40}, {41, 48}, {42, 46}, {43, 50}, {45, 49}, {51, 52},
                {0, 5}, {3, 8}, {4, 7}, {6, 11}, {9, 10}, {13, 18}, {16, 21}, {17, 20}, {19, 24}, {22, 23}, {25, 38}, {26, 31}, {29, 34}, {30, 33}, {32, 37}, {35, 36}, {41, 44}, {43, 46}, {45, 48}, {47, 50},
                {0, 1}, {2, 5}, {6, 9}, {7, 8}, {10, 11}, {13, 14}, {15, 18}, {19, 22}, {20, 21}, {23, 24}, {26, 27}, {28, 31}, {32, 35}, {33, 34}, {36, 37}, {40, 41}, {42, 43}, {45, 46}, {48, 49}, {50, 51},
                {0, 26}, {1, 3}, {2, 4}, {5, 6}, {9, 10}, {11, 24}, {13, 39}, {14, 16}, {15, 17}, {18, 19}, {22, 23}, {27, 29}, {28, 30}, {31, 32}, {35, 36}, {40, 42}, {41, 43}, {44, 45}, {46, 47}, {48, 50}, {49, 51},
                {0, 13}, {1, 2}, {3, 4}, {5, 7}, {6, 8}, {10, 23}, {12, 51}, {14, 15}, {16, 17}, {18, 20}, {19, 21}, {26, 39}, {27, 28}, {29, 30}, {31, 33}, {32, 34}, {41, 42}, {43, 46}, {45, 48}, {49, 50},
                {1, 27}, {2, 3}, {4, 5}, {6, 7}, {8, 9}, {12, 25}, {13, 26}, {14, 40}, {15, 16}, {17, 18}, {19, 20}, {21, 22}, {28, 29}, {30, 31}, {32, 33}, {34, 35}, {37, 50}, {38, 51}, {43, 44}, {45, 46}, {47, 48},
                {1, 14}, {2, 15}, {3, 4}, {5, 6}, {7, 33}, {8, 34}, {9, 35}, {11, 37}, {16, 17}, {18, 19}, {22, 52}, {24, 50}, {25, 38}, {27, 40}, {28, 41}, {29, 30}, {31, 32}, {42, 43}, {44, 45}, {46, 47}, {48, 49},
                {2, 28}, {3, 16}, {4, 43}, {5, 31}, {6, 32}, {9, 22}, {14, 27}, {15, 41}, {17, 30}, {19, 45}, {20, 46}, {21, 47}, {24, 37}, {29, 42}, {35, 52}, {36, 49},
                {3, 29}, {4, 17}, {5, 44}, {6, 19}, {7, 20}, {8, 21}, {10, 36}, {15, 28}, {16, 42}, {18, 31}, {22, 35}, {23, 49}, {30, 43}, {32, 45}, {33, 46}, {34, 47},
                {4, 13}, {6, 15}, {8, 26}, {10, 41}, {11, 42}, {12, 17}, {16, 29}, {19, 32}, {20, 33}, {21, 34}, {23, 36}, {25, 43}, {31, 48}, {37, 46}, {38, 47}, {40, 44},
                {1, 4}, {7, 16}, {8, 13}, {9, 31}, {10, 28}, {11, 29}, {17, 30}, {18, 40}, {21, 39}, {22, 44}, {23, 41}, {24, 42}, {35, 48}, {36, 45}, {38, 43}, {47, 52},
                {2, 8}, {5, 18}, {9, 27}, {10, 15}, {11, 16}, {12, 21}, {17, 26}, {19, 28}, {20, 29}, {23, 32}, {24, 33}, {25, 39}, {30, 34}, {31, 40}, {35, 44}, {36, 41}, {37, 42}, {43, 49}, {46, 47},
                {2, 4}, {5, 14}, {12, 17}, {15, 19}, {16, 20}, {21, 26}, {23, 28}, {24, 29}, {25, 30}, {27, 31}, {32, 36}, {33, 37}, {34, 39}, {35, 40}, {42, 48}, {49, 50},
                {5, 8}, {9, 14}, {12, 13}, {17, 21}, {18, 27}, {22, 31}, {25, 26}, {30, 34}, {33, 40}, {37, 44}, {38, 39}, {43, 48}, {47, 49}, {50, 52},
                {3, 9}, {6, 12}, {10, 13}, {14, 18}, {15, 17}, {19, 21}, {22, 27}, {23, 25}, {26, 28}, {30, 32}, {31, 35}, {34, 36}, {38, 41}, {39, 45}, {49, 50}, {51, 52},
                {3, 6}, {7, 14}, {10, 12}, {11, 18}, {16, 22}, {20, 27}, {24, 31}, {25, 26}, {29, 35}, {32, 33}, {36, 37}, {38, 40}, {39, 42}, {41, 44}, {45, 46},
                {3, 8}, {7, 9}, {11, 15}, {13, 14}, {16, 19}, {17, 18}, {20, 23}, {21, 22}, {24, 27}, {28, 31}, {29, 30}, {34, 35}, {36, 38}, {37, 40}, {39, 44}, {43, 45}, {46, 48},
                {3, 5}, {6, 8}, {7, 10}, {9, 12}, {11, 13}, {14, 15}, {16, 17}, {18, 19}, {20, 21}, {22, 23}, {24, 25}, {26, 27}, {28, 29}, {30, 31}, {32, 34}, {33, 35}, {37, 38}, {39, 41}, {42, 44}, {45, 46}, {47, 48},
                {3, 4}, {7, 8}, {9, 10}, {11, 12}, {13, 14}, {15, 16}, {17, 18}, {19, 20}, {21, 22}, {23, 24}, {25, 26}, {27, 28}, {29, 30}, {31, 32}, {33, 34}, {35, 36}, {39, 40}, {43, 44},
            }};
        }
    };
}}

#endif // CPPSORT_DETAIL_SORTING_NETWORK_SORT53_H_
