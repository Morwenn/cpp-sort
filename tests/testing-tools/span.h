/*
 * Copyright (c) 2016-2021 Morwenn
 * SPDX-License-Identifier: MIT
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

        template<typename Range>
        explicit span(Range& range):
            _begin(std::begin(range)),
            _end(std::end(range))
        {}

        auto begin() const { return _begin; }
        auto end() const { return _end; }

    private:

        Iterator _begin, _end;
};

template<typename Range>
auto make_span(Range& range)
    -> span<decltype(std::begin(range))>
{
    return span<decltype(std::begin(range))>(range);
}

#endif // CPPSORT_TESTSUITE_SPAN_H_
