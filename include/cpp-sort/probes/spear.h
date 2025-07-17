/*
 * Copyright (c) 2025 Morwenn
 * SPDX-License-Identifier: MIT
 */
#ifndef CPPSORT_PROBES_SPEAR_H_
#define CPPSORT_PROBES_SPEAR_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <functional>
#include <iterator>
#include <utility>
#include <cpp-sort/sorter_facade.h>
#include <cpp-sort/sorter_traits.h>
#include <cpp-sort/utility/functional.h>
#include <cpp-sort/utility/size.h>
#include "../detail/immovable_vector.h"
#include "../detail/iterator_traits.h"
#include "../detail/spinsort.h"
#include "../detail/type_traits.h"

namespace cppsort
{
namespace probe
{
    namespace detail
    {
        template<typename ForwardIterator, typename Compare, typename Projection>
        auto spear_probe_algo(ForwardIterator first, ForwardIterator last,
                              cppsort::detail::difference_type_t<ForwardIterator> size,
                              Compare compare, Projection projection)
            -> ::cppsort::detail::difference_type_t<ForwardIterator>
        {
            using difference_type = ::cppsort::detail::difference_type_t<ForwardIterator>;

            if (size < 2) {
                return 0;
            }

            ////////////////////////////////////////////////////////////
            // Indirectly sort the collection

            // Copy the iterators and their original index in a vector
            cppsort::detail::immovable_vector<
                std::pair<ForwardIterator, difference_type>
            > iter_idx(size);
            difference_type index = 0;
            for (auto it = first; it != last; ++it) {
                iter_idx.emplace_back(it, index);
                ++index;
            }

            // Sort the iterator/index pairs on pointed values
            // Note: we use a stable sort to ensure that we don't end up with
            //       to big values for equivalent elements. For example given
            //       the sequence (5, 1, 2, 8, 9, 5), we want the first 5 to
            //       have the sorted position 2, and the second 5 to have the
            //       sorted position 3. Otherwise we might get 10 as a result
            //       instead of the expected 8, and it will depend on the used
            //       unstable sort implementation.
            cppsort::detail::spinsort(
                iter_idx.begin(), iter_idx.end(),
                std::move(compare),
                &std::pair<ForwardIterator, difference_type>::first
                    | utility::indirect{}
                    | std::move(projection)
            );

            ////////////////////////////////////////////////////////////
            // Sum of distances between elements and their sorted
            // positions

            difference_type sum_dist = 0;
            difference_type current_pos = 0;
            for (auto const& it_idx: iter_idx) {
                auto original_pos = it_idx.second;
                if (original_pos < current_pos) {
                    sum_dist += current_pos - original_pos;
                } else {
                    sum_dist += original_pos - current_pos;
                }
                ++current_pos;
            }
            return sum_dist;
        }

        struct spear_impl
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
                return spear_probe_algo(std::begin(iterable), std::end(iterable),
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
                auto dist = std::distance(first, last);
                return spear_probe_algo(std::move(first), std::move(last), dist,
                                        std::move(compare), std::move(projection));
            }

            template<typename Integer>
            static constexpr auto max_for_size(Integer n)
                -> Integer
            {
                return n * n / 2;
            }
        };
    }

    inline constexpr sorter_facade<detail::spear_impl> spear{};
}}

#endif // CPPSORT_PROBES_SPEAR_H_
