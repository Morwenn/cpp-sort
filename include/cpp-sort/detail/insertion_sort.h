/*
    pdqsort.h - Pattern-defeating quicksort.

    Copyright (c) 2015 Orson Peters
    Modified in 2015 by Morwenn for inclusion into cpp-sort

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

namespace cppsort
{
namespace detail
{
    template<typename BidirectionalIterator, typename Compare>
    void insertion_sort(BidirectionalIterator first,
                        BidirectionalIterator last,
                        Compare compare,
                        std::bidirectional_iterator_tag)
    {
        if (first == last)
        {
            return;
        }

        for (BidirectionalIterator cur = std::next(first) ; cur != last ; ++cur)
        {
            BidirectionalIterator sift = cur;
            BidirectionalIterator sift_1 = std::prev(cur);

            // Compare first so we can avoid 2 moves for
            // an element already positioned correctly.
            if (compare(*sift, *sift_1))
            {
                auto tmp = std::move(*sift);
                do
                {
                    *sift-- = std::move(*sift_1);
                }
                while (sift != first && compare(tmp, *--sift_1));
                *sift = std::move(tmp);
            }
        }
    }

    template<typename ForwardIterator, typename Compare>
    void insertion_sort(ForwardIterator first, ForwardIterator last,
                        Compare compare,
                        std::forward_iterator_tag)
    {
        for (ForwardIterator it = first ; it != last ; ++it) {
            ForwardIterator insertion_point = std::upper_bound(first, it, *it, compare);
            std::rotate(insertion_point, it, std::next(it));
        }
    }

    template<typename ForwardIterator, typename Compare>
    void insertion_sort(ForwardIterator first, ForwardIterator last,
                        Compare compare)
    {
        using category = typename std::iterator_traits<ForwardIterator>::iterator_category;
        insertion_sort(first, last, compare, category{});
    }
}}

#endif // CPPSORT_DETAIL_INSERTION_SORT_H_
