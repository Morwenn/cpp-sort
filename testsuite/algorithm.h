/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2015-2018 Morwenn
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
#ifndef CPPSORT_TESTSUITE_ALGORITHM_H_
#define CPPSORT_TESTSUITE_ALGORITHM_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <functional>
#include <iterator>
#include <cpp-sort/utility/as_function.h>
#include <cpp-sort/utility/functional.h>

namespace helpers
{
    template<
        typename Iterator,
        typename Compare = std::less<>,
        typename Projection = cppsort::utility::identity
    >
    auto is_sorted(Iterator first, Iterator last,
                   Compare compare={}, Projection projection={})
        -> bool
    {
        auto&& comp = cppsort::utility::as_function(compare);
        auto&& proj = cppsort::utility::as_function(projection);

        for (auto it = std::next(first) ; it != last ; ++it) {
            if (comp(proj(*it), proj(*first))) {
                return false;
            }
        }
        return true;
    }

    template<
        typename ForwardIterator,
        typename T,
        typename Projection = cppsort::utility::identity
    >
    auto iota(ForwardIterator first, ForwardIterator last,
              T value, Projection projection={})
        -> void
    {
        auto&& proj = cppsort::utility::as_function(projection);

        while (first != last)
        {
            proj(*first++) = value;
            ++value;
        }
    }
}

#endif // CPPSORT_TESTSUITE_ALGORITHM_H_
