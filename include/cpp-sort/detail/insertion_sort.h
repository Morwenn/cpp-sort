/*
 * Copyright (c) 2015-2017 Morwenn
 * SPDX-License-Identifier: MIT
 */

/*
    pdqsort.h - Pattern-defeating quicksort.

    Copyright (c) 2015 Orson Peters

    This software is provided 'as-is', without any express or implied warranty. In no event will the
    authors be held liable for any damages arising from the use of this software.

    Permission is granted to anyone to use this software for any purpose, including commercial
    applications, and to alter it and redistribute it freely, subject to the following restrictions:

    1. The origin of this software must not be misrepresented; you must not claim that you wrote the
       original software. If you use this software in a product, an acknowledgment in the product
       documentation would be appreciated but is not required.

    2. Altered source versions must be plainly marked as such, and must not be misrepresented as
       being the original software.

    3. This notice may not be removed or altered from any source distribution.
*/
#ifndef CPPSORT_DETAIL_INSERTION_SORT_H_
#define CPPSORT_DETAIL_INSERTION_SORT_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <iterator>
#include <utility>
#include <cpp-sort/utility/as_function.h>
#include <cpp-sort/utility/iter_move.h>

namespace cppsort
{
namespace detail
{
    template<typename BidirectionalIterator, typename Compare, typename Projection>
    auto insertion_sort(BidirectionalIterator first, BidirectionalIterator last,
                        Compare compare, Projection projection)
        -> void
    {
        if (first == last) return;

        using utility::iter_move;
        auto&& comp = utility::as_function(compare);
        auto&& proj = utility::as_function(projection);

        for (BidirectionalIterator cur = std::next(first) ; cur != last ; ++cur) {
            BidirectionalIterator sift = cur;
            BidirectionalIterator sift_1 = std::prev(cur);

            // Compare first so we can avoid 2 moves for
            // an element already positioned correctly.
            if (comp(proj(*sift), proj(*sift_1))) {
                auto tmp = iter_move(sift);
                auto&& tmp_proj = proj(tmp);

                do {
                    *sift = iter_move(sift_1);
                } while (--sift != first && comp(tmp_proj, proj(*--sift_1)));
                *sift = std::move(tmp);
            }
        }
    }
}}

#endif // CPPSORT_DETAIL_INSERTION_SORT_H_
