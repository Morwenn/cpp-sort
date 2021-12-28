/*
 * Copyright (c) 2021 Morwenn
 * SPDX-License-Identifier: MIT
 */
#ifndef CPPSORT_COMPARATORS_NOT_FN_H_
#define CPPSORT_COMPARATORS_NOT_FN_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <type_traits>
#include <utility>
#include <cpp-sort/utility/as_function.h>

namespace cppsort
{
    ////////////////////////////////////////////////////////////
    // C++17 std::not_fn

    template<typename Predicate>
    class not_fn_t
    {
        private:

            Predicate predicate;

        public:

            not_fn_t() = delete;

            explicit not_fn_t(Predicate predicate):
                predicate(std::move(predicate))
            {}

            template<typename T1, typename T2>
            auto operator()(T1&& x, T2&& y)
                -> bool
            {
                auto&& pred = utility::as_function(predicate);
                return not pred(std::forward<T1>(x), std::forward<T2>(y));
            }

            template<typename T1, typename T2>
            auto operator()(T1&& x, T2&& y) const
                -> bool
            {
                auto&& pred = utility::as_function(predicate);
                return not pred(std::forward<T1>(x), std::forward<T2>(y));
            }
    };

    template<typename Predicate>
    auto not_fn(Predicate&& pred)
        -> not_fn_t<std::decay_t<Predicate>>
    {
        return not_fn_t<std::decay_t<Predicate>>(std::forward<Predicate>(pred));
    }
}

#endif // CPPSORT_COMPARATORS_NOT_FN_H_
