/*
 * Copyright (c) 2016-2025 Morwenn
 * SPDX-License-Identifier: MIT
 */
#ifndef CPPSORT_ADAPTERS_CONTAINER_AWARE_ADAPTER_H_
#define CPPSORT_ADAPTERS_CONTAINER_AWARE_ADAPTER_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <functional>
#include <type_traits>
#include <utility>
#include <cpp-sort/comparators/projection_compare.h>
#include <cpp-sort/sorter_facade.h>
#include <cpp-sort/sorter_traits.h>
#include <cpp-sort/utility/adapter_storage.h>
#include "../detail/type_traits.h"

namespace cppsort
{
    namespace detail
    {
        // Hide potential out-of-scope sort()
        struct nope_type {};
        template<typename... Args>
        auto sort(Args&&...)
            -> nope_type;

        struct adl_despair
        {
            template<typename Sorter, typename Range>
            auto operator()(Sorter sorter, Range& range) const
                -> decltype(sort(std::move(sorter), range))
            {
                return sort(std::move(sorter), range);
            }

            template<typename Sorter, typename Range, typename Function>
            auto operator()(Sorter sorter, Range& range, Function function) const
                -> decltype(sort(std::move(sorter), range, std::move(function)))
            {
                return sort(std::move(sorter), range, std::move(function));
            }

            template<typename Sorter, typename Range, typename Compare, typename Projection>
            auto operator()(Sorter sorter, Range& range, Compare compare, Projection projection) const
                -> decltype(sort(std::move(sorter), range,
                                 std::move(compare), std::move(projection)))
            {
                return sort(std::move(sorter), range,
                            std::move(compare), std::move(projection));
            }
        };

        template<typename Sorter, typename Range>
        struct can_sort:
            std::conjunction<
                std::is_invocable<adl_despair, Sorter, Range&>,
                std::negation<std::is_invocable_r<nope_type, adl_despair, Sorter, Range&>>
            >
        {};

        template<typename Sorter, typename Range, typename Compare>
        struct can_comparison_sort:
            std::conjunction<
                std::is_invocable<adl_despair, Sorter, Range&, Compare>,
                std::negation<std::is_invocable_r<nope_type, adl_despair, Sorter, Range&, Compare>>,
                is_projection<utility::identity, Range, Compare>
            >
        {};

        template<typename Sorter, typename Range, typename Projection>
        struct can_projection_sort:
            std::conjunction<
                std::is_invocable<adl_despair, Sorter, Range&, Projection>,
                std::negation<std::is_invocable_r<nope_type, adl_despair, Sorter, Range&, Projection>>,
                is_projection<Projection, Range>
            >
        {};

        template<typename Sorter, typename Range, typename Compare, typename Projection>
        struct can_comparison_projection_sort:
            std::conjunction<
                std::is_invocable<adl_despair, Sorter, Range&, Compare, Projection>,
                std::negation<std::is_invocable_r<nope_type, adl_despair, Sorter, Range&, Compare, Projection>>,
                is_projection<Projection, Range, Compare>
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
                typename Range
            >
            auto operator()(Range& range) const
                -> detail::enable_if_t<
                    detail::can_sort<Sorter, Range>::value,
                    conditional_t<
                        Stability,
                        std::false_type,
                        decltype(detail::adl_despair{}(this->get(), range))
                    >
                >
            {
                return detail::adl_despair{}(this->get(), range);
            }

            template<
                bool Stability = false,
                typename Range
            >
            auto operator()(Range& range) const
                -> detail::enable_if_t<
                    not detail::can_sort<Sorter, Range>::value,
                    conditional_t<
                        Stability,
                        cppsort::is_stable<Sorter(Range&)>,
                        decltype(this->get()(range))
                    >
                >
            {
                return this->get()(range);
            }

            template<
                bool Stability = false,
                typename Range,
                typename Compare
            >
            auto operator()(Range& range, Compare compare) const
                -> detail::enable_if_t<
                    detail::can_comparison_sort<Sorter, Range, Compare>::value,
                    conditional_t<
                        Stability,
                        std::false_type,
                        decltype(detail::adl_despair{}(this->get(), range, std::move(compare)))
                    >
                >
            {
                return detail::adl_despair{}(this->get(), range, std::move(compare));
            }

            template<
                bool Stability = false,
                typename Range,
                typename Compare
            >
            auto operator()(Range& range, Compare compare) const
                -> detail::enable_if_t<
                    not is_projection_v<Compare, Range> &&
                    not detail::can_comparison_sort<Sorter, Range, Compare>::value,
                    conditional_t<
                        Stability,
                        cppsort::is_stable<Sorter(Range&, Compare)>,
                        decltype(this->get()(range, std::move(compare)))
                    >
                >
            {
                return this->get()(range, std::move(compare));
            }

            template<
                bool Stability = false,
                typename Range,
                typename Projection
            >
            auto operator()(Range& range, Projection projection) const
                -> detail::enable_if_t<
                    not detail::can_comparison_sort<Sorter, Range, Projection>::value &&
                    detail::can_projection_sort<Sorter, Range, Projection>::value,
                    conditional_t<
                        Stability,
                        std::false_type,
                        decltype(detail::adl_despair{}(this->get(), range, std::move(projection)))
                    >
                >
            {
                return detail::adl_despair{}(this->get(), range, std::move(projection));
            }

            template<
                bool Stability = false,
                typename Range,
                typename Projection
            >
            auto operator()(Range& range, Projection projection) const
                -> detail::enable_if_t<
                    not detail::can_projection_sort<Sorter, Range, Projection>::value &&
                    detail::can_comparison_projection_sort<Sorter, Range, std::less<>, Projection>::value,
                    conditional_t<
                        Stability,
                        std::false_type,
                        decltype(detail::adl_despair{}(this->get(), range,
                                                       std::less{}, std::move(projection)))
                    >
                >
            {
                return detail::adl_despair{}(this->get(), range,
                                             std::less{}, std::move(projection));
            }

            template<
                bool Stability = false,
                typename Range,
                typename Projection
            >
            auto operator()(Range& range, Projection projection) const
                -> detail::enable_if_t<
                    not detail::can_projection_sort<Sorter, Range, Projection>::value &&
                    not detail::can_comparison_projection_sort<Sorter, Range, std::less<>, Projection>::value &&
                    detail::can_comparison_sort<
                        Sorter,
                        Range,
                        projection_compare_t<std::less<>, Projection>
                    >::value,
                    conditional_t<
                        Stability,
                        std::false_type,
                        decltype(detail::adl_despair{}(this->get(), range,
                                                       projection_compare(std::less{}, std::move(projection))))
                    >
                >
            {
                return detail::adl_despair{}(this->get(), range,
                                             projection_compare(std::less{}, std::move(projection)));
            }

            template<
                bool Stability = false,
                typename Range,
                typename Projection
            >
            auto operator()(Range& range, Projection projection) const
                -> detail::enable_if_t<
                    is_projection_v<Projection, Range> &&
                    not detail::can_projection_sort<Sorter, Range, Projection>::value &&
                    not detail::can_comparison_projection_sort<Sorter, Range, std::less<>, Projection>::value &&
                    not detail::can_comparison_sort<
                        Sorter,
                        Range,
                        projection_compare_t<std::less<>, Projection>
                    >::value,
                    conditional_t<
                        Stability,
                        cppsort::is_stable<Sorter(Range&, Projection)>,
                        decltype(this->get()(range, std::move(projection)))
                    >
                >
            {
                return this->get()(range, std::move(projection));
            }

            template<
                bool Stability = false,
                typename Range,
                typename Compare,
                typename Projection
            >
            auto operator()(Range& range, Compare compare, Projection projection) const
                -> detail::enable_if_t<
                    detail::can_comparison_projection_sort<Sorter, Range, Compare, Projection>::value,
                    conditional_t<
                        Stability,
                        std::false_type,
                        decltype(detail::adl_despair{}(this->get(), range,
                                                       std::move(compare), std::move(projection)))
                    >
                >
            {
                return detail::adl_despair{}(this->get(), range,
                                             std::move(compare), std::move(projection));
            }

            template<
                bool Stability = false,
                typename Range,
                typename Compare,
                typename Projection
            >
            auto operator()(Range& range, Compare compare, Projection projection) const
                -> detail::enable_if_t<
                    not detail::can_comparison_projection_sort<Sorter, Range, Compare, Projection>::value &&
                    detail::can_comparison_sort<
                        Sorter,
                        Range,
                        projection_compare_t<Compare, Projection>
                    >::value,
                    conditional_t<
                        Stability,
                        std::false_type,
                        decltype(detail::adl_despair{}(this->get(), range,
                                                       projection_compare(std::move(compare), std::move(projection))))
                    >
                >
            {
                return detail::adl_despair{}(this->get(), range,
                                             projection_compare(std::move(compare), std::move(projection)));
            }

            template<
                bool Stability = false,
                typename Range,
                typename Compare,
                typename Projection
            >
            auto operator()(Range& range, Compare compare, Projection projection) const
                -> detail::enable_if_t<
                    not detail::can_comparison_projection_sort<Sorter, Range, Compare, Projection>::value &&
                    not detail::can_comparison_sort<
                        Sorter,
                        Range,
                        projection_compare_t<Compare, Projection>
                    >::value,
                    conditional_t<
                        Stability,
                        cppsort::is_stable<Sorter(Range&, Compare, Projection)>,
                        decltype(this->get()(range, std::move(compare), std::move(projection)))
                    >
                >
            {
                return this->get()(range, std::move(compare), std::move(projection));
            }
        };
    }

    template<typename Sorter>
    struct container_aware_adapter:
        detail::container_aware_adapter_base<Sorter>,
        detail::sorter_facade_fptr<
            container_aware_adapter<Sorter>,
            std::is_empty_v<Sorter>
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

#ifdef CPPSORT_SORTERS_MEL_SORTER_DONE_
#include "../detail/container_aware/mel_sort.h"
#endif

#ifdef CPPSORT_SORTERS_MERGE_SORTER_DONE_
#include "../detail/container_aware/merge_sort.h"
#endif

#ifdef CPPSORT_SORTERS_SELECTION_SORTER_DONE_
#include "../detail/container_aware/selection_sort.h"
#endif

#define CPPSORT_ADAPTERS_CONTAINER_AWARE_ADAPTER_DONE_

#endif // CPPSORT_ADAPTERS_CONTAINER_AWARE_ADAPTER_H_
