/*
 * Copyright (c) 2023-2024 Morwenn
 * SPDX-License-Identifier: MIT
 */
#ifndef CPPSORT_METRICS_RUNNING_TIME_H_
#define CPPSORT_METRICS_RUNNING_TIME_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <chrono>
#include <utility>
#include <cpp-sort/fwd.h>
#include <cpp-sort/sorter_traits.h>
#include <cpp-sort/utility/adapter_storage.h>
#include <cpp-sort/utility/metrics_tools.h>
#include "../detail/checkers.h"

namespace cppsort::metrics
{
    ////////////////////////////////////////////////////////////
    // Tag

    struct running_time_tag {};

    ////////////////////////////////////////////////////////////
    // Metric

    template<typename Sorter, typename DurationType>
    struct running_time:
        utility::adapter_storage<Sorter>,
        cppsort::detail::check_iterator_category<Sorter>,
        cppsort::detail::check_is_always_stable<Sorter>
    {
        using tag_t = running_time_tag;
        using metric_t = utility::metric<DurationType, tag_t>;

        running_time() = default;

        constexpr explicit running_time(Sorter sorter):
            utility::adapter_storage<Sorter>(std::move(sorter))
        {}

        template<typename Self, typename... Args>
        auto operator()(this Self&& self, Args&&... args)
            -> decltype(
                std::forward<Self>(self).get()(std::forward<Args>(args)...),
                metric_t(std::declval<DurationType>())
            )
        {
            auto start = std::chrono::steady_clock::now();
            std::forward<Self>(self).get()(std::forward<Args>(args)...);
            auto stop = std::chrono::steady_clock::now();
            return metric_t(std::chrono::duration_cast<DurationType>(stop - start));
        }
    };
}

namespace cppsort
{
    ////////////////////////////////////////////////////////////
    // is_stable specialization

    template<typename Sorter, typename DurationType, typename... Args>
    struct is_stable<metrics::running_time<Sorter, DurationType>(Args...)>:
        is_stable<Sorter(Args...)>
    {};
}

#endif // CPPSORT_METRICS_RUNNING_TIME_H_
