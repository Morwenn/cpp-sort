/*
 * Copyright (c) 2015-2024 Morwenn
 * SPDX-License-Identifier: MIT
 */
#ifndef CPPSORT_DETAIL_COMPARISON_COUNTER_H_
#define CPPSORT_DETAIL_COMPARISON_COUNTER_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <type_traits>
#include <utility>
#include <cpp-sort/utility/as_function.h>
#include <cpp-sort/utility/branchless_traits.h>

namespace cppsort
{
    namespace detail
    {
        template<typename Compare, typename CountType>
        class comparison_counter
        {
            public:

                constexpr comparison_counter(Compare compare, CountType& count):
                    compare(std::move(compare)),
                    count(count)
                {}

                template<typename T, typename U>
                constexpr auto operator()(T&& lhs, U&& rhs)
                    -> decltype(auto)
                {
                    ++count;
                    auto&& comp = utility::as_function(compare);
                    return comp(std::forward<T>(lhs), std::forward<U>(rhs));
                }

                // Accessible member data
                [[no_unique_address]] Compare compare;

            private:

                // Comparison functions are generally passed by value,
                // therefore we need to know where is the original counter
                // in order to increment it
                CountType& count;
        };
    }

    namespace utility
    {
        template<typename Compare, typename CountType, typename T>
        struct is_probably_branchless_comparison<
            cppsort::detail::comparison_counter<Compare, CountType>,
            T
        >:
            // Lie about being branchless if needed: what matters is to get
            // an accurate count of the number of comparisons performed by
            // algorithms even when not under analysis
            is_probably_branchless_comparison<Compare, T>
        {};
    }
}

#endif // CPPSORT_DETAIL_COMPARISON_COUNTER_H_
