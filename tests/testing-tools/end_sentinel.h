/*
 * Copyright (c) 2022 Morwenn
 * SPDX-License-Identifier: MIT
 */
#ifndef CPPSORT_TESTSUITE_END_SENTINEL_H_
#define CPPSORT_TESTSUITE_END_SENTINEL_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <cpp-sort/mstd/iterator.h>
#include <cpp-sort/mstd/ranges.h>

////////////////////////////////////////////////////////////
// Sentinel around an iterator
//
// Simple sentinel type that is constructed with an iterator
// and compares it against any iterator compared to the sentinel
// instance.

template<cppsort::mstd::forward_iterator Iterator>
struct end_sentinel
{
    private:

        Iterator it_;

    public:

        end_sentinel() = default;
        end_sentinel(const end_sentinel&) = default;
        end_sentinel(end_sentinel&&) = default;
        end_sentinel& operator=(const end_sentinel&) = default;
        end_sentinel& operator=(end_sentinel&&) = default;

        constexpr explicit end_sentinel(Iterator it):
            it_(std::move(it))
        {}

        constexpr auto operator==(const Iterator& other) const
            -> bool
        {
            return it_ == other;
        }
};

////////////////////////////////////////////////////////////
// Range around an iterator and an end_sentinel
//
// Simple sentinel type that is constructed with an iterator
// and compares it against any iterator compared to the sentinel
// instance.

template<cppsort::mstd::forward_iterator Iterator>
struct end_sentinel_range
{
    private:

        Iterator begin_;
        Iterator end_;

    public:

        constexpr end_sentinel_range(Iterator it1, Iterator it2):
            begin_(std::move(it1)),
            end_(std::move(it2))
        {}

        template<cppsort::mstd::forward_range Range>
        constexpr explicit end_sentinel_range( Range range):
            begin_(cppsort::mstd::begin(range)),
            end_(cppsort::mstd::end(range))
        {}

        constexpr auto begin()
            -> Iterator
        {
            return begin_;
        }

        constexpr auto end()
            -> end_sentinel<Iterator>
        {
            return end_sentinel<Iterator>(end_);
        }
};

#endif // CPPSORT_TESTSUITE_END_SENTINEL_H_
