/*
 * Copyright (c) 2023-2024 Morwenn
 * SPDX-License-Identifier: MIT
 */
#ifndef CPPSORT_METRICS_PROJECTIONS_H_
#define CPPSORT_METRICS_PROJECTIONS_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <functional>
#include <utility>
#include <cpp-sort/fwd.h>
#include <cpp-sort/sorter_facade.h>
#include <cpp-sort/sorter_traits.h>
#include <cpp-sort/utility/adapter_storage.h>
#include <cpp-sort/utility/as_function.h>
#include <cpp-sort/utility/branchless_traits.h>
#include <cpp-sort/utility/metrics_tools.h>
#include "../detail/checkers.h"
#include "../detail/iterator_traits.h"

namespace cppsort::metrics::detail
{
    template<typename Projection, typename CountType>
    class projection_counter
    {
        public:

            projection_counter(Projection projection, CountType& count):
                projection(std::move(projection)),
                count(count)
            {}

            template<typename T>
            auto operator()(T&& value)
                -> decltype(auto)
            {
                ++count;
                auto&& proj = utility::as_function(projection);
                return proj(std::forward<T>(value));
            }

            // Accessible member data
            Projection projection;

        private:

            // Projection functions are generally passed by value,
            // therefore we need to know where is the original counter
            // in order to increment it
            CountType& count;
    };
}

namespace cppsort::utility
{
    template<typename Projection, typename CountType, typename T>
    struct is_probably_branchless_projection<
        cppsort::metrics::detail::projection_counter<Projection, CountType>,
        T
    >:
        // Lie about being branchless if needed: what matters is to get
        // an accurate count of the number of projections performed by
        // algorithms even when not under analysis
        is_probably_branchless_projection<Projection, T>
    {};
}

namespace cppsort::metrics
{
    ////////////////////////////////////////////////////////////
    // Tag

    struct projections_tag {};

    ////////////////////////////////////////////////////////////
    // Metric

    namespace detail
    {
        template<typename Sorter, typename CountType>
        struct projections_impl:
            utility::adapter_storage<Sorter>,
            cppsort::detail::check_iterator_category<Sorter>,
            cppsort::detail::check_is_always_stable<Sorter>
        {
            using tag_t = projections_tag;
            using metric_t = utility::metric<CountType, tag_t>;

            projections_impl() = default;

            constexpr explicit projections_impl(Sorter&& sorter):
                utility::adapter_storage<Sorter>(std::move(sorter))
            {}

            template<
                typename Self,
                typename ForwardIterator,
                typename Compare = std::less<>,
                typename Projection = std::identity
            >
                requires is_projection_iterator_v<Projection, ForwardIterator, Compare>
            auto operator()(this Self&& self, ForwardIterator first, ForwardIterator last,
                            Compare compare={}, Projection projection={})
                -> metric_t
            {
                CountType count(0);
                projection_counter<Projection, CountType> counter(std::move(projection), count);

                std::forward<Self>(self).get()(std::move(first), std::move(last),
                                               std::move(compare), std::move(counter));
                return metric_t(count);
            }
        };
    }

    template<typename Sorter, typename CountType>
    struct projections:
        sorter_facade<detail::projections_impl<Sorter, CountType>>
    {
        projections() = default;

        constexpr explicit projections(Sorter sorter):
            sorter_facade<detail::projections_impl<Sorter, CountType>>(std::move(sorter))
        {}
    };
}

namespace cppsort
{
    ////////////////////////////////////////////////////////////
    // is_stable specialization

    template<typename Sorter, typename CountType, typename... Args>
    struct is_stable<metrics::projections<Sorter, CountType>(Args...)>:
        is_stable<Sorter(Args...)>
    {};
}

#endif // CPPSORT_METRICS_PROJECTIONS_H_
