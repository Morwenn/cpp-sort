//----------------------------------------------------------------------------
/// @file algorithm.hpp
/// @brief low level functions of create, destroy, move and merge functions
///
/// @author Copyright (c) 2017 Francisco Jose Tapia (fjtapia@gmail.com )
///         Modified in 2019 by Morwenn for inclusion into cpp-sort
///         Distributed under the Boost Software License, Version 1.0.
///         ( See accompanying file LICENSE_1_0.txt or copy at
///           http://www.boost.org/LICENSE_1_0.txt  )
/// @version 0.1
///
/// @remarks
//-----------------------------------------------------------------------------
#ifndef CPPSORT_DETAIL_BOOST_COMMON_UTIL_ALGORITHM_H_
#define CPPSORT_DETAIL_BOOST_COMMON_UTIL_ALGORITHM_H_

#include <cstdint>
#include <iterator>
#include <type_traits>
#include <utility>
#include <cpp-sort/utility/iter_move.h>

namespace cppsort
{
namespace detail
{
namespace boost_common
{
namespace util
{
    // this array represent the number of bits needed for to represent the
    // first 256 numbers
    static constexpr const std::uint32_t tmsb[256] = {
        0, 1, 2, 2, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5,
        5, 5, 5, 5, 5, 5, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6,
        6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 7, 7, 7, 7,
        7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
        7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
        7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 8, 8, 8,
        8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
        8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
        8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
        8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
        8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
        8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8
    };

    //---------------------------------------------------------------------------
    //  function : nbits64
    /// @brief Obtain the number of bits of a number equal or greater than num
    /// @param num : Number to examine
    /// @exception none
    /// @return Number of bits
    //---------------------------------------------------------------------------
    inline auto nbits64(std::uint64_t num) noexcept
        -> std::uint32_t
    {
        std::uint32_t Pos = (num & 0xffffffff00000000ULL) ? 32 : 0;
        if ((num >> Pos) & 0xffff0000ULL) Pos += 16;
        if ((num >> Pos) & 0xff00ULL) Pos += 8;
        return (tmsb[num >> Pos] + Pos);
    }

    //-----------------------------------------------------------------------------
    //  function : destroy_object
    /// @brief destroy an object in the memory specified by ptr
    /// @param ptr : pointer to the object to destroy
    //-----------------------------------------------------------------------------
    template<typename Value_t>
    auto destroy_object(Value_t* ptr)
        -> void
    {
        ptr->~Value_t();
    }

    //-----------------------------------------------------------------------------
    //  function : move_construct
    /// @brief Move objets to uninitialized memory
    ///
    /// @param ptr : pointer to the memory where to create the objects
    /// @param first : iterator to the first element to move
    /// @param last : iterator to the last element to move
    //-----------------------------------------------------------------------------
    template<typename Iter_t, typename Value_t>
    auto move_construct(Value_t* ptr, Iter_t first, Iter_t last)
        -> Value_t*
    {
        for (; first != last ; ++first, (void) ++ptr) {
            using utility::iter_move;
            ::new(ptr) Value_t(iter_move(first));
        }
        return ptr;
    }

    //-----------------------------------------------------------------------------
    //  function : destroy
    /// @brief destroy the elements between first and last
    /// @param first : iterator to the first element to destroy
    /// @param last : iterator to the last element to destroy
    //-----------------------------------------------------------------------------
    template<typename Iter_t>
    auto destroy(Iter_t first, const Iter_t last)
        -> void
    {
        while (first != last) {
            destroy_object(&(*(first++)));
        }
    }
}}}}

#endif // CPPSORT_DETAIL_BOOST_COMMON_UTIL_ALGORITHM_H_
