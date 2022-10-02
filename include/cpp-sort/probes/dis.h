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
#include <type_traits>
#include <utility>
#include <cpp-sort/sorter_facade.h>
#include <cpp-sort/sorter_traits.h>
#include <cpp-sort/utility/as_function.h>
#include <cpp-sort/utility/functional.h>
#include <cpp-sort/utility/size.h>
#include <cpp-sort/utility/static_const.h>
#include "../detail/immovable_vector.h"
#include "../detail/is_p_sorted.h"
#include "../detail/iterator_traits.h"
#include "../detail/type_traits.h"

namespace cppsort
{
namespace probe
{
    namespace detail
    {
        template<typename ForwardIterator, typename Compare, typename Projection>
        auto inplace_dis_probe_algo(ForwardIterator first, ForwardIterator last,
                                    cppsort::detail::difference_type_t<ForwardIterator> size,
                                    Compare compare, Projection projection)
            -> ::cppsort::detail::difference_type_t<ForwardIterator>
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
                                       cppsort::detail::difference_type_t<BidirectionalIterator> size,
                                       Compare compare, Projection projection)
            -> ::cppsort::detail::difference_type_t<BidirectionalIterator>
        {
            using difference_type = ::cppsort::detail::difference_type_t<BidirectionalIterator>;
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

        template<typename BidirectionalIterator, typename Compare, typename Projection>
        auto dis_probe_algo(BidirectionalIterator first, BidirectionalIterator last,
                            cppsort::detail::difference_type_t<BidirectionalIterator> size,
                            Compare compare, Projection projection,
                            std::bidirectional_iterator_tag)
            -> ::cppsort::detail::difference_type_t<BidirectionalIterator>
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

        template<typename ForwardIterator, typename Compare, typename Projection>
        auto dis_probe_algo(ForwardIterator first, ForwardIterator last,
                            cppsort::detail::difference_type_t<ForwardIterator> size,
                            Compare compare, Projection projection,
                            std::forward_iterator_tag)
            -> ::cppsort::detail::difference_type_t<ForwardIterator>
        {
            return inplace_dis_probe_algo(first, last, size, compare, projection);
        }

        struct dis_impl
        {
            template<
                typename ForwardIterable,
                typename Compare = std::less<>,
                typename Projection = utility::identity,
                typename = cppsort::detail::enable_if_t<
                    is_projection_v<Projection, ForwardIterable, Compare>
                >
            >
            auto operator()(ForwardIterable&& iterable, Compare compare={}, Projection projection={}) const
                -> decltype(auto)
            {
                using category = cppsort::detail::iterator_category_t<
                    cppsort::detail::remove_cvref_t<decltype(std::begin(iterable))>
                >;
                return dis_probe_algo(std::begin(iterable), std::end(iterable),
                                      utility::size(iterable),
                                      std::move(compare), std::move(projection),
                                      category{});
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
                using category = cppsort::detail::iterator_category_t<ForwardIterator>;
                return dis_probe_algo(first, last, std::distance(first, last),
                                      std::move(compare), std::move(projection),
                                      category{});
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
        constexpr auto&& dis = utility::static_const<
            sorter_facade<detail::dis_impl>
        >::value;
    }
}}

#endif // CPPSORT_PROBES_DIS_H_
