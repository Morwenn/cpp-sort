/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2015 Morwenn
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
#ifndef CPPSORT_SORTERS_HYBRID_SORTER_H_
#define CPPSORT_SORTERS_HYBRID_SORTER_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <functional>
#include <iterator>

namespace cppsort
{
    template<typename... Sorters>
    class hybrid_sorter
    {
        private:

            template<typename Head, typename... Tail>
            struct merge_sorters:
                Head, merge_sorters<Tail...>
            {
                using Head::operator();
                using merge_sorters<Tail...>::operator();
            };

            template<typename Head>
            struct merge_sorters<Head>:
                Head
            {
                using Head::operator();
            };

            merge_sorters<Sorters...> sorters;

        public:

            template<
                typename Iterable,
                typename Compare = std::less<>
            >
            auto operator()(Iterable& iterable, Compare compare={}) const
                -> void
            {
                using category = typename std::iterator_traits<decltype(std::begin(iterable))>::iterator_category;
                sorters(iterable, compare, category{});
            }
    };
}

#endif // CPPSORT_SORTERS_HYBRID_SORTER_H_
