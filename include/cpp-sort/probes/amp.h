/*
 * Copyright (c) 2025 Morwenn
 * SPDX-License-Identifier: MIT
 */
#ifndef CPPSORT_PROBES_AMP_H_
#define CPPSORT_PROBES_AMP_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <algorithm>
#include <functional>
#include <iterator>
#include <cpp-sort/sorter_facade.h>
#include <cpp-sort/sorter_traits.h>
#include <cpp-sort/utility/as_function.h>
#include <cpp-sort/utility/functional.h>
#include "../detail/iterator_traits.h"
#include "../detail/type_traits.h"

namespace cppsort
{
namespace probe
{
    namespace detail
    {
        struct amp_impl
        {
            template<
                typename ForwardIterator,
                typename Compare = std::less<>,
                typename Projection = utility::identity,
                typename = cppsort::detail::enable_if_t<
                    is_projection_iterator_v<Projection, ForwardIterator, Compare>
                >
            >
            auto operator()(ForwardIterator first, ForwardIterator last,
                            Compare compare={}, Projection projection={}) const
                -> cppsort::detail::difference_type_t<ForwardIterator>
            {
                using difference_type = cppsort::detail::difference_type_t<ForwardIterator>;
                auto&& comp = utility::as_function(compare);
                auto&& proj = utility::as_function(projection);

                if (first == last || std::next(first) == last) {
                    return 0;
                }

                difference_type size = 0,
                                shadow = 0,
                                min = 0,
                                max = 0;

                auto current = first;
                auto next = std::next(current);
                do {
                    ++size;

                    if (comp(proj(*current), proj(*next))) {
                        max = (std::max)(max, ++shadow);
                    } else if (comp(proj(*next), proj(*current))) {
                        min = (std::min)(min, --shadow);
                    } else {
                        // Neighbours that compare equivalent don't contribute to the amplitude
                        --size;
                    }

                    ++current;
                    ++next;
                } while (next != last);

                return size - (max - min);
            }

            template<typename Integer>
            static constexpr auto max_for_size(Integer n)
                -> Integer
            {
                return n <= 2 ? 0 : n - 2;
            }
        };
    }

    inline constexpr sorter_facade<detail::amp_impl> amp{};
}}

#endif // CPPSORT_PROBES_AMP_H_
