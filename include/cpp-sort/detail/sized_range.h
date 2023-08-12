/*
 * Copyright (c) 2023 Morwenn
 * SPDX-License-Identifier: MIT
 */
#ifndef CPPSORT_DETAIL_SIZED_RANGE_H_
#define CPPSORT_DETAIL_SIZED_RANGE_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <iterator>
#include <type_traits>
#include "config.h"
#include "iterator_traits.h"

namespace cppsort
{
namespace detail
{
    // Very simple class to bundle together a pair of begin/end
    // iterators and a corresponding size - shallow constness

    template<typename ForwardIterator>
    class sized_range
    {
        public:

            constexpr sized_range(ForwardIterator begin, ForwardIterator end,
                                  difference_type_t<ForwardIterator> size):
                begin_(begin),
                end_(end),
                size_(size)
            {
                using category = iterator_category_t<ForwardIterator>;
                if (std::is_base_of<std::random_access_iterator_tag, category>::value) {
                    CPPSORT_ASSERT(std::distance(begin, end) == size);
                } else {
                    CPPSORT_AUDIT(std::distance(begin, end) == size);
                }
            }

            constexpr auto begin() const
                -> ForwardIterator
            {
                return begin_;
            }

            constexpr auto end() const
                -> ForwardIterator
            {
                return end_;
            }

            constexpr auto size() const noexcept
                -> difference_type_t<ForwardIterator>
            {
                return size_;
            }

        private:

            ForwardIterator begin_;
            ForwardIterator end_;
            difference_type_t<ForwardIterator> size_;
    };

    template<typename ForwardIterator>
    constexpr auto make_sized_range(ForwardIterator begin, ForwardIterator end,
                                    difference_type_t<ForwardIterator> size)
        -> sized_range<ForwardIterator>
    {
        return sized_range<ForwardIterator>(std::move(begin), std::move(end), size);
    }
}}

#endif // CPPSORT_DETAIL_SIZED_RANGE_H_
