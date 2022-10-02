/*
 * Copyright (c) 2021-2022 Morwenn
 * SPDX-License-Identifier: MIT
 */
#ifndef CPPSORT_DETAIL_LONGEST_NON_DESCENDING_SUBSEQUENCE_H_
#define CPPSORT_DETAIL_LONGEST_NON_DESCENDING_SUBSEQUENCE_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <iterator>
#include <type_traits>
#include <utility>
#include <vector>
#include <cpp-sort/utility/as_function.h>
#include "functional.h"
#include "iterator_traits.h"
#include "upper_bound.h"

namespace cppsort
{
namespace detail
{
    // Longest non-decreasing subsequence, computed with an altered
    // patience sorting algorithm - returns a pair containing the
    // size of the LNDS and the size of the collection

    template<
        bool RecomputeSize,
        typename ForwardIterator,
        typename Compare,
        typename Projection
    >
    auto longest_non_descending_subsequence(ForwardIterator first, ForwardIterator last,
                                            difference_type_t<ForwardIterator> size,
                                            Compare compare, Projection projection)
        -> std::pair<difference_type_t<ForwardIterator>, difference_type_t<ForwardIterator>>
    {
        constexpr bool is_random_access = std::is_base_of<
            std::random_access_iterator_tag,
            iterator_category_t<ForwardIterator>
        >::value;

        if (first == last) {
            return { 0, 0 };
        }
        if (std::next(first) == last) {
            return { 1, 1 };
        }

        // The size is only needed at the end to actually compute Rem, but
        // we can compute it as-we-go when it is not known in order to avoid
        // making two passes over the sequence - when the sequence is made
        // of random-access iterators, we only compute it once
        if (RecomputeSize && is_random_access) {
            size = std::distance(first, last);
        }

        auto&& proj = utility::as_function(projection);

        // Top (smaller) elements in patience sorting stacks
        std::vector<ForwardIterator> stack_tops;

        while (first != last) {
            auto it = detail::upper_bound(
                stack_tops.begin(), stack_tops.end(),
                proj(*first), compare, indirect(projection));

            if (it == stack_tops.end()) {
                // The element is bigger than everything else,
                // create a new "stack" to put it
                stack_tops.emplace_back(first);
            } else {
                // The element is strictly smaller than the top
                // of a given stack, replace the stack top
                *it = first;
            }
            ++first;

            if (RecomputeSize && not is_random_access) {
                // Compute the size as-we-go if iterators are not random-access
                ++size;
            }
        }

        return { stack_tops.size(), size };
    }
}}

#endif // CPPSORT_DETAIL_LONGEST_NON_DESCENDING_SUBSEQUENCE_H_
