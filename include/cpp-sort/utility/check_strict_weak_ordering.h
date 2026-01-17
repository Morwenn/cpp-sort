/*
 * Copyright (c) 2026 Morwenn
 * SPDX-License-Identifier: MIT
 */
#ifndef CPPSORT_UTILITY_CHECK_STRICT_WEAK_ORDERING_H_
#define CPPSORT_UTILITY_CHECK_STRICT_WEAK_ORDERING_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <functional>
#include <iterator>
#include <utility>
#include <cpp-sort/sorter_facade.h>
#include <cpp-sort/sorter_traits.h>
#include <cpp-sort/utility/functional.h>
#include "../detail/heapsort.h"
#include "../detail/is_sorted_until.h"
#include "../detail/type_traits.h"

namespace cppsort::utility
{
    ////////////////////////////////////////////////////////////
    // Check whether a comparison function implements a strict
    // weak ordering over a range of data, following an
    // algorithm described by danlark1 here:
    // https://github.com/danlark1/quadratic_strict_weak_ordering

    namespace detail
    {
        template<typename ForwardIterator, typename Compare, typename Projection>
        constexpr auto compare_equivalent(ForwardIterator it1, ForwardIterator it2,
                                          Compare compare, Projection projection)
            -> bool
        {
            return not compare(projection(*it1), projection(*it2))
                && not compare(projection(*it2), projection(*it1));
        }

        struct strict_weak_ordering_checker_impl
        {
            template<
                typename ForwardIterator,
                typename Compare = std::less<>,
                typename Projection = utility::identity,
                typename = cppsort::detail::enable_if_t<
                    is_projection_iterator_v<Projection, ForwardIterator, Compare>
                >
            >
            constexpr auto operator()(ForwardIterator first, ForwardIterator last,
                                      Compare compare={}, Projection projection={}) const
                -> bool
            {
                // In the comments below, we use the following abbreviations:
                // - R is the input range
                // - C is the comparison function
                // - P is the projeciton function

                auto&& comp = utility::as_function(compare);
                auto&& proj = utility::as_function(projection);

                while (first != last) {
                    // 1. Sort R
                    //
                    // Note: standard library implementations of heapsort supposedly do not
                    // crash when passed a comparison function that does not model a strict
                    // weak ordering, and ours happens to be copy-pasted from libc++
                    cppsort::detail::heapsort(first, last, compare, projection);

                    // 2. If the R is not sorted, then C does not model a strict weak ordering
                    if (not cppsort::detail::is_sorted(first, last, compare, projection)) {
                        return false;
                    }

                    // 3. Find first it such as *first < *it
                    auto it = std::next(first);
                    while (it != last && not comp(proj(*first), proj(*it))) {
                        ++it;
                    }

                    // 4. Check that all elements before it compare equivalent
                    for (auto it1 = first; it1 != it; ++it1) {
                        for (auto it2 = it1; it2 != it; ++it2) {
                            if (not compare_equivalent(it1, it2, comp, proj)) {
                                return false;
                            }
                        }
                    }

                    // 5. Check that all elements separated by it follow transitivity
                    for (auto it1 = first; it1 != it; ++it1) {
                        for (auto it2 = it; it2 != last; ++it2) {
                            if (comp(proj(*it2), proj(*it1))) {
                                return false;
                            }
                            if (not comp(proj(*it1), proj(*it2))) {
                                return false;
                            }
                        }
                    }

                    // Exclude leading elements that compare equivalent,
                    // start all over again with the rest of the elements
                    first = it;
                }

                // All checks passed, C models a strict weak ordering over R
                return true;
            }

            ////////////////////////////////////////////////////////////
            // Sorter traits

            using iterator_category = std::random_access_iterator_tag;
        };
    }

    struct strict_weak_ordering_checker:
        sorter_facade<detail::strict_weak_ordering_checker_impl>
    {};

    inline constexpr strict_weak_ordering_checker check_strict_weak_ordering{};
}

#endif // CPPSORT_UTILITY_CHECK_STRICT_WEAK_ORDERING_H_
