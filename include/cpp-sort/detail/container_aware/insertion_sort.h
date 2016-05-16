/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2016 Morwenn
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */
#ifndef CPPSORT_ADAPTERS_CONTAINER_AWARE_INSERTION_SORT_H_
#define CPPSORT_ADAPTERS_CONTAINER_AWARE_INSERTION_SORT_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <forward_list>
#include <functional>
#include <iterator>
#include <list>
#include <type_traits>
#include <cpp-sort/fwd.h>
#include <cpp-sort/sorter_traits.h>
#include <cpp-sort/utility/functional.h>
#include "../upper_bound.h"

namespace cppsort
{
    namespace detail
    {
        template<typename T, typename Compare, typename Projection>
        auto list_insertion_sort(std::list<T>& collection, Compare compare, Projection projection)
            -> void
        {
            auto&& proj = utility::as_function(projection);

            auto it = std::begin(collection);
            auto last = std::end(collection);
            if (it == last) return;

            ++it;
            while (it != last)
            {
                auto insertion_point = upper_bound(std::begin(collection), it, proj(*it),
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

        template<typename T, typename Compare, typename Projection>
        auto flist_insertion_sort(std::forward_list<T>& collection, Compare compare, Projection projection)
            -> void
        {
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
                    if (not compare(proj(*std::next(insertion_point)), value)) break;
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
        detail::container_aware_adapter_base<insertion_sorter>
    {
        using detail::container_aware_adapter_base<insertion_sorter>::operator();

        ////////////////////////////////////////////////////////////
        // std::list

        template<typename T>
        auto operator()(std::list<T>& iterable) const
            -> void
        {
            detail::list_insertion_sort(iterable, std::less<>{}, utility::identity{});
        }

        template<typename T, typename Compare>
        auto operator()(std::list<T>& iterable, Compare compare) const
            -> std::enable_if_t<
                is_projection_v<utility::identity, std::list<T>, Compare>
            >
        {
            detail::list_insertion_sort(iterable, compare, utility::identity{});
        }

        template<typename T, typename Projection>
        auto operator()(std::list<T>& iterable, Projection projection) const
            -> std::enable_if_t<
                is_projection_v<Projection, std::list<T>>
            >
        {
            detail::list_insertion_sort(iterable, std::less<>{}, projection);
        }

        template<
            typename T,
            typename Compare,
            typename Projection,
            typename = std::enable_if_t<
                is_projection_v<Projection, std::list<T>, Compare>
            >
        >
        auto operator()(std::list<T>& iterable, Compare compare, Projection projection) const
            -> void
        {
            detail::list_insertion_sort(iterable, compare, projection);
        }

        ////////////////////////////////////////////////////////////
        // std::forward_list

        template<typename T>
        auto operator()(std::forward_list<T>& iterable) const
            -> void
        {
            detail::flist_insertion_sort(iterable, std::less<>{}, utility::identity{});
        }

        template<typename T, typename Compare>
        auto operator()(std::forward_list<T>& iterable, Compare compare) const
            -> std::enable_if_t<
                is_projection_v<utility::identity, std::forward_list<T>, Compare>
            >
        {
            detail::flist_insertion_sort(iterable, compare, utility::identity{});
        }

        template<typename T, typename Projection>
        auto operator()(std::forward_list<T>& iterable, Projection projection) const
            -> std::enable_if_t<
                is_projection_v<Projection, std::forward_list<T>>
            >
        {
            detail::flist_insertion_sort(iterable, std::less<>{}, projection);
        }

        template<
            typename T,
            typename Compare,
            typename Projection,
            typename = std::enable_if_t<
                is_projection_v<Projection, std::forward_list<T>, Compare>
            >
        >
        auto operator()(std::forward_list<T>& iterable, Compare compare, Projection projection) const
            -> void
        {
            detail::flist_insertion_sort(iterable, compare, projection);
        }
    };
}

#endif // CPPSORT_ADAPTERS_CONTAINER_AWARE_INSERTION_SORT_H_
