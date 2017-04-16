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
#ifndef CPPSORT_ADAPTERS_STABLE_ADAPTER_H_
#define CPPSORT_ADAPTERS_STABLE_ADAPTER_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <functional>
#include <iterator>
#include <memory>
#include <tuple>
#include <type_traits>
#include <utility>
#include <cpp-sort/sorter_facade.h>
#include <cpp-sort/sorter_traits.h>
#include <cpp-sort/utility/as_function.h>
#include <cpp-sort/utility/functional.h>
#include "../detail/associate_iterator.h"
#include "../detail/checkers.h"
#include "../detail/iterator_traits.h"
#include "../detail/memory.h"

namespace cppsort
{
    namespace detail
    {
        ////////////////////////////////////////////////////////////
        // Stable comparison function

        template<
            typename Compare,
            typename Projection = utility::identity
        >
        class stable_compare
        {
            private:

                using projection_t = decltype(utility::as_function(std::declval<Projection&>()));
                using compare_t = decltype(utility::as_function(std::declval<Compare&>()));
                std::tuple<compare_t, projection_t> data;

            public:

                stable_compare(Compare compare, Projection projection={}):
                    data(utility::as_function(compare), utility::as_function(projection))
                {}

                auto compare() const
                    -> compare_t
                {
                    return std::get<0>(data);
                }

                auto projection() const
                    -> projection_t
                {
                    return std::get<1>(data);
                }

                template<typename T, typename U>
                auto operator()(T&& lhs, U&& rhs)
                    -> bool
                {
                    if (std::get<0>(data)(std::get<1>(data)(std::forward<T>(lhs).get()),
                                          std::get<1>(data)(std::forward<U>(rhs).get())))
                    {
                        return true;
                    }
                    if (std::get<0>(data)(std::get<1>(data)(std::forward<U>(rhs).get()),
                                          std::get<1>(data)(std::forward<T>(lhs).get())))
                    {
                        return false;
                    }
                    return lhs.data < rhs.data;
                }
        };

        template<typename Compare, typename Projection=utility::identity>
        auto make_stable_compare(Compare compare, Projection projection={})
            -> stable_compare<Compare, Projection>
        {
            return { compare, projection };
        }

        ////////////////////////////////////////////////////////////
        // Adapter

        template<typename Sorter>
        struct stable_adapter_impl:
            check_iterator_category<Sorter>
        {
            template<
                typename Iterator,
                typename Compare = std::less<>,
                typename Projection = utility::identity,
                typename = std::enable_if_t<is_projection_iterator_v<
                    Projection, Iterator, Compare
                >>
            >
            auto operator()(Iterator first, Iterator last,
                            Compare compare={}, Projection projection={}) const
                -> decltype(auto)
            {
                using difference_type = difference_type_t<Iterator>;
                using value_t = association<Iterator, difference_type>;

                ////////////////////////////////////////////////////////////
                // Bind index to iterator

                auto size = std::distance(first, last);
                std::unique_ptr<value_t, operator_deleter> iterators(
                    static_cast<value_t*>(::operator new(size * sizeof(value_t)))
                );
                destruct_n<value_t> d(0);
                std::unique_ptr<value_t, destruct_n<value_t>&> h2(iterators.get(), d);

                // Associate iterators to their position
                difference_type count = 0;
                for (auto ptr = iterators.get() ; first != last ; ++d, (void) ++first, ++ptr)
                {
                    ::new(ptr) value_t(first, count++);
                }

                ////////////////////////////////////////////////////////////
                // Sort but takes the index into account to ensure stability

                return Sorter{}(
                    make_associate_iterator(iterators.get()),
                    make_associate_iterator(iterators.get() + size),
                    make_stable_compare(std::move(compare), std::move(projection))
                );
            }

            ////////////////////////////////////////////////////////////
            // Sorter traits

            using is_always_stable = std::true_type;
        };
    }

    // Expose the underlying mechanism
    template<typename Sorter>
    struct make_stable:
        sorter_facade<detail::stable_adapter_impl<Sorter>>
    {};

    // Actual sorter
    template<typename Sorter>
    struct stable_adapter:
        detail::check_iterator_category<Sorter>
    {
        template<
            typename... Args,
            typename = std::enable_if_t<is_stable<Sorter(Args...)>::value>
        >
        auto operator()(Args&&... args) const
            -> decltype(Sorter{}(std::forward<Args>(args)...))
        {
            return Sorter{}(std::forward<Args>(args)...);
        }

        template<
            typename... Args,
            typename = std::enable_if_t<not is_stable<Sorter(Args...)>::value>,
            typename = void
        >
        auto operator()(Args&&... args) const
            -> decltype(make_stable<Sorter>{}(std::forward<Args>(args)...))
        {
            return make_stable<Sorter>{}(std::forward<Args>(args)...);
        }

        ////////////////////////////////////////////////////////////
        // Sorter traits

        using is_always_stable = std::true_type;
    };
}

#ifdef CPPSORT_ADAPTERS_SELF_SORT_ADAPTER_DONE_
#include "../detail/stable_adapter_self_sort_adapter.h"
#endif

#define CPPSORT_ADAPTERS_STABLE_ADAPTER_DONE_

#endif // CPPSORT_ADAPTERS_STABLE_ADAPTER_H_
