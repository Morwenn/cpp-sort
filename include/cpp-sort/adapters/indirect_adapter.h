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
#include <cpp-sort/utility/as_function.h>
#include <cpp-sort/utility/functional.h>
#include <cpp-sort/utility/iter_move.h>
#include "../detail/checkers.h"
#include "../detail/scope_exit.h"

namespace cppsort
{
    ////////////////////////////////////////////////////////////
    // Adapter

    namespace detail
    {
        template<typename Sorter>
        struct indirect_adapter_impl:
            check_is_always_stable<Sorter>
        {
            template<
                typename RandomAccessIterator,
                typename Compare = std::less<>,
                typename Projection = utility::identity,
                typename = std::enable_if_t<is_projection_iterator_v<
                    Projection, RandomAccessIterator, Compare
                >>
            >
            auto operator()(RandomAccessIterator first, RandomAccessIterator last,
                            Compare compare={}, Projection projection={}) const
                -> decltype(auto)
            {
                using utility::iter_move;
                auto&& proj = utility::as_function(projection);

                ////////////////////////////////////////////////////////////
                // Indirectly sort the iterators

                // Copy the iterators in a vector
                std::vector<RandomAccessIterator> iterators;
                iterators.reserve(std::distance(first, last));
                for (RandomAccessIterator it = first ; it != last ; ++it) {
                    iterators.push_back(it);
                }

#ifndef __cpp_lib_uncaught_exceptions
                // Sort the iterators on pointed values
                Sorter{}(std::begin(iterators), std::end(iterators), std::move(compare),
                         [&proj](RandomAccessIterator it) -> decltype(auto) { return proj(*it); });
#else
                // Work around the sorters that return void
                auto exit_function = make_scope_success([&] {
#endif
                    ////////////////////////////////////////////////////////////
                    // Move the values according the iterator's positions

                    std::vector<bool> sorted(std::distance(first, last), false);

                    // Element where the current cycle starts
                    RandomAccessIterator start = first;

                    while (start != last) {
                        // Find the element to put in current's place
                        RandomAccessIterator current = start;
                        auto next_pos = std::distance(first, current);
                        RandomAccessIterator next = iterators[next_pos];
                        sorted[next_pos] = true;

                        // Process the current cycle
                        if (next != current) {
                            auto tmp = iter_move(current);
                            while (next != start) {
                                *current = iter_move(next);
                                current = next;
                                auto next_pos = std::distance(first, next);
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

                if (first == last || std::next(first) == last) {
                    exit_function.release();
                }

                return Sorter{}(std::begin(iterators), std::end(iterators), std::move(compare),
                                [&proj](RandomAccessIterator it) -> decltype(auto) { return proj(*it); });
#endif
            }

            ////////////////////////////////////////////////////////////
            // Sorter traits

            using iterator_category = std::random_access_iterator_tag;
        };
    }

    template<typename Sorter>
    struct indirect_adapter:
        sorter_facade<detail::indirect_adapter_impl<Sorter>>
    {
        indirect_adapter() = default;

        // Automatic deduction guide
        constexpr explicit indirect_adapter(Sorter) noexcept {}
    };

    ////////////////////////////////////////////////////////////
    // is_stable specialization

    template<typename Sorter, typename... Args>
    struct is_stable<indirect_adapter<Sorter>(Args...)>:
        is_stable<Sorter(Args...)>
    {};
}

#endif // CPPSORT_ADAPTERS_INDIRECT_ADAPTER_H_
