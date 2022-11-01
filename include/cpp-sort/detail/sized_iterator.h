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
#include <cpp-sort/mstd/iterator.h>
#include "config.h"
#include "iterator_traits.h"

namespace cppsort::detail
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

            [[nodiscard]]
            auto base() const
                -> iterator_type
            {
                return _it;
            }

            [[nodiscard]]
            auto size() const
                -> difference_type
            {
                return _size;
            }

            ////////////////////////////////////////////////////////////
            // Element access

            [[nodiscard]]
            auto operator*() const
                -> reference
            {
                return *_it;
            }

            [[nodiscard]]
            auto operator->() const
                -> pointer
            {
                return &(operator*());
            }

            ////////////////////////////////////////////////////////////
            // Increment/decrement operators

            auto operator++()
                -> sized_iterator&
            {
                CPPSORT_UNREACHABLE;
                return *this;
            }
            auto operator--()
                -> sized_iterator&
            {
                CPPSORT_UNREACHABLE;
                return *this;
            }

            ////////////////////////////////////////////////////////////
            // operator-
            // Exists for mstd::distance to take advantage of

            friend constexpr auto operator-(const sized_iterator& lhs, const sized_iterator& rhs)
                -> difference_type
            {
                return lhs._size - rhs._size;
            }
            
            ////////////////////////////////////////////////////////////
            // Comparison operators
            // Shall not be called

            [[nodiscard]]
            friend auto operator==(const sized_iterator& lhs, const sized_iterator& rhs)
                -> bool
            {
                CPPSORT_UNREACHABLE;
                return lhs.base() == rhs.base();
            }

            [[nodiscard]]
            friend auto operator!=(const sized_iterator& lhs, const sized_iterator& rhs)
                -> bool
            {
                CPPSORT_UNREACHABLE;
                return lhs.base() != rhs.base();
            }

        private:

            Iterator _it;
            difference_type _size;
    };

    template<typename Iterator>
    [[nodiscard]]
    auto make_sized_iterator(Iterator it, difference_type_t<Iterator> size)
        -> sized_iterator<Iterator>
    {
        return { it, size };
    }
}

#endif // CPPSORT_DETAIL_SIZED_ITERATOR_H_
