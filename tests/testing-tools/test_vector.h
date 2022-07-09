/*
 * Copyright (c) 2021-2022 Morwenn
 * SPDX-License-Identifier: MIT
 */
#ifndef CPPSORT_TESTSUITE_TEST_VECTOR_H_
#define CPPSORT_TESTSUITE_TEST_VECTOR_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <cstddef>
#include <iterator>
#include <memory>
#include <new>
#include <utility>
#include <stdexcept>

////////////////////////////////////////////////////////////
// Vector with a tiny size
//
// Customizable vector type for testing purposes
//

struct test_vector_error:
    std::runtime_error
{
    using std::runtime_error::runtime_error;
};

template<typename T, typename DifferenceType>
class test_vector;

template<typename T, typename DifferenceType>
class test_vector_iterator
{
    public:

        ////////////////////////////////////////////////////////////
        // Public types

        using iterator_category = std::random_access_iterator_tag;
        using value_type        = T;
        using difference_type   = DifferenceType;
        using pointer           = value_type*;
        using reference         = value_type&;

        ////////////////////////////////////////////////////////////
        // Constructors & operator=

        test_vector_iterator() = default;
        test_vector_iterator(const test_vector_iterator&) = default;
        test_vector_iterator(test_vector_iterator&&) = default;
        test_vector_iterator& operator=(const test_vector_iterator&) = default;
        test_vector_iterator& operator=(test_vector_iterator&&) = default;

        constexpr explicit test_vector_iterator(T* ptr, test_vector<T, DifferenceType>* owner):
            ptr_(ptr),
            owner_(owner)
        {}

        ////////////////////////////////////////////////////////////
        // Element access

        auto operator*() const
            -> reference
        {
            return *ptr_;
        }

        auto operator->() const
            -> pointer
        {
            return &(operator*());
        }

        ////////////////////////////////////////////////////////////
        // Increment/decrement operators

        auto operator++()
            -> test_vector_iterator&
        {
            if (ptr_ == owner_->end_) {
                throw test_vector_error("operator++ creates out-of-bounds iterator");
            }
            ++ptr_;
            return *this;
        }

        auto operator++(int)
            -> test_vector_iterator
        {
            auto tmp = *this;
            operator++();
            return tmp;
        }

        auto operator--()
            -> test_vector_iterator&
        {
            if (ptr_ == owner_->memory_) {
                throw test_vector_error("operator-- creates out-of-bounds iterator");
            }
            --ptr_;
            return *this;
        }

        auto operator--(int)
            -> test_vector_iterator
        {
            auto tmp = *this;
            operator--();
            return tmp;
        }

        auto operator+=(difference_type increment)
            -> test_vector_iterator&
        {
            if (ptr_ + increment > owner_->end_) {
                throw test_vector_error("operator+= creates out-of-bounds iterator (end)");
            }
            if (ptr_ + increment < owner_->memory_) {
                throw test_vector_error("operator+= creates out-of-bounds iterator (begin)");
            }
            ptr_ += increment;
            return *this;
        }

        auto operator-=(difference_type increment)
            -> test_vector_iterator&
        {
            if (ptr_ - increment < owner_->memory_) {
                throw test_vector_error("operator-= creates out-of-bounds iterator (begin)");
            }
            if (ptr_ - increment > owner_->end_) {
                throw test_vector_error("operator-= creates out-of-bounds iterator (end)");
            }
            ptr_ -= increment;
            return *this;
        }

        ////////////////////////////////////////////////////////////
        // Elements access operators

        auto operator[](difference_type pos) const
            -> reference
        {
            if (ptr_ + pos > owner_->end_) {
                throw test_vector_error("operator[] creates out-of-bounds iterator (end)");
            }
            if (ptr_ + pos < owner_->memory_) {
                throw test_vector_error("operator[] creates out-of-bounds iterator (begin)");
            }
            return ptr_[pos];
        }

        ////////////////////////////////////////////////////////////
        // Comparison operators

        friend constexpr auto operator==(const test_vector_iterator& lhs, const test_vector_iterator& rhs)
            -> bool
        {
            return lhs.ptr_ == rhs.ptr_;
        }

        friend constexpr auto operator!=(const test_vector_iterator& lhs, const test_vector_iterator& rhs)
            -> bool
        {
            return lhs.ptr_ != rhs.ptr_;
        }

        ////////////////////////////////////////////////////////////
        // Relational operators

        friend auto operator<(const test_vector_iterator& lhs, const test_vector_iterator& rhs)
            -> bool
        {
            return lhs.ptr_ < rhs.ptr_;
        }

        friend auto operator<=(const test_vector_iterator& lhs, const test_vector_iterator& rhs)
            -> bool
        {
            return lhs.ptr_ <= rhs.ptr_;
        }

        friend auto operator>(const test_vector_iterator& lhs, const test_vector_iterator& rhs)
            -> bool
        {
            return lhs.ptr_ > rhs.ptr_;
        }

        friend auto operator>=(const test_vector_iterator& lhs, const test_vector_iterator& rhs)
            -> bool
        {
            return lhs.ptr_ >= rhs.ptr_;
        }

        ////////////////////////////////////////////////////////////
        // Arithmetic operators

        friend auto operator+(test_vector_iterator it, difference_type size)
            -> test_vector_iterator
        {
            it += size;
            return it;
        }

        friend auto operator+(difference_type size, test_vector_iterator it)
            -> test_vector_iterator
        {
            it += size;
            return it;
        }

        friend auto operator-(test_vector_iterator it, difference_type size)
            -> test_vector_iterator
        {
            it -= size;
            return it;
        }

        friend auto operator-(const test_vector_iterator& lhs, const test_vector_iterator& rhs)
            -> difference_type
        {
            return lhs.ptr_ - rhs.ptr_;
        }

    private:

        T* ptr_ = nullptr;
        test_vector<T, DifferenceType>* owner_ = nullptr;
};

template<typename T, typename DifferenceType>
class test_vector
{
    friend class test_vector_iterator<T, DifferenceType>;

    public:

        ////////////////////////////////////////////////////////////
        // Member types

        using value_type = T;
        using difference_type = DifferenceType;
        using reference = value_type&;
        using const_reference = const value_type&;
        using iterator = test_vector_iterator<T, DifferenceType>;

        ////////////////////////////////////////////////////////////
        // Make the collection immovable

        test_vector() = default;
        test_vector(const test_vector&) = delete;
        test_vector(test_vector&&) = delete;
        test_vector& operator=(const test_vector&) = delete;
        test_vector& operator=(test_vector&&) = delete;

        ////////////////////////////////////////////////////////////
        // Construction

        explicit test_vector(difference_type n):
            capacity_(n),
            memory_(
                static_cast<T*>(::operator new(n * sizeof(T)))
            ),
            end_(memory_)
        {}

        ////////////////////////////////////////////////////////////
        // Destruction

        ~test_vector()
        {
            // Destroy the constructed elements
            std::destroy(memory_, end_);
            // Free the allocated memory
            ::operator delete(memory_);
        }

        ////////////////////////////////////////////////////////////
        // Element access

        auto operator[](difference_type pos)
            -> T&
        {
            if (pos < 0) {
                throw test_vector_error("out-of-bounds access in operator[]");
            }
            return memory_[pos];
        }

        auto front()
            -> T&
        {
            if (memory_ == end_) {
                throw test_vector_error("front() called on empty collection");
            }
            return *memory_;
        }

        auto back()
            -> T&
        {
            if (memory_ == end_) {
                throw test_vector_error("back() called on empty collection");
            }
            return *(end_ - 1);
        }

        ////////////////////////////////////////////////////////////
        // Iterators

        auto begin()
            -> iterator
        {
            return iterator(memory_, this);
        }

        auto end()
            -> iterator
        {
            return iterator(end_, this);
        }

        ////////////////////////////////////////////////////////////
        // Modifiers

        auto clear() noexcept
            -> void
        {
            // Destroy the constructed elements
            std::destroy(memory_, end_);
            // Ensure the new size is zero
            end_ = memory_;
        }

        auto push_back(T& value)
            -> void
        {
            emplace_back(value);
        }

        auto push_back(T&& value)
            -> void
        {
            emplace_back(std::move(value));
        }

        template<typename... Args>
        auto emplace_back(Args&&... args)
            -> void
        {
            if (end_ - memory_ >= capacity_) {
                throw test_vector_error("capacity reached when calling emplace_back()");
            }
            ::new(end_) T(std::forward<Args>(args)...);
            ++end_;
        }

    private:

        std::ptrdiff_t capacity_ = 0;
        T* memory_ = nullptr;
        T* end_ = nullptr;
};

#endif // CPPSORT_TESTSUITE_TEST_VECTOR_H_
