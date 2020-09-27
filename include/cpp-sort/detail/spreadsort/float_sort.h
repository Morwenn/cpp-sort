/*
 * Copyright (c) 2015-2017 Morwenn
 * SPDX-License-Identifier: MIT
 */

//Templated Spreadsort-based implementation of float_sort and float_mem_cast

//          Copyright Steven J. Ross 2001 - 2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

// See http://www.boost.org/libs/sort/ for library home page.

/*
Some improvements suggested by:
Phil Endecott and Frank Gennari
*/
#ifndef CPPSORT_DETAIL_SPREADSORT_FLOAT_SORT_H_
#define CPPSORT_DETAIL_SPREADSORT_FLOAT_SORT_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <functional>
#include <utility>
#include "detail/constants.h"
#include "detail/float_sort.h"
#include "../pdqsort.h"

namespace cppsort
{
namespace detail
{
namespace spreadsort
{
  /*!
    \brief @c float_sort with casting to the appropriate size.

    \param[in] first Iterator pointer to first element.
    \param[in] last Iterator pointing to one beyond the end of data.

Some performance plots of runtime vs. n and log(range) are provided:\n
   <a href="../../doc/graph/windows_float_sort.htm"> windows_float_sort</a>
   \n
   <a href="../../doc/graph/osx_float_sort.htm"> osx_float_sort</a>



   \par A simple example of sorting some floating-point is:
   \code
     vector<float> vec;
     vec.push_back(1.0);
     vec.push_back(2.3);
     vec.push_back(1.3);
     spreadsort(vec.begin(), vec.end());
   \endcode
   \par The sorted vector contains ascending values "1.0 1.3 2.3".

  */
  template<typename RandomAccessIter, typename Projection>
  auto float_sort(RandomAccessIter first, RandomAccessIter last, Projection projection)
      -> void
  {
    if (last - first < detail::min_sort_size)
      pdqsort(std::move(first), std::move(last),
              std::less<>{}, std::move(projection));
    else
      detail::float_sort(std::move(first), std::move(last),
                         std::move(projection));
  }
}}}

#endif // CPPSORT_DETAIL_SPREADSORT_FLOAT_SORT_H_
