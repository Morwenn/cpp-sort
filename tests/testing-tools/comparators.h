/*
 * Copyright (c) 2026 Morwenn
 * SPDX-License-Identifier: MIT
 */
#ifndef CPPSORT_TESTSUITE_COMPARATORS_H_
#define CPPSORT_TESTSUITE_COMPARATORS_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <functional>
#include <cpp-sort/utility/as_function.h>
#include <cpp-sort/utility/functional.h>

namespace helpers
{
    // Identify the function that was called
    enum struct compare_result
    {
        total_less,
        total_greater,
        weak_less,
        weak_greater,
        partial_less,
        partial_greater,
    };

    // Collection of empty classes and empty comparison overloads used
    // to identify what overload was selected during resolution

    struct totally_comparable
    {
        friend constexpr auto total_less(const totally_comparable&,
                                         const totally_comparable&)
            -> compare_result
        {
            return compare_result::total_less;
        }
    };

    constexpr auto total_greater(const totally_comparable&,
                                 const totally_comparable&)
        -> compare_result
    {
        return compare_result::total_greater;
    }

    struct weakly_comparable
    {
        friend constexpr auto weak_less(const weakly_comparable&,
                                        const weakly_comparable&)
            -> compare_result
        {
            return compare_result::weak_less;
        }
    };

    constexpr auto weak_greater(const weakly_comparable&,
                                const weakly_comparable&)
        -> compare_result
    {
        return compare_result::weak_greater;
    }

    struct partially_comparable
    {
        friend constexpr auto partial_less(const partially_comparable&,
                                           const partially_comparable&)
            -> compare_result
        {
            return compare_result::partial_less;
        }
    };

    constexpr auto partial_greater(const partially_comparable&,
                                   const partially_comparable&)
        -> compare_result
    {
        return compare_result::partial_greater;
    }
}

#endif // CPPSORT_TESTSUITE_COMPARATORS_H_
