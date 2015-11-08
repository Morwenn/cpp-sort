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
#include "as_function.h"

namespace cppsort
{
namespace detail
{
    // Sorts the elements *a, *b and *c using comparison function comp.
    template<typename Iterator, typename Compare, typename Projection>
    void iter_sort3(Iterator a, Iterator b, Iterator c,
                    Compare compare, Projection projection)
    {
        auto&& proj = as_function(projection);

        if (!compare(proj(*b), proj(*a)))
        {
            if (!compare(proj(*c), proj(*b))) return;

            std::iter_swap(b, c);
            if (compare(proj(*b), proj(*a)))
            {
                std::iter_swap(a, b);
            }
            return;
        }

        if (compare(proj(*c), proj(*b)))
        {
            std::iter_swap(a, c);
            return;
        }

        std::iter_swap(a, b);
        if (compare(proj(*c), proj(*b)))
        {
            std::iter_swap(b, c);
        }
    }
}}

#endif // CPPSORT_DETAIL_ITER_SORT3_H_
