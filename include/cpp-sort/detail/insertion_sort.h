/*
    pdqsort.h - Pattern-defeating quicksort.

    Copyright (c) 2015 Orson Peters
    Modified in 2015-2016 by Morwenn for inclusion into cpp-sort

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
#include <algorithm>
#include <iterator>
#include <utility>
#include <cpp-sort/utility/as_function.h>
#include "iterator_traits.h"
#include "upper_bound.h"

namespace cppsort
{
namespace detail
{
    template<typename BidirectionalIterator, typename Compare, typename Projection>
    void insertion_sort(BidirectionalIterator first,
                        BidirectionalIterator last,
                        Compare compare, Projection projection,
                        std::bidirectional_iterator_tag)
    {
        if (first == last)
        {
            return;
        }

        auto&& proj = utility::as_function(projection);

        for (BidirectionalIterator cur = std::next(first) ; cur != last ; ++cur)
        {
            BidirectionalIterator sift = cur;
            BidirectionalIterator sift_1 = std::prev(cur);

            // Compare first so we can avoid 2 moves for
            // an element already positioned correctly.
            if (compare(proj(*sift), proj(*sift_1)))
            {
                auto tmp = std::move(*sift);
                auto&& tmp_proj = proj(tmp);
                do
                {
                    *sift-- = std::move(*sift_1);
                }
                while (sift != first && compare(tmp_proj, proj(*--sift_1)));
                *sift = std::move(tmp);
            }
        }
    }

    template<typename ForwardIterator, typename Compare, typename Projection>
    void insertion_sort(ForwardIterator first, ForwardIterator last,
                        Compare compare, Projection projection,
                        std::forward_iterator_tag)
    {
        auto&& proj = utility::as_function(projection);

        for (ForwardIterator it = first ; it != last ; ++it) {
            ForwardIterator insertion_point = upper_bound(first, it, proj(*it), compare, projection);
            std::rotate(insertion_point, it, std::next(it));
        }
    }

    template<typename ForwardIterator, typename Compare, typename Projection>
    void insertion_sort(ForwardIterator first, ForwardIterator last,
                        Compare compare, Projection projection)
    {
        using category = iterator_category_t<ForwardIterator>;
        insertion_sort(first, last, compare, projection, category{});
    }
}}

#endif // CPPSORT_DETAIL_INSERTION_SORT_H_
