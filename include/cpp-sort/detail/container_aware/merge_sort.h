/*
 * Copyright (c) 2016-2021 Morwenn
 * SPDX-License-Identifier: MIT
 */
#ifndef CPPSORT_DETAIL_CONTAINER_AWARE_MERGE_SORT_H_
#define CPPSORT_DETAIL_CONTAINER_AWARE_MERGE_SORT_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <forward_list>
#include <functional>
#include <iterator>
#include <list>
#include <type_traits>
#include <utility>
#include <cpp-sort/comparators/projection_compare.h>
#include <cpp-sort/fwd.h>
#include <cpp-sort/sorter_facade.h>
#include <cpp-sort/sorter_traits.h>
#include <cpp-sort/utility/functional.h>
#include <cpp-sort/utility/size.h>
#include "../type_traits.h"

namespace cppsort
{
    namespace detail
    {
        template<typename Compare, typename Projection, typename... Args>
        auto list_merge_sort(std::list<Args...>& collection,
                             Compare compare, Projection projection)
            -> void
        {
            if (collection.size() < 2) return;

            // Extract left part
            auto first = collection.begin();
            auto middle = std::next(first, collection.size() / 2);
            std::list<Args...> left(collection.get_allocator());
            left.splice(left.begin(), collection, first, middle);

            // Recursively sort, then merge
            list_merge_sort(left, compare, projection);
            list_merge_sort(collection, compare, projection);
            collection.merge(std::move(left), make_projection_compare(std::move(compare),
                                                                      std::move(projection)));
        }

        template<typename Compare, typename Projection,
                 typename Size, typename... Args>
        auto flist_merge_sort(std::forward_list<Args...>& collection, Size size,
                              Compare compare, Projection projection)
            -> void
        {
            if (size < 2) return;

            // Extract left part
            auto first = collection.before_begin();
            auto middle = std::next(first, size / 2 + 1);
            std::forward_list<Args...> left(collection.get_allocator());
            left.splice_after(left.before_begin(), collection, first, middle);

            // Recursively sort, then merge
            flist_merge_sort(left, size / 2, compare, projection);
            flist_merge_sort(collection, size - size / 2, compare, projection);
            collection.merge(std::move(left), make_projection_compare(std::move(compare),
                                                                      std::move(projection)));
        }
    }

    template<>
    struct container_aware_adapter<merge_sorter>:
        detail::container_aware_adapter_base<merge_sorter>,
        detail::sorter_facade_fptr<
            container_aware_adapter<merge_sorter>,
            std::is_empty<merge_sorter>::value
        >
    {
        using detail::container_aware_adapter_base<merge_sorter>::operator();

        container_aware_adapter() = default;
        constexpr explicit container_aware_adapter(merge_sorter) noexcept {}

        ////////////////////////////////////////////////////////////
        // std::list

        template<typename... Args>
        auto operator()(std::list<Args...>& iterable) const
            -> void
        {
            detail::list_merge_sort(iterable, std::less<>{}, utility::identity{});
        }

        template<typename Compare, typename... Args>
        auto operator()(std::list<Args...>& iterable, Compare compare) const
            -> detail::enable_if_t<
                is_projection_v<utility::identity, std::list<Args...>, Compare>
            >
        {
            detail::list_merge_sort(iterable, std::move(compare), utility::identity{});
        }

        template<typename Projection, typename... Args>
        auto operator()(std::list<Args...>& iterable, Projection projection) const
            -> detail::enable_if_t<
                is_projection_v<Projection, std::list<Args...>>
            >
        {
            detail::list_merge_sort(iterable, std::less<>{}, std::move(projection));
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
            detail::list_merge_sort(iterable, std::move(compare), std::move(projection));
        }

        ////////////////////////////////////////////////////////////
        // std::forward_list

        template<typename... Args>
        auto operator()(std::forward_list<Args...>& iterable) const
            -> void
        {
            detail::flist_merge_sort(iterable, utility::size(iterable),
                                     std::less<>{}, utility::identity{});
        }

        template<typename Compare, typename... Args>
        auto operator()(std::forward_list<Args...>& iterable, Compare compare) const
            -> detail::enable_if_t<
                is_projection_v<utility::identity, std::forward_list<Args...>, Compare>
            >
        {
            detail::flist_merge_sort(iterable, utility::size(iterable),
                                     std::move(compare), utility::identity{});
        }

        template<typename Projection, typename... Args>
        auto operator()(std::forward_list<Args...>& iterable, Projection projection) const
            -> detail::enable_if_t<
                is_projection_v<Projection, std::forward_list<Args...>>
            >
        {
            detail::flist_merge_sort(iterable, utility::size(iterable),
                                     std::less<>{}, std::move(projection));
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
            detail::flist_merge_sort(iterable, utility::size(iterable),
                                     std::move(compare), std::move(projection));
        }
    };
}

#endif // CPPSORT_DETAIL_CONTAINER_AWARE_MERGE_SORT_H_
