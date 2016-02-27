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
#include <new>
#include <type_traits>

namespace cppsort
{
namespace detail
{
    struct operator_deleter
    {
        template<typename T>
        auto operator()(T* ptr) const noexcept
            -> void
        {
            ::operator delete(ptr);
        }
    };

    struct destruct_n
    {
        private:

            std::size_t size;

            template<typename T>
            auto process(T* ptr, std::false_type) noexcept
                -> void
            {
                for (std::size_t i = 0 ; i < size ; ++i, (void) ++ptr)
                {
                    ptr->~T();
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
            auto operator()(T* ptr) noexcept
                -> void
            {
                process(ptr, std::integral_constant<bool, std::is_trivially_destructible<T>::value>{});
            }
    };
}}

#endif // CPPSORT_DETAIL_DESTRUCT_N_H_
