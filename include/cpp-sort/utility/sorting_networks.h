/*
 * Copyright (c) 2021 Morwenn
 * SPDX-License-Identifier: MIT
 */
#ifndef CPPSORT_UTILITY_SORTING_NETWORKS_H_
#define CPPSORT_UTILITY_SORTING_NETWORKS_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <array>
#include <cstddef>
#include <functional>
#include <utility>
#include <cpp-sort/utility/functional.h>
#include "../detail/swap_if.h"

namespace cppsort
{
namespace utility
{
    ////////////////////////////////////////////////////////////
    // Extremely simple but sufficient replacement for std::pair
    // to represent a pair of indices to compare and exchange in
    // sorting networks

    template<typename IndexType>
    struct index_pair
    {
        IndexType first, second;
    };

    ////////////////////////////////////////////////////////////
    // swap_index_pairs
    //
    // Let the compiler decide whether to unroll or not
    // depending on the number of elements to handle

    template<
        typename RandomAccessIterator,
        typename IndexType,
        std::size_t N,
        typename Compare = std::less<>,
        typename Projection = utility::identity
    >
    auto swap_index_pairs(RandomAccessIterator first, const std::array<index_pair<IndexType>, N>& index_pairs,
                          Compare compare={}, Projection projection={})
        -> void
    {
        for (const index_pair<IndexType>& pair: index_pairs) {
            cppsort::detail::iter_swap_if(first + pair.first, first + pair.second,
                                          compare, projection);
        }
    }

    ////////////////////////////////////////////////////////////
    // swap_index_pairs_force_unroll
    //
    // This is a best effort function to try to force the
    // compiler to generate unrolled code, but the result is
    // hardly ever perfect

    namespace detail
    {
        template<std::size_t CurrentPairIndex>
        struct index_pair_swapper_force_unroll
        {
            template<
                typename RandomAccessIterator,
                typename IndexType,
                std::size_t N,
                typename Compare,
                typename Projection
            >
            constexpr static auto do_it(RandomAccessIterator first,
                                        const std::array<index_pair<IndexType>, N>& index_pairs,
                                        Compare compare, Projection projection)
                -> void
            {
                static_assert(CurrentPairIndex < N, "this should never happen, what did you do? o_o");
                cppsort::detail::iter_swap_if(first + index_pairs[CurrentPairIndex].first,
                                              first + index_pairs[CurrentPairIndex].second,
                                              compare, projection);
                index_pair_swapper_force_unroll<CurrentPairIndex + 1>::do_it(first, index_pairs,
                                                                            std::move(compare), std::move(projection));
            }

            template<
                typename RandomAccessIterator,
                typename IndexType,
                typename Compare,
                typename Projection
            >
            constexpr static auto do_it(RandomAccessIterator first,
                                        const std::array<index_pair<IndexType>, CurrentPairIndex + 1>& index_pairs,
                                        Compare compare, Projection projection)
                -> void
            {
                cppsort::detail::iter_swap_if(first + index_pairs[CurrentPairIndex].first,
                                              first + index_pairs[CurrentPairIndex].second,
                                              std::move(compare), std::move(projection));
            }
        };
    }

    template<
        typename RandomAccessIterator,
        typename IndexType,
        std::size_t N,
        typename Compare = std::less<>,
        typename Projection = utility::identity
    >
    auto swap_index_pairs_force_unroll(RandomAccessIterator first,
                                       const std::array<index_pair<IndexType>, N>& index_pairs,
                                       Compare compare={}, Projection projection={})
        -> void
    {
        detail::index_pair_swapper_force_unroll<0>::do_it(first, index_pairs, std::move(compare), std::move(projection));
    }

    template<typename RandomAccessIterator, typename IndexType, typename Compare, typename Projection>
    auto swap_index_pairs_force_unroll(RandomAccessIterator, const std::array<index_pair<IndexType>, 0>&,
                                       Compare, Projection)
        -> void
    {}
}}

#endif // CPPSORT_UTILITY_SORTING_NETWORKS_H_
