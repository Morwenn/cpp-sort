/*
 * Copyright (c) 2016-2021 Morwenn
 * SPDX-License-Identifier: MIT
 */
#ifndef CPPSORT_DETAIL_CONTAINER_AWARE_INSERTION_SORT_H_
#define CPPSORT_DETAIL_CONTAINER_AWARE_INSERTION_SORT_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <forward_list>
#include <functional>
#include <iterator>
#include <list>
#include <type_traits>
#include <utility>
#include <cpp-sort/fwd.h>
#include <cpp-sort/sorter_facade.h>
#include <cpp-sort/sorter_traits.h>
#include <cpp-sort/utility/as_function.h>
#include <cpp-sort/utility/functional.h>
#include "../bitops.h"
#include "../type_traits.h"
#include "../upper_bound.h"

namespace cppsort
{
    namespace detail
    {
        template<typename Compare, typename Projection, typename... Args>
        auto list_insertion_sort(std::list<Args...>& collection,
                                 Compare compare, Projection projection)
            -> void
        {
            auto&& proj = utility::as_function(projection);

            auto it = collection.begin();
            auto last = collection.end();
            if (it == last) return;

            // Size of the list where a value is inserted
            typename std::list<Args...>::difference_type size = 1;

            ++it;
            while (it != last) {
                auto insertion_point = upper_bound_n(collection.begin(), size, proj(*it),
                                                     compare, projection);
                if (insertion_point == it) {
                    ++it;
                } else {
                    auto next = std::next(it);
                    collection.splice(insertion_point, collection, it);
                    it = next;
                }
                ++size;
            }
        }

        template<typename Compare, typename Projection, typename... Args>
        auto flist_insertion_sort(std::forward_list<Args...>& collection,
                                  Compare compare, Projection projection)
            -> void
        {
            auto&& comp = utility::as_function(compare);
            auto&& proj = utility::as_function(projection);

            auto it = collection.before_begin();
            auto last = collection.end();
            if (++it == last) return;

            // Size of the list where a value is inserted
            typename std::forward_list<Args...>::difference_type size = 1;

            while (std::next(it) != last) {
                // Binary search to find where to insert the value, we can't
                // use upper_bound because of the specificities of forward_list
                auto&& value = proj(*std::next(it));
                auto insertion_point = collection.before_begin();
                for (auto search_size = size ; search_size > 0 ;) {
                    auto search_it = std::next(insertion_point, half(search_size));
                    if (not comp(value, proj(*std::next(search_it)))) {
                        insertion_point = ++search_it;
                        search_size -= half(search_size) + 1;
                    } else {
                        search_size = half(search_size);
                    }
                }

                if (insertion_point == it) {
                    ++it;
                } else {
                    // We splice the elements after it to some position before it,
                    // so this effectively advances the iterator towards last
                    collection.splice_after(insertion_point, collection, it);
                }
                ++size;
            }
        }
    }

    template<>
    struct container_aware_adapter<insertion_sorter>:
        detail::container_aware_adapter_base<insertion_sorter>,
        detail::sorter_facade_fptr<
            container_aware_adapter<insertion_sorter>,
            std::is_empty<insertion_sorter>::value
        >
    {
        container_aware_adapter() = default;
        constexpr explicit container_aware_adapter(insertion_sorter) noexcept {}

        ////////////////////////////////////////////////////////////
        // std::list

        template<typename... Args>
        auto operator()(std::list<Args...>& iterable) const
            -> void
        {
            detail::list_insertion_sort(iterable, std::less<>{}, utility::identity{});
        }

        template<typename Compare, typename... Args>
        auto operator()(std::list<Args...>& iterable, Compare compare) const
            -> detail::enable_if_t<
                is_projection_v<utility::identity, std::list<Args...>, Compare>
            >
        {
            detail::list_insertion_sort(iterable, std::move(compare), utility::identity{});
        }

        template<typename Projection, typename... Args>
        auto operator()(std::list<Args...>& iterable, Projection projection) const
            -> detail::enable_if_t<
                is_projection_v<Projection, std::list<Args...>>
            >
        {
            detail::list_insertion_sort(iterable, std::less<>{}, std::move(projection));
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
            detail::list_insertion_sort(iterable, std::move(compare), std::move(projection));
        }

        ////////////////////////////////////////////////////////////
        // std::forward_list

        template<typename... Args>
        auto operator()(std::forward_list<Args...>& iterable) const
            -> void
        {
            detail::flist_insertion_sort(iterable, std::less<>{}, utility::identity{});
        }

        template<typename Compare, typename... Args>
        auto operator()(std::forward_list<Args...>& iterable, Compare compare) const
            -> detail::enable_if_t<
                is_projection_v<utility::identity, std::forward_list<Args...>, Compare>
            >
        {
            detail::flist_insertion_sort(iterable, std::move(compare), utility::identity{});
        }

        template<typename Projection, typename... Args>
        auto operator()(std::forward_list<Args...>& iterable, Projection projection) const
            -> detail::enable_if_t<
                is_projection_v<Projection, std::forward_list<Args...>>
            >
        {
            detail::flist_insertion_sort(iterable, std::less<>{}, std::move(projection));
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
            detail::flist_insertion_sort(iterable, std::move(compare), std::move(projection));
        }

        ////////////////////////////////////////////////////////////
        // Generic overload

        template<
            typename First, typename... Args,
            typename = detail::enable_if_t<
                not detail::is_specialization_of_v<detail::remove_cvref_t<First>, std::list> &&
                not detail::is_specialization_of_v<detail::remove_cvref_t<First>, std::forward_list>
            >
        >
        auto operator()(First&& first, Args&&... args) const
            -> decltype(detail::container_aware_adapter_base<insertion_sorter>::operator()(
                   std::forward<First>(first), std::forward<Args>(args)...
            ))
        {
            return detail::container_aware_adapter_base<insertion_sorter>::operator()(
                std::forward<First>(first), std::forward<Args>(args)...
            );
        }
    };
}

#endif // CPPSORT_DETAIL_CONTAINER_AWARE_INSERTION_SORT_H_
