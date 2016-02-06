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
#ifndef CPPSORT_DETAIL_ITERATOR_TRAITS_H_
#define CPPSORT_DETAIL_ITERATOR_TRAITS_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <iterator>

namespace cppsort
{
namespace detail
{
    //
    // A few type aliases to make it easier to write
    // code that uses std::iterator_traits; there are
    // equivalent aliases in the Ranges TS
    //

    template<typename Iterator>
    using difference_type_t = typename std::iterator_traits<Iterator>::difference_type;

    template<typename Iterator>
    using value_type_t = typename std::iterator_traits<Iterator>::value_type;

    template<typename Iterator>
    using pointer_t = typename std::iterator_traits<Iterator>::pointer;

    template<typename Iterator>
    using reference_t = typename std::iterator_traits<Iterator>::reference;

    template<typename Iterator>
    using iterator_category_t = typename std::iterator_traits<Iterator>::iterator_category;
}}

#endif // CPPSORT_DETAIL_ITERATOR_TRAITS_H_
