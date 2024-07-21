/*
 * Copyright (c) 2024 Morwenn
 * SPDX-License-Identifier: MIT
 */
#ifndef CPPSORT_DETAIL_FAKE_CATEGORY_ITERATOR_H_
#define CPPSORT_DETAIL_FAKE_CATEGORY_ITERATOR_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <iterator>
#include <type_traits>
#include <utility>
#include <cpp-sort/mstd/iterator.h>
#include "iterator_traits.h"
#include "type_traits.h"

namespace cppsort
{
namespace detail
{
    ////////////////////////////////////////////////////////////
    // Iterator with fàake iterator category: it wrap a given
    // iterator and pretends to have a different iterator
    // category as specified by users

    template<typename Iterator, typename IteratorTag>
    class fake_category_iterator
    {
        using real_category_tag = iterator_category_t<Iterator>;
        static_assert(
            std::is_base_of<IteratorTag, real_category_tag>::value,
            "The iterator tag of fake_iterator_category must be less "
            "specific than that of the wrapped one"
        );

        public:

            ////////////////////////////////////////////////////////////
            // Public types

            using iterator_category = IteratorTag;
            using iterator_type     = Iterator;
            using value_type        = std::iter_value_t<Iterator>;
            using difference_type   = mstd::iter_difference_t<Iterator>;
            using pointer           = pointer_t<Iterator>;
            using reference         = std::iter_reference_t<Iterator>;

            ////////////////////////////////////////////////////////////
            // Constructors

            fake_category_iterator() = default;

            explicit fake_category_iterator(Iterator it):
                _it(std::move(it))
            {}

            ////////////////////////////////////////////////////////////
            // Members access

            [[nodiscard]]
            auto base() const
                -> iterator_type
            {
                return _it;
            }

            ////////////////////////////////////////////////////////////
            // Element access

            [[nodiscard]]
            auto operator*() const
                -> decltype(*base())
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
                -> fake_category_iterator&
            {
                ++_it;
                return *this;
            }

            auto operator++(int)
                -> fake_category_iterator
            {
                auto res = *this;
                ++_it;
                return res;
            }

            auto operator--()
                -> fake_category_iterator&
                requires mstd::bidirectional_iterator<Iterator>
            {
                --_it;
                return *this;
            }

            auto operator--(int)
                -> fake_category_iterator
                requires mstd::bidirectional_iterator<Iterator>
            {
                auto res = *this;
                --_it;
                return res;
            }

            auto operator+=(difference_type increment)
                -> fake_category_iterator&
                requires mstd::random_access_iterator<Iterator>
            {
                _it += increment;
                return *this;
            }

            auto operator-=(difference_type increment)
                -> fake_category_iterator&
                requires mstd::random_access_iterator<Iterator>
            {
                _it -= increment;
                return *this;
            }

            ////////////////////////////////////////////////////////////
            // Elements access operators

            [[nodiscard]]
            auto operator[](difference_type pos) const
                -> decltype(base()[pos])
                requires mstd::random_access_iterator<Iterator>
            {
                return _it[pos];
            }

            ////////////////////////////////////////////////////////////
            // Comparison operators

            [[nodiscard]]
            friend auto operator==(const fake_category_iterator& lhs, const fake_category_iterator& rhs)
                -> bool
            {
                return lhs.base() == rhs.base();
            }

            [[nodiscard]]
            friend auto operator!=(const fake_category_iterator& lhs, const fake_category_iterator& rhs)
                -> bool
            {
                return lhs.base() != rhs.base();
            }

            ////////////////////////////////////////////////////////////
            // Relational operators

            [[nodiscard]]
            friend auto operator<(const fake_category_iterator& lhs, const fake_category_iterator& rhs)
                -> bool
                requires mstd::random_access_iterator<Iterator>
            {
                return lhs.base() < rhs.base();
            }

            [[nodiscard]]
            friend auto operator<=(const fake_category_iterator& lhs, const fake_category_iterator& rhs)
                -> bool
                requires mstd::random_access_iterator<Iterator>
            {
                return lhs.base() <= rhs.base();
            }

            [[nodiscard]]
            friend auto operator>(const fake_category_iterator& lhs, const fake_category_iterator& rhs)
                -> bool
                requires mstd::random_access_iterator<Iterator>
            {
                return lhs.base() > rhs.base();
            }

            [[nodiscard]]
            friend auto operator>=(const fake_category_iterator& lhs, const fake_category_iterator& rhs)
                -> bool
                requires mstd::random_access_iterator<Iterator>
            {
                return lhs.base() >= rhs.base();
            }

            ////////////////////////////////////////////////////////////
            // Arithmetic operators

            [[nodiscard]]
            friend auto operator+(fake_category_iterator it, difference_type size)
                -> fake_category_iterator
                requires mstd::random_access_iterator<Iterator>
            {
                it += size;
                return it;
            }

            [[nodiscard]]
            friend auto operator+(difference_type size, fake_category_iterator it)
                -> fake_category_iterator
                requires mstd::random_access_iterator<Iterator>
            {
                it += size;
                return it;
            }

            [[nodiscard]]
            friend auto operator-(fake_category_iterator it, difference_type size)
                -> fake_category_iterator
                requires mstd::random_access_iterator<Iterator>
            {
                it -= size;
                return it;
            }

            [[nodiscard]]
            friend auto operator-(const fake_category_iterator& lhs, const fake_category_iterator& rhs)
                -> difference_type
                requires mstd::random_access_iterator<Iterator>
            {
                return lhs.base() - rhs.base();
            }

            ////////////////////////////////////////////////////////////
            // iter_move/iter_swap

            friend auto iter_swap(fake_category_iterator lhs, fake_category_iterator rhs)
                -> void
            {
                mstd::iter_swap(lhs.base(), rhs.base());
            }

            [[nodiscard]]
            friend auto iter_move(fake_category_iterator it)
                -> mstd::iter_rvalue_reference_t<Iterator>
            {
                return mstd::iter_move(it.base());
            }

        private:

            Iterator _it;
    };
}}

#endif // CPPSORT_DETAIL_FAKE_CATEGORY_ITERATOR_H_
