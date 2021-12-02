/*
 * Copyright (c) 2015-2021 Morwenn
 * SPDX-License-Identifier: MIT
 */

// Details for templated Spreadsort-based integer_sort.

//          Copyright Steven J. Ross 2001 - 2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

// See http://www.boost.org/libs/sort for library home page.

/*
Some improvements suggested by:
Phil Endecott and Frank Gennari
*/
#ifndef CPPSORT_DETAIL_SPREADSORT_DETAIL_INTEGER_SORT_H_
#define CPPSORT_DETAIL_SPREADSORT_DETAIL_INTEGER_SORT_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <cstddef>
#include <cstdint>
#include <functional>
#include <type_traits>
#include <utility>
#include <vector>
#include <cpp-sort/utility/as_function.h>
#include <cpp-sort/utility/iter_move.h>
#include "common.h"
#include "constants.h"
#include "../../pdqsort.h"
#include "../../type_traits.h"

namespace cppsort
{
namespace detail
{
namespace spreadsort
{
  namespace detail {
    // Return true if the list is sorted.  Otherwise, find the minimum and
    // maximum using <.
    template<typename RandomAccessIter, typename Projection>
    auto is_sorted_or_find_extremes(RandomAccessIter current, RandomAccessIter last,
                                    RandomAccessIter & max, RandomAccessIter & min,
                                    Projection projection)
        -> bool
    {
      auto&& proj = utility::as_function(projection);

      min = max = current;
      //This assumes we have more than 1 element based on prior checks.
      while (!(proj(*(current + 1)) < proj(*current))) {
        //If everything is in sorted order, return
        if (++current == last - 1)
          return true;
      }

      //The maximum is the last sorted element
      max = current;
      //Start from the first unsorted element
      while (++current < last) {
        if (proj(*max) < proj(*current))
          max = current;
        else if (proj(*current) < proj(*min))
          min = current;
      }
      return false;
    }

    //Gets a non-negative right bit shift to operate as a logarithmic divisor
    template<unsigned log_mean_bin_size>
    auto get_log_divisor(std::size_t count, int log_range)
        -> int
    {
      int log_divisor;
      //If we can finish in one iteration without exceeding either
      //(2 to the max_finishing_splits) or n bins, do so
      if ((log_divisor = log_range - rough_log_2_size(count)) <= 0 &&
         log_range <= max_finishing_splits)
        log_divisor = 0;
      else {
        //otherwise divide the data into an optimized number of pieces
        log_divisor += log_mean_bin_size;
        //Cannot exceed max_splits or cache misses slow down bin lookups
        if ((log_range - log_divisor) > max_splits)
          log_divisor = log_range - max_splits;
      }
      return log_divisor;
    }

    //Implementation for recursive integer sorting
    template<typename RandomAccessIter, typename Div_type,
             typename Size_type, typename Projection>
    auto spreadsort_rec(RandomAccessIter first, RandomAccessIter last,
                        std::vector<RandomAccessIter> &bin_cache, unsigned cache_offset,
                        std::size_t *bin_sizes, Projection projection)
        -> void
    {
      using utility::iter_move;
      auto&& proj = utility::as_function(projection);

      //This step is roughly 10% of runtime, but it helps avoid worst-case
      //behavior and improve behavior with real data
      //If you know the maximum and minimum ahead of time, you can pass those
      //values in and skip this step for the first iteration
      RandomAccessIter max, min;
      if (is_sorted_or_find_extremes(first, last, max, min, projection))
        return;
      RandomAccessIter * target_bin;
      unsigned log_divisor = get_log_divisor<int_log_mean_bin_size>(
          last - first, rough_log_2_size(Size_type((proj(*max) >> 0) - (proj(*min) >> 0))));
      Div_type div_min = proj(*min) >> log_divisor;
      Div_type div_max = proj(*max) >> log_divisor;
      unsigned bin_count = unsigned(div_max - div_min) + 1;
      unsigned cache_end;
      RandomAccessIter * bins =
        size_bins(bin_sizes, bin_cache, cache_offset, cache_end, bin_count);

      //Calculating the size of each bin; this takes roughly 10% of runtime
      for (RandomAccessIter current = first; current != last;) {
        bin_sizes[std::size_t((proj(*current) >> log_divisor) - div_min)]++;
        ++current;
      }
      //Assign the bin positions
      bins[0] = first;
      for (unsigned u = 0; u < bin_count - 1; u++)
        bins[u + 1] = bins[u] + bin_sizes[u];

      RandomAccessIter nextbinstart = first;
      //Swap into place
      //This dominates runtime, mostly in the swap and bin lookups
      for (unsigned u = 0; u < bin_count - 1; ++u) {
        RandomAccessIter * local_bin = bins + u;
        nextbinstart += bin_sizes[u];
        //Iterating over each element in this bin
        for (RandomAccessIter current = *local_bin; current < nextbinstart;
            ++current) {
          //Swapping elements in current into place until the correct
          //element has been swapped in
          for (target_bin = (bins + ((proj(*current) >> log_divisor) - div_min));
              target_bin != local_bin;
            target_bin = bins + ((proj(*current) >> log_divisor) - div_min)) {
            //3-way swap; this is about 1% faster than a 2-way swap
            //The main advantage is less copies are involved per item
            //put in the correct place
            RandomAccessIter b = *target_bin;
            ++(*target_bin);
            RandomAccessIter * b_bin = bins + ((proj(*b) >> log_divisor) - div_min);
            if (b_bin != local_bin) {
              RandomAccessIter c = *b_bin;
              ++(*b_bin);
              auto tmp = iter_move(c);
              *c = iter_move(b);
              *b = iter_move(current);
              *current = std::move(tmp);
            } else {
              auto tmp = iter_move(b);
              *b = iter_move(current);
              *current = std::move(tmp);
            }
          }
        }
        *local_bin = nextbinstart;
      }
      bins[bin_count - 1] = last;

      //If we've bucketsorted, the array is sorted and we should skip recursion
      if (!log_divisor)
        return;
      //log_divisor is the remaining range; calculating the comparison threshold
      std::size_t max_count =
        get_min_count<int_log_mean_bin_size, int_log_min_split_count,
                      int_log_finishing_count>(log_divisor);

      //Recursing
      RandomAccessIter lastPos = first;
      for (unsigned u = cache_offset; u < cache_end; lastPos = bin_cache[u],
          (void) ++u) {
        Size_type count = bin_cache[u] - lastPos;
        //don't sort unless there are at least two items to Compare
        if (count < 2)
          continue;
        //using pdqsort if its worst-case is better
        if (count < max_count)
          pdqsort(lastPos, bin_cache[u], std::less<>{}, projection);
        else
          spreadsort_rec<RandomAccessIter, Div_type, Size_type>(lastPos,
                                                                bin_cache[u],
                                                                bin_cache,
                                                                cache_end,
                                                                bin_sizes,
                                                                projection);
      }
    }

    //Holds the bin vector and makes the initial recursive call
    //Only use spreadsort if the integer can fit in a std::size_t
    template<typename RandomAccessIter, typename Div_type, typename Projection>
    auto integer_sort(RandomAccessIter first, RandomAccessIter last,
                      Div_type, Projection projection)
        -> cppsort::detail::enable_if_t<
            sizeof(Div_type) <= sizeof(std::size_t),
            void
        >
    {
      std::size_t bin_sizes[1 << max_finishing_splits];
      std::vector<RandomAccessIter> bin_cache;
      spreadsort_rec<RandomAccessIter, Div_type, std::size_t, Projection>(
          first, last, bin_cache, 0, bin_sizes, projection);
    }

    //Holds the bin vector and makes the initial recursive call
    //Only use spreadsort if the integer can fit in a std::uintmax_t
    template<typename RandomAccessIter, typename Div_type, typename Projection>
    auto integer_sort(RandomAccessIter first, RandomAccessIter last,
                      Div_type, Projection projection)
        -> cppsort::detail::enable_if_t<
            (sizeof(Div_type) > sizeof(std::size_t)) &&
            sizeof(Div_type) <= sizeof(std::uintmax_t),
            void
        >
    {
      std::size_t bin_sizes[1 << max_finishing_splits];
      std::vector<RandomAccessIter> bin_cache;
      spreadsort_rec<RandomAccessIter, Div_type, std::uintmax_t, Projection>(
          first, last, bin_cache, 0, bin_sizes, projection);
    }
  }
}}}

#endif // CPPSORT_DETAIL_SPREADSORT_DETAIL_INTEGER_SORT_H_
