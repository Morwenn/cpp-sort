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
#ifndef CPPSORT_TESTSUITE_SPAN_H_
#define CPPSORT_TESTSUITE_SPAN_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <iterator>

////////////////////////////////////////////////////////////
// Very basic span type
//
// While the original goal of sorters is to sort some kinds
// of collections that are typically lvalues, there are more
// and more span-like types (GSL, Ranges v3...) and these
// might be passed to sorters as temporary values.
//
// Therefore sorters need to handle temporary span-like
// classes like the one below too.
//

template<typename Iterator>
class span
{
    public:

        template<typename Iterable>
        span(Iterable& iterable):
            _begin(std::begin(iterable)),
            _end(std::end(iterable))
        {}

        auto begin() const { return _begin; }
        auto end() const { return _end; }

    private:

        Iterator _begin, _end;
};

template<typename Iterable>
auto make_span(Iterable& iterable)
    -> span<decltype(std::begin(iterable))>
{
    return { iterable };
}

#endif // CPPSORT_TESTSUITE_SPAN_H_
