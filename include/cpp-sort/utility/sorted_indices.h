/*
 * Copyright (c) 2022-2024 Morwenn
 * SPDX-License-Identifier: MIT
 */
#ifndef CPPSORT_UTILITY_SORTED_INDICES_H_
#define CPPSORT_UTILITY_SORTED_INDICES_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <functional>
#include <iterator>
#include <numeric>
#include <utility>
#include <vector>
#include <cpp-sort/mstd/iterator.h>
#include <cpp-sort/mstd/ranges.h>
#include <cpp-sort/sorter_facade.h>
#include <cpp-sort/sorter_traits.h>
#include <cpp-sort/utility/adapter_storage.h>
#include "../detail/checkers.h"

namespace cppsort::utility
{
    namespace detail
    {
        template<typename Sorter>
        struct sorted_indices_impl:
            utility::adapter_storage<Sorter>,
            cppsort::detail::check_is_always_stable<Sorter>
        {
            sorted_indices_impl() = default;

            constexpr explicit sorted_indices_impl(Sorter&& sorter):
                utility::adapter_storage<Sorter>(std::move(sorter))
            {}

            template<
                mstd::random_access_iterator Iterator,
                mstd::sentinel_for<Iterator> Sentinel,
                typename Compare = std::less<>,
                typename Projection = std::identity
            >
                requires is_projection_iterator_v<Projection, Iterator, Compare>
            auto operator()(Iterator first, Sentinel last,
                            Compare compare={}, Projection projection={}) const
                -> std::vector<mstd::iter_difference_t<Iterator>>
            {
                using difference_type = mstd::iter_difference_t<Iterator>;

                // Create a vector of indices
                std::vector<difference_type> indices(mstd::distance(first, last), 0);
                std::iota(indices.begin(), indices.end(), 0);

                // Reorder the vector thanks to the passed sorter
                this->get()(indices, std::move(compare),
                            [&first, &projection](difference_type index) -> auto& {
                                return std::invoke(projection, first[index]);
                            });

                // Return the indices that would sort the array
                return indices;
            }

            ////////////////////////////////////////////////////////////
            // Sorter traits

            using iterator_category = std::random_access_iterator_tag;
        };
    }

    template<typename Sorter>
    struct sorted_indices:
        sorter_facade_base<detail::sorted_indices_impl<Sorter>>
    {
        sorted_indices() = default;

        constexpr explicit sorted_indices(Sorter sorter):
            sorter_facade_base<detail::sorted_indices_impl<Sorter>>(std::move(sorter))
        {}
    };
}

namespace cppsort
{
    ////////////////////////////////////////////////////////////
    // is_stable specialization

    template<typename Sorter, typename... Args>
    struct is_stable<cppsort::utility::sorted_indices<Sorter>(Args...)>:
        is_stable<Sorter(Args...)>
    {};
}

#endif // CPPSORT_UTILITY_SORTED_INDICES_H_
