/*
 * Copyright (c) 2022 Morwenn
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
#include <type_traits>
#include <utility>
#include <vector>
#include <cpp-sort/sorter_facade.h>
#include <cpp-sort/sorter_traits.h>
#include <cpp-sort/utility/adapter_storage.h>
#include <cpp-sort/utility/as_function.h>
#include <cpp-sort/utility/functional.h>
#include "../detail/checkers.h"
#include "../detail/iterator_traits.h"
#include "../detail/type_traits.h"

namespace cppsort
{
namespace utility
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
                typename RandomAccessIterator,
                typename Compare = std::less<>,
                typename Projection = utility::identity,
                typename = cppsort::detail::enable_if_t<is_projection_iterator_v<
                    Projection, RandomAccessIterator, Compare
                >>
            >
            auto operator()(RandomAccessIterator first, RandomAccessIterator last,
                            Compare compare={}, Projection projection={}) const
                -> std::vector<cppsort::detail::difference_type_t<RandomAccessIterator>>
            {
                static_assert(
                    std::is_base_of<
                        iterator_category,
                        cppsort::detail::iterator_category_t<RandomAccessIterator>
                    >::value,
                    "sorted_indices requires at least random-access iterators"
                );

                using difference_type = cppsort::detail::difference_type_t<RandomAccessIterator>;
                auto&& proj = utility::as_function(projection);

                // Create a vector of indices
                std::vector<difference_type> indices(last - first, 0);
                std::iota(indices.begin(), indices.end(), 0);

                // Reorder the vector thanks to the passed sorter
                this->get()(indices, std::move(compare),
                            [&first, &proj](difference_type index) -> auto& {
                                return proj(first[index]);
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
        sorter_facade<detail::sorted_indices_impl<Sorter>>
    {
        sorted_indices() = default;

        constexpr explicit sorted_indices(Sorter sorter):
            sorter_facade<detail::sorted_indices_impl<Sorter>>(std::move(sorter))
        {}
    };
}}

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
