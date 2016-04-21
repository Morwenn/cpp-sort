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
#ifndef CPPSORT_SORTERS_SPREAD_SORTER_STRING_SPREAD_SORTER_H_
#define CPPSORT_SORTERS_SPREAD_SORTER_STRING_SPREAD_SORTER_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <cstdint>
#include <functional>
#include <iterator>
#include <string>
#include <type_traits>
#include <cpp-sort/sorter_facade.h>
#include <cpp-sort/utility/functional.h>
#include <cpp-sort/utility/static_const.h>
#include "../../detail/iterator_traits.h"
#include "../../detail/spreadsort/string_sort.h"

namespace cppsort
{
    ////////////////////////////////////////////////////////////
    // Sorter

    namespace detail
    {
        struct string_spread_sorter_impl
        {
            ////////////////////////////////////////////////////////////
            // Ascending string sort

            template<
                typename RandomAccessIterator,
                typename Projection = utility::identity
            >
            auto operator()(RandomAccessIterator first, RandomAccessIterator last,
                            Projection projection={}) const
                -> std::enable_if_t<
                    std::is_same<
                        projected_t<RandomAccessIterator, Projection>,
                        std::string
                    >::value
                >
            {
                unsigned char unused = '\0';
                spreadsort::string_sort(first, last, projection, unused);
            }

            template<
                typename RandomAccessIterator,
                typename Projection = utility::identity
            >
            auto operator()(RandomAccessIterator first, RandomAccessIterator last,
                            Projection projection={}) const
                -> std::enable_if_t<
                    std::is_same<
                        projected_t<RandomAccessIterator, Projection>,
                        std::wstring
                    >::value && (sizeof(wchar_t) == 2)
                >
            {
                std::uint16_t unused = 0;
                spreadsort::string_sort(first, last, projection, unused);
            }

            ////////////////////////////////////////////////////////////
            // Descending string sort

            template<
                typename RandomAccessIterator,
                typename Projection = utility::identity
            >
            auto operator()(RandomAccessIterator first, RandomAccessIterator last,
                            std::greater<> compare, Projection projection={}) const
                -> std::enable_if_t<
                    std::is_same<
                        projected_t<RandomAccessIterator, Projection>,
                        std::string
                    >::value
                >
            {
                unsigned char unused = '\0';
                spreadsort::reverse_string_sort(first, last, compare, projection, unused);
            }

            template<
                typename RandomAccessIterator,
                typename Projection = utility::identity
            >
            auto operator()(RandomAccessIterator first, RandomAccessIterator last,
                            std::greater<> compare, Projection projection={}) const
                -> std::enable_if_t<
                    std::is_same<
                        projected_t<RandomAccessIterator, Projection>,
                        std::wstring
                    >::value && (sizeof(wchar_t) == 2)
                >
            {
                std::uint16_t unused = 0;
                spreadsort::reverse_string_sort(first, last, compare, projection, unused);
            }

            ////////////////////////////////////////////////////////////
            // Sorter traits

            using iterator_category = std::random_access_iterator_tag;
            using is_stable = std::false_type;
        };
    }

    struct string_spread_sorter:
        sorter_facade<detail::string_spread_sorter_impl>
    {};

    ////////////////////////////////////////////////////////////
    // Sort function

    namespace
    {
        constexpr auto&& string_spread_sort
            = utility::static_const<string_spread_sorter>::value;
    }
}

#endif // CPPSORT_SORTERS_SPREAD_SORTER_STRING_SPREAD_SORTER_H_
