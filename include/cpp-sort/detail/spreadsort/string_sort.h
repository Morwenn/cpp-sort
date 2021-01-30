/*
 * Copyright (c) 2015-2020 Morwenn
 * SPDX-License-Identifier: MIT
 */

//Templated hybrid string_sort

//          Copyright Steven J. Ross 2001 - 2009.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

// See http://www.boost.org/libs/sort/ for library home page.

/*
Some improvements suggested by:
Phil Endecott and Frank Gennari
*/
#ifndef CPPSORT_DETAIL_SPREADSORT_STRING_SORT_H_
#define CPPSORT_DETAIL_SPREADSORT_STRING_SORT_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <functional>
#include <utility>
#include "detail/constants.h"
#include "detail/string_sort.h"
#include "../pdqsort.h"

namespace cppsort
{
namespace detail
{
namespace spreadsort
{

/*! \brief String sort algorithm using random access iterators, allowing character-type overloads.\n
  (All variants fall back to @c pdqsort if the data size is too small, < @c detail::min_sort_size).

  \details @c string_sort is a fast templated in-place hybrid radix/comparison algorithm,
which in testing tends to be roughly 50% to 2X faster than @c pdqsort for large tests (>=100kB).\n
\par
Worst-case performance is <em>  O(N * (lg(range)/s + s)) </em>,
so @c integer_sort is asymptotically faster
than pure comparison-based algorithms. @c s is @c max_splits, which defaults to 11,
so its worst-case with default settings for 32-bit integers is
<em> O(N * ((32/11) </em> slow radix-based iterations fast comparison-based iterations).\n\n
Some performance plots of runtime vs. n and log(range) are provided:\n
<a href="../../doc/graph/windows_string_sort.htm"> windows_string_sort</a>\n
<a href="../../doc/graph/osx_string_sort.htm"> osx_string_sort</a>

   \tparam RandomAccessIter <a href="http://www.cplusplus.com/reference/iterator/RandomAccessIterator/">Random access iterator</a>
   \tparam Unsigned_char_type  Unsigned character type used for string.
   \param[in] first Iterator pointer to first element.
   \param[in] last Iterator pointing to one beyond the end of data.
   \param[in] unused value with the same type as the result of the [] operator, defining the Unsigned_char_type.  The actual value is unused.

   \pre [@c first, @c last) is a valid range.
   \pre @c RandomAccessIter @c value_type is mutable.
   \pre @c RandomAccessIter @c value_type is <a href="http://en.cppreference.com/w/cpp/concept/LessThanComparable">LessThanComparable</a>
   \pre @c RandomAccessIter @c value_type supports the @c operator>>,
   which returns an integer-type right-shifted a specified number of bits.
   \post The elements in the range [@c first, @c last) are sorted in ascending order.

   \throws std::exception Propagates exceptions if any of the element comparisons, the element swaps (or moves),
   the right shift, subtraction of right-shifted elements, callables,
   or any operations on iterators throw.

   \warning Throwing an exception may cause data loss. This will also throw if a small vector resize throws, in which case there will be no data loss.
   \warning Invalid arguments cause undefined behaviour.
   \note @c spreadsort function provides a wrapper that calls the fastest sorting algorithm available for a data type,
   enabling faster generic-programming.

   \remark The lesser of <em> O(N*log(N)) </em> comparisons and <em> O(N*log(K/S + S)) </em>operations worst-case, where:
   \remark  *  N is @c last - @c first,
   \remark  *  K is the log of the range in bits (32 for 32-bit integers using their full range),
   \remark  *  S is a constant called max_splits, defaulting to 11 (except for strings where it is the log of the character size).

*/

  template<typename RandomAccessIter, typename Projection, typename Unsigned_char_type>
  auto string_sort(RandomAccessIter first, RandomAccessIter last,
                   Projection projection, Unsigned_char_type unused)
      -> void
  {
    //Don't sort if it's too small to optimize
    if (last - first < detail::min_sort_size)
      pdqsort(std::move(first), std::move(last),
              std::less<>{}, std::move(projection));
    else
      detail::string_sort(std::move(first), std::move(last),
                          std::move(projection), unused);
  }

/*! \brief String sort algorithm using random access iterators, allowing character-type overloads.

  (All variants fall back to @c pdqsort if the data size is too small, < detail::min_sort_size).

  \details @c integer_sort is a fast templated in-place hybrid radix/comparison algorithm,
which in testing tends to be roughly 50% to 2X faster than @c pdqsort for large tests (>=100kB).\n
Worst-case performance is <em>  O(N * (lg(range)/s + s)) </em>,
so @c integer_sort is asymptotically faster
than pure comparison-based algorithms. @c s is @c max_splits, which defaults to 11,
so its worst-case with default settings for 32-bit integers is
<em> O(N * ((32/11) </em> slow radix-based iterations fast comparison-based iterations).\n\n
Some performance plots of runtime vs. n and log(range) are provided:\n
   <a href="../../doc/graph/windows_integer_sort.htm"> windows_integer_sort</a>
   \n
   <a href="../../doc/graph/osx_integer_sort.htm"> osx_integer_sort</a>


   \tparam RandomAccessIter <a href="http://www.cplusplus.com/reference/iterator/RandomAccessIterator/">Random access iterator</a>
   \tparam Comp callable type to use for comparison.
   \tparam Unsigned_char_type Unsigned character type used for string.

   \param[in] first Iterator pointer to first element.
   \param[in] last Iterator pointing to one beyond the end of data.
   \param[in] comp A binary callable that returns whether the first element passed to it should go before the second in order.
   \param[in] unused value with the same type as the result of the [] operator, defining the Unsigned_char_type.  The actual value is unused.

   \pre [@c first, @c last) is a valid range.
   \pre @c RandomAccessIter @c value_type is mutable.
   \pre @c RandomAccessIter @c value_type is <a href="http://en.cppreference.com/w/cpp/concept/LessThanComparable">LessThanComparable</a>
   \pre @c RandomAccessIter @c value_type supports the @c operator>>,
   which returns an integer-type right-shifted a specified number of bits.
   \post The elements in the range [@c first, @c last) are sorted in ascending order.

   \return @c void.

   \throws  std::exception Propagates exceptions if any of the element comparisons, the element swaps (or moves),
   the right shift, subtraction of right-shifted elements, callables,
   or any operations on iterators throw.

   \warning Throwing an exception may cause data loss. This will also throw if a small vector resize throws, in which case there will be no data loss.
   \warning Invalid arguments cause undefined behaviour.
   \note @c spreadsort function provides a wrapper that calls the fastest sorting algorithm available for a data type,
   enabling faster generic-programming.

   \remark The lesser of <em> O(N*log(N)) </em> comparisons and <em> O(N*log(K/S + S)) </em>operations worst-case, where:
   \remark  *  N is @c last - @c first,
   \remark  *  K is the log of the range in bits (32 for 32-bit integers using their full range),
   \remark  *  S is a constant called max_splits, defaulting to 11 (except for strings where it is the log of the character size).
*/
  template<typename RandomAccessIter, typename Compare,
           typename Projection, typename Unsigned_char_type>
  auto reverse_string_sort(RandomAccessIter first, RandomAccessIter last,
                           Compare comp, Projection projection,
                           Unsigned_char_type unused)
      -> void
  {
    //Don't sort if it's too small to optimize.
    if (last - first < detail::min_sort_size)
      pdqsort(std::move(first), std::move(last),
              std::move(comp), std::move(projection));
    else
      detail::reverse_string_sort(std::move(first), std::move(last),
                                  std::move(projection), unused);
  }
}}}

#endif // CPPSORT_DETAIL_SPREADSORT_STRING_SORT_H_
