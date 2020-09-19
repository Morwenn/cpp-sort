/*
 * Copyright (c) 2016-2020 Morwenn
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
#include "../detail/iterator_traits.h"

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

                // Head an tail of encroaching lists
                std::vector<std::pair<ForwardIterator, ForwardIterator>> lists;

                while (first != last) {
                    auto&& value = proj(*first);

                    // Binary search for an encroaching list where
                    // value <= list.first or value >= list.second

                    // Whether the found value is smaller than the head
                    // of the found encroaching list or greater than its
                    // tail
                    bool value_is_smaller = true;

                    auto size = lists.size();
                    auto res_it = lists.begin();
                    while (size > 0) {
                        auto it = res_it;
                        std::advance(it, size / 2);
                        if (not comp(proj(*it->first), value)) {
                            size /= 2;
                            value_is_smaller = true;
                        } else if (not comp(value, proj(*it->second))) {
                            size /= 2;
                            value_is_smaller = false;
                        } else {
                            res_it = ++it;
                            size -= size / 2 + 1;
                        }
                    }

                    if (res_it != lists.end()) {
                        // Change the head or tail of the found encroaching list
                        // if any has been found
                        if (value_is_smaller) {
                            res_it->first = first;
                        } else {
                            res_it->second = first;
                        }
                    } else {
                        // Create a new encroaching list if the element
                        // didn't fit in any of the existing ones
                        lists.emplace_back(first, first);
                    }

                    ++first;
                }

                return lists.size() ? lists.size() - 1 : 0;
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
