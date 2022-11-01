/*
 * Copyright (c) 2015-2022 Morwenn
 * SPDX-License-Identifier: MIT
 */
#ifndef CPPSORT_SORTERS_GRAIL_SORTER_H_
#define CPPSORT_SORTERS_GRAIL_SORTER_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <functional>
#include <iterator>
#include <type_traits>
#include <utility>
#include <cpp-sort/mstd/iterator.h>
#include <cpp-sort/mstd/type_traits.h>
#include <cpp-sort/sorter_facade.h>
#include <cpp-sort/sorter_traits.h>
#include <cpp-sort/utility/buffer.h>
#include "../detail/grail_sort.h"

namespace cppsort
{
    ////////////////////////////////////////////////////////////
    // Sorter

    namespace detail
    {
        template<typename BufferProvider>
        struct grail_sorter_impl
        {
            template<
                mstd::random_access_iterator Iterator,
                mstd::sentinel_for<Iterator> Sentinel,
                typename Compare = std::less<>,
                typename Projection = std::identity,
                typename = mstd::enable_if_t<
                    is_projection_iterator_v<Projection, Iterator, Compare>
                >
            >
            auto operator()(Iterator first, Sentinel last,
                            Compare compare={}, Projection projection={}) const
                -> void
            {
                auto last_it = mstd::next(first, std::move(last));
                grail::grail_sort<BufferProvider>(std::move(first), std::move(last_it),
                                                  std::move(compare), std::move(projection));
            }

            ////////////////////////////////////////////////////////////
            // Sorter traits

            using iterator_category = std::random_access_iterator_tag;
            using is_always_stable = std::true_type;
        };
    }

    template<
        typename BufferProvider = utility::fixed_buffer<0>
    >
    struct grail_sorter:
        sorter_facade<detail::grail_sorter_impl<BufferProvider>>
    {};

    ////////////////////////////////////////////////////////////
    // Sort function

    inline constexpr grail_sorter grail_sort{};
}

#endif // CPPSORT_SORTERS_GRAIL_SORTER_H_
