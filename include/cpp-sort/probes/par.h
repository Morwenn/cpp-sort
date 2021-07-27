/*
 * Copyright (c) 2016-2021 Morwenn
 * SPDX-License-Identifier: MIT
 */
#ifndef CPPSORT_PROBES_PAR_H_
#define CPPSORT_PROBES_PAR_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <algorithm>
#include <functional>
#include <iterator>
#include <new>
#include <type_traits>
#include <utility>
#include <vector>
#include <cpp-sort/sorter_facade.h>
#include <cpp-sort/sorter_traits.h>
#include <cpp-sort/utility/functional.h>
#include <cpp-sort/utility/size.h>
#include <cpp-sort/utility/static_const.h>
#include "../detail/is_p_sorted.h"
#include "../detail/iterator_traits.h"

namespace cppsort
{
namespace probe
{
    namespace detail
    {
        template<typename RandomAccessIterator, typename Compare, typename Projection>
        auto legacy_par_probe_algo(RandomAccessIterator first, RandomAccessIterator last,
                                   cppsort::detail::difference_type_t<RandomAccessIterator> size,
                                   Compare compare, Projection projection)
            -> ::cppsort::detail::difference_type_t<RandomAccessIterator>
        {
            auto res = 0;
            while (size > 0) {
                auto p = res;
                p += size / 2;
                if (cppsort::detail::is_p_sorted(first, last, p, compare, projection)) {
                    size /= 2;
                } else {
                    res = ++p;
                    size -= size / 2 + 1;
                }
            }
            return res;
        }

        template<typename RandomAccessIterator, typename Compare, typename Projection>
        auto new_par_probe_algo(RandomAccessIterator first, RandomAccessIterator last,
                            cppsort::detail::difference_type_t<RandomAccessIterator> size,
                            Compare compare, Projection projection)
            -> ::cppsort::detail::difference_type_t<RandomAccessIterator>
        {
            using difference_type = ::cppsort::detail::difference_type_t<RandomAccessIterator>;
            auto&& comp = utility::as_function(compare);
            auto&& proj = utility::as_function(projection);

            // Algorithm described in *Roughly Sorting: Sequential and Parallel Approach*
            // by T. Altman and Y. Igarashi

            if (size < 2) {
                return 0;
            }

            // Algorithm LR
            std::vector<RandomAccessIterator> b = { first };
            for (auto it = std::next(first) ; it != last ; ++it) {
                if (comp(proj(*b.back()), proj(*it))) {
                    b.push_back(it);
                } else {
                    b.push_back(b.back());
                }
            }

            // Algorithm RL
            std::vector<RandomAccessIterator> c = { std::prev(last) };
            auto rfirst = std::make_reverse_iterator(last);
            auto rlast = std::make_reverse_iterator(first);
            for (auto it = std::next(rfirst) ; it != rlast ; ++it) {
                if (comp(proj(*it), proj(*c.back()))) {
                    c.push_back(std::prev(it.base()));
                } else {
                    c.push_back(c.back());
                }
            }
            std::reverse(c.begin(), c.end());

            // Algorithm DM
            std::vector<difference_type> d = {};
            difference_type i = c.size();
            for (auto j = i ; j > 0 ; --j) {
                while (j <= i && i >= 1 && not comp(proj(*b[j - 1]), proj(*c[i - 1]))
                       && (j == 1 || not comp(proj(*c[i - 1]), proj(*b[j - 2])))) {
                    d.push_back(i - j);
                    --i;
                }
            }

            // Compute radius = max(dm)
            return *std::max_element(d.begin(), d.end());
        }

        template<typename RandomAccessIterator, typename Compare, typename Projection>
        auto par_probe_algo(RandomAccessIterator first, RandomAccessIterator last,
                            cppsort::detail::difference_type_t<RandomAccessIterator> size,
                            Compare compare, Projection projection,
                            std::random_access_iterator_tag)
            -> ::cppsort::detail::difference_type_t<RandomAccessIterator>
        {
            try {
                return new_par_probe_algo(first, last, size, compare, projection);
            } catch (std::bad_alloc&) {
                // Old O(n^2 log n) algorithm, kept to avoid a breaking
                // when no extra memory is available, might be removed
                // in the future
                return legacy_par_probe_algo(
                    first, last, last - first,
                    std::move(compare), std::move(projection)
                );
            }
        }

        template<typename BidirectionalIterator, typename Compare, typename Projection>
        auto par_probe_algo(BidirectionalIterator first, BidirectionalIterator last,
                            cppsort::detail::difference_type_t<BidirectionalIterator> size,
                            Compare compare, Projection projection,
                            std::bidirectional_iterator_tag)
            -> ::cppsort::detail::difference_type_t<BidirectionalIterator>
        {
            // The O(n^2 log n) fallback requires random-access iterators
            return new_par_probe_algo(first, last, size, compare, projection);
        }

        struct par_impl
        {
            template<
                typename BidirectionalIterable,
                typename Compare = std::less<>,
                typename Projection = utility::identity,
                typename = std::enable_if_t<
                    is_projection_v<Projection, BidirectionalIterable, Compare>
                >
            >
            auto operator()(BidirectionalIterable&& iterable,
                            Compare compare={}, Projection projection={}) const
                -> decltype(auto)
            {
                using category = cppsort::detail::iterator_category_t<
                    cppsort::detail::remove_cvref_t<decltype(std::begin(iterable))>
                >;
                return par_probe_algo(std::begin(iterable), std::end(iterable),
                                      utility::size(iterable),
                                      std::move(compare), std::move(projection),
                                      category{});
            }

            template<
                typename BidirectionalIterator,
                typename Compare = std::less<>,
                typename Projection = utility::identity,
                typename = std::enable_if_t<
                    is_projection_iterator_v<Projection, BidirectionalIterator, Compare>
                >
            >
            auto operator()(BidirectionalIterator first, BidirectionalIterator last,
                            Compare compare={}, Projection projection={}) const
                -> decltype(auto)
            {
                using category = cppsort::detail::iterator_category_t<BidirectionalIterator>;
                return par_probe_algo(first, last, std::distance(first, last),
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
        constexpr auto&& par = utility::static_const<
            sorter_facade<detail::par_impl>
        >::value;
    }
}}

#endif // CPPSORT_PROBES_PAR_H_
