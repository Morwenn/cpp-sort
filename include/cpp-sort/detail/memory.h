// -*- C++ -*-
//===-------------------------- algorithm ---------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is dual licensed under the MIT and the University of Illinois Open
// Source Licenses. See LICENSE.TXT for details.
//
// Modified in 2016-2018 by Morwenn for inclusion into cpp-sort
//
//===----------------------------------------------------------------------===//
#ifndef CPPSORT_DETAIL_DESTRUCT_N_H_
#define CPPSORT_DETAIL_DESTRUCT_N_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <cstddef>
#include <limits>
#include <memory>
#include <new>
#include <type_traits>

namespace cppsort
{
namespace detail
{
    ////////////////////////////////////////////////////////////
    // Deleter for ::operator new(std::size_t)

    struct operator_deleter
    {
        template<typename T>
        auto operator()(T* pointer) const noexcept
            -> void
        {
            ::operator delete(pointer);
        }
    };

    ////////////////////////////////////////////////////////////
    // Deleter for placement new-allocated memory

    template<
        typename T,
        bool = std::is_trivially_destructible<T>::value
    >
    struct destruct_n;

    template<typename T>
    struct destruct_n<T, true>
    {
        explicit destruct_n(std::size_t) noexcept {}

        auto operator++() noexcept
            -> void
        {}

        auto operator()(T*) noexcept
            -> void
        {}
    };

    template<typename T>
    struct destruct_n<T, false>
    {
        explicit destruct_n(std::size_t s) noexcept:
            size(s)
        {}

        auto operator++() noexcept
            -> void
        {
            ++size;
        }

        auto operator()(T* pointer) noexcept
            -> void
        {
            for (std::size_t i = 0 ; i < size ; ++i, (void) ++pointer)
            {
                pointer->~T();
            }
        }

        // Number of allocated objects to destroy
        std::size_t size;
    };

    ////////////////////////////////////////////////////////////
    // Reimplement get_temporary_buffer because C++20

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

    ////////////////////////////////////////////////////////////
    // Wrapper are std::unique_ptr for temporary buffers

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
            temporary_buffer(temporary_buffer&&) = default;
            temporary_buffer(const temporary_buffer&) = delete;

            constexpr temporary_buffer(std::nullptr_t) noexcept {}

            explicit temporary_buffer(std::ptrdiff_t count)
            {
                auto tmp = get_temporary_buffer<T>(count);
                buffer.reset(tmp.first);
                buffer_size = tmp.second;
            }

            ~temporary_buffer() = default;

            ////////////////////////////////////////////////////////////
            // Assignment operator

            temporary_buffer& operator=(temporary_buffer&&) = default;
            temporary_buffer& operator=(const temporary_buffer&) = delete;

            ////////////////////////////////////////////////////////////
            // Data access

            auto data() const noexcept
                -> pointer
            {
                return buffer.get();
            }

            auto size() const noexcept
                -> std::ptrdiff_t
            {
                return buffer_size;
            }

            ////////////////////////////////////////////////////////////
            // Modifiers

            auto try_grow(std::ptrdiff_t count)
                -> bool
            {
                if (count <= buffer_size) {
                    return false;
                }
                auto tmp = get_temporary_buffer<T>(count);
                if (tmp.second <= buffer_size) {
                    // If the allocated buffer isn't bigger, keep the old one
                    ::operator delete(tmp.first, std::nothrow);
                    return false;
                }
                // If the allocated buffer is big enough, replace the previous one
                buffer.reset(tmp.first);
                buffer_size = tmp.second;
                return true;
            }

        private:

            std::unique_ptr<T[], operator_deleter> buffer = nullptr;
            std::ptrdiff_t buffer_size = 0;
    };
}}

#endif // CPPSORT_DETAIL_DESTRUCT_N_H_
