/*
 * Copyright (c) 2019-2020 Morwenn
 * SPDX-License-Identifier: MIT
 */

//----------------------------------------------------------------------------
/// @file merge.hpp
/// @brief low level merge functions
///
/// @author Copyright (c) 2016 Francisco Jose Tapia (fjtapia@gmail.com )
///         Distributed under the Boost Software License, Version 1.0.
///         ( See accompanying file LICENSE_1_0.txt or copy at
///           http://www.boost.org/LICENSE_1_0.txt  )
/// @version 0.1
///
/// @remarks
//-----------------------------------------------------------------------------

#ifndef CPPSORT_DETAIL_BOOST_COMMON_UTIL_MERGE_H_
#define CPPSORT_DETAIL_BOOST_COMMON_UTIL_MERGE_H_

#include <algorithm>
#include <functional>
#include <iterator>
#include <memory>
#include <type_traits>
#include <cpp-sort/utility/as_function.h>
#include <cpp-sort/utility/iter_move.h>
#include "../../buffered_inplace_merge.h"
#include "../../config.h"
#include "../../iterator_traits.h"
#include "../../merge_move.h"
#include "../../move.h"

namespace cppsort
{
namespace detail
{
namespace boost_common
{
namespace util
{
    //-----------------------------------------------------------------------------
    //  function : merge
    /// @brief Merge two contiguous buffers pointed by buf1 and buf2, and put
    ///        in the buffer pointed by buf_out
    ///
    /// @param buf1 : iterator to the first element in the first buffer
    /// @param end_buf1 : final iterator of first buffer
    /// @param buf2 : iterator to the first iterator to the second buffer
    /// @param end_buf2 : final iterator of the second buffer
    /// @param buf_out : buffer where move the elements merged
    /// @param compare : comparison object
    /// @param projection : projection object
    //-----------------------------------------------------------------------------
    template<typename RandomAccessIterator1, typename RandomAccessIterator2, typename RandomAccessIterator3,
             typename Compare, typename Projection>
    auto merge(RandomAccessIterator1 buf1, RandomAccessIterator1 end_buf1,
               RandomAccessIterator2 buf2, RandomAccessIterator2 end_buf2,
               RandomAccessIterator3 buf_out,
               Compare compare, Projection projection)
        -> RandomAccessIterator3
    {
        using utility::iter_move;

        constexpr std::size_t min_size = 1024;

        auto&& comp = utility::as_function(compare);
        auto&& proj = utility::as_function(projection);

        if (std::size_t((end_buf1 - buf1) + (end_buf2 - buf2)) >= min_size) {
            if (buf1 == end_buf1) return cppsort::detail::move(buf2, end_buf2, buf_out);
            if (buf2 == end_buf2) return cppsort::detail::move(buf1, end_buf1, buf_out);

            if (not comp(proj(*buf2), proj(*std::prev(end_buf1)))) {
                auto mid = cppsort::detail::move(buf1, end_buf1, buf_out);
                return cppsort::detail::move(buf2, end_buf2, mid);
            }

            if (comp(proj(*std::prev(end_buf2)), proj(*buf1))) {
                auto mid = cppsort::detail::move(buf2, end_buf2, buf_out);
                return cppsort::detail::move(buf1, end_buf1, mid);
            }
        }

        return cppsort::detail::merge_move(buf1, end_buf1, buf2, end_buf2, buf_out,
                                           compare, projection, projection);
    }

    //---------------------------------------------------------------------------
    //  function : merge_half
    /// @brief : Merge two buffers. The first buffer is in a separate memory.
    ///          The second buffer have a empty space before buf2 of the same size
    ///          than the (end_buf1 - buf1)
    ///
    /// @param buf1 : iterator to the first element of the first buffer
    /// @param end_buf1 : iterator to the last element of the first buffer
    /// @param buf2 : iterator to the first element of the second buffer
    /// @param end_buf2 : iterator to the last element of the second buffer
    /// @param buf_out : iterator to the first element to the buffer where put
    ///                  the result
    /// @param compare : object for Compare two elements of the type pointed
    ///                  by the RandomAccessIterator1 and RandomAccessIterator2
    /// @param projection : projection object
    //---------------------------------------------------------------------------
    template<typename RandomAccessIterator1, typename RandomAccessIterator2,
             typename Compare, typename Projection>
    auto merge_half(RandomAccessIterator1 buf1, RandomAccessIterator1 end_buf1,
                    RandomAccessIterator2 buf2, RandomAccessIterator2 end_buf2,
                    RandomAccessIterator2 buf_out,
                    Compare compare, Projection projection)
        -> RandomAccessIterator2
    {
        using utility::iter_move;
        CPPSORT_ASSERT(buf2 - buf_out == end_buf1 - buf1);

        constexpr std::size_t min_size = 1024;

        auto&& comp = utility::as_function(compare);
        auto&& proj = utility::as_function(projection);

        if (std::size_t((end_buf1 - buf1) + (end_buf2 - buf2)) >= min_size) {
            if (buf1 == end_buf1) return end_buf2;
            if (buf2 == end_buf2) return cppsort::detail::move(buf1, end_buf1, buf_out);

            if (not comp(proj(*buf2), proj(*std::prev(end_buf1)))) {
                cppsort::detail::move(buf1, end_buf1, buf_out);
                return end_buf2;
            }

            if (comp(proj(*std::prev(end_buf2)), proj(*buf1))) {
                auto mid = cppsort::detail::move(buf2, end_buf2, buf_out);
                return cppsort::detail::move(buf1, end_buf1, mid);
            }
        }

        cppsort::detail::half_inplace_merge(buf1, end_buf1, buf2, end_buf2, buf_out,
                                            end_buf1 - buf1, compare, projection);
        return end_buf2;
    }
}}}}

#endif // CPPSORT_DETAIL_BOOST_COMMON_UTIL_MERGE_H_
