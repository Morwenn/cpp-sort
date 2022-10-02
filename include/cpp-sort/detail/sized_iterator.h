/*
 * Copyright (c) 2020-2022 Morwenn
 * SPDX-License-Identifier: MIT
 */
#ifndef CPPSORT_DETAIL_SIZED_ITERATOR_H_
#define CPPSORT_DETAIL_SIZED_ITERATOR_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <utility>
#include "attributes.h"
#include "iterator_traits.h"

namespace cppsort
{
namespace detail
{
    ////////////////////////////////////////////////////////////
    // Mostly a hack to avoid some gratuitous performance loss
    // by passing bidirectional iterators + size to a function
    // accepting a pair of iterators. It is worse than the
    // equivalent C++20 features, but should be good enough for
    // the internal use we make of it.
    //
    // NOTE: the full iterator features are not provided, this
    //       is intentional to avoid unintentional uses of the
    //       class in the library's internals.

    template<typename Iterator>
    class sized_iterator
    {
        public:

            ////////////////////////////////////////////////////////////
            // Public types

            using iterator_category = iterator_category_t<Iterator>;
            using iterator_type     = Iterator;
            using value_type        = value_type_t<Iterator>;
            using difference_type   = difference_type_t<Iterator>;
            using pointer           = pointer_t<Iterator>;
            using reference         = reference_t<Iterator>;

            ////////////////////////////////////////////////////////////
            // Constructors

            sized_iterator() = default;

            constexpr sized_iterator(Iterator it, difference_type size):
                _it(std::move(it)),
                _size(size)
            {}

            ////////////////////////////////////////////////////////////
            // Members access

            CPPSORT_ATTRIBUTE_NODISCARD
            constexpr auto base() const
                -> iterator_type
            {
                return _it;
            }

            CPPSORT_ATTRIBUTE_NODISCARD
            constexpr auto size() const
                -> difference_type
            {
                return _size;
            }

            ////////////////////////////////////////////////////////////
            // Element access

            CPPSORT_ATTRIBUTE_NODISCARD
            constexpr auto operator*() const
                -> reference
            {
                return *_it;
            }

            CPPSORT_ATTRIBUTE_NODISCARD
            constexpr auto operator->() const
                -> pointer
            {
                return &(operator*());
            }

        private:

            Iterator _it;
            difference_type _size;
    };

    // Alternative to std::distance meant to be picked up by ADL in
    // specific places, uses the size of the *second* iterator
    template<typename Iterator>
    CPPSORT_ATTRIBUTE_NODISCARD
    constexpr auto distance(sized_iterator<Iterator>, sized_iterator<Iterator> last)
        -> difference_type_t<Iterator>
    {
        return last.size();
    }

    template<typename Iterator>
    CPPSORT_ATTRIBUTE_NODISCARD
    constexpr auto make_sized_iterator(Iterator it, difference_type_t<Iterator> size)
        -> sized_iterator<Iterator>
    {
        return { it, size };
    }

}}

#endif // CPPSORT_DETAIL_SIZED_ITERATOR_H_
