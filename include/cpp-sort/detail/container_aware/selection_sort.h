/*
 * Copyright (c) 2016-2024 Morwenn
 * SPDX-License-Identifier: MIT
 */
#ifndef CPPSORT_DETAIL_CONTAINER_AWARE_SELECTION_SORT_H_
#define CPPSORT_DETAIL_CONTAINER_AWARE_SELECTION_SORT_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <forward_list>
#include <functional>
#include <list>
#include <type_traits>
#include <utility>
#include <cpp-sort/fwd.h>
#include <cpp-sort/mstd/iterator.h>
#include <cpp-sort/sorter_facade.h>
#include <cpp-sort/sorter_traits.h>
#include <cpp-sort/utility/as_function.h>
#include "../min_element.h"

namespace cppsort
{
    namespace detail
    {
        template<typename Compare, typename Projection, typename... Args>
        auto list_selection_sort(std::list<Args...>& collection,
                                 Compare compare, Projection projection)
            -> void
        {
            auto it = collection.begin();
            auto last = collection.end();
            while (it != last)
            {
                auto min_it = min_element(it, last, compare, projection);
                if (min_it == it)
                {
                    ++it;
                }
                else
                {
                    collection.splice(it, collection, min_it);
                }
            }
        }

        template<typename Compare, typename Projection, typename... Args>
        auto flist_selection_sort(std::forward_list<Args...>& collection,
                                  Compare compare, Projection projection)
            -> void
        {
            auto&& comp = utility::as_function(compare);
            auto&& proj = utility::as_function(projection);

            auto first = collection.before_begin();
            auto last = collection.end();
            while (mstd::next(first) != last)
            {
                auto min_it = first;
                auto it = first;
                while (mstd::next(it) != last)
                {
                    if (comp(proj(*mstd::next(it)), proj(*mstd::next(min_it))))
                    {
                        min_it = it;
                    }
                    ++it;
                }

                collection.splice_after(first, collection, min_it);
                ++first;
            }
        }
    }

    template<>
    struct container_aware_adapter<selection_sorter>:
        detail::container_aware_adapter_base<selection_sorter>,
        detail::sorter_facade_fptr<
            container_aware_adapter<selection_sorter>,
            std::is_empty_v<selection_sorter>
        >
    {
        using detail::container_aware_adapter_base<selection_sorter>::operator();

        container_aware_adapter() = default;
        constexpr explicit container_aware_adapter(selection_sorter) noexcept {}

        ////////////////////////////////////////////////////////////
        // std::list

        template<typename... Args>
        auto operator()(std::list<Args...>& collection) const
            -> void
        {
            detail::list_selection_sort(collection, std::less{}, std::identity{});
        }

        template<typename Compare, typename... Args>
            requires is_projection_v<std::identity, std::list<Args...>, Compare>
        auto operator()(std::list<Args...>& collection, Compare compare) const
            -> void
        {
            detail::list_selection_sort(collection, std::move(compare), std::identity{});
        }

        template<typename Projection, typename... Args>
            requires is_projection_v<Projection, std::list<Args...>>
        auto operator()(std::list<Args...>& collection, Projection projection) const
            -> void
        {
            detail::list_selection_sort(collection, std::less{}, std::move(projection));
        }

        template<typename Compare, typename Projection, typename... Args>
            requires is_projection_v<Projection, std::list<Args...>, Compare>
        auto operator()(std::list<Args...>& collection,
                        Compare compare, Projection projection) const
            -> void
        {
            detail::list_selection_sort(collection, std::move(compare), std::move(projection));
        }

        ////////////////////////////////////////////////////////////
        // std::forward_list

        template<typename... Args>
        auto operator()(std::forward_list<Args...>& collection) const
            -> void
        {
            detail::flist_selection_sort(collection, std::less{}, std::identity{});
        }

        template<typename Compare, typename... Args>
            requires is_projection_v<std::identity, std::forward_list<Args...>, Compare>
        auto operator()(std::forward_list<Args...>& collection, Compare compare) const
            -> void
        {
            detail::flist_selection_sort(collection, std::move(compare), std::identity{});
        }

        template<typename Projection, typename... Args>
            requires is_projection_v<Projection, std::forward_list<Args...>>
        auto operator()(std::forward_list<Args...>& collection, Projection projection) const
            -> void
        {
            detail::flist_selection_sort(collection, std::less{}, std::move(projection));
        }

        template<typename Compare, typename Projection, typename... Args>
            requires is_projection_v<Projection, std::forward_list<Args...>, Compare>
        auto operator()(std::forward_list<Args...>& collection,
                        Compare compare, Projection projection) const
            -> void
        {
            detail::flist_selection_sort(collection, std::move(compare), std::move(projection));
        }
    };
}

#endif // CPPSORT_DETAIL_CONTAINER_AWARE_SELECTION_SORT_H_
