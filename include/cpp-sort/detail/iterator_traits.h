/*
 * Copyright (c) 2016-2024 Morwenn
 * SPDX-License-Identifier: MIT
 */
#ifndef CPPSORT_DETAIL_ITERATOR_TRAITS_H_
#define CPPSORT_DETAIL_ITERATOR_TRAITS_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <iterator>
#include <type_traits>
#include <utility>
#include <cpp-sort/mstd/iterator.h>

namespace cppsort::detail
{
    // A few type aliases to make it easier to get types
    // associated to indirect and iterator types

    template<typename Iterator>
    using pointer_t = typename std::iterator_traits<Iterator>::pointer;

    template<typename Iterator>
    using iterator_category_t = typename std::iterator_traits<Iterator>::iterator_category;

    template<typename Iterator>
    using iterator_concept_t = typename mstd::detail::iter_concept<Iterator>;

    // Additional common type to use instead of value_t
    template<typename Iterator>
    using rvalue_type_t = std::remove_cvref_t<mstd::iter_rvalue_reference_t<Iterator>>;

    // Handy addition from time to time
    template<typename Iterator, typename Projection>
    using projected_t = std::remove_cvref_t<std::invoke_result_t<Projection, decltype(*std::declval<Iterator&>())>>;
}

#endif // CPPSORT_DETAIL_ITERATOR_TRAITS_H_
