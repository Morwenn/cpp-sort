/*
 * Copyright (c) 2021-2022 Morwenn
 * SPDX-License-Identifier: MIT
 */
#ifndef CPPSORT_DETAIL_CONTAINER_AWARE_MEL_SORT_H_
#define CPPSORT_DETAIL_CONTAINER_AWARE_MEL_SORT_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <forward_list>
#include <functional>
#include <iterator>
#include <list>
#include <type_traits>
#include <utility>
#include <vector>
#include <cpp-sort/comparators/flip.h>
#include <cpp-sort/comparators/projection_compare.h>
#include <cpp-sort/fwd.h>
#include <cpp-sort/sorter_facade.h>
#include <cpp-sort/sorter_traits.h>
#include <cpp-sort/utility/as_function.h>
#include <cpp-sort/utility/functional.h>
#include <cpp-sort/utility/size.h>
#include "../lower_bound.h"
#include "../type_traits.h"

namespace cppsort
{
    namespace detail
    {
        template<typename Compare, typename Projection, typename... Args>
        auto list_mel_sort(std::list<Args...>& collection, Compare compare, Projection projection)
            -> void
        {
            // NOTE: see detail/melsort.h for detailed explanations
            auto&& comp = utility::as_function(compare);
            auto&& proj = utility::as_function(projection);

            if (collection.size() < 2) return;

            // Encroaching lists
            std::vector<std::list<Args...>> lists;
            lists.emplace_back();
            lists.back().splice(lists.back().begin(), collection, collection.begin());

            ////////////////////////////////////////////////////////////
            // Create encroaching lists

            while (not collection.empty()) {
                auto&& value = proj(collection.front());

                auto& last_list = lists.back();
                if (not comp(value, proj(last_list.back()))) {
                    // Element belongs to the tails (bigger elements)
                    auto insertion_point = detail::lower_bound(
                        lists.begin(), std::prev(lists.end()), value, cppsort::flip(compare),
                        [&proj](auto& list) -> decltype(auto) { return proj(list.back()); }
                    );
                    insertion_point->splice(insertion_point->end(), collection, collection.begin());
                } else if (not comp(proj(last_list.front()), value)) {
                    // Element belongs to the heads (smaller elements)
                    auto insertion_point = detail::lower_bound(
                        lists.begin(), std::prev(lists.end()), value, compare,
                        [&proj](auto& list) -> decltype(auto) { return proj(list.front()); }
                    );
                    insertion_point->splice(insertion_point->begin(), collection, collection.begin());
                } else {
                    // Element does not belong to the existing encroaching lists,
                    // create a new list for it
                    lists.emplace_back();
                    lists.back().splice(lists.back().begin(), collection, collection.begin());
                }
            }

            ////////////////////////////////////////////////////////////
            // Merge encroaching lists

            while (lists.size() > 1) {
                if (lists.size() % 2 != 0) {
                    auto last_it = std::prev(lists.end());
                    auto last_1_it = std::prev(last_it);
                    last_1_it->merge(*last_it, make_projection_compare(comp, proj));
                    lists.pop_back();
                }

                auto first_it = lists.begin();
                auto half_it = first_it + lists.size() / 2;
                while (half_it != lists.end()) {
                    first_it->merge(*half_it, make_projection_compare(comp, proj));
                    ++first_it;
                    ++half_it;
                }

                lists.erase(lists.begin() + lists.size() / 2, lists.end());
            }

            // Merge lists back into the original collection
            collection.splice(collection.begin(), std::move(lists.front()));
        }

        template<typename Compare, typename Projection, typename... Args>
        auto flist_mel_sort(std::forward_list<Args...>& collection, Compare compare, Projection projection)
            -> void
        {
            // NOTE: see detail/melsort.h for detailed explanations
            auto&& comp = utility::as_function(compare);
            auto&& proj = utility::as_function(projection);

            if (collection.begin() == collection.end() || std::next(collection.begin()) == collection.end()) {
                return;
            }

            // A list and its last element, the latter being required
            // to implement melsort for forward iterators
            struct flist
            {
                flist():
                    list(),
                    last(list.begin())
                {}

                std::forward_list<Args...> list;
                typename std::forward_list<Args...>::iterator last;
            };

            // Encroaching lists
            std::vector<flist> lists;
            lists.emplace_back();
            lists.back().list.splice_after(lists.back().list.before_begin(),
                                           collection, collection.before_begin());
            lists.back().last = lists.back().list.begin();

            ////////////////////////////////////////////////////////////
            // Create encroaching lists

            while (not collection.empty()) {
                auto&& value = proj(collection.front());

                auto& last_list = lists.back();
                if (not comp(value, proj(*last_list.last))) {
                    // Element belongs to the tails (bigger elements)
                    auto insertion_point = detail::lower_bound(
                        lists.begin(), std::prev(lists.end()), value, cppsort::flip(compare),
                        [&proj](auto& list) -> decltype(auto) { return proj(*list.last); }
                    );
                    insertion_point->list.splice_after(insertion_point->last, collection,
                                                       collection.before_begin());
                    insertion_point->last = std::next(insertion_point->last);
                } else if (not comp(proj(last_list.list.front()), value)) {
                    // Element belongs to the heads (smaller elements)
                    auto insertion_point = detail::lower_bound(
                        lists.begin(), std::prev(lists.end()), value, compare,
                        [&proj](auto& list) -> decltype(auto) { return proj(list.list.front()); }
                    );
                    insertion_point->list.splice_after(insertion_point->list.before_begin(),
                                                       collection, collection.before_begin());
                } else {
                    // Element does not belong to the existing encroaching lists,
                    // create a new list for it
                    lists.emplace_back();
                    lists.back().list.splice_after(lists.back().list.before_begin(),
                                                   collection, collection.before_begin());
                    lists.back().last = lists.back().list.begin();
                }
            }

            ////////////////////////////////////////////////////////////
            // Merge encroaching lists

            while (lists.size() > 1) {
                if (lists.size() % 2 != 0) {
                    auto last_it = std::prev(lists.end());
                    auto last_1_it = std::prev(last_it);
                    last_1_it->list.merge(last_it->list, make_projection_compare(comp, proj));
                    lists.pop_back();
                }

                auto first_it = lists.begin();
                auto half_it = first_it + lists.size() / 2;
                while (half_it != lists.end()) {
                    first_it->list.merge(half_it->list, make_projection_compare(comp, proj));
                    ++first_it;
                    ++half_it;
                }

                lists.erase(lists.begin() + lists.size() / 2, lists.end());
            }

            // Merge lists back into the original collection
            collection.splice_after(collection.before_begin(), std::move(lists.front().list));
        }
    }

    template<>
    struct container_aware_adapter<mel_sorter>:
        detail::container_aware_adapter_base<mel_sorter>,
        detail::sorter_facade_fptr<
            container_aware_adapter<mel_sorter>,
            std::is_empty<mel_sorter>::value
        >
    {
        using detail::container_aware_adapter_base<mel_sorter>::operator();

        container_aware_adapter() = default;
        constexpr explicit container_aware_adapter(mel_sorter) noexcept {}

        ////////////////////////////////////////////////////////////
        // std::list

        template<typename... Args>
        auto operator()(std::list<Args...>& iterable) const
            -> void
        {
            detail::list_mel_sort(iterable, std::less<>{}, utility::identity{});
        }

        template<typename Compare, typename... Args>
        auto operator()(std::list<Args...>& iterable, Compare compare) const
            -> detail::enable_if_t<
                is_projection_v<utility::identity, std::list<Args...>, Compare>
            >
        {
            detail::list_mel_sort(iterable, std::move(compare), utility::identity{});
        }

        template<typename Projection, typename... Args>
        auto operator()(std::list<Args...>& iterable, Projection projection) const
            -> detail::enable_if_t<
                is_projection_v<Projection, std::list<Args...>>
            >
        {
            detail::list_mel_sort(iterable, std::less<>{}, std::move(projection));
        }

        template<
            typename Compare,
            typename Projection,
            typename... Args,
            typename = detail::enable_if_t<
                is_projection_v<Projection, std::list<Args...>, Compare>
            >
        >
        auto operator()(std::list<Args...>& iterable,
                        Compare compare, Projection projection) const
            -> void
        {
            detail::list_mel_sort(iterable, std::move(compare), std::move(projection));
        }

        ////////////////////////////////////////////////////////////
        // std::forward_list

        template<typename... Args>
        auto operator()(std::forward_list<Args...>& iterable) const
            -> void
        {
            detail::flist_mel_sort(iterable, std::less<>{}, utility::identity{});
        }

        template<typename Compare, typename... Args>
        auto operator()(std::forward_list<Args...>& iterable, Compare compare) const
            -> detail::enable_if_t<
                is_projection_v<utility::identity, std::forward_list<Args...>, Compare>
            >
        {
            detail::flist_mel_sort(iterable, std::move(compare), utility::identity{});
        }

        template<typename Projection, typename... Args>
        auto operator()(std::forward_list<Args...>& iterable, Projection projection) const
            -> detail::enable_if_t<
                is_projection_v<Projection, std::forward_list<Args...>>
            >
        {
            detail::flist_mel_sort(iterable, std::less<>{}, std::move(projection));
        }

        template<
            typename Compare,
            typename Projection,
            typename... Args,
            typename = detail::enable_if_t<
                is_projection_v<Projection, std::forward_list<Args...>, Compare>
            >
        >
        auto operator()(std::forward_list<Args...>& iterable,
                        Compare compare, Projection projection) const
            -> void
        {
            detail::flist_mel_sort(iterable, std::move(compare), std::move(projection));
        }
    };
}

#endif // CPPSORT_DETAIL_CONTAINER_AWARE_MEL_SORT_H_
