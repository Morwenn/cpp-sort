/*
 * Copyright (c) 2023 Morwenn
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
#include <cpp-sort/utility/metrics_tools.h>
#include "../detail/checkers.h"
#include "../detail/iterator_traits.h"
#include "../detail/type_traits.h"

namespace cppsort
{
namespace metrics
{
    namespace detail
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
                typename ForwardIterator,
                typename Compare = std::less<>,
                typename Projection = utility::identity,
                typename = cppsort::detail::enable_if_t<is_projection_iterator_v<
                    Projection, ForwardIterator, Compare
                >>
            >
            auto operator()(ForwardIterator first, ForwardIterator last,
                            Compare compare={}, Projection projection={}) const
                -> metric_t
            {
                CountType count(0);
                projection_counter<Projection, CountType> counter(std::move(projection), count);

                this->get()(first, last, std::move(compare), std::move(counter));
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
}}

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
