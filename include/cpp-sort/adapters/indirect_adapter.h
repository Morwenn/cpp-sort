/*
 * Copyright (c) 2015-2022 Morwenn
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
#include <cpp-sort/sorter_facade.h>
#include <cpp-sort/sorter_traits.h>
#include <cpp-sort/utility/adapter_storage.h>
#include <cpp-sort/utility/functional.h>
#include <cpp-sort/utility/iter_move.h>
#include <cpp-sort/utility/size.h>
#include "../detail/checkers.h"
#include "../detail/functional.h"
#include "../detail/immovable_vector.h"
#include "../detail/indiesort.h"
#include "../detail/iterator_traits.h"
#include "../detail/scope_exit.h"
#include "../detail/type_traits.h"

namespace cppsort
{
    ////////////////////////////////////////////////////////////
    // Adapter

    namespace detail
    {
        template<typename ForwardIterator, typename Sorter, typename Compare, typename Projection>
        auto sort_indirectly(std::forward_iterator_tag, Sorter&& sorter,
                             ForwardIterator first, ForwardIterator last,
                             difference_type_t<ForwardIterator> size,
                             Compare compare, Projection projection)
            -> decltype(cppsort::detail::indiesort(std::forward<Sorter>(sorter),
                                                   first, last, size,
                                                   std::move(compare), std::move(projection)))
        {
            return cppsort::detail::indiesort(std::forward<Sorter>(sorter),
                                              first, last, size,
                                              std::move(compare), std::move(projection));
        }

        template<typename RandomAccessIterator, typename Sorter, typename Compare, typename Projection>
        auto sort_indirectly(std::random_access_iterator_tag, Sorter&& sorter,
                             RandomAccessIterator first, RandomAccessIterator last,
                             difference_type_t<RandomAccessIterator> size,
                             Compare compare, Projection projection)
#ifdef __cpp_lib_uncaught_exceptions
        -> decltype(std::forward<Sorter>(sorter)(
            (RandomAccessIterator*)0, (RandomAccessIterator*)0,
            std::move(compare), indirect(projection)
        ))
#else
        -> std::enable_if_t<
                has_comparison_projection_sort_iterator<
                    Sorter,
                    RandomAccessIterator*,
                    Compare,
                    indirect_t<Projection>
                >::value
            >
#endif
        {
            using utility::iter_move;

            ////////////////////////////////////////////////////////////
            // Indirectly sort the iterators

            immovable_vector<RandomAccessIterator> iterators(size);
            for (auto it = first; it != last; ++it) {
                iterators.emplace_back(it);
            }

#ifndef __cpp_lib_uncaught_exceptions
            // Sort the iterators on pointed values
            std::forward<Sorter>(sorter)(
                iterators.begin(), iterators.end(),
                std::move(compare), indirect(projection)
            );
#else
            // Work around the sorters that return void
            auto exit_function = make_scope_success([&] {
#endif
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
                        auto tmp = iter_move(current);
                        while (next != start) {
                            *current = iter_move(next);
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
#ifdef __cpp_lib_uncaught_exceptions
            });

            if (size < 2) {
                exit_function.deactivate();
            }

            return std::forward<Sorter>(sorter)(
                iterators.begin(), iterators.end(),
                std::move(compare), indirect(projection)
            );
#endif
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
                typename ForwardIterable,
                typename Compare = std::less<>,
                typename Projection = utility::identity,
                typename = detail::enable_if_t<
                    is_projection_v<Projection, ForwardIterable, Compare>
                >
            >
            auto operator()(ForwardIterable&& iterable,
                            Compare compare={}, Projection projection={}) const
                -> decltype(sort_indirectly(iterator_category_t<remove_cvref_t<decltype(std::begin(iterable))>>{},
                                            this->get(),
                                            std::begin(iterable), std::end(iterable),
                                            cppsort::utility::size(iterable),
                                            std::move(compare), std::move(projection)))
            {
                auto size = cppsort::utility::size(iterable);
                return sort_indirectly(iterator_category_t<remove_cvref_t<decltype(std::begin(iterable))>>{},
                                       this->get(),
                                       std::begin(iterable), std::end(iterable), size,
                                       std::move(compare), std::move(projection));
            }

            template<
                typename ForwardIterator,
                typename Compare = std::less<>,
                typename Projection = utility::identity,
                typename = detail::enable_if_t<is_projection_iterator_v<
                    Projection, ForwardIterator, Compare
                >>
            >
            auto operator()(ForwardIterator first, ForwardIterator last,
                            Compare compare={}, Projection projection={}) const
                -> decltype(sort_indirectly(iterator_category_t<ForwardIterator>{},
                                            this->get(), first, last,
                                            std::distance(first, last),
                                            std::move(compare), std::move(projection)))
            {
                auto size = std::distance(first, last);
                return sort_indirectly(iterator_category_t<ForwardIterator>{},
                                       this->get(), first, last, size,
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
