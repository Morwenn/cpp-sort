/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2015-2017 Morwenn
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
#ifndef CPPSORT_UTILITY_SIZE_H_
#define CPPSORT_UTILITY_SIZE_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <cstddef>
#include <iterator>
#include <type_traits>
#include <utility>
#include <cpp-sort/utility/detection.h>

namespace cppsort
{
namespace utility
{
    namespace detail
    {
        template<typename Iterable>
        using has_size_method_t = decltype(std::declval<const Iterable&>().size());
    }

    template<
        typename Iterable,
        typename = std::enable_if_t<is_detected_v<detail::has_size_method_t, Iterable>>
    >
    constexpr auto size(const Iterable& iterable)
        -> decltype(iterable.size())
    {
        return iterable.size();
    }

    template<
        typename Iterable,
        typename = std::enable_if_t<not is_detected_v<detail::has_size_method_t, Iterable>>
    >
    constexpr auto size(const Iterable& iterable)
        -> decltype(std::distance(std::begin(iterable), std::end(iterable)))
    {
        return std::distance(std::begin(iterable), std::end(iterable));
    }

    template<typename T, std::size_t N>
    constexpr auto size(const T (&array)[N]) noexcept
        -> std::size_t
    {
        return N;
    }
}}

#endif // CPPSORT_UTILITY_SIZE_H_
