/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2016-2019 Morwenn
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
#ifndef CPPSORT_ADAPTERS_CONTAINER_AWARE_ADAPTER_H_
#define CPPSORT_ADAPTERS_CONTAINER_AWARE_ADAPTER_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <functional>
#include <type_traits>
#include <utility>
#include <cpp-sort/sort.h>
#include <cpp-sort/sorter_facade.h>
#include <cpp-sort/sorter_traits.h>
#include <cpp-sort/utility/adapter_storage.h>
#include "../detail/projection_compare.h"
#include "../detail/type_traits.h"

namespace cppsort
{
    namespace detail
    {
        // Hide the generic cppsort::sort
        struct nope_type {};
        template<typename... Args>
        auto sort(Args&&...)
            -> nope_type;

        struct adl_despair
        {
            template<typename Sorter, typename Iterable>
            auto operator()(Sorter sorter, Iterable& iterable) const
                -> decltype(sort(std::move(sorter), iterable))
            {
                return sort(std::move(sorter), iterable);
            }

            template<typename Sorter, typename Iterable, typename Function>
            auto operator()(Sorter sorter, Iterable& iterable, Function function) const
                -> decltype(sort(std::move(sorter), iterable, std::move(function)))
            {
                return sort(std::move(sorter), iterable, std::move(function));
            }

            template<typename Sorter, typename Iterable, typename Compare, typename Projection>
            auto operator()(Sorter sorter, Iterable& iterable, Compare compare, Projection projection) const
                -> decltype(sort(std::move(sorter), iterable,
                                 std::move(compare), std::move(projection)))
            {
                return sort(std::move(sorter), iterable,
                            std::move(compare), std::move(projection));
            }
        };

        template<typename Sorter, typename Iterable>
        struct can_sort:
            conjunction<
                is_invocable<adl_despair, Sorter, Iterable&>,
                negation<is_invocable_r<nope_type, adl_despair, Sorter, Iterable&>>
            >
        {};

        template<typename Sorter, typename Iterable, typename Compare>
        struct can_comparison_sort:
            conjunction<
                is_invocable<adl_despair, Sorter, Iterable&, Compare>,
                negation<is_invocable_r<nope_type, adl_despair, Sorter, Iterable&, Compare>>,
                is_projection<utility::identity, Iterable, Compare>
            >
        {};

        template<typename Sorter, typename Iterable, typename Projection>
        struct can_projection_sort:
            conjunction<
                is_invocable<adl_despair, Sorter, Iterable&, Projection>,
                negation<is_invocable_r<nope_type, adl_despair, Sorter, Iterable&, Projection>>,
                is_projection<Projection, Iterable>
            >
        {};

        template<typename Sorter, typename Iterable, typename Compare, typename Projection>
        struct can_comparison_projection_sort:
            conjunction<
                is_invocable<adl_despair, Sorter, Iterable&, Compare, Projection>,
                negation<is_invocable_r<nope_type, adl_despair, Sorter, Iterable&, Compare, Projection>>,
                is_projection<Projection, Iterable, Compare>
            >
        {};

        template<typename Sorter>
        struct container_aware_adapter_base:
            utility::adapter_storage<Sorter>
        {
            container_aware_adapter_base() = default;

            constexpr explicit container_aware_adapter_base(Sorter&& sorter):
                utility::adapter_storage<Sorter>(std::move(sorter))
            {}

            template<
                bool Stability = false,
                typename Iterable
            >
            auto operator()(Iterable& iterable) const
                -> std::enable_if_t<
                    detail::can_sort<Sorter, Iterable>::value,
                    conditional_t<
                        Stability,
                        std::false_type,
                        decltype(detail::adl_despair{}(this->get(), iterable))
                    >
                >
            {
                return detail::adl_despair{}(this->get(), iterable);
            }

            template<
                bool Stability = false,
                typename Iterable
            >
            auto operator()(Iterable& iterable) const
                -> std::enable_if_t<
                    not detail::can_sort<Sorter, Iterable>::value,
                    conditional_t<
                        Stability,
                        cppsort::is_stable<Sorter(Iterable&)>,
                        decltype(cppsort::sort(this->get(), iterable))
                    >
                >
            {
                return cppsort::sort(this->get(), iterable);
            }

            template<
                bool Stability = false,
                typename Iterable,
                typename Compare
            >
            auto operator()(Iterable& iterable, Compare compare) const
                -> std::enable_if_t<
                    detail::can_comparison_sort<Sorter, Iterable, Compare>::value,
                    conditional_t<
                        Stability,
                        std::false_type,
                        decltype(detail::adl_despair{}(this->get(), iterable, std::move(compare)))
                    >
                >
            {
                return detail::adl_despair{}(this->get(), iterable, std::move(compare));
            }

            template<
                bool Stability = false,
                typename Iterable,
                typename Compare
            >
            auto operator()(Iterable& iterable, Compare compare) const
                -> std::enable_if_t<
                    not is_projection<Compare, Iterable>::value &&
                    not detail::can_comparison_sort<Sorter, Iterable, Compare>::value,
                    conditional_t<
                        Stability,
                        cppsort::is_stable<Sorter(Iterable&, Compare)>,
                        decltype(cppsort::sort(this->get(), iterable, std::move(compare)))
                    >
                >
            {
                return cppsort::sort(this->get(), iterable, std::move(compare));
            }

            template<
                bool Stability = false,
                typename Iterable,
                typename Projection
            >
            auto operator()(Iterable& iterable, Projection projection) const
                -> std::enable_if_t<
                    not detail::can_comparison_sort<Sorter, Iterable, Projection>::value &&
                    detail::can_projection_sort<Sorter, Iterable, Projection>::value,
                    conditional_t<
                        Stability,
                        std::false_type,
                        decltype(detail::adl_despair{}(this->get(), iterable, std::move(projection)))
                    >
                >
            {
                return detail::adl_despair{}(this->get(), iterable, std::move(projection));
            }

            template<
                bool Stability = false,
                typename Iterable,
                typename Projection
            >
            auto operator()(Iterable& iterable, Projection projection) const
                -> std::enable_if_t<
                    not detail::can_projection_sort<Sorter, Iterable, Projection>::value &&
                    detail::can_comparison_projection_sort<Sorter, Iterable, std::less<>, Projection>::value,
                    conditional_t<
                        Stability,
                        std::false_type,
                        decltype(detail::adl_despair{}(this->get(), iterable,
                                                       std::less<>{}, std::move(projection)))
                    >
                >
            {
                return detail::adl_despair{}(this->get(), iterable,
                                             std::less<>{}, std::move(projection));
            }

            template<
                bool Stability = false,
                typename Iterable,
                typename Projection
            >
            auto operator()(Iterable& iterable, Projection projection) const
                -> std::enable_if_t<
                    not detail::can_projection_sort<Sorter, Iterable, Projection>::value &&
                    not detail::can_comparison_projection_sort<Sorter, Iterable, std::less<>, Projection>::value &&
                    detail::can_comparison_sort<
                        Sorter,
                        Iterable,
                        detail::projection_compare<std::less<>, Projection>
                    >::value,
                    conditional_t<
                        Stability,
                        std::false_type,
                        decltype(detail::adl_despair{}(this->get(), iterable,
                                                       detail::make_projection_compare(std::less<>{},
                                                                                       std::move(projection))))
                    >
                >
            {
                return detail::adl_despair{}(this->get(), iterable,
                                             detail::make_projection_compare(std::less<>{},
                                                                             std::move(projection)));
            }

            template<
                bool Stability = false,
                typename Iterable,
                typename Projection
            >
            auto operator()(Iterable& iterable, Projection projection) const
                -> std::enable_if_t<
                    is_projection<Projection, Iterable>::value &&
                    not detail::can_projection_sort<Sorter, Iterable, Projection>::value &&
                    not detail::can_comparison_projection_sort<Sorter, Iterable, std::less<>, Projection>::value &&
                    not detail::can_comparison_sort<
                        Sorter,
                        Iterable,
                        detail::projection_compare<std::less<>, Projection>
                    >::value,
                    conditional_t<
                        Stability,
                        cppsort::is_stable<Sorter(Iterable&, Projection)>,
                        decltype(cppsort::sort(this->get(), iterable, std::move(projection)))
                    >
                >
            {
                return cppsort::sort(this->get(), iterable, std::move(projection));
            }

            template<
                bool Stability = false,
                typename Iterable,
                typename Compare,
                typename Projection
            >
            auto operator()(Iterable& iterable, Compare compare, Projection projection) const
                -> std::enable_if_t<
                    detail::can_comparison_projection_sort<Sorter, Iterable, Compare, Projection>::value,
                    conditional_t<
                        Stability,
                        std::false_type,
                        decltype(detail::adl_despair{}(this->get(), iterable,
                                                       std::move(compare), std::move(projection)))
                    >
                >
            {
                return detail::adl_despair{}(this->get(), iterable,
                                             std::move(compare), std::move(projection));
            }

            template<
                bool Stability = false,
                typename Iterable,
                typename Compare,
                typename Projection
            >
            auto operator()(Iterable& iterable, Compare compare, Projection projection) const
                -> std::enable_if_t<
                    not detail::can_comparison_projection_sort<Sorter, Iterable, Compare, Projection>::value &&
                    detail::can_comparison_sort<
                        Sorter,
                        Iterable,
                        detail::projection_compare<Compare, Projection>
                    >::value,
                    conditional_t<
                        Stability,
                        std::false_type,
                        decltype(detail::adl_despair{}(this->get(), iterable,
                                                       detail::make_projection_compare(std::move(compare),
                                                                                       std::move(projection))))
                    >
                >
            {
                return detail::adl_despair{}(this->get(), iterable,
                                             detail::make_projection_compare(std::move(compare),
                                                                             std::move(projection)));
            }

            template<
                bool Stability = false,
                typename Iterable,
                typename Compare,
                typename Projection
            >
            auto operator()(Iterable& iterable, Compare compare, Projection projection) const
                -> std::enable_if_t<
                    not detail::can_comparison_projection_sort<Sorter, Iterable, Compare, Projection>::value &&
                    not detail::can_comparison_sort<
                        Sorter,
                        Iterable,
                        detail::projection_compare<Compare, Projection>
                    >::value,
                    conditional_t<
                        Stability,
                        cppsort::is_stable<Sorter(Iterable&, Compare, Projection)>,
                        decltype(cppsort::sort(this->get(), iterable,
                                               std::move(compare), std::move(projection)))
                    >
                >
            {
                return cppsort::sort(this->get(), iterable,
                                     std::move(compare), std::move(projection));
            }
        };
    }

    template<typename Sorter>
    struct container_aware_adapter:
        detail::container_aware_adapter_base<Sorter>,
        detail::sorter_facade_fptr<
            container_aware_adapter<Sorter>,
            std::is_empty<Sorter>::value
        >
    {
        container_aware_adapter() = default;

        constexpr explicit container_aware_adapter(Sorter sorter):
            detail::container_aware_adapter_base<Sorter>(std::move(sorter))
        {}
    };

    ////////////////////////////////////////////////////////////
    // is_stable specialization

    template<typename Sorter, typename... Args>
    struct is_stable<container_aware_adapter<Sorter>(Args...)>:
        decltype(container_aware_adapter<Sorter>{}.template operator()<true>(std::declval<Args&>()...))
    {};
}

#ifdef CPPSORT_SORTERS_INSERTION_SORTER_DONE_
#include "../detail/container_aware/insertion_sort.h"
#endif

#ifdef CPPSORT_SORTERS_MERGE_SORTER_DONE_
#include "../detail/container_aware/merge_sort.h"
#endif

#ifdef CPPSORT_SORTERS_SELECTION_SORTER_DONE_
#include "../detail/container_aware/selection_sort.h"
#endif

#define CPPSORT_ADAPTERS_CONTAINER_AWARE_ADAPTER_DONE_

#endif // CPPSORT_ADAPTERS_CONTAINER_AWARE_ADAPTER_H_
