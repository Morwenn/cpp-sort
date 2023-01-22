/*
 * Copyright (c) 2021-2023 Morwenn
 * SPDX-License-Identifier: MIT
 */
#ifndef CPPSORT_DETAIL_IMMOVABLE_VECTOR_H_
#define CPPSORT_DETAIL_IMMOVABLE_VECTOR_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <cstddef>
#include <memory>
#include <utility>
#include <cpp-sort/mstd/iterator.h>
#include "config.h"

namespace cppsort::detail
{
    ////////////////////////////////////////////////////////////
    // Immovable vector
    //
    // Lightweight class similar to std::vector, but with a few
    // key differences: it allocates a buffer sufficient to store
    // a given number of elements during construction, and it can
    // not be moved. While its original goal was to provide an
    // std::vector-like class for immovable types, it is also
    // through the library as a light contiguous collection when
    // the number of elements to allocate is already known at
    // construction time.

    template<typename T>
    class immovable_vector
    {
        public:

            // Make the collection immovable
            immovable_vector(const immovable_vector&) = delete;
            immovable_vector(immovable_vector&&) = delete;
            immovable_vector& operator=(const immovable_vector&) = delete;
            immovable_vector& operator=(immovable_vector&&) = delete;

            ////////////////////////////////////////////////////////////
            // Construction

            constexpr explicit immovable_vector(std::ptrdiff_t n):
                alloc_(),
                capacity_(n),
                memory_(alloc_.allocate(n)),
                end_(memory_)
            {}

            ////////////////////////////////////////////////////////////
            // Destruction

            constexpr ~immovable_vector()
            {
                // Destroy the constructed elements
                std::destroy(memory_, end_);

                // Free the allocated memory
                alloc_.deallocate(memory_, capacity_);
            }

            ////////////////////////////////////////////////////////////
            // Element access

            constexpr auto operator[](std::ptrdiff_t pos) noexcept
                -> T&
            {
                CPPSORT_ASSERT(pos <= end_ - memory_);
                return memory_[pos];
            }

            auto front() noexcept
                -> T&
            {
                CPPSORT_ASSERT(memory_ != end_);
                return *memory_;
            }

            auto back() noexcept
                -> T&
            {
                CPPSORT_ASSERT(end_ - memory_ > 0);
                return *(end_ - 1);
            }

            ////////////////////////////////////////////////////////////
            // Iterators

            constexpr auto begin() noexcept
                -> T*
            {
                return memory_;
            }

            constexpr auto end() noexcept
                -> T*
            {
                return end_;
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

            template<typename... Args>
            constexpr auto emplace_back(Args&&... args)
                -> T*
            {
                CPPSORT_ASSERT(end_ - memory_ < capacity_);
                std::construct_at(end_, std::forward<Args>(args)...);
                return std::exchange(end_, end_ + 1);
            }

            template<typename Iterator>
            auto insert_back(Iterator first, Iterator last)
                -> void
            {
                auto writer = end_;
                try {
                    for (; first != last; ++first) {
                        std::construct_at(writer, mstd::iter_move(first));
                        ++writer;
                    }
                } catch (...) {
                    // Cleanup
                    std::destroy(end_, writer);
                    throw;
                }
                end_ = writer;
            }

        private:

            [[no_unique_address]] std::allocator<T> alloc_;
            std::ptrdiff_t capacity_;
            T* memory_; // owning
            T* end_;
    };
}

#endif // CPPSORT_DETAIL_IMMOVABLE_VECTOR_H_
