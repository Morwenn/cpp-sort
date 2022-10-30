/*
 * Copyright (c) 2016-2022 Morwenn
 * SPDX-License-Identifier: MIT
 */
#ifndef CPPSORT_PROBES_DIS_H_
#define CPPSORT_PROBES_DIS_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <algorithm>
#include <functional>
#include <iterator>
#include <new>
#include <utility>
#include <cpp-sort/mstd/iterator.h>
#include <cpp-sort/mstd/ranges.h>
#include <cpp-sort/mstd/type_traits.h>
#include <cpp-sort/sorter_facade.h>
#include <cpp-sort/sorter_traits.h>
#include "../detail/immovable_vector.h"
#include "../detail/is_p_sorted.h"

namespace cppsort::probe
{
    namespace detail
    {
        template<typename ForwardIterator, typename Sentinel, typename Compare, typename Projection>
        auto inplace_dis_probe_algo(ForwardIterator first, Sentinel last,
                                    mstd::iter_difference_t<ForwardIterator> size,
                                    Compare compare, Projection projection)
            -> mstd::iter_difference_t<ForwardIterator>
        {
            // Simple algorithm in O(n log n) time and O(1) space

            auto res = 0;
            auto res_it = first;
            while (size > 0) {
                auto p = res;
                auto pth = res_it;
                p += size / 2;
                std::advance(pth, size / 2);
                if (cppsort::detail::is_p_sorted(first, last, pth, compare, projection)) {
                    size /= 2;
                } else {
                    res = ++p;
                    res_it = ++pth;
                    size -= size / 2 + 1;
                }
            }
            return res;
        }

        template<typename BidirectionalIterator, typename Compare, typename Projection>
        auto allocating_dis_probe_algo(BidirectionalIterator first, BidirectionalIterator last,
                                       mstd::iter_difference_t<BidirectionalIterator> size,
                                       Compare compare, Projection projection)
            -> mstd::iter_difference_t<BidirectionalIterator>
        {
            using difference_type = mstd::iter_difference_t<BidirectionalIterator>;
            auto&& comp = utility::as_function(compare);
            auto&& proj = utility::as_function(projection);

            // Space-optimized version of the algorithm described in *Roughly Sorting:
            // Sequential and Parallel Approach* by T. Altman and Y. Igarashi

            if (size < 2) {
                return 0;
            }

            // Algorithm LR: cumulative max from left to right
            cppsort::detail::immovable_vector<BidirectionalIterator> lr_cummax(size);
            lr_cummax.emplace_back(first);
            for (auto it = std::next(first) ; it != last ; ++it) {
                if (comp(proj(*lr_cummax.back()), proj(*it))) {
                    lr_cummax.emplace_back(it);
                } else {
                    lr_cummax.emplace_back(lr_cummax.back());
                }
            }

            // Merged algorithms without extra storage:
            // - RL: cumulative min from right to left
            // - DM: max distance of an inversion
            difference_type res = 0;
            difference_type i = size;
            auto rl_it = std::prev(last); // Iterator to the current RL element
            auto rl_min_it = rl_it; // Iterator to the current minimum of RL
            for (auto j = i ; j > 0 ; --j) {
                while (j <= i && not comp(proj(*lr_cummax[j - 1]), proj(*rl_min_it))
                       && (j == 1 || not comp(proj(*rl_min_it), proj(*lr_cummax[j - 2])))) {
                    // Compute the next value of DM
                    res = std::max(res, i - j);
                    // Compute the next value of RL
                    if (--i <= res) {
                        return res;
                    }
                    --rl_it;
                    if (comp(proj(*rl_it), proj(*rl_min_it))) {
                        rl_min_it = rl_it;
                    }
                }
            }

            return res;
        }

        template<
            mstd::bidirectional_iterator Iterator,
            typename Compare,
            typename Projection
        >
        auto dis_probe_algo(Iterator first, Iterator last,
                            mstd::iter_difference_t<Iterator> size,
                            Compare compare, Projection projection)
            -> mstd::iter_difference_t<Iterator>
        {
            try {
                return allocating_dis_probe_algo(first, last, size, compare, projection);
            } catch (std::bad_alloc&) {
                return inplace_dis_probe_algo(
                    first, last, size,
                    std::move(compare), std::move(projection)
                );
            }
        }

        template<
            mstd::forward_iterator Iterator,
            mstd::sentinel_for<Iterator> Sentinel,
            typename Compare,
            typename Projection
        >
        auto dis_probe_algo(Iterator first, Sentinel last,
                            mstd::iter_difference_t<Iterator> size,
                            Compare compare, Projection projection)
            -> mstd::iter_difference_t<Iterator>
        {
            return inplace_dis_probe_algo(first, last, size, compare, projection);
        }

        struct dis_impl
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
                return dis_probe_algo(mstd::begin(range), mstd::end(range),
                                      mstd::distance(range),
                                      std::move(compare), std::move(projection));
            }

            template<
                mstd::forward_iterator Iterator,
                mstd::sentinel_for<Iterator> Sentinel,
                typename Compare = std::less<>,
                typename Projection = std::identity,
                typename = mstd::enable_if_t<
                    is_projection_iterator_v<Projection, Iterator, Compare>
                >
            >
            auto operator()(Iterator first, Sentinel last,
                            Compare compare={}, Projection projection={}) const
                -> decltype(auto)
            {
                return dis_probe_algo(first, last,
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

    inline constexpr sorter_facade<detail::dis_impl> dis{};
}

#endif // CPPSORT_PROBES_DIS_H_
