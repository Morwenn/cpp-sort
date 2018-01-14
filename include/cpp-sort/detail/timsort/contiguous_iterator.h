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
#ifndef CPPSORT_DETAIL_TIMSORT_CONTIGUOUS_ITERATOR_H_
#define CPPSORT_DETAIL_TIMSORT_CONTIGUOUS_ITERATOR_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <iterator>
#include <string>
#include <type_traits>
#include <utility>
#include <valarray>
#include <vector>
#include "../iterator_traits.h"

#if __cplusplus > 201402L && defined(__has_include)
#   if __has_include(<string_view>)
#       include <string_view>
#   endif
#endif


namespace cppsort
{
namespace detail
{
    template<typename Iterator>
    struct is_valarray_iterator
    {
        private:

            using _value_type = value_type_t<Iterator>;
            using _va = std::valarray<_value_type>;
            using _begin_result = std::decay_t<decltype(std::begin(std::declval<_va>()))>;
            using _begin_cresult = std::decay_t<decltype(std::begin(std::declval<const _va>()))>;
            using _end_result = std::decay_t<decltype(std::begin(std::declval<_va>()))>;
            using _end_cresult = std::decay_t<decltype(std::begin(std::declval<const _va>()))>;

        public:

            static constexpr bool value =
                std::is_same<Iterator, _begin_result>::value ||
                std::is_same<Iterator, _begin_cresult>::value ||
                std::is_same<Iterator, _end_result>::value ||
                std::is_same<Iterator, _end_cresult>::value;
    };

    template<
        typename Iterator,
        bool = std::is_pod<value_type_t<Iterator>>::value
    >
    struct is_string_iterator;

    template<typename Iterator>
    struct is_string_iterator<Iterator, true>
    {
        using _value_type = value_type_t<Iterator>;
        static constexpr bool value =
            std::is_same<Iterator, typename std::basic_string<_value_type>::iterator>::value ||
            std::is_same<Iterator, typename std::basic_string<_value_type>::const_iterator>::value;
    };

    template<typename Iterator>
    struct is_string_iterator<Iterator, false>:
        std::false_type
    {};

    template<
        typename Iterator,
        bool = std::is_pod<value_type_t<Iterator>>::value
    >
    struct is_string_view_iterator;

    template<typename Iterator>
    struct is_string_view_iterator<Iterator, true>
    {
        using _value_type = value_type_t<Iterator>;
        static constexpr bool value =
            std::is_same<Iterator, typename std::basic_string<_value_type>::iterator>::value ||
            std::is_same<Iterator, typename std::basic_string<_value_type>::const_iterator>::value;
    };

    template<typename Iterator>
    struct is_string_view_iterator<Iterator, false>:
        std::false_type
    {};

#if __cplusplus > 201402L && defined(__has_include)
#   if __has_include(<string_view>)
    template<typename Iterator>
    struct is_stringview_iterator
    {
        using _value_type = value_type_t<Iterator>;
        static constexpr bool value =
            std::is_same<Iterator, typename std::basic_string_view<_value_type>::iterator>::value ||
            std::is_same<Iterator, typename std::basic_string_view<_value_type>::const_iterator>::value;
    };
#   endif
#endif

    template<typename Iterator>
    struct is_vector_iterator
    {
        using _value_type = value_type_t<Iterator>;
        static constexpr bool value =
            std::is_same<Iterator, typename std::vector<_value_type>::iterator>::value ||
            std::is_same<Iterator, typename std::vector<_value_type>::const_iterator>::value;
    };

    template<typename Iterator>
    struct is_contiguous_iterator
    {
        using _value_type = value_type_t<Iterator>;
        static constexpr bool value =
            (is_vector_iterator<Iterator>::value && not std::is_same<_value_type, bool>::value) ||
            is_string_iterator<Iterator>::value ||
            is_string_view_iterator<Iterator>::value ||
            is_valarray_iterator<Iterator>::value;
    };

    template<typename T>
    struct is_contiguous_iterator<T*>:
        std::true_type
    {};

    template<typename T>
    struct is_contiguous_iterator<const T*>:
        std::true_type
    {};

    template<typename Iterator>
    struct is_contiguous_iterator<const Iterator>:
        is_contiguous_iterator<Iterator>
    {};

    template<typename Iterator>
    struct is_contiguous_iterator<std::reverse_iterator<std::reverse_iterator<Iterator>>>:
        is_contiguous_iterator<Iterator>
    {};


    namespace
    {
        template<typename Iterator>
        constexpr bool is_contiguous_iterator_v
            = is_contiguous_iterator<Iterator>::value;
    }

    template<typename Iterator>
    struct is_reverse_contiguous_iterator:
        std::false_type
    {};

    template<typename Iterator>
    struct is_reverse_contiguous_iterator<std::reverse_iterator<Iterator>>:
        is_contiguous_iterator<Iterator>
    {};

    namespace
    {
        template<typename Iterator>
        constexpr bool is_reverse_contiguous_iterator_v
            = is_reverse_contiguous_iterator<Iterator>::value;
    }
}}

#endif // CPPSORT_DETAIL_TIMSORT_CONTIGUOUS_ITERATOR_H_
