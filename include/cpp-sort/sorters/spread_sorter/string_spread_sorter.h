/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2015 Morwenn
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
#include <iterator>
#include <string>
#include <type_traits>
#include <cpp-sort/sorter_facade.h>
#include <cpp-sort/sorter_traits.h>
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

            template<typename RandomAccessIterator>
            auto operator()(RandomAccessIterator first, RandomAccessIterator last) const
                -> std::enable_if_t<
                    std::is_same<typename std::iterator_traits<RandomAccessIterator>::value_type,
                                 typename std::string>::value
                >
            {
                spreadsort::string_sort(first, last);
            }

            template<typename RandomAccessIterator>
            auto operator()(RandomAccessIterator first, RandomAccessIterator last) const
                -> std::enable_if_t<
                    std::is_same<typename std::iterator_traits<RandomAccessIterator>::value_type,
                                 typename std::wstring
                    >::value && (sizeof(wchar_t) == 2)
                >
            {
                std::uint16_t unused = 0;
                spreadsort::string_sort(first, last, unused);
            }

            ////////////////////////////////////////////////////////////
            // Descending string sort

            template<typename RandomAccessIterator>
            auto operator()(RandomAccessIterator first, RandomAccessIterator last,
                            std::greater<> compare) const
                -> std::enable_if_t<
                    std::is_same<typename std::iterator_traits<RandomAccessIterator>::value_type,
                                 typename std::string>::value
                >
            {
                spreadsort::reverse_string_sort(first, last, compare);
            }

            template<typename RandomAccessIterator>
            auto operator()(RandomAccessIterator first, RandomAccessIterator last,
                            std::greater<std::string> compare) const
                -> std::enable_if_t<
                    std::is_same<typename std::iterator_traits<RandomAccessIterator>::value_type,
                                 typename std::string>::value
                >
            {
                spreadsort::reverse_string_sort(first, last, compare);
            }

            template<typename RandomAccessIterator>
            auto operator()(RandomAccessIterator first, RandomAccessIterator last,
                            std::greater<> compare) const
                -> std::enable_if_t<
                    std::is_same<typename std::iterator_traits<RandomAccessIterator>::value_type,
                                 typename std::wstring
                    >::value && (sizeof(wchar_t) == 2)
                >
            {
                std::uint16_t unused = 0;
                spreadsort::reverse_string_sort(first, last, compare, unused);
            }

            template<typename RandomAccessIterator>
            auto operator()(RandomAccessIterator first, RandomAccessIterator last,
                            std::greater<std::wstring> compare) const
                -> std::enable_if_t<
                    std::is_same<typename std::iterator_traits<RandomAccessIterator>::value_type,
                                 typename std::wstring
                    >::value && (sizeof(wchar_t) == 2)
                >
            {
                std::uint16_t unused = 0;
                spreadsort::reverse_string_sort(first, last, compare, unused);
            }
        };
    }

    struct string_spread_sorter:
        sorter_facade<detail::string_spread_sorter_impl>
    {};

    ////////////////////////////////////////////////////////////
    // Sorter traits

    template<>
    struct sorter_traits<string_spread_sorter>
    {
        using iterator_category = std::random_access_iterator_tag;
        static constexpr bool is_stable = false;
    };
}

#endif // CPPSORT_SORTERS_SPREAD_SORTER_STRING_SPREAD_SORTER_H_
