/*
 * Copyright (c) 2016-2022 Morwenn
 * SPDX-License-Identifier: MIT
 */
#ifndef CPPSORT_PROBES_HAM_H_
#define CPPSORT_PROBES_HAM_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <functional>
#include <iterator>
#include <type_traits>
#include <utility>
#include <cpp-sort/mstd/iterator.h>
#include <cpp-sort/mstd/ranges.h>
#include <cpp-sort/mstd/type_traits.h>
#include <cpp-sort/sorter_facade.h>
#include <cpp-sort/sorter_traits.h>
#include <cpp-sort/utility/as_function.h>
#include "../detail/functional.h"
#include "../detail/immovable_vector.h"
#include "../detail/iterator_traits.h"
#include "../detail/pdqsort.h"

namespace cppsort::probe
{
    namespace detail
    {
        template<typename ForwardIterator, typename Compare, typename Projection>
        auto ham_probe_algo(ForwardIterator first, ForwardIterator last,
                            cppsort::detail::difference_type_t<ForwardIterator> size,
                            Compare compare, Projection projection)
            -> ::cppsort::detail::difference_type_t<ForwardIterator>
        {
            using difference_type = ::cppsort::detail::difference_type_t<ForwardIterator>;
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
                iterators.begin(), iterators.end(), compare,
                cppsort::detail::indirect(projection)
            );

            ////////////////////////////////////////////////////////////
            // Count the number of values not in place

            difference_type count = 0;
            for (auto&& it: iterators) {
                if (comp(proj(*first), proj(*it)) ||
                    comp(proj(*it), proj(*first))) {
                    ++count;
                }
                ++first;
            }
            return count;
        }

        struct ham_impl
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
                return ham_probe_algo(mstd::begin(range), mstd::end(range),
                                      mstd::distance(range),
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
                return ham_probe_algo(first, last,
                                      std::distance(first, last),
                                      std::move(compare), std::move(projection));
            }

            template<typename Integer>
            static constexpr auto max_for_size(Integer n)
                -> Integer
            {
                return n == 0 ? 0 : n;
            }
        };
    }

    inline constexpr sorter_facade<detail::ham_impl> ham{};
}

#endif // CPPSORT_PROBES_HAM_H_
