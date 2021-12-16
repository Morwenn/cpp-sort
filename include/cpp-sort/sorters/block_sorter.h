/*
 * Copyright (c) 2015-2021 Morwenn
 * SPDX-License-Identifier: MIT
 */
#ifndef CPPSORT_SORTERS_BLOCK_SORTER_H_
#define CPPSORT_SORTERS_BLOCK_SORTER_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <functional>
#include <iterator>
#include <type_traits>
#include <utility>
#include <cpp-sort/sorter_facade.h>
#include <cpp-sort/sorter_traits.h>
#include <cpp-sort/utility/buffer.h>
#include <cpp-sort/utility/functional.h>
#include <cpp-sort/utility/static_const.h>
#include "../detail/attributes.h"
#include "../detail/iterator_traits.h"
#include "../detail/type_traits.h"
#include "../detail/wiki_sort.h"

namespace cppsort
{
    ////////////////////////////////////////////////////////////
    // Sorter

    namespace detail
    {
        template<typename BufferProvider>
        struct block_sorter_impl
        {
            template<
                typename RandomAccessIterator,
                typename Compare = std::less<>,
                typename Projection = utility::identity,
                typename = detail::enable_if_t<is_projection_iterator_v<
                    Projection, RandomAccessIterator, Compare
                >>
            >
            auto operator()(RandomAccessIterator first, RandomAccessIterator last,
                            Compare compare={}, Projection projection={}) const
                -> void
            {
                static_assert(
                    std::is_base_of<
                        iterator_category,
                        iterator_category_t<RandomAccessIterator>
                    >::value,
                    "block_sorter requires at least random-access iterators"
                );

                wiki_sort<BufferProvider>(std::move(first), std::move(last),
                                          std::move(compare), std::move(projection));
            }

            ////////////////////////////////////////////////////////////
            // Sorter traits

            using iterator_category = std::random_access_iterator_tag;
            using is_always_stable = std::true_type;
        };
    }

    template<
        typename BufferProvider = utility::fixed_buffer<512>
    >
    struct block_sorter:
        sorter_facade<detail::block_sorter_impl<BufferProvider>>
    {};

    ////////////////////////////////////////////////////////////
    // Sort function

    namespace
    {
        CPPSORT_DEPRECATED("block_sort() is deprecated and will be removed in version 2.0.0, use wiki_sort() instead")
        constexpr auto&& block_sort
            = utility::static_const<block_sorter<>>::value;
    }
}

#endif // CPPSORT_SORTERS_BLOCK_SORTER_H_
