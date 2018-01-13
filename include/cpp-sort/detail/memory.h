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
    // Deleter for std::get_temporary_buffer

    struct temporary_buffer_deleter
    {
        template<typename T>
        auto operator()(T* pointer) const
            noexcept(noexcept(std::return_temporary_buffer(pointer)))
            -> void
        {
            std::return_temporary_buffer(pointer);
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
    // C++17 memory algorithms

    template<typename T>
    auto destroy_at(T* ptr)
        -> void
    {
        ptr->~T();
    }

    template<typename ForwardIterator>
    auto destroy(ForwardIterator first, ForwardIterator last)
        -> void
    {
        for (; first != last ; ++first) {
            destroy_at(std::addressof(*first));
        }
    }
}}

#endif // CPPSORT_DETAIL_DESTRUCT_N_H_
