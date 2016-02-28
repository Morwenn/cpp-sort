// -*- C++ -*-
//===-------------------------- algorithm ---------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is dual licensed under the MIT and the University of Illinois Open
// Source Licenses. See LICENSE.TXT for details.
//
// Modified in 2016 by Morwenn for inclusion into cpp-sort
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

    struct destruct_n
    {
        private:

            std::size_t size;

            template<typename T>
            auto process(T* pointer, std::false_type) noexcept
                -> void
            {
                for (std::size_t i = 0 ; i < size ; ++i, (void) ++pointer)
                {
                    pointer->~T();
                }
            }

            template<typename T>
            auto process(T*, std::true_type) noexcept
                -> void
            {}

            auto incr(std::false_type) noexcept
                -> void
            {
                ++size;
            }

            auto incr(std::true_type) noexcept
                -> void
            {}

            auto set(std::size_t s, std::false_type) noexcept
                -> void
            {
                size = s;
            }

            auto set(std::size_t, std::true_type) noexcept
                -> void
            {}

        public:

            explicit destruct_n(std::size_t s) noexcept:
                size(s)
            {}

            template<typename T>
            auto incr(T*) noexcept
                -> void
            {
                incr(std::integral_constant<bool, std::is_trivially_destructible<T>::value>{});
            }

            template<typename T>
            auto set(std::size_t s, T*) noexcept
                -> void
            {
                set(s, std::integral_constant<bool, std::is_trivially_destructible<T>::value>{});
            }

            template<typename T>
            auto operator()(T* pointer) noexcept
                -> void
            {
                process(pointer, std::integral_constant<bool, std::is_trivially_destructible<T>::value>{});
            }
    };
}}

#endif // CPPSORT_DETAIL_DESTRUCT_N_H_
