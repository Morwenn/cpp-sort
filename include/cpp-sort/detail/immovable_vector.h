/*
 * Copyright (c) 2021 Morwenn
 * SPDX-License-Identifier: MIT
 */
#ifndef CPPSORT_DETAIL_IMMOVABLE_VECTOR_H_
#define CPPSORT_DETAIL_IMMOVABLE_VECTOR_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <cstddef>
#include <new>
#include <utility>
#include <cpp-sort/utility/iter_move.h>
#include "config.h"
#include "memory.h"

namespace cppsort
{
namespace detail
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

            explicit immovable_vector(std::ptrdiff_t n):
                capacity_(n),
                memory_(
                    static_cast<T*>(::operator new(n * sizeof(T)))
                ),
                end_(memory_)
            {}

            ////////////////////////////////////////////////////////////
            // Destruction

            ~immovable_vector()
            {
                // Destroy the constructed elements
                detail::destroy(memory_, end_);

                // Free the allocated memory
#ifdef __cpp_sized_deallocation
                ::operator delete(memory_, capacity_ * sizeof(T));
#else
                ::operator delete(memory_);
#endif
            }

            ////////////////////////////////////////////////////////////
            // Element access

            auto operator[](std::ptrdiff_t pos)
                -> T&
            {
                CPPSORT_ASSERT(pos <= end_ - memory_);
                return memory_[pos];
            }

            auto front()
                -> T&
            {
                CPPSORT_ASSERT(memory_ != end_);
                return *memory_;
            }

            auto back()
                -> T&
            {
                CPPSORT_ASSERT(end_ - memory_ > 0);
                return *(end_ - 1);
            }

            ////////////////////////////////////////////////////////////
            // Iterators

            auto begin()
                -> T*
            {
                return memory_;
            }

            auto end()
                -> T*
            {
                return end_;
            }

            ////////////////////////////////////////////////////////////
            // Capacity

            auto is_empty() const noexcept
                -> bool
            {
                return memory_ != end_;
            }

            auto size() const noexcept
                -> std::ptrdiff_t
            {
                return end_ - memory_;
            }

            auto capacity() const noexcept
                -> std::ptrdiff_t
            {
                return capacity_;
            }

            ////////////////////////////////////////////////////////////
            // Modifiers

            auto clear() noexcept
                -> void
            {
                // Destroy the constructed elements
                detail::destroy(memory_, end_);

                // Ensure the new size is zero
                end_ = memory_;
            }

            template<typename... Args>
            auto emplace_back(Args&&... args)
                -> void
            {
                CPPSORT_ASSERT(end_ - memory_ < capacity_);
                ::new(end_) T(std::forward<Args>(args)...);
                ++end_;
            }

            template<typename Iterator>
            auto insert_back(Iterator first, Iterator last)
                -> void
            {
                auto writer = end_;
                try {
                    for (; first != last; ++first) {
                        using utility::iter_move;
                        ::new(writer) T(iter_move(first));
                        ++writer;
                    }
                } catch (...) {
                    // Cleanup
                    detail::destroy(end_, writer);
                    throw;
                }
                end_ = writer;
            }

            auto resize(std::ptrdiff_t count)
                -> void
            {
                CPPSORT_ASSERT(count >= 0);
                CPPSORT_ASSERT(count <= capacity_);

                if (count < size()) {
                    auto new_end = memory_ + count;
                    detail::destroy(new_end, end_);
                    end_ = new_end;
                } else if (count > size()) {
                    auto writer = end_;
                    try {
                        while (writer != end_) {
                            ::new(writer) T();
                            ++writer;
                        }
                    } catch (...) {
                        // Cleanup
                        detail::destroy(end_, writer);
                        throw;
                    }
                    end_ = writer;
                }
            }

        private:

            std::ptrdiff_t capacity_;
            T* memory_;
            T* end_;
    };
}}

#endif // CPPSORT_DETAIL_IMMOVABLE_VECTOR_H_
