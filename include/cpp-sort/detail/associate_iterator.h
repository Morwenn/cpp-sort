/*
 * Copyright (c) 2016-2021 Morwenn
 * SPDX-License-Identifier: MIT
 */
#ifndef CPPSORT_DETAIL_ASSOCIATE_ITERATOR_H_
#define CPPSORT_DETAIL_ASSOCIATE_ITERATOR_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <type_traits>
#include <utility>
#include <cpp-sort/utility/iter_move.h>
#include "attributes.h"
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
        // Public types
        using iterator_type = Iterator;
        using data_type = Data;

        // Public members
        Iterator it;
        Data data;

        // Can't copy it, can't move-construct it either
        association(const association&) = delete;
        association(association&&) = delete;
        association& operator=(const association&) = delete;

        association(Iterator it, const Data& data):
            it(std::move(it)),
            data(data)
        {}

        association(Iterator it, Data&& data):
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

        auto operator=(associated_value<value_type_t<Iterator>, Data>&& other)
            -> association&
        {
            *it = std::move(other.value);
            data = std::move(other.data);
            return *this;
        }

        CPPSORT_ATTRIBUTE_NODISCARD
        auto get()
            -> decltype(*it)
        {
            return *it;
        }

        CPPSORT_ATTRIBUTE_NODISCARD
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

        CPPSORT_ATTRIBUTE_NODISCARD
        auto get()
            -> Value&
        {
            return value;
        }

        CPPSORT_ATTRIBUTE_NODISCARD
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

            using iterator_category = std::random_access_iterator_tag;
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

            CPPSORT_ATTRIBUTE_NODISCARD
            auto operator[](difference_type pos) const
                -> decltype(base()[pos])
            {
                return _it[pos];
            }

            ////////////////////////////////////////////////////////////
            // Comparison operators

            CPPSORT_ATTRIBUTE_NODISCARD
            friend auto operator==(const associate_iterator& lhs, const associate_iterator& rhs)
                -> bool
            {
                return lhs.base() == rhs.base();
            }

            CPPSORT_ATTRIBUTE_NODISCARD
            friend auto operator!=(const associate_iterator& lhs, const associate_iterator& rhs)
                -> bool
            {
                return lhs.base() != rhs.base();
            }

            ////////////////////////////////////////////////////////////
            // Relational operators

            CPPSORT_ATTRIBUTE_NODISCARD
            friend auto operator<(const associate_iterator& lhs, const associate_iterator& rhs)
                -> bool
            {
                return lhs.base() < rhs.base();
            }

            CPPSORT_ATTRIBUTE_NODISCARD
            friend auto operator<=(const associate_iterator& lhs, const associate_iterator& rhs)
                -> bool
            {
                return lhs.base() <= rhs.base();
            }

            CPPSORT_ATTRIBUTE_NODISCARD
            friend auto operator>(const associate_iterator& lhs, const associate_iterator& rhs)
                -> bool
            {
                return lhs.base() > rhs.base();
            }

            CPPSORT_ATTRIBUTE_NODISCARD
            friend auto operator>=(const associate_iterator& lhs, const associate_iterator& rhs)
                -> bool
            {
                return lhs.base() >= rhs.base();
            }

            ////////////////////////////////////////////////////////////
            // Arithmetic operators

            CPPSORT_ATTRIBUTE_NODISCARD
            friend auto operator+(associate_iterator it, difference_type size)
                -> associate_iterator
            {
                it += size;
                return it;
            }

            CPPSORT_ATTRIBUTE_NODISCARD
            friend auto operator+(difference_type size, associate_iterator it)
                -> associate_iterator
            {
                it += size;
                return it;
            }

            CPPSORT_ATTRIBUTE_NODISCARD
            friend auto operator-(associate_iterator it, difference_type size)
                -> associate_iterator
            {
                it -= size;
                return it;
            }

            CPPSORT_ATTRIBUTE_NODISCARD
            friend auto operator-(const associate_iterator& lhs, const associate_iterator& rhs)
                -> difference_type
            {
                return lhs.base() - rhs.base();
            }

            ////////////////////////////////////////////////////////////
            // iter_move/iter_swap

            friend auto iter_swap(associate_iterator lhs, associate_iterator rhs)
                -> void
            {
                using utility::iter_swap;
                iter_swap(lhs.base(), rhs.base());
            }

            CPPSORT_ATTRIBUTE_NODISCARD
            friend auto iter_move(associate_iterator it)
                -> associated_value<
                    value_type_t<typename value_type_t<Iterator>::iterator_type>,
                    typename value_type_t<Iterator>::data_type
                >
            {
                return {
                    std::move(*(it->it)),
                    std::move(it->data)
                };
            }

        private:

            Iterator _it;
    };

    ////////////////////////////////////////////////////////////
    // Construction function

    template<typename Iterator>
    CPPSORT_ATTRIBUTE_NODISCARD
    auto make_associate_iterator(Iterator it)
        -> associate_iterator<Iterator>
    {
        return associate_iterator<Iterator>(std::move(it));
    }
}}

#endif // CPPSORT_DETAIL_ASSOCIATE_ITERATOR_H_
