/*
 * Copyright (c) 2021 Morwenn
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
#include <cpp-sort/comparators/projection_compare.h>
#include <cpp-sort/fwd.h>
#include <cpp-sort/sorter_facade.h>
#include <cpp-sort/sorter_traits.h>
#include <cpp-sort/utility/functional.h>
#include <cpp-sort/utility/size.h>

namespace cppsort
{
    namespace detail
    {
        template<typename Compare, typename Projection, typename... Args>
        auto list_mel_sort(std::list<Args...>& collection, Compare compare, Projection projection)
            -> void
        {
            auto&& comp = utility::as_function(compare);
            auto&& proj = utility::as_function(projection);

            if (collection.size() < 2) return;

            // Encroaching lists
            std::vector<std::list<Args...>> lists;

            ////////////////////////////////////////////////////////////
            // Create encroaching lists

            while (not collection.empty()) {
                auto&& value = proj(collection.front());

                // Binary search for an encroaching list where
                // value >= list.second or value <= list.first

                // Whether the found value is smaller than the head
                // of the found encroaching list or greater than its
                // tail
                bool value_is_smaller = false;

                auto size = lists.size();
                auto enc_it = lists.begin();
                while (size > 0) {
                    auto tmp_it = enc_it;
                    std::advance(tmp_it, size / 2);
                    if (not comp(value, proj(tmp_it->back()))) {
                        size /= 2;
                        value_is_smaller = false;
                    } else if (not comp(proj(tmp_it->front()), value)) {
                        size /= 2;
                        value_is_smaller = true;
                    } else {
                        enc_it = ++tmp_it;
                        size -= size / 2 + 1;
                    }
                }

                if (enc_it != lists.end()) {
                    // Add the new element to the head or tail or the most
                    // suitable encroaching list if any has been found
                    if (value_is_smaller) {
                        enc_it->splice(enc_it->begin(), collection, collection.begin());
                    } else {
                        enc_it->splice(enc_it->end(), collection, collection.begin());
                    }
                } else {
                    // Create a new encroaching list if the element
                    // didn't fit in any of the existing ones
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
            auto&& comp = utility::as_function(compare);
            auto&& proj = utility::as_function(projection);

            if (collection.begin() == collection.end() || std::next(collection.begin()) == collection.end()) {
                return;
            }

            // A list and its last element
            struct flist
            {
                using iterator = typename std::forward_list<Args...>::iterator;

                flist():
                    list(),
                    last(list.begin())
                {}

                std::forward_list<Args...> list;
                iterator last;
            };

            // Encroaching lists
            std::vector<flist> lists;

            ////////////////////////////////////////////////////////////
            // Create encroaching lists

            while (not collection.empty()) {
                auto&& value = proj(collection.front());

                // Binary search for an encroaching list where
                // value >= list.second or value <= list.first

                // Whether the found value is smaller than the head
                // of the found encroaching list or greater than its
                // tail
                bool value_is_smaller = false;

                auto size = lists.size();
                auto enc_it = lists.begin();
                while (size > 0) {
                    auto tmp_it = enc_it;
                    std::advance(tmp_it, size / 2);
                    if (not comp(value, proj(*tmp_it->last))) {
                        size /= 2;
                        value_is_smaller = false;
                    } else if (not comp(proj(tmp_it->list.front()), value)) {
                        size /= 2;
                        value_is_smaller = true;
                    } else {
                        enc_it = ++tmp_it;
                        size -= size / 2 + 1;
                    }
                }

                if (enc_it != lists.end()) {
                    // Add the new element to the head or tail or the most
                    // suitable encroaching list if any has been found
                    if (value_is_smaller) {
                        enc_it->list.splice_after(enc_it->list.before_begin(), collection, collection.before_begin());
                    } else {
                        enc_it->list.splice_after(enc_it->last, collection, collection.before_begin());
                        enc_it->last = std::next(enc_it->last);
                    }
                } else {
                    // Create a new encroaching list if the element
                    // didn't fit in any of the existing ones
                    lists.emplace_back();
                    lists.back().list.splice_after(lists.back().list.before_begin(), collection, collection.before_begin());
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
            -> std::enable_if_t<
                is_projection_v<utility::identity, std::list<Args...>, Compare>
            >
        {
            detail::list_mel_sort(iterable, std::move(compare), utility::identity{});
        }

        template<typename Projection, typename... Args>
        auto operator()(std::list<Args...>& iterable, Projection projection) const
            -> std::enable_if_t<
                is_projection_v<Projection, std::list<Args...>>
            >
        {
            detail::list_mel_sort(iterable, std::less<>{}, std::move(projection));
        }

        template<
            typename Compare,
            typename Projection,
            typename... Args,
            typename = std::enable_if_t<
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
            -> std::enable_if_t<
                is_projection_v<utility::identity, std::forward_list<Args...>, Compare>
            >
        {
            detail::flist_mel_sort(iterable, std::move(compare), utility::identity{});
        }

        template<typename Projection, typename... Args>
        auto operator()(std::forward_list<Args...>& iterable, Projection projection) const
            -> std::enable_if_t<
                is_projection_v<Projection, std::forward_list<Args...>>
            >
        {
            detail::flist_mel_sort(iterable, std::less<>{}, std::move(projection));
        }

        template<
            typename Compare,
            typename Projection,
            typename... Args,
            typename = std::enable_if_t<
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
