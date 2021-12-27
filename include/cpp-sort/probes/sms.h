/*
 * Copyright (c) 2021 Morwenn
 * SPDX-License-Identifier: MIT
 */
#ifndef CPPSORT_PROBES_SMS_H_
#define CPPSORT_PROBES_SMS_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <cmath>
#include <functional>
#include <iterator>
#include <utility>
#include <cpp-sort/sorter_facade.h>
#include <cpp-sort/sorter_traits.h>
#include <cpp-sort/utility/as_function.h>
#include <cpp-sort/utility/functional.h>
#include <cpp-sort/utility/size.h>
#include <cpp-sort/utility/static_const.h>
#include "../detail/functional.h"
#include "../detail/immovable_vector.h"
#include "../detail/iterator_traits.h"
#include "../detail/longest_ascending_subsequence.h"
#include "../detail/type_traits.h"

namespace cppsort
{
namespace probe
{
    namespace detail
    {
        template<typename ForwardIterator, typename Compare, typename Projection>
        auto sms_probe_algo(ForwardIterator first, ForwardIterator last,
                            cppsort::detail::difference_type_t<ForwardIterator> size,
                            Compare compare, Projection projection)
            -> ::cppsort::detail::difference_type_t<ForwardIterator>
        {
            if (size < 2) {
                return 0;
            }

            // Copy the iterators in a vector
            cppsort::detail::immovable_vector<ForwardIterator> iterators(size);
            for (auto it = first; it != last; ++it) {
                iterators.emplace_back(it);
            }

            ::cppsort::detail::difference_type_t<ForwardIterator> res = 0;
            do {
                ++res;

                auto lis = longest_ascending_subsequence(
                    iterators.begin(), iterators.end(),
                    compare, // TODO: ok?
                    cppsort::detail::indirect(projection)
                );
                auto lds = longest_ascending_subsequence(
                    iterators.begin(), iterators.end(),
                    cppsort::detail::invert(compare), // TODO: ok?
                    cppsort::detail::indirect(projection)
                );

                // Remove the elements from the main iterators collection corresponding
                // to either the longest non-ascending subsequence or to the longest
                // non-descending subsequence, whichever is the bigger of the two
                auto longer_it = lds.size() > lis.size() ? lds.begin() : lis.begin();
                auto out = iterators.begin();
                for (auto it = iterators.begin(); it != iterators.end(); ++it) {
                    if (it == *longer_it) {
                        ++longer_it;
                    } else {
                        if (it != out) {
                            *out = std::move(*it);
                        }
                        ++out;
                    }
                }

                iterators.resize(out - iterators.begin());
            } while (iterators.size() > 0);

            return res - 1;
        }

        struct sms_impl
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
                return sms_probe_algo(std::begin(iterable), std::end(iterable),
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
                return sms_probe_algo(first, last, std::distance(first, last),
                                      std::move(compare), std::move(projection));
            }

            template<typename Integer>
            static constexpr auto max_for_size(Integer n)
                -> Integer
            {
                return static_cast<Integer>(std::sqrt(n)); // TODO
            }
        };
    }

    namespace
    {
        constexpr auto&& sms = utility::static_const<
            sorter_facade<detail::sms_impl>
        >::value;
    }
}}

#endif // CPPSORT_PROBES_SMS_H_
