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
#ifndef CPPSORT_DETAIL_ITER_SORT3_H_
#define CPPSORT_DETAIL_ITER_SORT3_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <algorithm>

namespace cppsort
{
namespace detail
{
    // Sorts the elements *a, *b and *c using comparison function comp.
    template<class Iter, class Compare>
    void iter_sort3(Iter a, Iter b, Iter c, Compare comp) {
        if (!comp(*b, *a)) {
            if (!comp(*c, *b)) return;

            std::iter_swap(b, c);
            if (comp(*b, *a)) std::iter_swap(a, b);

            return;
        }

        if (comp(*c, *b)) {
            std::iter_swap(a, c);
            return;
        }

        std::iter_swap(a, b);
        if (comp(*c, *b)) std::iter_swap(b, c);
    }
}}

#endif // CPPSORT_DETAIL_ITER_SORT3_H_
