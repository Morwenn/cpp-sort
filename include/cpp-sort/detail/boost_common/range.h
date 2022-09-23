/*
 * Copyright (c) 2019-2022 Morwenn
 * SPDX-License-Identifier: MIT
 */

//----------------------------------------------------------------------------
/// @file range.hpp
/// @brief Define a range [first, last), and the associated operations
///
/// @author Copyright (c) 2016 Francisco José Tapia (fjtapia@gmail.com )
///         Distributed under the Boost Software License, Version 1.0.
///         ( See accompanyingfile LICENSE_1_0.txt or copy at
///           http://www.boost.org/LICENSE_1_0.txt  )
/// @version 0.1
///
/// @remarks
//-----------------------------------------------------------------------------

#ifndef CPPSORT_DETAIL_BOOST_COMMON_RANGE_H_
#define CPPSORT_DETAIL_BOOST_COMMON_RANGE_H_

#include <functional>
#include <iterator>
#include <memory>
#include <vector>
#include "util/merge.h"
#include "../config.h"
#include "../iterator_traits.h"
#include "../move.h"

namespace cppsort
{
namespace detail
{
namespace boost_common
{
    ///---------------------------------------------------------------------------
    /// @struct range
    /// @brief this represent a range between two iterators
    /// @remarks
    //----------------------------------------------------------------------------
    template<typename RandomAccessIterator>
    struct range
    {
        RandomAccessIterator first, last;

        //------------------------------------------------------------------------
        //  function : range
        /// @brief default constructor
        //------------------------------------------------------------------------
        range() = default;

        //------------------------------------------------------------------------
        //  function : range
        /// @brief constructor with two parameters
        /// @param frs : iterator to the first element
        /// @param lst : iterator to the last element
        //-----------------------------------------------------------------------
        range(const RandomAccessIterator& frs, const RandomAccessIterator& lst):
            first(frs),
            last(lst)
        {}

        //-----------------------------------------------------------------------
        //  function : empty
        /// @brief indicate if the range is empty
        /// @return true : empty false : not empty
        //-----------------------------------------------------------------------
        auto empty() const
            -> bool
        {
            return first == last;
        }

        //-----------------------------------------------------------------------
        //  function : not_empty
        /// @brief indicate if the range is not empty
        /// @return  true : not empty false : empty
        //-----------------------------------------------------------------------
        auto not_empty() const
            -> bool
        {
            return first != last;
        }

        //-----------------------------------------------------------------------
        //  function : valid
        /// @brief  Indicate if the range is well constructed, and valid
        /// @return true : valid,  false : not valid
        //-----------------------------------------------------------------------
        auto valid() const
            -> bool
        {
            return (last - first) >= 0;
        }

        //-----------------------------------------------------------------------
        //  function : size
        /// @brief  return the size of the range
        /// @return size
        //-----------------------------------------------------------------------
        auto size() const
            -> difference_type_t<RandomAccessIterator>
        {
            return last - first;
        }

        //------------------------------------------------------------------------
        //  function : front
        /// @brief return an iterator to the first element of the range
        /// @return iterator
        //-----------------------------------------------------------------------
        auto front() const
            -> RandomAccessIterator
        {
            return first;
        }

        //-------------------------------------------------------------------------
        //  function : back
        /// @brief return an iterator to the last element of the range
        /// @return iterator
        //-------------------------------------------------------------------------
        auto back() const
            -> RandomAccessIterator
        {
            return std::prev(last);
        }
    };

    //-----------------------------------------------------------------------------
    //  function : move_forward
    /// @brief Move initialized objets from the range src to dest
    /// @param dest : range where move the objects
    /// @param src : range from where move the objects
    /// @return range with the objects moved and the size adjusted
    //-----------------------------------------------------------------------------
    template<typename Iter1_t, typename Iter2_t>
    auto move_forward(const range<Iter2_t>& dest, const range<Iter1_t>& src)
        -> range<Iter2_t>
    {
        CPPSORT_ASSERT(dest.size() >= src.size());
        auto it_aux = cppsort::detail::move(src.first, src.last, dest.first);
        return range<Iter2_t>(dest.first, it_aux);
    }

    //
    //-----------------------------------------------------------------------------
    //  function : move_backward
    /// @brief Move initialized objets from the range src to dest
    /// @param dest : range where move the objects
    /// @param src : range from where move the objects
    /// @return range with the objects moved and the size adjusted
    //-----------------------------------------------------------------------------
    template<typename Iter1_t, typename Iter2_t>
    auto move_backward(const range<Iter2_t>& dest, const range<Iter1_t>& src)
        -> range<Iter2_t>
    {
        CPPSORT_ASSERT(dest.size() >= src.size());
        auto it_aux = cppsort::detail::move_backward(src.first, src.last, dest.first + src.size());
        return range<Iter2_t>(dest.first, dest.src.size());
    }

    //-----------------------------------------------------------------------------
    //  function : merge
    /// @brief Merge two contiguous ranges src1 and src2, and put the result in
    ///        the range dest, returning the range merged
    ///
    /// @param dest : range where locate the lements merged. the size of dest
    ///               must be  greater or equal than the sum of the sizes of
    ///               src1 and src2
    /// @param src1 : first range to merge
    /// @param src2 : second range to merge
    /// @param compare : comparison object
    /// @param projection : projection object
    /// @return range with the elements merged and the size adjusted
    //-----------------------------------------------------------------------------
    template<typename Iter1_t, typename Iter2_t, typename Iter3_t, typename Compare, typename Projection>
    auto merge(const range<Iter3_t>& dest, const range<Iter1_t>& src1, const range<Iter2_t>& src2,
               Compare compare, Projection projection)
        -> range<Iter3_t>
    {
        auto it_aux = util::merge(src1.first, src1.last, src2.first, src2.last, dest.first,
                                  compare, projection);
        return range<Iter3_t>(dest.first, it_aux);
    }

    //---------------------------------------------------------------------------
    //  function : half_merge
    /// @brief : Merge two initialized buffers. The first buffer is in a separate
    ///          memory
    //
    /// @param dest : range where finish the two buffers merged
    /// @param src1 : first range to merge in a separate memory
    /// @param src2 : second range to merge, in the final part of the
    ///               range where deposit the final results
    /// @param compare : object for compare two elements of the type pointed
    ///                  by the Iter1_t and Iter2_t
    /// @param projection : projection object
    /// @return : range with the two buffers merged
    //---------------------------------------------------------------------------
    template<typename Iter1_t, typename Iter2_t, typename Compare, typename Projection>
    auto merge_half(const range<Iter2_t>& dest, const range<Iter1_t>& src1, const range<Iter2_t>& src2,
                    Compare compare, Projection projection)
        -> range<Iter2_t>
    {
        auto it_aux = util::merge_half(src1.first, src1.last, src2.first, src2.last, dest.first,
                                       compare, projection);
        return range<Iter2_t>(dest.first, it_aux);
    }
}}}

#endif  // CPPSORT_DETAIL_BOOST_COMMON_RANGE_H_
