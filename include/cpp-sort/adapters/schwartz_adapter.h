/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2016 Morwenn
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
#ifndef CPPSORT_ADAPTERS_SCHWARTZ_ADAPTER_H_
#define CPPSORT_ADAPTERS_SCHWARTZ_ADAPTER_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <functional>
#include <iterator>
#include <memory>
#include <type_traits>
#include <utility>
#include <vector>
#include <cpp-sort/sorter_facade.h>
#include <cpp-sort/sorter_traits.h>
#include <cpp-sort/sorters/std_sorter.h>
#include <cpp-sort/sorters/std_stable_sorter.h>
#include <cpp-sort/utility/as_function.h>
#include <cpp-sort/utility/functional.h>
#include "../detail/associate_iterator.h"
#include "../detail/checkers.h"
#include "../detail/memory.h"

namespace cppsort
{
    ////////////////////////////////////////////////////////////
    // Adapter

    namespace detail
    {
        template<typename Sorter>
        struct schwartz_adapter_impl:
            check_iterator_category<Sorter>,
            check_is_stable<Sorter>
        {
            static_assert(not std::is_same<Sorter, std_sorter>::value,
                          "std_sorter doesn't work with schwartz_adapter");
            static_assert(not std::is_same<Sorter, std_stable_sorter>::value,
                          "std_stable_sorter doesn't work with schwartz_adapter");

            template<
                typename ForwardIterator,
                typename Compare = std::less<>,
                typename Projection = utility::identity,
                typename = std::enable_if_t<is_projection_iterator<
                    Projection, ForwardIterator, Compare
                >>
            >
            auto operator()(ForwardIterator first, ForwardIterator last,
                            Compare compare={}, Projection projection={}) const
                -> void
            {
                auto&& proj = utility::as_function(projection);
                using proj_t = std::decay_t<decltype(proj(*first))>;
                using value_t = association<ForwardIterator, proj_t>;

                // Collection of projected elements
                auto size = std::distance(first, last);
                std::unique_ptr<value_t, operator_deleter> projected(
                    static_cast<value_t*>(::operator new(size * sizeof(value_t)))
                );
                destruct_n d(0);
                std::unique_ptr<value_t, destruct_n&> h2(projected.get(), d);

                // Associate iterator to projected element
                for (auto ptr = projected.get() ; first != last ; ++first, (void) ++ptr)
                {
                    ::new(ptr) value_t(first, proj(*first));
                    d.incr((value_t*)nullptr);
                }

                // Indirectly sort the original sequence
                Sorter{}(
                    detail::make_associate_iterator(projected.get()),
                    detail::make_associate_iterator(projected.get() + size),
                    detail::association_compare<Compare>(compare)
                );
            }
        };
    }

    template<typename Sorter>
    struct schwartz_adapter:
        sorter_facade<detail::schwartz_adapter_impl<Sorter>>
    {};
}

#ifdef CPPSORT_ADAPTERS_SMALL_ARRAY_ADAPTER_H_
#include "../detail/schwartz_small_array.h"
#endif

#endif // CPPSORT_ADAPTERS_SCHWARTZ_ADAPTER_H_
