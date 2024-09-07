/*
 * Copyright (c) 2023-2024 Morwenn
 * SPDX-License-Identifier: MIT
 */
#ifndef CPPSORT_METRICS_COMPARISONS_H_
#define CPPSORT_METRICS_COMPARISONS_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <functional>
#include <utility>
#include <cpp-sort/fwd.h>
#include <cpp-sort/sorter_facade.h>
#include <cpp-sort/sorter_traits.h>
#include <cpp-sort/utility/adapter_storage.h>
#include <cpp-sort/utility/metrics_tools.h>
#include "../detail/checkers.h"
#include "../detail/comparison_counter.h"

namespace cppsort::metrics
{
    ////////////////////////////////////////////////////////////
    // Tag

    struct comparisons_tag {};

    ////////////////////////////////////////////////////////////
    // Metric

    namespace detail
    {
        template<typename Sorter, typename CountType>
        struct comparisons_impl:
            utility::adapter_storage<Sorter>,
            cppsort::detail::check_iterator_category<Sorter>,
            cppsort::detail::check_is_always_stable<Sorter>
        {
            using tag_t = comparisons_tag;
            using metric_t = utility::metric<CountType, tag_t>;

            comparisons_impl() = default;

            constexpr explicit comparisons_impl(Sorter&& sorter):
                utility::adapter_storage<Sorter>(std::move(sorter))
            {}

            template<
                typename Self,
                typename Iterable,
                typename Compare = std::less<>
            >
                requires (not is_projection_v<Compare, Iterable>)
            auto operator()(this Self&& self, Iterable&& iterable, Compare compare={})
                -> metric_t
            {
                CountType count(0);
                using cppsort::detail::comparison_counter;
                comparison_counter<Compare, CountType> cmp(std::move(compare), count);
                std::forward<Self>(self).get()(std::forward<Iterable>(iterable), std::move(cmp));
                return metric_t(count);
            }

            template<
                typename Self,
                typename Iterator,
                typename Compare = std::less<>
            >
                requires (not is_projection_iterator_v<Compare, Iterator>)
            auto operator()(this Self&& self, Iterator first, Iterator last,
                            Compare compare={})
                -> metric_t
            {
                CountType count(0);
                using cppsort::detail::comparison_counter;
                comparison_counter<Compare, CountType> cmp(std::move(compare), count);
                std::forward<Self>(self).get()(std::move(first), std::move(last), std::move(cmp));
                return metric_t(count);
            }

            template<
                typename Self,
                typename Iterable,
                typename Compare,
                typename Projection
            >
                requires is_projection_v<Projection, Iterable, Compare>
            auto operator()(this Self&& self, Iterable&& iterable,
                            Compare compare, Projection projection)
                -> metric_t
            {
                CountType count(0);
                using cppsort::detail::comparison_counter;
                comparison_counter<Compare, CountType> cmp(std::move(compare), count);
                std::forward<Self>(self).get()(std::forward<Iterable>(iterable),
                                               std::move(cmp), std::move(projection));
                return metric_t(count);
            }

            template<
                typename Self,
                typename Iterator,
                typename Compare,
                typename Projection
            >
                requires is_projection_iterator_v<Projection, Iterator, Compare>
            auto operator()(this Self&& self, Iterator first, Iterator last,
                            Compare compare, Projection projection)
                -> metric_t
            {
                CountType count(0);
                using cppsort::detail::comparison_counter;
                comparison_counter<Compare, CountType> cmp(std::move(compare), count);
                std::forward<Self>(self).get()(std::move(first), std::move(last),
                                               std::move(cmp), std::move(projection));
                return metric_t(count);
            }
        };
    }

    template<typename Sorter, typename CountType>
    struct comparisons:
        sorter_facade<detail::comparisons_impl<
            Sorter,
            CountType
        >>
    {
        comparisons() = default;

        constexpr explicit comparisons(Sorter sorter):
            sorter_facade<detail::comparisons_impl<Sorter, CountType>>(std::move(sorter))
        {}
    };
}

namespace cppsort
{
    ////////////////////////////////////////////////////////////
    // is_stable specialization

    template<typename Sorter, typename CountType, typename... Args>
    struct is_stable<metrics::comparisons<Sorter, CountType>(Args...)>:
        is_stable<Sorter(Args...)>
    {};
}

#endif // CPPSORT_METRICS_COMPARISONS_H_
