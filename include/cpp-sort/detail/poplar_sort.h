/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2016-2017 Morwenn
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
#ifndef CPPSORT_DETAIL_POPLAR_SORT_H_
#define CPPSORT_DETAIL_POPLAR_SORT_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <iterator>
#include <utility>
#include <vector>
#include <cpp-sort/utility/as_function.h>
#include <cpp-sort/utility/bitops.h>
#include <cpp-sort/utility/iter_move.h>
#include "insertion_sort.h"
#include "iterator_traits.h"

namespace cppsort
{
namespace detail
{
    template<typename RandomAccessIterator, typename Compare, typename Projection>
    auto sift(RandomAccessIterator first, difference_type_t<RandomAccessIterator> size,
              Compare compare, Projection projection)
        -> void
    {
        if (size < 2) return;

        auto&& proj = utility::as_function(projection);

        auto root = first + (size - 1);
        auto child_root1 = root - 1;
        auto child_root2 = first + (size / 2 - 1);

        auto max_root = root;
        if (compare(proj(*max_root), proj(*child_root1)))
        {
            max_root = child_root1;
        }
        if (compare(proj(*max_root), proj(*child_root2)))
        {
            max_root = child_root2;
        }

        if (max_root != root)
        {
            using utility::iter_swap;
            iter_swap(root, max_root);
            sift(max_root - (size / 2 - 1), size / 2,
                 std::move(compare), std::move(projection));
        }
    }

    template<typename RandomAccessIterator, typename Compare, typename Projection>
    auto relocate(RandomAccessIterator first,
                  const std::vector<difference_type_t<RandomAccessIterator>>& roots,
                  difference_type_t<RandomAccessIterator> nb_poplars,
                  Compare compare, Projection projection)
        -> void
    {
        using difference_type = difference_type_t<RandomAccessIterator>;
        auto&& proj = utility::as_function(projection);

        difference_type m = nb_poplars;
        for (difference_type j = 1 ; j < nb_poplars ; ++j)
        {
            if (compare(proj(first[roots[m]-1]), proj(first[roots[j]-1])))
            {
                m = j;
            }
        }

        if (m != nb_poplars)
        {
            using utility::iter_swap;
            iter_swap(first + (roots[m] - 1), first + (roots[nb_poplars] - 1));
            sift(first + roots[m-1], roots[m] - roots[m-1],
                 std::move(compare), std::move(projection));
        }
    }

    template<typename RandomAccessIterator, typename Compare, typename Projection>
    auto make_poplar(RandomAccessIterator first, RandomAccessIterator last,
                     Compare compare, Projection projection)
        -> void
    {
        auto size = std::distance(first, last);
        if (size < 16)
        {
            // A sorted collection is a valid poplar heap;
            // when the heap is small, using insertion sort
            // should be faster
            insertion_sort(std::move(first), std::move(last),
                           std::move(compare), std::move(projection));
            return;
        }

        auto&& proj = utility::as_function(projection);

        auto middle = first + size / 2;
        make_poplar(first, middle, compare, projection);
        make_poplar(middle, std::prev(last), compare, projection);

        // Make sure that the poplar on the right has a bigger root
        // that the one on the left. Apparently, it makes things
        // faster, probably helping the branch predictor or
        // something...
        if (compare(proj(*(last - 2)), proj(*std::prev(middle))))
        {
            using utility::iter_swap;
            iter_swap(std::prev(middle), last - 2);
            sift(first, middle - first, compare, projection);
        }

        sift(std::move(first), size, std::move(compare), std::move(projection));
    }

    template<typename RandomAccessIterator, typename Compare, typename Projection>
    auto poplar_sort(RandomAccessIterator first, RandomAccessIterator last,
                     Compare compare, Projection projection)
        -> void
    {
        using difference_type = difference_type_t<RandomAccessIterator>;

        // Size of the unsorted subsequence
        auto size = std::distance(first, last);
        if (size < 2) return;

        std::vector<difference_type> roots(utility::log2(size) + 1, 0);
        auto poplar_size = utility::hyperfloor(size) - 1;
        difference_type nb_poplars = 0;

        // Make the poplar heap
        auto it = first;
        do
        {
            if (std::distance(it, last) >= poplar_size)
            {
                make_poplar(it, it + poplar_size, compare, projection);
                roots[++nb_poplars] = std::distance(first, it + poplar_size);
                it += poplar_size;
            }
            else
            {
                poplar_size = (poplar_size + 1) / 2 - 1;
            }
        } while (poplar_size > 0);

        // Sort the poplar heap
        while (size > 1)
        {
            relocate(first, roots, nb_poplars, compare, projection);
            if (roots[nb_poplars-1] == size - 1)
            {
                --nb_poplars;
            }
            else
            {
                roots[nb_poplars] = (roots[nb_poplars-1] + size) / 2;
                ++nb_poplars;
                roots[nb_poplars] = size - 1;
            }

            --size;
        }
    }
}}

#endif // CPPSORT_DETAIL_POPLAR_SORT_H_
