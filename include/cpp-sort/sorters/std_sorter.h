/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2015-2016 Morwenn
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
#ifndef CPPSORT_SORTERS_STD_SORTER_H_
#define CPPSORT_SORTERS_STD_SORTER_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <algorithm>
#include <functional>
#include <iterator>
#include <type_traits>
#include <cpp-sort/fwd.h>
#include <cpp-sort/sorter_facade.h>
#include <cpp-sort/sorter_traits.h>
#include <cpp-sort/utility/static_const.h>
#include "../detail/iterator_traits.h"

namespace cppsort
{
    ////////////////////////////////////////////////////////////
    // Sorter

    namespace detail
    {
        struct std_sorter_impl
        {
            template<
                typename RandomAccessIterator,
                typename Compare = std::less<>,
                typename = std::enable_if_t<not is_projection_iterator_v<
                    Compare, RandomAccessIterator
                >>
            >
            auto operator()(RandomAccessIterator first, RandomAccessIterator last,
                            Compare compare={}) const
                -> void
            {
                static_assert(
                    std::is_base_of<
                        std::random_access_iterator_tag,
                        iterator_category_t<RandomAccessIterator>
                    >::value,
                    "std_sorter requires at least random-access iterators"
                );

                std::sort(first, last, compare);
            }

            ////////////////////////////////////////////////////////////
            // Sorter traits

            using iterator_category = std::random_access_iterator_tag;
            using is_stable = std::false_type;
        };
    }

    struct std_sorter:
        sorter_facade<detail::std_sorter_impl>
    {};

    ////////////////////////////////////////////////////////////
    // Stable sorter

    namespace detail
    {
        struct std_stable_sorter_impl
        {
            template<
                typename RandomAccessIterator,
                typename Compare = std::less<>,
                typename = std::enable_if_t<not is_projection_iterator_v<
                    Compare, RandomAccessIterator
                >>
            >
            auto operator()(RandomAccessIterator first, RandomAccessIterator last,
                            Compare compare={}) const
                -> void
            {
                static_assert(
                    std::is_base_of<
                        std::random_access_iterator_tag,
                        iterator_category_t<RandomAccessIterator>
                    >::value,
                    "stable_adapter<std_sorter> requires at least random-access iterators"
                );

                std::stable_sort(first, last, compare);
            }

            ////////////////////////////////////////////////////////////
            // Sorter traits

            using iterator_category = std::random_access_iterator_tag;
            using is_stable = std::true_type;
        };
    }

    template<>
    struct stable_adapter<std_sorter>:
        sorter_facade<detail::std_stable_sorter_impl>
    {};

    ////////////////////////////////////////////////////////////
    // Sort function

    namespace
    {
        constexpr auto&& std_sort
            = utility::static_const<std_sorter>::value;
    }
}

#endif // CPPSORT_SORTERS_STD_SORTER_H_
