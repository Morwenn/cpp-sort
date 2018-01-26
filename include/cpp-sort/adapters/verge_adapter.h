/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2017-2018 Morwenn
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
#ifndef CPPSORT_ADAPTERS_VERGE_ADAPTER_H_
#define CPPSORT_ADAPTERS_VERGE_ADAPTER_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <functional>
#include <iterator>
#include <type_traits>
#include <utility>
#include <cpp-sort/sorter_facade.h>
#include <cpp-sort/sorter_traits.h>
#include <cpp-sort/utility/adapter_storage.h>
#include <cpp-sort/utility/functional.h>
#include "../detail/vergesort.h"

namespace cppsort
{
    ////////////////////////////////////////////////////////////
    // Adapter

    namespace detail
    {
        template<typename FallbackSorter>
        struct verge_adapter_impl:
            utility::adapter_storage<FallbackSorter>
        {
            public:

                verge_adapter_impl() = default;

                constexpr verge_adapter_impl(FallbackSorter sorter):
                    utility::adapter_storage<FallbackSorter>(std::move(sorter))
                {}

            private:

                template<
                    typename Self,
                    typename RandomAccessIterator,
                    typename Compare = std::less<>,
                    typename Projection = utility::identity,
                    typename = std::enable_if_t<
                        is_projection_iterator_v<Projection, RandomAccessIterator, Compare>
                    >
                >
                static auto _call_sorter(Self& self, RandomAccessIterator first, RandomAccessIterator last,
                                         Compare compare={}, Projection projection={})
                    -> void
                {
                    static_assert(
                        std::is_base_of<
                            std::bidirectional_iterator_tag,
                            iterator_category_t<RandomAccessIterator>
                        >::value,
                        "verge_adapter requires at least random-access iterators"
                    );

                    vergesort(std::move(first), std::move(last),
                              std::move(compare), std::move(projection),
                              self.utility::template adapter_storage<FallbackSorter>::get());
                }

                using this_class = verge_adapter_impl<FallbackSorter>;

            public:

                template<typename... Args>
                auto operator()(Args&&... args) const
                    noexcept(noexcept(_call_sorter(std::declval<const this_class&>(), std::forward<Args>(args)...)))
                    -> decltype(_call_sorter(*this, std::forward<Args>(args)...))
                {
                    return _call_sorter(*this, std::forward<Args>(args)...);
                }

                template<typename... Args>
                auto operator()(Args&&... args)
                    noexcept(noexcept(_call_sorter(std::declval<this_class&>(), std::forward<Args>(args)...)))
                    -> decltype(_call_sorter(*this, std::forward<Args>(args)...))
                {
                    return _call_sorter(*this, std::forward<Args>(args)...);
                }

                ////////////////////////////////////////////////////////////
                // Sorter traits

                using iterator_category = std::random_access_iterator_tag;
                using is_always_stable = std::false_type;
        };
    }

    template<typename FallbackSorter>
    struct verge_adapter:
        sorter_facade<detail::verge_adapter_impl<FallbackSorter>>
    {
        verge_adapter() = default;

        constexpr verge_adapter(FallbackSorter sorter):
            sorter_facade<detail::verge_adapter_impl<FallbackSorter>>(std::move(sorter))
        {}
    };
}

#endif // CPPSORT_ADAPTERS_VERGE_ADAPTER_H_
