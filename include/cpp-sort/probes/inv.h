/*
 * Copyright (c) 2016-2022 Morwenn
 * SPDX-License-Identifier: MIT
 */
#ifndef CPPSORT_PROBES_INV_H_
#define CPPSORT_PROBES_INV_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <functional>
#include <iterator>
#include <memory>
#include <type_traits>
#include <utility>
#include <cpp-sort/mstd/iterator.h>
#include <cpp-sort/mstd/ranges.h>
#include <cpp-sort/mstd/type_traits.h>
#include <cpp-sort/sorter_facade.h>
#include <cpp-sort/sorter_traits.h>
#include <cpp-sort/utility/size.h>
#include "../detail/count_inversions.h"
#include "../detail/functional.h"
#include "../detail/iterator_traits.h"

namespace cppsort::probe
{
    namespace detail
    {
        template<typename ForwardIterator, typename Compare, typename Projection>
        auto inv_probe_algo(ForwardIterator first, ForwardIterator last,
                            cppsort::detail::difference_type_t<ForwardIterator> size,
                            Compare compare, Projection projection)
            -> ::cppsort::detail::difference_type_t<ForwardIterator>
        {
            using difference_type = ::cppsort::detail::difference_type_t<ForwardIterator>;

            if (size < 2) {
                return 0;
            }

            auto iterators = std::make_unique<ForwardIterator[]>(size);
            auto buffer = std::make_unique<ForwardIterator[]>(size);

            auto store = iterators.get();
            for (ForwardIterator it = first ; it != last ; ++it) {
                *store++ = it;
            }

            return cppsort::detail::count_inversions<difference_type>(
                iterators.get(), iterators.get() + size, buffer.get(),
                std::move(compare),
                cppsort::detail::indirect(std::move(projection))
            );
        }

        struct inv_impl
        {
            template<
                mstd::forward_range Range,
                typename Compare = std::less<>,
                typename Projection = std::identity,
                typename = mstd::enable_if_t<
                    is_projection_v<Projection, Range, Compare>
                >
            >
            auto operator()(Range&& range, Compare compare={}, Projection projection={}) const
                -> decltype(auto)
            {
                return inv_probe_algo(mstd::begin(range), mstd::end(range),
                                      utility::size(range),
                                      std::move(compare), std::move(projection));
            }

            template<
                mstd::forward_iterator Iterator,
                typename Compare = std::less<>,
                typename Projection = std::identity,
                typename = mstd::enable_if_t<
                    is_projection_iterator_v<Projection, Iterator, Compare>
                >
            >
            auto operator()(Iterator first, Iterator last,
                            Compare compare={}, Projection projection={}) const
                -> decltype(auto)
            {
                return inv_probe_algo(first, last,
                                      std::distance(first, last),
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

    inline constexpr sorter_facade<detail::inv_impl> inv{};
}

#endif // CPPSORT_PROBES_INV_H_
