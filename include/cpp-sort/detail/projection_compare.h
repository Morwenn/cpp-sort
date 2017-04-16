/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2016-2017 Morwenn
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
#ifndef CPPSORT_DETAIL_PROJECTION_COMPARE_H_
#define CPPSORT_DETAIL_PROJECTION_COMPARE_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <tuple>
#include <utility>
#include <cpp-sort/utility/as_function.h>

namespace cppsort
{
namespace detail
{
    template<typename Compare, typename Projection>
    class projection_compare
    {
        private:

            using compare_t = decltype(utility::as_function(std::declval<Compare&>()));
            using projection_t = decltype(utility::as_function(std::declval<Projection&>()));
            std::tuple<compare_t, projection_t> data;

        public:

            projection_compare(Compare compare, Projection projection):
                data(utility::as_function(compare), utility::as_function(projection))
            {}

            auto compare() const
                -> compare_t
            {
                return std::get<0>(data);
            }

            auto projection() const
                -> projection_t
            {
                return std::get<1>(data);
            }

            template<typename T, typename U>
            auto operator()(T&& lhs, U&& rhs)
                noexcept(noexcept(std::get<0>(data)(std::get<1>(data)(std::forward<T>(lhs)),
                                                    std::get<1>(data)(std::forward<U>(rhs)))))
                -> decltype(std::get<0>(data)(std::get<1>(data)(std::forward<T>(lhs)),
                                              std::get<1>(data)(std::forward<U>(rhs))))
            {
                return std::get<0>(data)(std::get<1>(data)(std::forward<T>(lhs)),
                                         std::get<1>(data)(std::forward<U>(rhs)));
            }
    };

    template<typename Compare, typename Projection>
    auto make_projection_compare(Compare compare, Projection projection)
        -> projection_compare<Compare, Projection>
    {
        return { compare, projection };
    }
}}

#endif // CPPSORT_DETAIL_PROJECTION_COMPARE_H_
