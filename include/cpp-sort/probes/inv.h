/*
 * Copyright (c) 2016-2024 Morwenn
 * SPDX-License-Identifier: MIT
 */
#ifndef CPPSORT_PROBES_INV_H_
#define CPPSORT_PROBES_INV_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <functional>
#include <memory>
#include <utility>
#include <cpp-sort/mstd/iterator.h>
#include <cpp-sort/mstd/ranges.h>
#include <cpp-sort/sorter_facade.h>
#include <cpp-sort/sorter_traits.h>
#include <cpp-sort/utility/functional.h>
#include "../detail/count_inversions.h"

namespace cppsort::probe
{
    namespace detail
    {
        template<typename ForwardIterator, typename Sentinel, typename Compare, typename Projection>
        auto inv_probe_algo(ForwardIterator first, Sentinel last,
                            mstd::iter_difference_t<ForwardIterator> size,
                            Compare compare, Projection projection)
            -> mstd::iter_difference_t<ForwardIterator>
        {
            using difference_type = mstd::iter_difference_t<ForwardIterator>;

            if (size < 2) {
                return 0;
            }

            auto iterators = std::make_unique<ForwardIterator[]>(size);
            auto buffer = std::make_unique<ForwardIterator[]>(size);

            auto store = iterators.get();
            for (auto it = first; it != last; ++it) {
                *store++ = it;
            }

            return cppsort::detail::count_inversions<difference_type>(
                iterators.get(), iterators.get() + size, buffer.get(),
                std::move(compare),
                utility::indirect{} | std::move(projection)
            );
        }

        struct inv_impl
        {
            template<
                mstd::forward_range Range,
                typename Compare = std::less<>,
                typename Projection = std::identity
            >
                requires is_projection_v<Projection, Range, Compare>
            auto operator()(Range&& range, Compare compare={}, Projection projection={}) const
                -> mstd::range_difference_t<Range>
            {
                return inv_probe_algo(mstd::begin(range), mstd::end(range),
                                      mstd::distance(range),
                                      std::move(compare), std::move(projection));
            }

            template<
                mstd::forward_iterator Iterator,
                mstd::sentinel_for<Iterator> Sentinel,
                typename Compare = std::less<>,
                typename Projection = std::identity
            >
                requires is_projection_iterator_v<Projection, Iterator, Compare>
            auto operator()(Iterator first, Sentinel last,
                            Compare compare={}, Projection projection={}) const
                -> mstd::iter_difference_t<Iterator>
            {
                return inv_probe_algo(first, last,
                                      mstd::distance(first, last),
                                      std::move(compare), std::move(projection));
            }

            template<typename Integer>
            static constexpr auto max_for_size(Integer n)
                -> Integer
            {
                return n == 0 ? 0 : n * (n - 1) / 2;
            }
        };
    }

    inline constexpr sorter_facade_base<detail::inv_impl> inv{};
}

#endif // CPPSORT_PROBES_INV_H_
