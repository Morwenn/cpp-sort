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
#include <cpp-sort/utility/iter_move.h>
#include "attributes.h"
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
            using value_type        = value_type_t<Iterator>;
            using difference_type   = difference_type_t<Iterator>;
            using pointer           = pointer_t<Iterator>;
            using reference         = reference_t<Iterator>;

            ////////////////////////////////////////////////////////////
            // Constructors

            fake_category_iterator() = default;

            explicit fake_category_iterator(Iterator it):
                _it(std::move(it))
            {}

            ////////////////////////////////////////////////////////////
            // Members access

            CPPSORT_ATTRIBUTE_NODISCARD
            auto base() const
                -> iterator_type
            {
                return _it;
            }

            ////////////////////////////////////////////////////////////
            // Element access

            CPPSORT_ATTRIBUTE_NODISCARD
            auto operator*() const
                -> decltype(*base())
            {
                return *_it;
            }

            CPPSORT_ATTRIBUTE_NODISCARD
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
            {
                --_it;
                return *this;
            }

            auto operator--(int)
                -> fake_category_iterator
            {
                auto res = *this;
                --_it;
                return res;
            }

            auto operator+=(difference_type increment)
                -> fake_category_iterator&
            {
                _it += increment;
                return *this;
            }

            auto operator-=(difference_type increment)
                -> fake_category_iterator&
            {
                _it -= increment;
                return *this;
            }

            ////////////////////////////////////////////////////////////
            // Elements access operators

            CPPSORT_ATTRIBUTE_NODISCARD
            auto operator[](difference_type pos) const
                -> decltype(base()[pos])
            {
                return _it[pos];
            }

            ////////////////////////////////////////////////////////////
            // Comparison operators

            CPPSORT_ATTRIBUTE_NODISCARD
            friend auto operator==(const fake_category_iterator& lhs, const fake_category_iterator& rhs)
                -> bool
            {
                return lhs.base() == rhs.base();
            }

            CPPSORT_ATTRIBUTE_NODISCARD
            friend auto operator!=(const fake_category_iterator& lhs, const fake_category_iterator& rhs)
                -> bool
            {
                return lhs.base() != rhs.base();
            }

            ////////////////////////////////////////////////////////////
            // Relational operators

            CPPSORT_ATTRIBUTE_NODISCARD
            friend auto operator<(const fake_category_iterator& lhs, const fake_category_iterator& rhs)
                -> bool
            {
                return lhs.base() < rhs.base();
            }

            CPPSORT_ATTRIBUTE_NODISCARD
            friend auto operator<=(const fake_category_iterator& lhs, const fake_category_iterator& rhs)
                -> bool
            {
                return lhs.base() <= rhs.base();
            }

            CPPSORT_ATTRIBUTE_NODISCARD
            friend auto operator>(const fake_category_iterator& lhs, const fake_category_iterator& rhs)
                -> bool
            {
                return lhs.base() > rhs.base();
            }

            CPPSORT_ATTRIBUTE_NODISCARD
            friend auto operator>=(const fake_category_iterator& lhs, const fake_category_iterator& rhs)
                -> bool
            {
                return lhs.base() >= rhs.base();
            }

            ////////////////////////////////////////////////////////////
            // Arithmetic operators

            CPPSORT_ATTRIBUTE_NODISCARD
            friend auto operator+(fake_category_iterator it, difference_type size)
                -> fake_category_iterator
            {
                it += size;
                return it;
            }

            CPPSORT_ATTRIBUTE_NODISCARD
            friend auto operator+(difference_type size, fake_category_iterator it)
                -> fake_category_iterator
            {
                it += size;
                return it;
            }

            CPPSORT_ATTRIBUTE_NODISCARD
            friend auto operator-(fake_category_iterator it, difference_type size)
                -> fake_category_iterator
            {
                it -= size;
                return it;
            }

            CPPSORT_ATTRIBUTE_NODISCARD
            friend auto operator-(const fake_category_iterator& lhs, const fake_category_iterator& rhs)
                -> difference_type
            {
                return lhs.base() - rhs.base();
            }

            ////////////////////////////////////////////////////////////
            // iter_move/iter_swap

            friend auto iter_swap(fake_category_iterator lhs, fake_category_iterator rhs)
                -> void
            {
                using utility::iter_swap;
                iter_swap(lhs.base(), rhs.base());
            }

            CPPSORT_ATTRIBUTE_NODISCARD
            friend auto iter_move(fake_category_iterator it)
                -> decltype(auto)
            {
                using utility::iter_move;
                return iter_move(it.base());
            }

        private:

            Iterator _it;
    };
}}

#endif // CPPSORT_DETAIL_FAKE_CATEGORY_ITERATOR_H_
