/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2015-2017 Morwenn
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
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
