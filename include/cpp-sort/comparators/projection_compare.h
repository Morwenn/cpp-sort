/*
 * Copyright (c) 2016-2021 Morwenn
 * SPDX-License-Identifier: MIT
 */
#ifndef CPPSORT_COMPARATORS_PROJECTION_COMPARE_H_
#define CPPSORT_COMPARATORS_PROJECTION_COMPARE_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <tuple>
#include <utility>
#include <cpp-sort/utility/as_function.h>
#include <cpp-sort/utility/branchless_traits.h>
#include "../detail/type_traits.h"

namespace cppsort
{
    template<typename Compare, typename Projection>
    class projection_compare
    {
        private:

            using compare_t = detail::remove_cvref_t<
                decltype(utility::as_function(std::declval<Compare>()))
            >;
            using projection_t = detail::remove_cvref_t<
                decltype(utility::as_function(std::declval<Projection>()))
            >;
            std::tuple<compare_t, projection_t> data;

        public:

            projection_compare(Compare compare, Projection projection):
                data(utility::as_function(compare), utility::as_function(projection))
            {}

            template<typename T, typename U>
            constexpr auto operator()(T&& lhs, U&& rhs)
                noexcept(noexcept(std::get<0>(data)(std::get<1>(data)(std::forward<T>(lhs)),
                                                    std::get<1>(data)(std::forward<U>(rhs)))))
                -> decltype(std::get<0>(data)(std::get<1>(data)(std::forward<T>(lhs)),
                                              std::get<1>(data)(std::forward<U>(rhs))))
            {
                return std::get<0>(data)(std::get<1>(data)(std::forward<T>(lhs)),
                                         std::get<1>(data)(std::forward<U>(rhs)));
            }

            template<typename T, typename U>
            constexpr auto operator()(T&& lhs, U&& rhs) const
                noexcept(noexcept(std::get<0>(data)(std::get<1>(data)(std::forward<T>(lhs)),
                                                    std::get<1>(data)(std::forward<U>(rhs)))))
                -> decltype(std::get<0>(data)(std::get<1>(data)(std::forward<T>(lhs)),
                                              std::get<1>(data)(std::forward<U>(rhs))))
            {
                return std::get<0>(data)(std::get<1>(data)(std::forward<T>(lhs)),
                                         std::get<1>(data)(std::forward<U>(rhs)));
            }

            using is_transparent = void;
    };

    template<typename Compare, typename Projection>
    auto make_projection_compare(Compare compare, Projection projection)
        -> projection_compare<Compare, Projection>
    {
        return { std::move(compare), std::move(projection) };
    }

    namespace utility
    {
        template<typename Compare, typename Projection, typename T>
        struct is_probably_branchless_comparison<projection_compare<Compare, Projection>, T>:
            cppsort::detail::conjunction<
                is_probably_branchless_projection<Projection, T>,
                is_probably_branchless_comparison<
                    Compare,
                    cppsort::detail::invoke_result_t<Projection, T>
                >
            >
        {};
    }
}

#endif // CPPSORT_COMPARATORS_PROJECTION_COMPARE_H_
