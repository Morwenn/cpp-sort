/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2016 Morwenn
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
#ifndef CPPSORT_DETAIL_NEATSORT_H_
#define CPPSORT_DETAIL_NEATSORT_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <algorithm>
#include <iterator>
#include <list>
#include <cpp-sort/utility/as_function.h>
#include <cpp-sort/utility/inplace_merge.h>
#include "upper_bound.h"

namespace cppsort
{
namespace detail
{
    template<typename RandomAccessIterator, typename Compare, typename Projection>
    auto neatsort(RandomAccessIterator first, RandomAccessIterator last,
                  Compare compare, Projection projection)
        -> void
    {
        auto&& proj = utility::as_function(projection);

        // Some heuristic constant, see the original paper
        static constexpr double p = 1.3;

        if (std::distance(first, last) < 2) return;

        // Vector to store the beginning of each run
        std::list<RandomAccessIterator> runs = { first };

        auto current = first;
        auto next = std::next(first);

        ////////////////////////////////////////////////////////////
        // Create runs

        while (true)
        {
            // Look for ascending run
            while (next != last && not compare(proj(*next), proj(*current)))
            {
                ++current;
                ++next;
            }

            // If the run is too small
            if (std::distance(runs.back(), next) == 1)
            {
                // Look for a descending run and reverse
                while (next != last && not compare(proj(*current), proj(*next)))
                {
                    ++current;
                    ++next;
                }
                std::reverse(runs.back(), next);

                // Check whether we can make a bigger run with
                // the following elements too
                while (next != last && not compare(proj(*next), proj(*current)))
                {
                    ++current;
                    ++next;
                }

                // If the beginning of the run is greater than the
                // end of the previous run, make that a big run
                if (runs.size() > 1 && compare(proj(*std::prev(runs.back())), proj(*runs.back())))
                {
                    runs.pop_back();
                }
            }

            // Store the beginning of the next run
            runs.push_back(next);
            if (next == last) break;

            ++current;
            ++next;
        }

        ////////////////////////////////////////////////////////////
        // Merge runs

        while (runs.size() > 3)
        {
            auto j = std::begin(runs);
            while (j != std::end(runs))
            {
                // Beginning of the runs
                auto&& one = j++;
                if (j == std::end(runs)) break;
                auto&& two = j++;
                if (j == std::end(runs)) break;
                auto&& three = j++;
                if (j == std::end(runs)) break;

                if (std::distance(*one, *two) < p * (std::distance(*two, *three) + std::distance(*three, *j)))
                {
                    utility::inplace_merge(*one, *two, *three, compare, projection);
                    runs.erase(two);
                    --j;
                }
                else
                {
                    utility::inplace_merge(*two, *three, *j, compare, projection);
                    runs.erase(three);
                }
            }
        }

        // Merge potential remaining runs
        if (runs.size() == 3)
        {
            utility::inplace_merge(
                *std::begin(runs),
                *std::next(std::begin(runs)),
                *std::next(std::next(std::begin(runs))),
                compare, projection
            );
        }
    }
}}

#endif // CPPSORT_DETAIL_NEATSORT_H_
