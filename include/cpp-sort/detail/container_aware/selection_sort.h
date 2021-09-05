/*
 * Copyright (c) 2016-2021 Morwenn
 * SPDX-License-Identifier: MIT
 */
#ifndef CPPSORT_DETAIL_CONTAINER_AWARE_SELECTION_SORT_H_
#define CPPSORT_DETAIL_CONTAINER_AWARE_SELECTION_SORT_H_

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
#include "../min_element.h"
#include "../type_traits.h"

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
            while (std::next(first) != last)
            {
                auto min_it = first;
                auto it = first;
                while (std::next(it) != last)
                {
                    if (comp(proj(*std::next(it)), proj(*std::next(min_it))))
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
            std::is_empty<selection_sorter>::value
        >
    {
        using detail::container_aware_adapter_base<selection_sorter>::operator();

        container_aware_adapter() = default;
        constexpr explicit container_aware_adapter(selection_sorter) noexcept {}

        ////////////////////////////////////////////////////////////
        // std::list

        template<typename... Args>
        auto operator()(std::list<Args...>& iterable) const
            -> void
        {
            detail::list_selection_sort(iterable, std::less<>{}, utility::identity{});
        }

        template<typename Compare, typename... Args>
        auto operator()(std::list<Args...>& iterable, Compare compare) const
            -> detail::enable_if_t<
                is_projection_v<utility::identity, std::list<Args...>, Compare>
            >
        {
            detail::list_selection_sort(iterable, std::move(compare), utility::identity{});
        }

        template<typename Projection, typename... Args>
        auto operator()(std::list<Args...>& iterable, Projection projection) const
            -> detail::enable_if_t<
                is_projection_v<Projection, std::list<Args...>>
            >
        {
            detail::list_selection_sort(iterable, std::less<>{}, std::move(projection));
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
            detail::list_selection_sort(iterable, std::move(compare), std::move(projection));
        }

        ////////////////////////////////////////////////////////////
        // std::forward_list

        template<typename... Args>
        auto operator()(std::forward_list<Args...>& iterable) const
            -> void
        {
            detail::flist_selection_sort(iterable, std::less<>{}, utility::identity{});
        }

        template<typename Compare, typename... Args>
        auto operator()(std::forward_list<Args...>& iterable, Compare compare) const
            -> detail::enable_if_t<
                is_projection_v<utility::identity, std::forward_list<Args...>, Compare>
            >
        {
            detail::flist_selection_sort(iterable, std::move(compare), utility::identity{});
        }

        template<typename Projection, typename... Args>
        auto operator()(std::forward_list<Args...>& iterable, Projection projection) const
            -> detail::enable_if_t<
                is_projection_v<Projection, std::forward_list<Args...>>
            >
        {
            detail::flist_selection_sort(iterable, std::less<>{}, std::move(projection));
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
            detail::flist_selection_sort(iterable, std::move(compare), std::move(projection));
        }
    };
}

#endif // CPPSORT_DETAIL_CONTAINER_AWARE_SELECTION_SORT_H_
