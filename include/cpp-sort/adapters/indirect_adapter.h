/*
 * Copyright (c) 2015-2024 Morwenn
 * SPDX-License-Identifier: MIT
 */
#ifndef CPPSORT_ADAPTERS_INDIRECT_ADAPTER_H_
#define CPPSORT_ADAPTERS_INDIRECT_ADAPTER_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <functional>
#include <iterator>
#include <type_traits>
#include <utility>
#include <vector>
#include <cpp-sort/mstd/iterator.h>
#include <cpp-sort/mstd/ranges.h>
#include <cpp-sort/sorter_facade.h>
#include <cpp-sort/sorter_traits.h>
#include <cpp-sort/utility/adapter_storage.h>
#include <cpp-sort/utility/functional.h>
#include "../detail/checkers.h"
#include "../detail/immovable_vector.h"
#include "../detail/indiesort.h"
#include "../detail/scope_exit.h"

namespace cppsort
{
    ////////////////////////////////////////////////////////////
    // Adapter

    namespace detail
    {
        template<
            mstd::forward_iterator Iterator,
            typename Sorter,
            typename Compare,
            typename Projection
        >
        auto sort_indirectly(Sorter&& sorter, Iterator first, Iterator last,
                             mstd::iter_difference_t<Iterator> size,
                             Compare compare, Projection projection)
            -> decltype(cppsort::detail::indiesort(std::forward<Sorter>(sorter),
                                                   first, last, size,
                                                   std::move(compare), std::move(projection)))
        {
            return cppsort::detail::indiesort(std::forward<Sorter>(sorter),
                                              first, last, size,
                                              std::move(compare), std::move(projection));
        }

        template<
            mstd::random_access_iterator Iterator,
            typename Sorter,
            typename Compare,
            typename Projection
        >
        auto sort_indirectly(Sorter&& sorter, Iterator first, Iterator last,
                             mstd::iter_difference_t<Iterator> size,
                             Compare compare, Projection projection)
        -> decltype(std::forward<Sorter>(sorter)(
            (Iterator*)0, (Iterator*)0,
            std::move(compare),
            utility::indirect{} | std::move(projection)
        ))
        {
            ////////////////////////////////////////////////////////////
            // Indirectly sort the iterators

            immovable_vector<Iterator> iterators(size);
            for (auto it = first; it != last; ++it) {
                iterators.emplace_back(it);
            }

            // Work around the sorters that return void
            auto exit_function = make_scope_success([&] {

                ////////////////////////////////////////////////////////////
                // Move the values according the iterator's positions

                std::vector<bool> sorted(last - first, false);

                // Element where the current cycle starts
                auto start = first;

                while (start != last) {
                    // Find the element to put in current's place
                    auto current = start;
                    auto next_pos = current - first;
                    auto next = iterators[next_pos];
                    sorted[next_pos] = true;

                    // Process the current cycle
                    if (next != current) {
                        auto tmp = mstd::iter_move(current);
                        while (next != start) {
                            *current = mstd::iter_move(next);
                            current = next;
                            auto next_pos = next - first;
                            next = iterators[next_pos];
                            sorted[next_pos] = true;
                        }
                        *current = std::move(tmp);
                    }

                    // Find the next cycle
                    do {
                        ++start;
                    } while (start != last && sorted[start - first]);

                }
            });

            if (size < 2) {
                exit_function.deactivate();
            }

            return std::forward<Sorter>(sorter)(
                iterators.begin(), iterators.end(),
                std::move(compare),
                utility::indirect{} | std::move(projection)
            );
        }

        template<typename Sorter>
        struct indirect_adapter_impl:
            utility::adapter_storage<Sorter>,
            check_is_always_stable<Sorter>
        {
            indirect_adapter_impl() = default;

            constexpr explicit indirect_adapter_impl(Sorter&& sorter):
                utility::adapter_storage<Sorter>(std::move(sorter))
            {}

            template<
                typename Self,
                mstd::forward_range Range,
                typename Compare = std::less<>,
                typename Projection = std::identity
            >
                requires is_projection_v<Projection, Range, Compare>
            auto operator()(this Self&& self, Range&& range,
                            Compare compare={}, Projection projection={})
                -> decltype(sort_indirectly(std::forward<Self>(self).get(),
                                            mstd::begin(range), mstd::end(range),
                                            mstd::distance(range),
                                            std::move(compare), std::move(projection)))
            {
                return sort_indirectly(std::forward<Self>(self).get(),
                                       mstd::begin(range), mstd::end(range),
                                       mstd::distance(range),
                                       std::move(compare), std::move(projection));
            }

            template<
                typename Self,
                mstd::forward_iterator Iterator,
                mstd::sentinel_for<Iterator> Sentinel,
                typename Compare = std::less<>,
                typename Projection = std::identity
            >
                requires is_projection_iterator_v<Projection, Iterator, Compare>
            auto operator()(this Self&& self, Iterator first, Sentinel last,
                            Compare compare={}, Projection projection={})
                -> decltype(sort_indirectly(std::forward<Self>(self).get(),
                                            first, mstd::next(first, last),
                                            mstd::distance(first, last),
                                            std::move(compare), std::move(projection)))
            {
                return sort_indirectly(std::forward<Self>(self).get(),
                                       first, mstd::next(first, last),
                                       mstd::distance(first, last),
                                       std::move(compare), std::move(projection));
            }

            ////////////////////////////////////////////////////////////
            // Sorter traits

            using iterator_category = std::forward_iterator_tag;
        };
    }

    template<typename Sorter>
    struct indirect_adapter:
        sorter_facade<detail::indirect_adapter_impl<Sorter>>
    {
        indirect_adapter() = default;

        constexpr explicit indirect_adapter(Sorter sorter):
            sorter_facade<detail::indirect_adapter_impl<Sorter>>(std::move(sorter))
        {}
    };

    ////////////////////////////////////////////////////////////
    // is_stable specialization

    template<typename Sorter, typename... Args>
    struct is_stable<indirect_adapter<Sorter>(Args...)>:
        is_stable<Sorter(Args...)>
    {};
}

#endif // CPPSORT_ADAPTERS_INDIRECT_ADAPTER_H_
