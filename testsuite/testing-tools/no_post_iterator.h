/*
 * Copyright (c) 2015-2017 Morwenn
 * SPDX-License-Identifier: MIT
 */
#ifndef CPPSORT_TESTSUITE_NO_POST_ITERATOR_H_
#define CPPSORT_TESTSUITE_NO_POST_ITERATOR_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <utility>
#include <cpp-sort/detail/iterator_traits.h> // yay, detail
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
        using value_type        = cppsort::detail::value_type_t<Iterator>;
        using difference_type   = cppsort::detail::difference_type_t<Iterator>;
        using pointer           = cppsort::detail::pointer_t<Iterator>;
        using reference         = cppsort::detail::reference_t<Iterator>;

        ////////////////////////////////////////////////////////////
        // Constructors

        no_post_iterator() = default;

        explicit no_post_iterator(Iterator it):
            _it(std::move(it))
        {}

        ////////////////////////////////////////////////////////////
        // Members access

        auto base() const
            -> iterator_type
        {
            return _it;
        }

        ////////////////////////////////////////////////////////////
        // Element access

        auto operator*() const
            -> decltype(*base())
        {
            return *base();
        }

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

        auto operator[](difference_type pos)
            -> decltype(base()[pos])
        {
            return base()[pos];
        }

        auto operator[](difference_type pos) const
            -> decltype(base()[pos])
        {
            return base()[pos];
        }

    private:

        Iterator _it;
};

template<typename Iterator>
auto iter_swap(no_post_iterator<Iterator> lhs, no_post_iterator<Iterator> rhs)
    -> void
{
    using cppsort::utility::iter_swap;
    iter_swap(lhs.base(), rhs.base());
}

////////////////////////////////////////////////////////////
// Comparison operators

template<typename Iterator1, typename Iterator2>
auto operator==(const no_post_iterator<Iterator1>& lhs,
                const no_post_iterator<Iterator2>& rhs)
    -> bool
{
    return lhs.base() == rhs.base();
}

template<typename Iterator1, typename Iterator2>
auto operator!=(const no_post_iterator<Iterator1>& lhs,
                const no_post_iterator<Iterator2>& rhs)
    -> bool
{
    return lhs.base() != rhs.base();
}

////////////////////////////////////////////////////////////
// Relational operators

template<typename Iterator1, typename Iterator2>
auto operator<(const no_post_iterator<Iterator1>& lhs,
               const no_post_iterator<Iterator2>& rhs)
    -> bool
{
    return lhs.base() < rhs.base();
}

template<typename Iterator1, typename Iterator2>
auto operator<=(const no_post_iterator<Iterator1>& lhs,
                const no_post_iterator<Iterator2>& rhs)
    -> bool
{
    return lhs.base() <= rhs.base();
}

template<typename Iterator1, typename Iterator2>
auto operator>(const no_post_iterator<Iterator1>& lhs,
               const no_post_iterator<Iterator2>& rhs)
    -> bool
{
    return lhs.base() > rhs.base();
}

template<typename Iterator1, typename Iterator2>
auto operator>=(const no_post_iterator<Iterator1>& lhs,
                const no_post_iterator<Iterator2>& rhs)
    -> bool
{
    return lhs.base() >= rhs.base();
}

////////////////////////////////////////////////////////////
// Arithmetic operators

template<typename Iterator>
auto operator+(no_post_iterator<Iterator> it,
               cppsort::detail::difference_type_t<no_post_iterator<Iterator>> size)
    -> no_post_iterator<Iterator>
{
    return it += size;
}

template<typename Iterator>
auto operator+(cppsort::detail::difference_type_t<no_post_iterator<Iterator>> size,
               no_post_iterator<Iterator> it)
    -> no_post_iterator<Iterator>
{
    return it += size;
}

template<typename Iterator>
auto operator-(no_post_iterator<Iterator> it,
               cppsort::detail::difference_type_t<no_post_iterator<Iterator>> size)
    -> no_post_iterator<Iterator>
{
    return it -= size;
}

template<typename Iterator>
auto operator-(const no_post_iterator<Iterator>& lhs, const no_post_iterator<Iterator>& rhs)
    -> cppsort::detail::difference_type_t<no_post_iterator<Iterator>>
{
    return lhs.base() - rhs.base();
}

////////////////////////////////////////////////////////////
// Construction function

template<typename Iterator>
auto make_no_post_iterator(Iterator it)
    -> no_post_iterator<Iterator>
{
    return no_post_iterator<Iterator>(std::move(it));
}

#endif // CPPSORT_TESTSUITE_NO_POST_ITERATOR_H_
