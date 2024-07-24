/*
 * Copyright (c) 2021-2024 Morwenn
 * SPDX-License-Identifier: MIT
 */
#ifndef CPPSORT_PROBES_BLOCK_H_
#define CPPSORT_PROBES_BLOCK_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <functional>
#include <utility>
#include <cpp-sort/mstd/iterator.h>
#include <cpp-sort/mstd/ranges.h>
#include <cpp-sort/sorter_facade.h>
#include <cpp-sort/sorter_traits.h>
#include <cpp-sort/utility/as_function.h>
#include <cpp-sort/utility/functional.h>
#include "../detail/immovable_vector.h"
#include "../detail/pdqsort.h"

namespace cppsort::probe
{
    namespace detail
    {
        template<typename ForwardIterator, typename Sentinel, typename Compare, typename Projection>
        auto block_probe_algo(ForwardIterator first, Sentinel last,
                              mstd::iter_difference_t<ForwardIterator> size,
                              Compare compare, Projection projection)
            -> mstd::iter_difference_t<ForwardIterator>
        {
            using difference_type = mstd::iter_difference_t<ForwardIterator>;
            auto&& comp = utility::as_function(compare);
            auto&& proj = utility::as_function(projection);

            if (size < 2) {
                return 0;
            }

            ////////////////////////////////////////////////////////////
            // Indirectly sort the iterators

            // Copy the iterators in a vector
            cppsort::detail::immovable_vector<ForwardIterator> iterators(size);
            for (auto it = first; it != last; ++it) {
                iterators.emplace_back(it);
            }

            // Sort the iterators on pointed values
            cppsort::detail::pdqsort(
                iterators.begin(), iterators.end(),
                compare, utility::indirect{} | projection
            );

            ////////////////////////////////////////////////////////////
            // Count the number of consecutive pairs in the original
            // collection that can't be found in the sorted one

            difference_type count = 0;
            auto it_last_1 = mstd::prev(iterators.end());
            for (auto it = iterators.begin(); it != it_last_1; ++it) {
                auto orig_next = mstd::next(*it);
                if (orig_next == last) {
                    if (comp(proj(**it), proj(**it_last_1)) || comp(proj(**it_last_1), proj(**it))) {
                        ++count;
                    }
                    continue;
                }
                auto sorted_next = *mstd::next(it);
                if (comp(proj(*orig_next), proj(*sorted_next)) || comp(proj(*sorted_next), proj(*orig_next))) {
                    ++count;
                }
            }
            return count;
        }

        struct block_impl
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
                return block_probe_algo(mstd::begin(range), mstd::end(range),
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
                return block_probe_algo(first, last,
                                        mstd::distance(first, last),
                                        std::move(compare), std::move(projection));
            }

            template<typename Integer>
            static constexpr auto max_for_size(Integer n)
                -> Integer
            {
                return n == 0 ? 0 : n - 1;
            }
        };
    }

    inline constexpr sorter_facade<detail::block_impl> block{};
}

#endif // CPPSORT_PROBES_BLOCK_H_
