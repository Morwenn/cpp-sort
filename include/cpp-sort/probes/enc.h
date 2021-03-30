/*
 * Copyright (c) 2016-2021 Morwenn
 * SPDX-License-Identifier: MIT
 */
#ifndef CPPSORT_PROBES_ENC_H_
#define CPPSORT_PROBES_ENC_H_

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
#include <cpp-sort/utility/static_const.h>
#include "../detail/functional.h"
#include "../detail/iterator_traits.h"
#include "../detail/lower_bound.h"

namespace cppsort
{
namespace probe
{
    namespace detail
    {
        struct enc_impl
        {
            template<
                typename ForwardIterator,
                typename Compare = std::less<>,
                typename Projection = utility::identity,
                typename = std::enable_if_t<
                    is_projection_iterator_v<Projection, ForwardIterator, Compare>
                >
            >
            auto operator()(ForwardIterator first, ForwardIterator last,
                            Compare compare={}, Projection projection={}) const
                -> cppsort::detail::difference_type_t<ForwardIterator>
            {
                auto&& comp = utility::as_function(compare);
                auto&& proj = utility::as_function(projection);

                if (first == last || std::next(first) == last) {
                    return 0;
                }

                // Heads an tails of encroaching lists
                std::vector<std::pair<ForwardIterator, ForwardIterator>> lists;
                lists.emplace_back(first, first);
                ++first;

                ////////////////////////////////////////////////////////////
                // Create encroaching lists

                while (first != last) {
                    auto&& value = proj(*first);

                    auto& last_list = lists.back();
                    if (not comp(value, proj(*last_list.second))) {
                        // Element belongs to the tails (bigger elements)
                        auto insertion_point = cppsort::detail::lower_bound(
                            lists.begin(), std::prev(lists.end()), value,
                            cppsort::detail::invert(compare),
                            [&proj](auto& list) -> decltype(auto) { return proj(*list.second); }
                        );
                        insertion_point->second = first;
                    } else if (not comp(proj(*last_list.first), value)) {
                        // Element belongs to the heads (smaller elements)
                        auto insertion_point = cppsort::detail::lower_bound(
                            lists.begin(), std::prev(lists.end()), value, compare,
                            [&proj](auto& list) -> decltype(auto) { return proj(*list.first); }
                        );
                        insertion_point->first = first;
                    } else {
                        // Element does not belong to the existing encroaching lists,
                        // create a new list for it
                        lists.emplace_back(first, first);
                    }

                    ++first;
                }

                return lists.size() - 1;
            }

            template<typename Integer>
            static constexpr auto max_for_size(Integer n)
                -> Integer
            {
                return n == 0 ? 0 : (n + 1) / 2 - 1;
            }
        };
    }

    namespace
    {
        constexpr auto&& enc = utility::static_const<
            sorter_facade<detail::enc_impl>
        >::value;
    }
}}

#endif // CPPSORT_PROBES_ENC_H_
