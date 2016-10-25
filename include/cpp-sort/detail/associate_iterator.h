/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2016 Morwenn
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */
#ifndef CPPSORT_DETAIL_ASSOCIATE_ITERATOR_H_
#define CPPSORT_DETAIL_ASSOCIATE_ITERATOR_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <type_traits>
#include <utility>
#include <cpp-sort/utility/iter_move.h>
#include "iterator_traits.h"

namespace cppsort
{
namespace detail
{
    //
    // This header contains utility classes used when an adapter
    // needs to associate a value to every element of a collection
    // and uses this value to sort the collection
    //
    // The iterator will be associated to a value in association,
    // and the utility class associate_iterator will make sure that
    // the changes are propagated to the original collection
    //
    // The class associated_value is used when an element is moved
    // out of the original collection, to make sure that it is still
    // associated to the additional data
    //

    template<typename Iterator, typename Data>
    struct association;

    template<typename Value, typename Data>
    struct associated_value;

    template<typename Iterator, typename Data>
    struct association
    {
        // Public members
        Iterator it;
        Data data;

        // Can't copy it, can't move-construct it either
        association(const association&) = delete;
        association(association&&) = delete;
        association& operator=(const association&) = delete;

        association(Iterator it, Data data):
            it(std::move(it)),
            data(std::move(data))
        {}

        auto operator=(association&& other) noexcept
            -> association&
        {
            *it = std::move(*other.it);
            data = std::move(other.data);
            return *this;
        }

        auto operator=(associated_value<std::decay_t<decltype(*it)>, Data>&& other)
            -> association&
        {
            *it = std::move(other.value);
            data = std::move(other.data);
            return *this;
        }

        auto get()
            -> decltype(*it)
        {
            return *it;
        }

        auto get() const
            -> decltype(*it)
        {
            return *it;
        }
    };

    template<typename Iterator, typename Data>
    auto swap(association<Iterator, Data>& lhs, association<Iterator, Data>& rhs)
        -> void
    {
        using std::swap;
        using utility::iter_swap;
        iter_swap(lhs.it, rhs.it);
        swap(lhs.data, rhs.data);
    }

    template<typename Value, typename Data>
    struct associated_value
    {
        // Public members
        Value value;
        Data data;

        // Can't copy it, can't move-construct it either
        associated_value(const associated_value&) = delete;
        associated_value& operator=(const associated_value&) = delete;

        associated_value(Value&& value, Data&& data):
            value(std::move(value)),
            data(std::move(data))
        {}

        associated_value(associated_value&& other):
            value(std::move(other.value)),
            data(std::move(other.data))
        {}

        auto operator=(associated_value&& other)
            -> associated_value&
        {
            value = std::move(other.value);
            data = std::move(other.data);
            return *this;
        }

        auto get()
            -> Value&
        {
            return value;
        }

        auto get() const
            -> const Value&
        {
            return value;
        }

        // Silence GCC -Winline warning
        ~associated_value() noexcept {}
    };

    ////////////////////////////////////////////////////////////
    // Iterator used to sort groups

    template<typename Iterator>
    class associate_iterator
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

            associate_iterator() = default;

            explicit associate_iterator(Iterator it):
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
                -> associate_iterator&
            {
                ++_it;
                return *this;
            }

            auto operator++(int)
                -> associate_iterator
            {
                auto tmp = *this;
                operator++();
                return tmp;
            }

            auto operator--()
                -> associate_iterator&
            {
                --_it;
                return *this;
            }

            auto operator--(int)
                -> associate_iterator
            {
                auto tmp = *this;
                operator--();
                return tmp;
            }

            auto operator+=(difference_type increment)
                -> associate_iterator&
            {
                _it += increment;
                return *this;
            }

            auto operator-=(difference_type increment)
                -> associate_iterator&
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
    auto iter_swap(associate_iterator<Iterator> lhs, associate_iterator<Iterator> rhs)
        -> void
    {
        using utility::iter_swap;
        iter_swap(lhs.base(), rhs.base());
    }

    template<typename Iterator>
    auto iter_move(associate_iterator<Iterator> it)
        -> associated_value<
            std::decay_t<decltype(*(it->it))>,
            std::decay_t<decltype(it->data)>
        >
    {
        return {
            std::move(*(it->it)),
            std::move(it->data)
        };
    }

    ////////////////////////////////////////////////////////////
    // Comparison operators

    template<typename Iterator1, typename Iterator2>
    auto operator==(const associate_iterator<Iterator1>& lhs,
                    const associate_iterator<Iterator2>& rhs)
        -> bool
    {
        return lhs.base() == rhs.base();
    }

    template<typename Iterator1, typename Iterator2>
    auto operator!=(const associate_iterator<Iterator1>& lhs,
                    const associate_iterator<Iterator2>& rhs)
        -> bool
    {
        return lhs.base() != rhs.base();
    }

    ////////////////////////////////////////////////////////////
    // Relational operators

    template<typename Iterator1, typename Iterator2>
    auto operator<(const associate_iterator<Iterator1>& lhs,
                   const associate_iterator<Iterator2>& rhs)
        -> bool
    {
        return lhs.base() < rhs.base();
    }

    template<typename Iterator1, typename Iterator2>
    auto operator<=(const associate_iterator<Iterator1>& lhs,
                    const associate_iterator<Iterator2>& rhs)
        -> bool
    {
        return lhs.base() <= rhs.base();
    }

    template<typename Iterator1, typename Iterator2>
    auto operator>(const associate_iterator<Iterator1>& lhs,
                   const associate_iterator<Iterator2>& rhs)
        -> bool
    {
        return lhs.base() > rhs.base();
    }

    template<typename Iterator1, typename Iterator2>
    auto operator>=(const associate_iterator<Iterator1>& lhs,
                    const associate_iterator<Iterator2>& rhs)
        -> bool
    {
        return lhs.base() >= rhs.base();
    }

    ////////////////////////////////////////////////////////////
    // Arithmetic operators

    template<typename Iterator>
    auto operator+(associate_iterator<Iterator> it, difference_type_t<associate_iterator<Iterator>> size)
        -> associate_iterator<Iterator>
    {
        return it += size;
    }

    template<typename Iterator>
    auto operator+(difference_type_t<associate_iterator<Iterator>> size, associate_iterator<Iterator> it)
        -> associate_iterator<Iterator>
    {
        return it += size;
    }

    template<typename Iterator>
    auto operator-(associate_iterator<Iterator> it, difference_type_t<associate_iterator<Iterator>> size)
        -> associate_iterator<Iterator>
    {
        return it -= size;
    }

    template<typename Iterator>
    auto operator-(const associate_iterator<Iterator>& lhs, const associate_iterator<Iterator>& rhs)
        -> difference_type_t<associate_iterator<Iterator>>
    {
        return lhs.base() - rhs.base();
    }

    ////////////////////////////////////////////////////////////
    // Construction function

    template<typename Iterator>
    auto make_associate_iterator(Iterator it)
        -> associate_iterator<Iterator>
    {
        return associate_iterator<Iterator>(std::move(it));
    }
}}

#endif // CPPSORT_DETAIL_ASSOCIATE_ITERATOR_H_
