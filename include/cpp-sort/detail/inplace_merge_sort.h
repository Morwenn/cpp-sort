/*
    Copyright (c) 2013-2014 Stephan Brumme
    Modified in 2015 by Morwenn for inclusion into cpp-sort

    This software is provided 'as-is', without any express or implied warranty. In no event will the
    authors be held liable for any damages arising from the use of this software.

    Permission is granted to anyone to use this software for any purpose, including commercial
    applications, and to alter it and redistribute it freely, subject to the following restrictions:

    1. The origin of this software must not be misrepresented; you must not claim that you wrote the
       original software.

    2. If you use this software in a product, an acknowledgment in the product documentation would be
       appreciated but is not required.

    3. Altered source versions must be plainly marked as such, and must not be misrepresented as
       being the original software.
*/
#ifndef CPPSORT_DETAIL_INPLACE_MERGE_SORT_H_
#define CPPSORT_DETAIL_INPLACE_MERGE_SORT_H_

#include <cstddef>
#include <iterator>
#include <utility>

namespace cppsort
{
namespace detail
{
    /// in-place Merge Sort, allow user-defined less-than operator
    template <typename iterator, typename LessThan>
    void inplace_merge_sort(iterator first, iterator last, LessThan lessThan, std::size_t size)
    {
      // one element is always sorted
      if (size <= 1)
        return;

      // divide into two partitions
      std::size_t firstHalf  = size / 2;
      std::size_t secondHalf = size - firstHalf;
      iterator mid = first;
      std::advance(mid, firstHalf);

      // recursively sort them
      inplace_merge_sort(first, mid,  lessThan, firstHalf);
      inplace_merge_sort(mid,   last, lessThan, secondHalf);

      // merge partitions (left starts at "first", right starts and "mid")
      // move iterators towards the end until they meet
      iterator right = mid;
      while (first != mid)
      {
        // next value of both partitions in wrong order (smaller one belongs to the left)
        if (lessThan(*right, *first))
        {
          // this value must be moved to the right partition
          typename std::iterator_traits<iterator>::value_type misplaced = std::move(*first);

          // this value belongs to the left partition
          *first = std::move(*right);

          // misplaced value must be inserted at correct position in the right partition
          iterator scan = right;
          iterator next = scan;
          ++next;
          // move smaller one position to the left
          while (next != last && lessThan(*next, misplaced))
            *scan++ = std::move(*next++);

          // found the spot !
          *scan = std::move(misplaced);
        }

        ++first;
      }
    }
}}

#endif // CPPSORT_DETAIL_INPLACE_MERGE_SORT_H_
