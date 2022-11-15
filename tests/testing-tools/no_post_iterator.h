/*
 * Copyright (c) 2015-2022 Morwenn
 * SPDX-License-Identifier: MIT
 */
#ifndef CPPSORT_TESTSUITE_NO_POST_ITERATOR_H_
#define CPPSORT_TESTSUITE_NO_POST_ITERATOR_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <iterator>
#include <utility>
#include <cpp-sort/detail/iterator_traits.h> // yay, detail
#include <cpp-sort/mstd/iterator.h>
#include <cpp-sort/utility/iter_move.h>

////////////////////////////////////////////////////////////
// Collection without post-inc/decrement iterators
//
// While standard iterator concepts require iterators to
// provide both post-incrementation and post-decrementation
// operators (if decrementation is supported), we shouldn't
// require them in cpp-sort: they do have their shortcomings
// and not every collection provides them
//
// Hence we create an iterator wrapper that does not provide
// post-increment not post-decrement operations
//

////////////////////////////////////////////////////////////
// Iterator wrapper

template<typename Iterator>
class no_post_iterator
{
    public:

        ////////////////////////////////////////////////////////////
        // Public types

        using iterator_category = cppsort::detail::iterator_category_t<Iterator>;
        using iterator_type     = Iterator;
        using value_type        = std::iter_value_t<Iterator>;
        using difference_type   = cppsort::mstd::iter_difference_t<Iterator>;
        using pointer           = cppsort::detail::pointer_t<Iterator>;
        using reference         = std::iter_reference_t<Iterator>;

        ////////////////////////////////////////////////////////////
        // Constructors

        no_post_iterator() = default;

        explicit no_post_iterator(Iterator it):
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
            -> no_post_iterator&
        {
            ++_it;
            return *this;
        }

        auto operator--()
            -> no_post_iterator&
        {
            --_it;
            return *this;
        }

        auto operator+=(difference_type increment)
            -> no_post_iterator&
        {
            _it += increment;
            return *this;
        }

        auto operator-=(difference_type increment)
            -> no_post_iterator&
        {
            _it -= increment;
            return *this;
        }

        ////////////////////////////////////////////////////////////
        // Elements access operators

        [[nodiscard]]
        auto operator[](difference_type pos) const
            -> decltype(base()[pos])
        {
            return _it[pos];
        }

        ////////////////////////////////////////////////////////////
        // Comparison operators

        [[nodiscard]]
        friend auto operator==(const no_post_iterator& lhs, const no_post_iterator& rhs)
            -> bool
        {
            return lhs.base() == rhs.base();
        }

        [[nodiscard]]
        friend auto operator!=(const no_post_iterator& lhs, const no_post_iterator& rhs)
            -> bool
        {
            return lhs.base() != rhs.base();
        }

        ////////////////////////////////////////////////////////////
        // Relational operators

        [[nodiscard]]
        friend auto operator<(const no_post_iterator& lhs, const no_post_iterator& rhs)
            -> bool
        {
            return lhs.base() < rhs.base();
        }

        [[nodiscard]]
        friend auto operator<=(const no_post_iterator& lhs, const no_post_iterator& rhs)
            -> bool
        {
            return lhs.base() <= rhs.base();
        }

        [[nodiscard]]
        friend auto operator>(const no_post_iterator& lhs, const no_post_iterator& rhs)
            -> bool
        {
            return lhs.base() > rhs.base();
        }

        [[nodiscard]]
        friend auto operator>=(const no_post_iterator& lhs, const no_post_iterator& rhs)
            -> bool
        {
            return lhs.base() >= rhs.base();
        }

        ////////////////////////////////////////////////////////////
        // Arithmetic operators

        [[nodiscard]]
        friend auto operator+(no_post_iterator it, difference_type size)
            -> no_post_iterator
        {
            it += size;
            return it;
        }

        [[nodiscard]]
        friend auto operator+(difference_type size, no_post_iterator it)
            -> no_post_iterator
        {
            it += size;
            return it;
        }

        [[nodiscard]]
        friend auto operator-(no_post_iterator it, difference_type size)
            -> no_post_iterator
        {
            it -= size;
            return it;
        }

        [[nodiscard]]
        friend auto operator-(const no_post_iterator& lhs, const no_post_iterator& rhs)
            -> difference_type
        {
            return lhs.base() - rhs.base();
        }

        ////////////////////////////////////////////////////////////
        // iter_swap

        friend auto iter_swap(no_post_iterator lhs, no_post_iterator rhs)
            -> void
        {
            using cppsort::utility::iter_swap;
            iter_swap(lhs.base(), rhs.base());
        }

    private:

        Iterator _it;
};

////////////////////////////////////////////////////////////
// Construction function

template<typename Iterator>
auto make_no_post_iterator(Iterator it)
    -> no_post_iterator<Iterator>
{
    return no_post_iterator<Iterator>(std::move(it));
}

#endif // CPPSORT_TESTSUITE_NO_POST_ITERATOR_H_
