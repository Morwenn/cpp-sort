/*
 * Copyright (c) 2021 Morwenn
 * SPDX-License-Identifier: MIT
 */
#ifndef CPPSORT_PROBES_BLOCK_H_
#define CPPSORT_PROBES_BLOCK_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <functional>
#include <iterator>
#include <type_traits>
#include <utility>
#include <vector>
#include <cpp-sort/sorter_facade.h>
#include <cpp-sort/sorter_traits.h>
#include <cpp-sort/utility/as_function.h>
#include <cpp-sort/utility/functional.h>
#include <cpp-sort/utility/size.h>
#include <cpp-sort/utility/static_const.h>
#include "../detail/functional.h"
#include "../detail/immovable_vector.h"
#include "../detail/iterator_traits.h"
#include "../detail/pdqsort.h"
#include "../detail/type_traits.h"

namespace cppsort
{
namespace probe
{
    namespace detail
    {
        template<typename ForwardIterator, typename Compare, typename Projection>
        auto block_probe_algo(ForwardIterator first, ForwardIterator last,
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
            // Count the number of consecutive pairs in the original
            // collection that can't be found in the sorted one

            difference_type count = 0;
            auto it_last_1 = std::prev(iterators.end());
            for (auto it = iterators.begin(); it != it_last_1; ++it) {
                auto orig_next = std::next(*it);
                if (orig_next == last) {
                    if (comp(proj(**it), proj(**it_last_1)) || comp(proj(**it_last_1), proj(**it))) {
                        ++count;
                    }
                    continue;
                }
                auto sorted_next = *std::next(it);
                if (comp(proj(*orig_next), proj(*sorted_next)) || comp(proj(*sorted_next), proj(*orig_next))) {
                    ++count;
                }
            }
            return count;
        }

        struct block_impl
        {
            template<
                typename ForwardIterable,
                typename Compare = std::less<>,
                typename Projection = utility::identity,
                typename = cppsort::detail::enable_if_t<
                    is_projection_v<Projection, ForwardIterable, Compare>
                >
            >
            auto operator()(ForwardIterable&& iterable,
                            Compare compare={}, Projection projection={}) const
                -> decltype(auto)
            {
                return block_probe_algo(std::begin(iterable), std::end(iterable),
                                        utility::size(iterable),
                                        std::move(compare), std::move(projection));
            }

            template<
                typename ForwardIterator,
                typename Compare = std::less<>,
                typename Projection = utility::identity,
                typename = cppsort::detail::enable_if_t<
                    is_projection_iterator_v<Projection, ForwardIterator, Compare>
                >
            >
            auto operator()(ForwardIterator first, ForwardIterator last,
                            Compare compare={}, Projection projection={}) const
                -> decltype(auto)
            {
                return block_probe_algo(first, last, std::distance(first, last),
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

    namespace
    {
        constexpr auto&& block = utility::static_const<
            sorter_facade<detail::block_impl>
        >::value;
    }
}}

#endif // CPPSORT_PROBES_BLOCK_H_
