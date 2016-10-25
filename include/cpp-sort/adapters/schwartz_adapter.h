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
#include <cpp-sort/fwd.h>
#include <cpp-sort/sorter_facade.h>
#include <cpp-sort/sorter_traits.h>
#include <cpp-sort/utility/as_function.h>
#include "../detail/associate_iterator.h"
#include "../detail/checkers.h"
#include "../detail/memory.h"

namespace cppsort
{
    namespace detail
    {
        ////////////////////////////////////////////////////////////
        // Adapter

        template<typename Sorter>
        struct schwartz_adapter_impl:
            check_iterator_category<Sorter>,
            check_is_always_stable<Sorter>
        {
            template<typename ForwardIterator, typename Compare, typename Projection>
            auto operator()(ForwardIterator first, ForwardIterator last,
                            Compare compare, Projection projection) const
                -> std::enable_if_t<is_projection_iterator_v<
                    Projection, ForwardIterator, Compare
                >>
            {
                static_assert(not std::is_same<Sorter, std_sorter>::value,
                              "std_sorter doesn't work with schwartz_adapter");
                static_assert(not std::is_same<Sorter, stable_adapter<std_sorter>>::value,
                              "stable_adapter<std_sorter> doesn't work with schwartz_adapter");

                auto&& proj = utility::as_function(projection);
                using proj_t = std::decay_t<decltype(proj(*first))>;
                using value_t = association<ForwardIterator, proj_t>;

                // Collection of projected elements
                auto size = std::distance(first, last);
                std::unique_ptr<value_t, operator_deleter> projected(
                    static_cast<value_t*>(::operator new(size * sizeof(value_t)))
                );
                destruct_n<value_t> d(0);
                std::unique_ptr<value_t, destruct_n<value_t>&> h2(projected.get(), d);

                // Associate iterator to projected element
                for (auto ptr = projected.get() ; first != last ; ++d, (void) ++first, ++ptr)
                {
                    ::new(ptr) value_t(first, proj(*first));
                }

                // Indirectly sort the original sequence
                Sorter{}(
                    make_associate_iterator(projected.get()),
                    make_associate_iterator(projected.get() + size),
                    std::move(compare),
                    [](const auto& value) -> auto& { return value.data; }
                );
            }

            template<typename ForwardIterator, typename Compare=std::less<>>
            auto operator()(ForwardIterator first, ForwardIterator last,
                            Compare compare={}) const
                -> std::enable_if_t<not is_projection_iterator_v<
                    Compare, ForwardIterator
                >>
            {
                // No projection to handle, forward everything to
                // the adapted sorter
                Sorter{}(std::move(first), std::move(last), std::move(compare));
            }
        };
    }

    template<typename Sorter>
    struct schwartz_adapter:
        sorter_facade<detail::schwartz_adapter_impl<Sorter>>
    {};

    ////////////////////////////////////////////////////////////
    // is_stable specialization

    template<typename Sorter, typename... Args>
    struct is_stable<schwartz_adapter<Sorter>(Args...)>:
        is_stable<Sorter(Args...)>
    {};
}

#ifdef CPPSORT_ADAPTERS_SMALL_ARRAY_ADAPTER_H_
#include "../detail/schwartz_small_array.h"
#endif

#endif // CPPSORT_ADAPTERS_SCHWARTZ_ADAPTER_H_
