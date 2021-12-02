/*
 * Copyright (c) 2015-2021 Morwenn
 * SPDX-License-Identifier: MIT
 */

// Details for templated Spreadsort-based float_sort.

//          Copyright Steven J. Ross 2001 - 2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

// See http://www.boost.org/libs/sort for library home page.

/*
Some improvements suggested by:
Phil Endecott and Frank Gennari
*/
#ifndef CPPSORT_DETAIL_SPREADSORT_DETAIL_FLOAT_SORT_H_
#define CPPSORT_DETAIL_SPREADSORT_DETAIL_FLOAT_SORT_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <cstddef>
#include <cstdint>
#include <functional>
#include <limits>
#include <type_traits>
#include <utility>
#include <vector>
#include <cpp-sort/utility/as_function.h>
#include <cpp-sort/utility/iter_move.h>
#include "common.h"
#include "constants.h"
#include "integer_sort.h"
#include "../../iterator_traits.h"
#include "../../memcpy_cast.h"
#include "../../pdqsort.h"
#include "../../type_traits.h"

namespace cppsort
{
namespace detail
{
namespace spreadsort
{
namespace detail
{
    //Specialized swap loops for floating-point casting
    template<typename RandomAccessIter, typename Div_type, typename Projection>
    auto inner_float_swap_loop(RandomAccessIter * bins,
                               const RandomAccessIter & nextbinstart, unsigned ii,
                               const unsigned log_divisor, const Div_type div_min,
                               Projection projection)
        -> void
    {
      using utility::iter_move;
      auto&& proj = utility::as_function(projection);

      RandomAccessIter * local_bin = bins + ii;
      for (RandomAccessIter current = *local_bin; current < nextbinstart;
          ++current) {
        for (RandomAccessIter * target_bin =
            (bins + ((memcpy_cast<Div_type>(proj(*current)) >>
                      log_divisor) - div_min));  target_bin != local_bin;
          target_bin = bins + ((memcpy_cast<Div_type>(proj(*current)) >> log_divisor)
                            - div_min)) {
          RandomAccessIter b = *target_bin;
          ++(*target_bin);
          RandomAccessIter * b_bin = bins + ((memcpy_cast<Div_type>(proj(*b)) >> log_divisor)
                                          - div_min);
          //Three-way swap; if the item to be swapped doesn't belong in the
          //current bin, swap it to where it belongs
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

    template<typename RandomAccessIter, typename Div_type, typename Projection>
    auto float_swap_loop(RandomAccessIter * bins,
                         RandomAccessIter & nextbinstart, unsigned ii,
                         const std::size_t *bin_sizes,
                         const unsigned log_divisor, const Div_type div_min,
                         Projection projection)
        -> void
    {
      nextbinstart += bin_sizes[ii];
      inner_float_swap_loop<RandomAccessIter, Div_type>
        (bins, nextbinstart, ii, log_divisor, div_min, projection);
    }

    // Return true if the list is sorted.  Otherwise, find the minimum and
    // maximum.  Values are cast to Cast_type before comparison.
    template<typename RandomAccessIter, typename Cast_type, typename Projection>
    auto is_sorted_or_find_extremes(RandomAccessIter current, RandomAccessIter last,
                                    Cast_type & max, Cast_type & min,
                                    Projection projection)
        -> bool
    {
      auto&& proj = utility::as_function(projection);

      min = max = memcpy_cast<Cast_type>(proj(*current));
      RandomAccessIter prev = current;
      bool sorted = true;
      while (++current < last) {
        Cast_type value = memcpy_cast<Cast_type>(proj(*current));
        sorted &= proj(*current) >= proj(*prev);
        prev = current;
        if (max < value)
          max = value;
        else if (value < min)
          min = value;
      }
      return sorted;
    }

    //Special-case sorting of positive floats with casting
    template<typename RandomAccessIter, typename Div_type,
             typename Size_type, typename Projection>
    auto positive_float_sort_rec(RandomAccessIter first, RandomAccessIter last,
                                 std::vector<RandomAccessIter> &bin_cache, unsigned cache_offset,
                                 std::size_t *bin_sizes, Projection projection)
        -> void
    {
      Div_type max, min;
      if (is_sorted_or_find_extremes<RandomAccessIter, Div_type>(
        first, last, max, min, projection))
        return;

      auto&& proj = utility::as_function(projection);

      unsigned log_divisor = get_log_divisor<float_log_mean_bin_size>(
          last - first, rough_log_2_size(Size_type(max - min)));
      Div_type div_min = min >> log_divisor;
      Div_type div_max = max >> log_divisor;
      unsigned bin_count = unsigned(div_max - div_min) + 1;
      unsigned cache_end;
      RandomAccessIter * bins = size_bins(bin_sizes, bin_cache, cache_offset,
                                          cache_end, bin_count);

      //Calculating the size of each bin
      for (RandomAccessIter current = first; current != last;) {
        bin_sizes[unsigned((memcpy_cast<Div_type>(
            proj(*current)) >> log_divisor) - div_min)]++;
        ++current;
      }
      bins[0] = first;
      for (unsigned u = 0; u < bin_count - 1; u++)
        bins[u + 1] = bins[u] + bin_sizes[u];


      //Swap into place
      RandomAccessIter nextbinstart = first;
      for (unsigned u = 0; u < bin_count - 1; ++u)
        float_swap_loop<RandomAccessIter, Div_type>
          (bins, nextbinstart, u, bin_sizes, log_divisor, div_min, projection);
      bins[bin_count - 1] = last;

      //Return if we've completed bucketsorting
      if (!log_divisor)
        return;

      //Recursing
      std::size_t max_count = get_min_count<float_log_mean_bin_size,
                                            float_log_min_split_count,
                                            float_log_finishing_count>(log_divisor);
      RandomAccessIter lastPos = first;
      for (unsigned u = cache_offset; u < cache_end; lastPos = bin_cache[u],
          ++u) {
        std::size_t count = bin_cache[u] - lastPos;
        if (count < 2)
          continue;
        if (count < max_count)
          pdqsort(lastPos, bin_cache[u], std::less<>{}, projection);
        else
          positive_float_sort_rec<RandomAccessIter, Div_type, Size_type>
            (lastPos, bin_cache[u], bin_cache, cache_end, bin_sizes, projection);
      }
    }

    //Sorting negative floats
    //Bins are iterated in reverse because max_neg_float = min_neg_int
    template<typename RandomAccessIter, typename Div_type,
             typename Size_type, typename Projection>
    auto negative_float_sort_rec(RandomAccessIter first, RandomAccessIter last,
                                 std::vector<RandomAccessIter> &bin_cache,
                                 unsigned cache_offset, std::size_t *bin_sizes,
                                 Projection projection)
        -> void
    {
      Div_type max, min;
      if (is_sorted_or_find_extremes<RandomAccessIter, Div_type>(
        first, last, max, min, projection))
        return;

      auto&& proj = utility::as_function(projection);

      unsigned log_divisor = get_log_divisor<float_log_mean_bin_size>(
          last - first, rough_log_2_size(Size_type(max - min)));
      Div_type div_min = min >> log_divisor;
      Div_type div_max = max >> log_divisor;
      unsigned bin_count = unsigned(div_max - div_min) + 1;
      unsigned cache_end;
      RandomAccessIter * bins = size_bins(bin_sizes, bin_cache, cache_offset,
                                          cache_end, bin_count);

      //Calculating the size of each bin
      for (RandomAccessIter current = first; current != last;) {
        bin_sizes[unsigned((memcpy_cast<Div_type>(
            proj(*current)) >> log_divisor) - div_min)]++;
        ++current;
      }
      bins[bin_count - 1] = first;
      for (int ii = bin_count - 2; ii >= 0; --ii)
        bins[ii] = bins[ii + 1] + bin_sizes[ii + 1];

      //Swap into place
      RandomAccessIter nextbinstart = first;
      //The last bin will always have the correct elements in it
      for (int ii = bin_count - 1; ii > 0; --ii)
        float_swap_loop<RandomAccessIter, Div_type>
          (bins, nextbinstart, ii, bin_sizes, log_divisor, div_min, projection);
      //Update the end position because we don't process the last bin
      bin_cache[cache_offset] = last;

      //Return if we've completed bucketsorting
      if (!log_divisor)
        return;

      //Recursing
      std::size_t max_count = get_min_count<float_log_mean_bin_size,
                                            float_log_min_split_count,
                                            float_log_finishing_count>(log_divisor);
      RandomAccessIter lastPos = first;
      for (int ii = cache_end - 1; ii >= static_cast<int>(cache_offset);
          lastPos = bin_cache[ii], (void) --ii) {
        std::size_t count = bin_cache[ii] - lastPos;
        if (count < 2)
          continue;
        if (count < max_count)
          pdqsort(lastPos, bin_cache[ii], std::less<>{}, projection);
        else
          negative_float_sort_rec<RandomAccessIter, Div_type, Size_type>
            (lastPos, bin_cache[ii], bin_cache, cache_end, bin_sizes, projection);
      }
    }

    //Casting special-case for floating-point sorting
    template<typename RandomAccessIter, typename Div_type,
             typename Size_type, typename Projection>
    auto float_sort_rec(RandomAccessIter first, RandomAccessIter last,
                        std::vector<RandomAccessIter> &bin_cache, unsigned cache_offset,
                        std::size_t *bin_sizes, Projection projection)
        -> void
    {
      Div_type max, min;
      if (is_sorted_or_find_extremes<RandomAccessIter, Div_type>(
        first, last, max, min, projection))
        return;

      auto&& proj = utility::as_function(projection);

      unsigned log_divisor = get_log_divisor<float_log_mean_bin_size>(
          last - first, rough_log_2_size(Size_type(max - min)));
      Div_type div_min = min >> log_divisor;
      Div_type div_max = max >> log_divisor;
      unsigned bin_count = unsigned(div_max - div_min) + 1;
      unsigned cache_end;
      RandomAccessIter * bins = size_bins(bin_sizes, bin_cache, cache_offset,
                                          cache_end, bin_count);

      //Calculating the size of each bin
      for (RandomAccessIter current = first; current != last;) {
        bin_sizes[unsigned((memcpy_cast<Div_type>(
            proj(*current)) >> log_divisor) - div_min)]++;
        ++current;
      }
      //The index of the first positive bin
      //Must be divided small enough to fit into an integer
      unsigned first_positive = (div_min < 0) ? unsigned(-div_min) : 0;
      //Resetting if all bins are negative
      if (cache_offset + first_positive > cache_end)
        first_positive = cache_end - cache_offset;
      //Reversing the order of the negative bins
      //Note that because of the negative/positive ordering direction flip
      //We can not depend upon bin order and positions matching up
      //so bin_sizes must be reused to contain the end of the bin
      if (first_positive > 0) {
        bins[first_positive - 1] = first;
        for (int ii = first_positive - 2; ii >= 0; --ii) {
          bins[ii] = first + bin_sizes[ii + 1];
          bin_sizes[ii] += bin_sizes[ii + 1];
        }
        //Handling positives following negatives
        if (first_positive < bin_count) {
          bins[first_positive] = first + bin_sizes[0];
          bin_sizes[first_positive] += bin_sizes[0];
        }
      }
      else
        bins[0] = first;
      for (unsigned u = first_positive; u < bin_count - 1; u++) {
        bins[u + 1] = first + bin_sizes[u];
        bin_sizes[u + 1] += bin_sizes[u];
      }

      //Swap into place
      RandomAccessIter nextbinstart = first;
      for (unsigned u = 0; u < bin_count; ++u) {
        nextbinstart = first + bin_sizes[u];
        inner_float_swap_loop<RandomAccessIter, Div_type>
          (bins, nextbinstart, u, log_divisor, div_min, projection);
      }

      if (!log_divisor)
        return;

      //Handling negative values first
      std::size_t max_count = get_min_count<float_log_mean_bin_size,
                                            float_log_min_split_count,
                                            float_log_finishing_count>(log_divisor);
      RandomAccessIter lastPos = first;
      for (int ii = cache_offset + first_positive - 1;
           ii >= static_cast<int>(cache_offset);
           lastPos = bin_cache[ii--]) {
        std::size_t count = bin_cache[ii] - lastPos;
        if (count < 2)
          continue;
        if (count < max_count)
          pdqsort(lastPos, bin_cache[ii], std::less<>{}, projection);
        //sort negative values using reversed-bin spreadsort
        else
          negative_float_sort_rec<RandomAccessIter, Div_type, Size_type>
            (lastPos, bin_cache[ii], bin_cache, cache_end, bin_sizes, projection);
      }

      for (unsigned u = cache_offset + first_positive; u < cache_end;
          lastPos = bin_cache[u], (void) ++u) {
        std::size_t count = bin_cache[u] - lastPos;
        if (count < 2)
          continue;
        if (count < max_count)
          pdqsort(lastPos, bin_cache[u], std::less<>{}, projection);
        //sort positive values using normal spreadsort
        else
          positive_float_sort_rec<RandomAccessIter, Div_type, Size_type>
            (lastPos, bin_cache[u], bin_cache, cache_end, bin_sizes, projection);
      }
    }

    //Checking whether the value type is a float, and trying a 32-bit integer
    template<typename RandomAccessIter, typename Projection>
    auto float_sort(RandomAccessIter first, RandomAccessIter last, Projection projection)
        -> cppsort::detail::enable_if_t<
            sizeof(std::uint32_t) == sizeof(projected_t<RandomAccessIter, Projection>) &&
            std::numeric_limits<projected_t<RandomAccessIter, Projection>>::is_iec559,
            void
        >
    {
      std::size_t bin_sizes[1 << max_finishing_splits];
      std::vector<RandomAccessIter> bin_cache;
      float_sort_rec<RandomAccessIter, std::int32_t, std::uint32_t>
        (first, last, bin_cache, 0, bin_sizes, projection);
    }

    //Checking whether the value type is a double, and using a 64-bit integer
    template<typename RandomAccessIter, typename Projection>
    auto float_sort(RandomAccessIter first, RandomAccessIter last, Projection projection)
        -> cppsort::detail::enable_if_t<
            sizeof(std::uint64_t) == sizeof(projected_t<RandomAccessIter, Projection>) &&
            std::numeric_limits<projected_t<RandomAccessIter, Projection>>::is_iec559,
            void
        >
    {
      std::size_t bin_sizes[1 << max_finishing_splits];
      std::vector<RandomAccessIter> bin_cache;
      float_sort_rec<RandomAccessIter, std::int64_t, std::uint64_t>
        (first, last, bin_cache, 0, bin_sizes, projection);
    }
}}}}

#endif // CPPSORT_DETAIL_SPREADSORT_DETAIL_FLOAT_SORT_H_
