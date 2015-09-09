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
#ifndef CPPSORT_DETAIL_QUICKSORT_H_
#define CPPSORT_DETAIL_QUICKSORT_H_

#include <algorithm>
#include <iterator>

namespace cppsort
{
namespace detail
{
    template <typename iterator, typename LessThan>
    void quicksort(iterator first, iterator last, LessThan lessThan, std::size_t size)
    {
      // already sorted ?
      if (size <= 1)
        return;

      iterator pivot = last;
      --pivot;

      // choose middle element as pivot (good choice for partially sorted data)
      if (size > 2)
      {
        iterator middle = first;
        std::advance(middle, size / 2);
        std::iter_swap(middle, pivot);
      }

      // scan beginning from left and right end and swap misplaced elements
      iterator left  = first;
      iterator right = pivot;
      while (left != right)
      {
        // look for mismatches
        while (!lessThan(*pivot, *left)  && left != right)
          ++left;
        while (!lessThan(*right, *pivot) && left != right)
          --right;
        // swap two values which are both on the wrong side of the pivot element
        if (left != right)
          std::iter_swap(left, right);
      }

      // move pivot to its final position
      if (pivot != left && lessThan(*pivot, *left))
        std::iter_swap(pivot, left);

      // subdivide
      std::size_t size_left = std::distance(first, left);
      quicksort(first,  left, lessThan, size_left);
      // *left itself is already sorted
      quicksort(++left, last, lessThan, size - size_left - 1);
    }
}}

#endif // CPPSORT_DETAIL_QUICKSORT_H_
