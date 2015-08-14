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
#ifndef CPPSORT_DETAIL_COMPARISON_COUNTER_H_
#define CPPSORT_DETAIL_COMPARISON_COUNTER_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <cstddef>

namespace cppsort
{
namespace detail
{
    template<
        typename Compare,
        typename CountType = std::size_t
    >
    class comparison_counter
    {
        public:

            comparison_counter(Compare compare):
                compare(compare),
                count(0),
                counter(*this)
            {}

            template<typename T, typename U>
            auto operator()(T&& lhs, U&& rhs)
                -> decltype(auto)
            {
                ++counter.count;
                return compare(std::forward<T>(lhs), std::forward<U>(rhs));
            }

            // Accessible member data
            Compare compare;
            CountType count;

        private:

            // Comparison functions are generally passed by value,
            // therefore we need to know which is the original counter
            // in order to increment the right count
            comparison_counter& counter;
    };
}}

#endif // CPPSORT_DETAIL_COMPARISON_COUNTER_H_
