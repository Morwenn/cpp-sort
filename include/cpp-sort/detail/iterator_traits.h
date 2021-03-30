/*
 * Copyright (c) 2016-2018 Morwenn
 * SPDX-License-Identifier: MIT
 */
#ifndef CPPSORT_DETAIL_ITERATOR_TRAITS_H_
#define CPPSORT_DETAIL_ITERATOR_TRAITS_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <iterator>
#include <utility>
#include <cpp-sort/utility/iter_move.h>
#include "type_traits.h"

namespace cppsort
{
namespace detail
{
    //
    // A few type aliases to make it easier to write
    // code that uses std::iterator_traits; there are
    // equivalent aliases in the Ranges TS
    //

    template<typename Iterator>
    using difference_type_t = typename std::iterator_traits<Iterator>::difference_type;

    template<typename Iterator>
    using value_type_t = typename std::iterator_traits<Iterator>::value_type;

    template<typename Iterator>
    using pointer_t = typename std::iterator_traits<Iterator>::pointer;

    template<typename Iterator>
    using reference_t = typename std::iterator_traits<Iterator>::reference;

    template<typename Iterator>
    using iterator_category_t = typename std::iterator_traits<Iterator>::iterator_category;

    // Addition used by proxy iterators from P0022
    template<typename Iterator>
    using rvalue_reference_t = utility::rvalue_reference_t<Iterator>;

    // Additional common type to use instead of value_t
    template<typename Iterator>
    using rvalue_type_t = remove_cvref_t<utility::rvalue_reference_t<Iterator>>;

    // Handy addition from time to time
    template<typename Iterator, typename Projection>
    using projected_t = remove_cvref_t<invoke_result_t<Projection, decltype(*std::declval<Iterator&>())>>;
}}

#endif // CPPSORT_DETAIL_ITERATOR_TRAITS_H_
