/*
 * Copyright (c) 2015-2022 Morwenn
 * SPDX-License-Identifier: MIT
 */

// Details for a templated general-case hybrid-radix string_sort.

//          Copyright Steven J. Ross 2001 - 2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

// See http://www.boost.org/libs/sort for library home page.

/*
Some improvements suggested by:
Phil Endecott and Frank Gennari
*/
#ifndef CPPSORT_DETAIL_SPREADSORT_DETAIL_STRING_SORT_H_
#define CPPSORT_DETAIL_SPREADSORT_DETAIL_STRING_SORT_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <algorithm>
#include <cstddef>
#include <cstring>
#include <iterator>
#include <memory>
#include <tuple>
#include <type_traits>
#include <vector>
#include <cpp-sort/utility/as_function.h>
#include <cpp-sort/utility/functional.h>
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
    //Offsetting on identical characters.  This function works a chunk of
    //characters at a time for cache efficiency and optimal worst-case
    //performance.
    template<typename Unsigned_char_type, typename RandomAccessIter, typename Projection>
    auto update_offset(RandomAccessIter first, RandomAccessIter finish,
                       std::size_t &char_offset, Projection projection)
        -> void
    {
      auto&& proj = utility::as_function(projection);

      constexpr int max_step_size = 64;
      constexpr int char_size = sizeof(Unsigned_char_type);

      std::size_t nextOffset = char_offset;
      int step_size = max_step_size / char_size;
      while (true) {
        RandomAccessIter curr = first;
        do {
          //Ignore empties, but if the nextOffset would exceed the length or
          //not match, exit; we've found the last matching character
          //This will reduce the step_size if the current step doesn't match.
          if (proj(*curr).size() > char_offset) {
            if(proj(*curr).size() <= (nextOffset + step_size)) {
              step_size = proj(*curr).size() - nextOffset - 1;
              if (step_size < 1) {
                char_offset = nextOffset;
                return;
              }
            }
            const int step_byte_size = step_size * char_size;
            if (std::memcmp(proj(*curr).data() + nextOffset, proj(*first).data() + nextOffset,
                            step_byte_size) != 0) {
              if (step_size == 1) {
                char_offset = nextOffset;
                return;
              }
              step_size = (step_size > 4) ? 4 : 1;
              continue;
            }
          }
          ++curr;
        } while (curr != finish);
        nextOffset += step_size;
      }
    }

    //This comparator assumes strings are identical up to char_offset
    template<typename Projection, typename Unsigned_char_type>
    struct offset_less_than
    {
        offset_less_than(std::size_t char_offset, Projection projection):
            data(char_offset, projection)
        {}

        template<typename T, typename U>
        auto operator()(const T& x, const U& y) const
            -> bool
        {
            auto&& proj = utility::as_function(std::get<1>(data));
            auto&& proj_x = proj(x);
            auto&& proj_y = proj(y);

            std::size_t minSize = (std::min)(proj_x.size(), proj_y.size());
            for (std::size_t u = std::get<0>(data); u < minSize; ++u) {
                static_assert(sizeof(proj_x[u]) == sizeof(Unsigned_char_type), "");
                if (static_cast<Unsigned_char_type>(proj_x[u]) != static_cast<Unsigned_char_type>(proj_y[u])) {
                    return static_cast<Unsigned_char_type>(proj_x[u]) <
                           static_cast<Unsigned_char_type>(proj_y[u]);
                }
            }
            return proj_x.size() < proj_y.size();
        }

        // Pack char_offset and projection
        std::tuple<std::size_t, Projection> data;
    };

    //Compares strings assuming they are identical up to char_offset
    template<typename Projection, typename Unsigned_char_type>
    struct offset_greater_than
    {
        offset_greater_than(std::size_t char_offset, Projection projection):
            data(char_offset, projection)
        {}

        template<typename T, typename U>
        auto operator()(const T& x, const U& y) const
            -> bool
        {
            auto&& proj = utility::as_function(std::get<1>(data));
            auto&& proj_x = proj(x);
            auto&& proj_y = proj(y);

            std::size_t minSize = (std::min)(proj_x.size(), proj_y.size());
            for (std::size_t u = std::get<0>(data); u < minSize; ++u) {
                static_assert(sizeof(proj_x[u]) == sizeof(Unsigned_char_type), "");
                if (static_cast<Unsigned_char_type>(proj_x[u]) != static_cast<Unsigned_char_type>(proj_y[u])) {
                    return static_cast<Unsigned_char_type>(proj_x[u]) >
                           static_cast<Unsigned_char_type>(proj_y[u]);
                }
            }
            return proj_x.size() > proj_y.size();
        }

        // Pack char_offset and projection
        std::tuple<std::size_t, Projection> data;
    };

    //String sorting recursive implementation
    template<typename Unsigned_char_type, typename RandomAccessIter, typename Projection>
    auto string_sort_rec(RandomAccessIter first, RandomAccessIter last,
                         std::size_t char_offset,
                         std::vector<RandomAccessIter> &bin_cache,
                         unsigned cache_offset, std::size_t *bin_sizes,
                         Projection projection)
        -> void
    {
      auto&& proj = utility::as_function(projection);

      //This section makes handling of long identical substrings much faster
      //with a mild average performance impact.
      //Iterate to the end of the empties.  If all empty, return
      while (proj(*first).size() <= char_offset) {
        if (++first == last)
          return;
      }
      RandomAccessIter finish = std::prev(last);
      //Getting the last non-empty
      for (;proj(*finish).size() <= char_offset; --finish);
      ++finish;
      //Offsetting on identical characters.  This section works
      //a few characters at a time for optimal worst-case performance.
      update_offset<Unsigned_char_type>(first, finish, char_offset, projection);

      constexpr unsigned bin_count = (1 << (sizeof(Unsigned_char_type)*8));
      //Equal worst-case of radix and comparison is when bin_count = n*log(n).
      constexpr unsigned max_size = bin_count;
      constexpr unsigned membin_count = bin_count + 1;
      unsigned cache_end;
      RandomAccessIter * bins = size_bins(bin_sizes, bin_cache, cache_offset,
                                          cache_end, membin_count) + 1;

      //Calculating the size of each bin; this takes roughly 10% of runtime
      for (RandomAccessIter current = first; current != last; ++current) {
        if (proj(*current).size() <= char_offset) {
          bin_sizes[0]++;
        }
        else
          bin_sizes[static_cast<Unsigned_char_type>(proj(*current)[char_offset])
                    + 1]++;
      }
      //Assign the bin positions
      bin_cache[cache_offset] = first;
      for (unsigned u = 0; u < membin_count - 1; u++)
        bin_cache[cache_offset + u + 1] =
          bin_cache[cache_offset + u] + bin_sizes[u];

      //Swap into place
      RandomAccessIter next_bin_start = first;
      //handling empty bins
      RandomAccessIter * local_bin = &(bin_cache[cache_offset]);
      next_bin_start +=  bin_sizes[0];
      RandomAccessIter * target_bin;
      //Iterating over each element in the bin of empties
      for (RandomAccessIter current = *local_bin; current < next_bin_start;
          ++current) {
        //empties belong in this bin
        while (proj(*current).size() > char_offset) {
          target_bin = bins + static_cast<Unsigned_char_type>(proj(*current)[char_offset]);
          iter_swap(current, *target_bin);
          ++(*target_bin);
        }
      }
      *local_bin = next_bin_start;
      //iterate backwards to find the last bin with elements in it
      //this saves iterations in multiple loops
      unsigned last_bin = bin_count - 1;
      for (; last_bin && !bin_sizes[last_bin + 1]; --last_bin);
      //This dominates runtime, mostly in the swap and bin lookups
      for (unsigned u = 0; u < last_bin; ++u) {
        local_bin = bins + u;
        next_bin_start += bin_sizes[u + 1];
        //Iterating over each element in this bin
        for (RandomAccessIter current = *local_bin; current < next_bin_start;
            ++current) {
          //Swapping into place until the correct element has been swapped in
          for (target_bin = bins + static_cast<Unsigned_char_type>
               (proj(*current)[char_offset]); target_bin != local_bin;
               target_bin = bins + static_cast<Unsigned_char_type>
               (proj(*current)[char_offset])) {
            iter_swap(current, *target_bin);
            ++(*target_bin);
          }
        }
        *local_bin = next_bin_start;
      }
      bins[last_bin] = last;
      //Recursing
      RandomAccessIter lastPos = bin_cache[cache_offset];
      //Skip this loop for empties
      for (unsigned u = cache_offset + 1; u < cache_offset + last_bin + 2;
          lastPos = bin_cache[u], (void) ++u) {
        std::size_t count = bin_cache[u] - lastPos;
        //don't sort unless there are at least two items to Compare
        if (count < 2)
          continue;
        //using pdqsort if its worst-case is better
        if (count < max_size)
          pdqsort(lastPos, bin_cache[u],
                  offset_less_than<Projection, Unsigned_char_type>(
                    char_offset + 1, projection),
                  utility::identity{});
        else
          string_sort_rec<Unsigned_char_type>(lastPos, bin_cache[u], char_offset + 1,
                                              bin_cache, cache_end, bin_sizes, projection);
      }
    }

    //Sorts strings in reverse order, with empties at the end
    template<typename Unsigned_char_type, typename RandomAccessIter, typename Projection>
    auto reverse_string_sort_rec(RandomAccessIter first, RandomAccessIter last,
                                 std::size_t char_offset,
                                 std::vector<RandomAccessIter> &bin_cache,
                                 unsigned cache_offset, std::size_t *bin_sizes,
                                 Projection projection)
        -> void
    {
      auto&& proj = utility::as_function(projection);

      //This section makes handling of long identical substrings much faster
      //with a mild average performance impact.
      RandomAccessIter curr = first;
      //Iterate to the end of the empties.  If all empty, return
      while (proj(*curr).size() <= char_offset) {
        if (++curr == last)
          return;
      }
      //Getting the last non-empty
      while (proj(*(--last)).size() <= char_offset);
      ++last;
      //Offsetting on identical characters.  This section works
      //a few characters at a time for optimal worst-case performance.
      update_offset<Unsigned_char_type>(curr, last, char_offset, projection);
      RandomAccessIter * target_bin;

      constexpr unsigned bin_count = (1 << (sizeof(Unsigned_char_type)*8));
      //Equal worst-case of radix and comparison when bin_count = n*log(n).
      constexpr unsigned max_size = bin_count;
      constexpr unsigned membin_count = bin_count + 1;
      constexpr unsigned max_bin = bin_count - 1;
      unsigned cache_end;
      RandomAccessIter * bins = size_bins(bin_sizes, bin_cache, cache_offset,
                                          cache_end, membin_count);
      RandomAccessIter * end_bin = std::addressof(bin_cache[cache_offset + max_bin]);

      //Calculating the size of each bin; this takes roughly 10% of runtime
      for (RandomAccessIter current = first; current != last; ++current) {
        if (proj(*current).size() <= char_offset) {
          bin_sizes[bin_count]++;
        }
        else
          bin_sizes[max_bin - static_cast<Unsigned_char_type>
            (proj(*current)[char_offset])]++;
      }
      //Assign the bin positions
      bin_cache[cache_offset] = first;
      for (unsigned u = 0; u < membin_count - 1; u++)
        bin_cache[cache_offset + u + 1] =
          bin_cache[cache_offset + u] + bin_sizes[u];

      //Swap into place
      RandomAccessIter next_bin_start = last;
      //handling empty bins
      RandomAccessIter * local_bin = &(bin_cache[cache_offset + bin_count]);
      RandomAccessIter lastFull = *local_bin;
      //Iterating over each element in the bin of empties
      for (RandomAccessIter current = *local_bin; current < next_bin_start;
          ++current) {
        //empties belong in this bin
        while (proj(*current).size() > char_offset) {
          target_bin = end_bin - static_cast<Unsigned_char_type>(proj(*current)[char_offset]);
          iter_swap(current, *target_bin);
          ++(*target_bin);
        }
      }
      *local_bin = next_bin_start;
      next_bin_start = first;
      //iterate backwards to find the last non-empty bin
      //this saves iterations in multiple loops
      unsigned last_bin = max_bin;
      for (; last_bin && !bin_sizes[last_bin]; --last_bin);
      //This dominates runtime, mostly in the swap and bin lookups
      for (unsigned u = 0; u < last_bin; ++u) {
        local_bin = bins + u;
        next_bin_start += bin_sizes[u];
        //Iterating over each element in this bin
        for (RandomAccessIter current = *local_bin; current < next_bin_start;
            ++current) {
          //Swapping into place until the correct element has been swapped in
          for (target_bin =
               end_bin - static_cast<Unsigned_char_type>(proj(*current)[char_offset]);
               target_bin != local_bin;
               target_bin =
               end_bin - static_cast<Unsigned_char_type>(proj(*current)[char_offset])) {
            iter_swap(current, *target_bin);
            ++(*target_bin);
          }
        }
        *local_bin = next_bin_start;
      }
      bins[last_bin] = lastFull;
      //Recursing
      RandomAccessIter lastPos = first;
      //Skip this loop for empties
      for (unsigned u = cache_offset; u <= cache_offset + last_bin;
          lastPos = bin_cache[u], (void) ++u) {
        std::size_t count = bin_cache[u] - lastPos;
        //don't sort unless there are at least two items to Compare
        if (count < 2)
          continue;
        //using pdqsort if its worst-case is better
        if (count < max_size)
          pdqsort(lastPos, bin_cache[u],
                  offset_greater_than<Projection, Unsigned_char_type>(
                    char_offset + 1, projection),
                  utility::identity{});
        else
          reverse_string_sort_rec<Unsigned_char_type>(lastPos, bin_cache[u], char_offset + 1,
                                                      bin_cache, cache_end, bin_sizes, projection);
      }
    }

    //Holds the bin vector and makes the initial recursive call
    template<typename RandomAccessIter, typename Projection,
             typename Unsigned_char_type>
    auto string_sort(RandomAccessIter first, RandomAccessIter last,
                     Projection projection, Unsigned_char_type)
        -> cppsort::detail::enable_if_t<sizeof(Unsigned_char_type) <= 2, void>
    {
      std::size_t bin_sizes[(1 << (8 * sizeof(Unsigned_char_type))) + 1];
      std::vector<RandomAccessIter> bin_cache;
      string_sort_rec<Unsigned_char_type>(first, last, 0, bin_cache, 0,
                                          bin_sizes, projection);
    }

    //Holds the bin vector and makes the initial recursive call
    template<typename RandomAccessIter, typename Projection,
             typename Unsigned_char_type>
    auto reverse_string_sort(RandomAccessIter first, RandomAccessIter last,
                             Projection projection, Unsigned_char_type)
        -> cppsort::detail::enable_if_t<sizeof(Unsigned_char_type) <= 2, void>
    {
      std::size_t bin_sizes[(1 << (8 * sizeof(Unsigned_char_type))) + 1];
      std::vector<RandomAccessIter> bin_cache;
      reverse_string_sort_rec<Unsigned_char_type>(first, last, 0, bin_cache, 0,
                                                  bin_sizes, projection);
    }
  }
}}}

#endif // CPPSORT_DETAIL_SPREADSORT_DETAIL_STRING_SORT_H_
