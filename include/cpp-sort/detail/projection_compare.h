/*
 * Copyright (c) 2016-2020 Morwenn
 * SPDX-License-Identifier: MIT
 */
#ifndef CPPSORT_DETAIL_PROJECTION_COMPARE_H_
#define CPPSORT_DETAIL_PROJECTION_COMPARE_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <tuple>
#include <utility>
#include <cpp-sort/utility/as_function.h>
#include "type_traits.h"

namespace cppsort
{
namespace detail
{
    template<typename Compare, typename Projection>
    class projection_compare
    {
        private:

            using compare_t = remove_cvref_t<decltype(utility::as_function(std::declval<Compare&>()))>;
            using projection_t = remove_cvref_t<decltype(utility::as_function(std::declval<Projection&>()))>;
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
