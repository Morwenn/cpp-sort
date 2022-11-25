/*
 * Copyright (c) 2016-2022 Morwenn
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
#include <cpp-sort/mstd/type_traits.h>
#include <cpp-sort/sorter_facade.h>
#include <cpp-sort/sorter_traits.h>
#include <cpp-sort/utility/adapter_storage.h>

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
            template<typename Sorter, typename Collection>
            auto operator()(Sorter sorter, Collection& collection) const
                -> decltype(sort(std::move(sorter), collection))
            {
                return sort(std::move(sorter), collection);
            }

            template<typename Sorter, typename Collection, typename Function>
            auto operator()(Sorter sorter, Collection& collection, Function function) const
                -> decltype(sort(std::move(sorter), collection, std::move(function)))
            {
                return sort(std::move(sorter), collection, std::move(function));
            }

            template<typename Sorter, typename Collection, typename Compare, typename Projection>
            auto operator()(Sorter sorter, Collection& collection, Compare compare, Projection projection) const
                -> decltype(sort(std::move(sorter), collection,
                                 std::move(compare), std::move(projection)))
            {
                return sort(std::move(sorter), collection,
                            std::move(compare), std::move(projection));
            }
        };

        template<typename Sorter, typename Collection>
        struct can_sort:
            std::conjunction<
                std::is_invocable<adl_despair, Sorter, Collection&>,
                std::negation<std::is_invocable_r<nope_type, adl_despair, Sorter, Collection&>>
            >
        {};

        template<typename Sorter, typename Collection, typename Compare>
        struct can_comparison_sort:
            std::conjunction<
                std::is_invocable<adl_despair, Sorter, Collection&, Compare>,
                std::negation<std::is_invocable_r<nope_type, adl_despair, Sorter, Collection&, Compare>>,
                is_projection<std::identity, Collection, Compare>
            >
        {};

        template<typename Sorter, typename Collection, typename Projection>
        struct can_projection_sort:
            std::conjunction<
                std::is_invocable<adl_despair, Sorter, Collection&, Projection>,
                std::negation<std::is_invocable_r<nope_type, adl_despair, Sorter, Collection&, Projection>>,
                is_projection<Projection, Collection>
            >
        {};

        template<typename Sorter, typename Collection, typename Compare, typename Projection>
        struct can_comparison_projection_sort:
            std::conjunction<
                std::is_invocable<adl_despair, Sorter, Collection&, Compare, Projection>,
                std::negation<std::is_invocable_r<nope_type, adl_despair, Sorter, Collection&, Compare, Projection>>,
                is_projection<Projection, Collection, Compare>
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
                typename Collection
            >
                requires detail::can_sort<Sorter, Collection>::value
            auto operator()(Collection& collection) const
                -> mstd::conditional_t<
                    Stability,
                    std::false_type,
                    decltype(detail::adl_despair{}(this->get(), collection))
                >
            {
                return detail::adl_despair{}(this->get(), collection);
            }

            template<
                bool Stability = false,
                typename Collection
            >
                requires (not detail::can_sort<Sorter, Collection>::value)
            auto operator()(Collection& collection) const
                -> mstd::conditional_t<
                    Stability,
                    cppsort::is_stable<Sorter(Collection&)>,
                    decltype(this->get()(collection))
                >
            {
                return this->get()(collection);
            }

            template<
                bool Stability = false,
                typename Collection,
                typename Compare
            >
                requires detail::can_comparison_sort<Sorter, Collection, Compare>::value
            auto operator()(Collection& collection, Compare compare) const
                -> mstd::conditional_t<
                    Stability,
                    std::false_type,
                    decltype(detail::adl_despair{}(this->get(), collection, std::move(compare)))
                >
            {
                return detail::adl_despair{}(this->get(), collection, std::move(compare));
            }

            template<
                bool Stability = false,
                typename Collection,
                typename Compare
            >
                requires (
                    not is_projection_v<Compare, Collection> &&
                    not detail::can_comparison_sort<Sorter, Collection, Compare>::value
                )
            auto operator()(Collection& collection, Compare compare) const
                -> mstd::conditional_t<
                    Stability,
                    cppsort::is_stable<Sorter(Collection&, Compare)>,
                    decltype(this->get()(collection, std::move(compare)))
                >
            {
                return this->get()(collection, std::move(compare));
            }

            template<
                bool Stability = false,
                typename Collection,
                typename Projection
            >
                requires (
                    not detail::can_comparison_sort<Sorter, Collection, Projection>::value &&
                    detail::can_projection_sort<Sorter, Collection, Projection>::value
                )
            auto operator()(Collection& collection, Projection projection) const
                -> mstd::conditional_t<
                    Stability,
                    std::false_type,
                    decltype(detail::adl_despair{}(this->get(), collection, std::move(projection)))
                >
            {
                return detail::adl_despair{}(this->get(), collection, std::move(projection));
            }

            template<
                bool Stability = false,
                typename Collection,
                typename Projection
            >
                requires (
                    not detail::can_projection_sort<Sorter, Collection, Projection>::value &&
                    detail::can_comparison_projection_sort<Sorter, Collection, std::less<>, Projection>::value
                )
            auto operator()(Collection& collection, Projection projection) const
                -> mstd::conditional_t<
                    Stability,
                    std::false_type,
                    decltype(detail::adl_despair{}(this->get(), collection, std::less{}, std::move(projection)))
                >
            {
                return detail::adl_despair{}(this->get(), collection, std::less{}, std::move(projection));
            }

            template<
                bool Stability = false,
                typename Collection,
                typename Projection
            >
                requires (
                    not detail::can_projection_sort<Sorter, Collection, Projection>::value &&
                    not detail::can_comparison_projection_sort<Sorter, Collection, std::less<>, Projection>::value &&
                    detail::can_comparison_sort<
                        Sorter,
                        Collection,
                        projection_compare_t<std::less<>, Projection>
                    >::value
                )
            auto operator()(Collection& collection, Projection projection) const
                -> mstd::conditional_t<
                    Stability,
                    std::false_type,
                    decltype(detail::adl_despair{}(this->get(), collection,
                                                   projection_compare(std::less{}, std::move(projection))))
                >
            {
                return detail::adl_despair{}(this->get(), collection,
                                             projection_compare(std::less{}, std::move(projection)));
            }

            template<
                bool Stability = false,
                typename Collection,
                typename Projection
            >
                requires (
                    is_projection_v<Projection, Collection> &&
                    not detail::can_projection_sort<Sorter, Collection, Projection>::value &&
                    not detail::can_comparison_projection_sort<Sorter, Collection, std::less<>, Projection>::value &&
                    not detail::can_comparison_sort<
                        Sorter,
                        Collection,
                        projection_compare_t<std::less<>, Projection>
                    >::value
                )
            auto operator()(Collection& collection, Projection projection) const
                -> mstd::conditional_t<
                    Stability,
                    cppsort::is_stable<Sorter(Collection&, Projection)>,
                    decltype(this->get()(collection, std::move(projection)))
                >
            {
                return this->get()(collection, std::move(projection));
            }

            template<
                bool Stability = false,
                typename Collection,
                typename Compare,
                typename Projection
            >
                requires detail::can_comparison_projection_sort<Sorter, Collection, Compare, Projection>::value
            auto operator()(Collection& collection, Compare compare, Projection projection) const
                -> mstd::conditional_t<
                    Stability,
                    std::false_type,
                    decltype(detail::adl_despair{}(this->get(), collection,
                                                   std::move(compare), std::move(projection)))
                >
            {
                return detail::adl_despair{}(this->get(), collection,
                                             std::move(compare), std::move(projection));
            }

            template<
                bool Stability = false,
                typename Collection,
                typename Compare,
                typename Projection
            >
                requires (
                    not detail::can_comparison_projection_sort<Sorter, Collection, Compare, Projection>::value &&
                    detail::can_comparison_sort<
                        Sorter,
                        Collection,
                        projection_compare_t<Compare, Projection>
                    >::value
                )
            auto operator()(Collection& collection, Compare compare, Projection projection) const
                -> mstd::conditional_t<
                    Stability,
                    std::false_type,
                    decltype(detail::adl_despair{}(this->get(), collection,
                                                   projection_compare(std::move(compare), std::move(projection))))
                >
            {
                return detail::adl_despair{}(this->get(), collection,
                                             projection_compare(std::move(compare), std::move(projection)));
            }

            template<
                bool Stability = false,
                typename Collection,
                typename Compare,
                typename Projection
            >
                requires (
                    not detail::can_comparison_projection_sort<Sorter, Collection, Compare, Projection>::value &&
                    not detail::can_comparison_sort<
                        Sorter,
                        Collection,
                        projection_compare_t<Compare, Projection>
                    >::value
                )
            auto operator()(Collection& collection, Compare compare, Projection projection) const
                -> mstd::conditional_t<
                    Stability,
                    cppsort::is_stable<Sorter(Collection&, Compare, Projection)>,
                    decltype(this->get()(collection, std::move(compare), std::move(projection)))
                >
            {
                return this->get()(collection, std::move(compare), std::move(projection));
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
