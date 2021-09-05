/*
 * Copyright (c) 2021 Morwenn
 * SPDX-License-Identifier: MIT
 */
#ifndef CPPSORT_FIXED_ODD_EVEN_MERGE_NETWORK_SORTER_H_
#define CPPSORT_FIXED_ODD_EVEN_MERGE_NETWORK_SORTER_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <array>
#include <cstddef>
#include <functional>
#include <iterator>
#include <type_traits>
#include <utility>
#include <cpp-sort/sorter_facade.h>
#include <cpp-sort/sorter_traits.h>
#include <cpp-sort/utility/sorting_networks.h>
#include "../detail/attributes.h"
#include "../detail/bitops.h"
#include "../detail/empty_sorter.h"
#include "../detail/iterator_traits.h"
#include "../detail/make_array.h"
#include "../detail/type_traits.h"

namespace cppsort
{
    ////////////////////////////////////////////////////////////
    // Adapter

    template<std::size_t N>
    struct odd_even_merge_network_sorter;

    namespace detail
    {
        template<typename DifferenceType>
        constexpr auto odd_even_merge_pairs_number(DifferenceType n) noexcept
            -> DifferenceType
        {
            DifferenceType nb_pairs = 0;

            for (DifferenceType p = 1; p < n; p *= 2) {
                for (auto k = p; k > 0; k /= 2) {
                    for (auto j = k % p; j < n - k; j += 2 * k) {
                        for (DifferenceType i = 0; i < k; ++i) {
                            if ((i + j) / (p * 2) == (i + j + k) / (p * 2)) {
                                ++nb_pairs;
                            }
                        }
                    }
                }
            }

            return nb_pairs;
        }

        template<std::size_t N>
        struct odd_even_merge_network_sorter_impl
        {
            template<typename DifferenceType=std::ptrdiff_t>
            CPPSORT_ATTRIBUTE_NODISCARD
            static constexpr auto index_pairs()
                -> auto
            {
                constexpr DifferenceType n = N;
                constexpr DifferenceType nb_pairs = odd_even_merge_pairs_number(n);

                utility::index_pair<DifferenceType> pairs[nb_pairs] = {};
                std::size_t current_pair_idx = 0;

                for (DifferenceType p = 1; p < n; p *= 2) {
                    for (auto k = p; k > 0; k /= 2) {
                        for (auto j = k % p; j < n - k; j += 2 * k) {
                            for (DifferenceType i = 0; i < k; ++i) {
                                if ((i + j) / (p * 2) == (i + j + k) / (p * 2)) {
                                    pairs[current_pair_idx] = { i + j, i + j + k };
                                    ++current_pair_idx;
                                }
                            }
                        }
                    }
                }

                return cppsort::detail::make_array(pairs);
            }

            template<
                typename RandomAccessIterator,
                typename Compare = std::less<>,
                typename Projection = utility::identity,
                typename = detail::enable_if_t<is_projection_iterator_v<
                    Projection, RandomAccessIterator, Compare
                >>
            >
            constexpr auto operator()(RandomAccessIterator first, RandomAccessIterator,
                            Compare compare={}, Projection projection={}) const
                -> void
            {
                using difference_type = difference_type_t<RandomAccessIterator>;
                auto pairs = index_pairs<difference_type>();
                utility::swap_index_pairs(first, pairs, std::move(compare), std::move(projection));
            }
        };

        template<>
        struct odd_even_merge_network_sorter_impl<0u>:
            cppsort::detail::empty_network_sorter_impl
        {};

        template<>
        struct odd_even_merge_network_sorter_impl<1u>:
            cppsort::detail::empty_network_sorter_impl
        {};
    }

    template<std::size_t N>
    struct odd_even_merge_network_sorter:
        sorter_facade<detail::odd_even_merge_network_sorter_impl<N>>
    {
        static_assert(N == 0 || detail::has_single_bit(N),
                      "odd_even_merge_network_sorter only works when N is a power of 2");
    };

    ////////////////////////////////////////////////////////////
    // Sorter traits

    template<std::size_t N>
    struct sorter_traits<odd_even_merge_network_sorter<N>>
    {
        using iterator_category = std::random_access_iterator_tag;
        using is_always_stable = std::false_type;
    };

    template<>
    struct fixed_sorter_traits<odd_even_merge_network_sorter>
    {
        using iterator_category = std::random_access_iterator_tag;
        using is_always_stable = std::false_type;
    };
}

#endif // CPPSORT_FIXED_ODD_EVEN_MERGE_NETWORK_SORTER_H_
