// -*- C++ -*-
//===-------------------------- algorithm ---------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is dual licensed under the MIT and the University of Illinois Open
// Source Licenses. See LICENSE.TXT for details.
//
// Modified in 2016-2019 by Morwenn for inclusion into cpp-sort
//
//===----------------------------------------------------------------------===//
#ifndef CPPSORT_DETAIL_MEMORY_H_
#define CPPSORT_DETAIL_MEMORY_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <cstddef>
#include <limits>
#include <new>
#include <type_traits>
#include "type_traits.h"

namespace cppsort
{
namespace detail
{
    ////////////////////////////////////////////////////////////
    // Deleter for ::operator new(std::size_t)

    struct operator_deleter
    {
        operator_deleter() = default;

#ifdef __cpp_sized_deallocation
        std::size_t size = 0;

        constexpr explicit operator_deleter(std::size_t size) noexcept:
            size(size)
        {}

        inline auto operator()(void* pointer) const noexcept
            -> void
        {
            ::operator delete(pointer, size);
        }
#else
        constexpr explicit operator_deleter(std::size_t) noexcept {}

        inline auto operator()(void* pointer) const noexcept
            -> void
        {
            ::operator delete(pointer);
        }
#endif
    };

    ////////////////////////////////////////////////////////////
    // Deleter for placement new-allocated memory

    struct destruct_n_trivial
    {
        explicit constexpr destruct_n_trivial(std::size_t) noexcept {}

        constexpr auto operator++() noexcept
            -> void
        {}

        constexpr auto operator()(void*) noexcept
            -> void
        {}
    };

    template<typename T>
    struct destruct_n_impl
    {
        explicit constexpr destruct_n_impl(std::size_t s) noexcept:
            size(s)
        {}

        constexpr auto operator++() noexcept
            -> void
        {
            ++size;
        }

        constexpr auto operator()(T* pointer) noexcept
            -> void
        {
            for (std::size_t i = 0 ; i < size ; ++i) {
                pointer->~T();
                ++pointer;
            }
        }

        // Number of allocated objects to destroy
        std::size_t size;
    };

    template<typename T>
    using destruct_n = conditional_t<
        std::is_trivially_destructible<T>::value,
        destruct_n_trivial,
        destruct_n_impl<T>
    >;

    ////////////////////////////////////////////////////////////
    // Reimplement the functions get_temporary_buffer and
    // return_temporary_buffer because of their removal in C++20

    template<typename T>
    auto get_temporary_buffer(ptrdiff_t count) noexcept
        -> std::pair<T*, std::ptrdiff_t>
    {
        std::pair<T*, std::ptrdiff_t> res(nullptr, 0);

        // Don't allocate more than possible
        const ptrdiff_t max = std::numeric_limits<std::ptrdiff_t>::max() / sizeof(T);
        if (count > max) {
            count = max;
        }

        // Try to gradually allocate less memory until we get a valid buffer
        // or until the amount of memory to allocate reaches 0
        while (count > 0) {
            res.first = static_cast<T*>(::operator new(count * sizeof(T), std::nothrow));
            if (res.first) {
                res.second = count;
                break;
            }
            count /= 2;
        }
        return res;
    }

    template<typename T>
    auto return_temporary_buffer(T* ptr, std::size_t count) noexcept
        -> void
    {
#ifdef __cpp_sized_deallocation
        ::operator delete(ptr, count * sizeof(T));
#else
        (void)count;
        ::operator delete(ptr);
#endif
    }

    ////////////////////////////////////////////////////////////
    // Thin wrapper around get/return_temporary_buffer

    template<typename T>
    class temporary_buffer
    {
        public:

            ////////////////////////////////////////////////////////////
            // Member types

            using pointer = T*;
            using element_type = T;

            ////////////////////////////////////////////////////////////
            // Construction & destruction

            temporary_buffer() = default;
            temporary_buffer(const temporary_buffer&) = delete;

            temporary_buffer(temporary_buffer&& other) noexcept:
                buffer(other.buffer),
                buffer_size(other.buffer_size)
            {
                other.buffer = nullptr;
                other.buffer_size = 0;
            }

            constexpr temporary_buffer(std::nullptr_t) noexcept {}

            explicit temporary_buffer(std::ptrdiff_t count) noexcept
            {
                auto tmp = get_temporary_buffer<T>(count);
                buffer = tmp.first;
                buffer_size = tmp.second;
            }

            ~temporary_buffer() noexcept
            {
                return_temporary_buffer<T>(buffer, buffer_size);
            }

            ////////////////////////////////////////////////////////////
            // Assignment operator

            temporary_buffer& operator=(const temporary_buffer&) = delete;

            auto operator=(temporary_buffer&& other) noexcept
                -> temporary_buffer&
            {
                using std::swap;
                swap(buffer, other.buffer);
                swap(buffer_size, other.buffer_size);
                return *this;
            }

            ////////////////////////////////////////////////////////////
            // Data access

            auto data() const noexcept
                -> pointer
            {
                return buffer;
            }

            auto size() const noexcept
                -> std::ptrdiff_t
            {
                return buffer_size;
            }

            ////////////////////////////////////////////////////////////
            // Modifiers

            auto try_grow(std::ptrdiff_t count) noexcept
                -> bool
            {
                if (count <= buffer_size) {
                    return false;
                }
                auto tmp = get_temporary_buffer<T>(count);
                if (tmp.second <= buffer_size) {
                    // If the allocated buffer isn't bigger, keep the old one
                    return_temporary_buffer<T>(tmp.first, tmp.second);
                    return false;
                }
                // If the allocated buffer is big enough, replace the previous one
                return_temporary_buffer(buffer, buffer_size);
                buffer = tmp.first;
                buffer_size = tmp.second;
                return true;
            }

        private:

            T* buffer = nullptr;
            std::ptrdiff_t buffer_size = 0;
    };
}}

#endif // CPPSORT_DETAIL_MEMORY_H_
