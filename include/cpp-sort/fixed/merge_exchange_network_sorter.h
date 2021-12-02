/*
 * Copyright (c) 2021 Morwenn
 * SPDX-License-Identifier: MIT
 */
#ifndef CPPSORT_FIXED_MERGE_EXCHANGE_NETWORK_SORTER_H_
#define CPPSORT_FIXED_MERGE_EXCHANGE_NETWORK_SORTER_H_

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
    struct merge_exchange_network_sorter;

    namespace detail
    {
        template<typename DifferenceType>
        constexpr auto merge_exchange_pairs_number(DifferenceType n) noexcept
            -> DifferenceType
        {
            DifferenceType nb_pairs = 0;
            DifferenceType t = detail::ceil_log2(n);

            for (DifferenceType p = 1 << (t - 1); p > 0; p /= 2) {
                DifferenceType q = 1 << (t - 1);
                DifferenceType r = 0;
                DifferenceType d = p;

                while (d > 0) {
                    for (DifferenceType i = 0; i < n - d; ++i) {
                        if ((i & p) == r) {
                            ++nb_pairs;
                        }
                    }
                    d = q - p;
                    q /= 2;
                    r = p;
                }
            }

            return nb_pairs;
        }

        template<std::size_t N>
        struct merge_exchange_network_sorter_impl
        {
            template<typename DifferenceType=std::ptrdiff_t>
            CPPSORT_ATTRIBUTE_NODISCARD
            static constexpr auto index_pairs()
                -> auto
            {
                constexpr DifferenceType n = N;
                constexpr DifferenceType nb_pairs = merge_exchange_pairs_number(n);

                utility::index_pair<DifferenceType> pairs[nb_pairs] = {};
                std::size_t current_pair_idx = 0;

                DifferenceType t = detail::ceil_log2(n);
                for (DifferenceType p = 1 << (t - 1); p > 0; p /= 2) {
                    DifferenceType q = 1 << (t - 1);
                    DifferenceType r = 0;
                    DifferenceType d = p;

                    while (d > 0) {
                        for (DifferenceType i = 0; i < n - d; ++i) {
                            if ((i & p) == r) {
                                pairs[current_pair_idx] = { i, i + d };
                                ++current_pair_idx;
                            }
                        }
                        d = q - p;
                        q /= 2;
                        r = p;
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
        struct merge_exchange_network_sorter_impl<0u>:
            cppsort::detail::empty_network_sorter_impl
        {};

        template<>
        struct merge_exchange_network_sorter_impl<1u>:
            cppsort::detail::empty_network_sorter_impl
        {};
    }

    template<std::size_t N>
    struct merge_exchange_network_sorter:
        sorter_facade<detail::merge_exchange_network_sorter_impl<N>>
    {};

    ////////////////////////////////////////////////////////////
    // Sorter traits

    template<std::size_t N>
    struct sorter_traits<merge_exchange_network_sorter<N>>
    {
        using iterator_category = std::random_access_iterator_tag;
        using is_always_stable = std::false_type;
    };

    template<>
    struct fixed_sorter_traits<merge_exchange_network_sorter>
    {
        using iterator_category = std::random_access_iterator_tag;
        using is_always_stable = std::false_type;
    };
}

#endif // CPPSORT_FIXED_MERGE_EXCHANGE_NETWORK_SORTER_H_
