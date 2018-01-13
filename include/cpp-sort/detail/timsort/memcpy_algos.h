/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2018 Timothy Van Slyke
 * Modified in 2018 by Morwenn for inclusion into cpp-sort
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
#ifndef CPPSORT_DETAIL_TIMSORT_MEMCPY_ALGOS_H_
#define CPPSORT_DETAIL_TIMSORT_MEMCPY_ALGOS_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <algorithm>
#include <cstring>
#include <type_traits>
#include "contiguous_iterator.h"
#include "iter.h"

namespace cppsort
{
namespace detail
{
    template<typename Iterator>
    constexpr bool has_memcpy_safe_value_type_v =
        std::is_trivially_copyable<iterator_value_type_t<Iterator>>::value;

    template<typename Iterator>
    static constexpr bool can_forward_memcpy_v
        = is_contiguous_iterator_v<Iterator> && has_memcpy_safe_value_type_v<Iterator>;

    template<typename Iterator>
    static constexpr bool can_reverse_memcpy_v
        = is_reverse_contiguous_iterator_v<Iterator> && has_memcpy_safe_value_type_v<Iterator>;

    template<typename Iterator>
    struct GetMemcpyIterator
    {
        static auto get(Iterator iter) noexcept
            -> iterator_value_type_t<Iterator>*
        {
            return &(*iter);
        }
    };

    template<typename T>
    struct GetMemcpyIterator<T*>
    {
        static auto get(T* iter) noexcept
            -> T*
        {
            return iter;
        }
    };

    template<typename T>
    struct GetMemcpyIterator<const T*>
    {
        static auto get(const T* iter) noexcept
            -> const T*
        {
            return iter;
        }
    };

    template<typename Iterator>
    auto get_memcpy_iterator(Iterator iter) noexcept
    {
        return GetMemcpyIterator<Iterator>::get(iter);
    }

    /*
     * Try to memcpy to the destination range, otherwise use std::move.
     */
    template<typename SrcIt, typename DestIt>
    auto move_or_memcpy(SrcIt begin, SrcIt end, DestIt dest)
        -> std::enable_if_t<
            can_forward_memcpy_v<SrcIt> && can_forward_memcpy_v<DestIt>,
            DestIt
        >
    {
        using value_type = iterator_value_type_t<SrcIt>;
        std::memcpy(get_memcpy_iterator(dest), get_memcpy_iterator(begin), (end - begin) * sizeof(value_type));
        return dest + (end - begin);
    }

    template<typename SrcIt, typename DestIt>
    auto move_or_memcpy(SrcIt begin, SrcIt end, DestIt dest)
        -> std::enable_if_t<
            can_reverse_memcpy_v<SrcIt> && can_reverse_memcpy_v<DestIt>,
            DestIt
        >
    {
        using value_type = iterator_value_type_t<SrcIt>;
        std::memcpy(get_memcpy_iterator(dest + (end - begin) - 1),
                    get_memcpy_iterator(end - 1),
                    (end - begin) * sizeof(value_type));
        return dest + (end - begin);
    }

    template<typename SrcIt, typename DestIt>
    auto move_or_memcpy(SrcIt begin, SrcIt end, DestIt dest)
        -> std::enable_if_t<
            not(can_forward_memcpy_v<SrcIt> && can_forward_memcpy_v<DestIt>) &&
            not(can_reverse_memcpy_v<SrcIt> && can_reverse_memcpy_v<DestIt>),
            DestIt
        >
    {
        return std::move(begin, end, dest);
    }
}}

#endif // CPPSORT_DETAIL_TIMSORT_MEMCPY_ALGOS_H_
