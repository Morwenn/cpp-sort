/*
 * Copyright (c) 2016-2020 Morwenn
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
#include "../std_list_traits.h"
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

            ++it;
            while (it != last)
            {
                auto insertion_point = upper_bound(collection.begin(), it, proj(*it),
                                                   compare, projection);
                if (insertion_point == it)
                {
                    ++it;
                }
                else
                {
                    auto next = std::next(it);
                    collection.splice(insertion_point, collection, it);
                    it = next;
                }
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
            if (std::next(it) == last) return;

            ++it;
            while (std::next(it) != last)
            {
                // Linear search in list
                auto&& value = proj(*std::next(it));
                auto insertion_point = collection.before_begin();
                while (std::next(insertion_point) != last)
                {
                    if (not comp(proj(*std::next(insertion_point)), value)) break;
                    ++insertion_point;
                }

                if (insertion_point == it)
                {
                    ++it;
                }
                else
                {
                    auto next = std::next(it);
                    collection.splice_after(insertion_point, collection, it);
                    it = next;
                }
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
            -> std::enable_if_t<
                is_projection_v<utility::identity, std::list<Args...>, Compare>
            >
        {
            detail::list_insertion_sort(iterable, std::move(compare), utility::identity{});
        }

        template<typename Projection, typename... Args>
        auto operator()(std::list<Args...>& iterable, Projection projection) const
            -> std::enable_if_t<
                is_projection_v<Projection, std::list<Args...>>
            >
        {
            detail::list_insertion_sort(iterable, std::less<>{}, std::move(projection));
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
            -> std::enable_if_t<
                is_projection_v<utility::identity, std::forward_list<Args...>, Compare>
            >
        {
            detail::flist_insertion_sort(iterable, std::move(compare), utility::identity{});
        }

        template<typename Projection, typename... Args>
        auto operator()(std::forward_list<Args...>& iterable, Projection projection) const
            -> std::enable_if_t<
                is_projection_v<Projection, std::forward_list<Args...>>
            >
        {
            detail::flist_insertion_sort(iterable, std::less<>{}, std::move(projection));
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
            detail::flist_insertion_sort(iterable, std::move(compare), std::move(projection));
        }

        ////////////////////////////////////////////////////////////
        // Generic overload

        template<
            typename First, typename... Args,
            typename = std::enable_if_t<
                not detail::is_std_list<detail::remove_cvref_t<First>>::value &&
                not detail::is_std_forward_list<detail::remove_cvref_t<First>>::value
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
